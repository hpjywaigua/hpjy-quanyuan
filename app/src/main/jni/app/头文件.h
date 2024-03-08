#include <stdio.h>
#include <conio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/socket.h>
#include <malloc.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <locale>
#include <string>
#include <codecvt>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/uio.h>
#include <cstdio>
#include <string>
#include <cstdlib>

using namespace std;

#define AIM_PI 3.141592653589793238
double RadianSystem = 180.0 / AIM_PI; // 转为弧度

//公共变量区域
int ipid;
float matrix[50], px, py;

#if defined(__arm__)
int process_vm_readv_syscall = 376;
int process_vm_writev_syscall = 377;
#elif defined(__aarch64__)
int process_vm_readv_syscall = 270;
int process_vm_writev_syscall = 271;
#elif defined(__i386__)
int process_vm_readv_syscall = 347;
int process_vm_writev_syscall = 348;
#else
int process_vm_readv_syscall = 310;
int process_vm_writev_syscall = 311;
#endif

ssize_t process_v(pid_t __pid, const struct iovec* __local_iov, unsigned long __local_iov_count,
const struct iovec* __remote_iov, unsigned long __remote_iov_count, unsigned long __flags, bool iswrite) {
  return syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall), __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}

bool pvm(int ipid , void* address, void* buffer, size_t size, bool iswrite) {
  struct iovec local[1];
  struct iovec remote[1];

  local[0].iov_base = buffer;
  local[0].iov_len = size;
  remote[0].iov_base = address;
  remote[0].iov_len = size;

  if (ipid < 0) {
    return false;
  }

  ssize_t bytes = process_v(ipid, local, 1, remote, 1, 0, iswrite);
  return bytes == size;
}
bool vm_readv(long int address, void *buffer, size_t size) {
  return pvm(ipid, reinterpret_cast<void *>(address), buffer, size, false);
}

/* 写内存 */
bool vm_writev(long int address, void *buffer, size_t size) {
  return pvm(ipid, reinterpret_cast<void *>(address), buffer, size, true);
}

typedef char PACKAGENAME;
int getPID(const char *packageName)
{
  int id = -1;
  DIR *dir;
  FILE *fp;
  char filename[64];
  char cmdline[64];
  struct dirent *entry;
  dir = opendir("/proc");
  while ((entry = readdir(dir)) != NULL)
  {
    id = atoi(entry->d_name);
    if (id != 0)
    {
      sprintf(filename, "/proc/%d/cmdline", id);
      fp = fopen(filename, "r");
      if (fp)
      {
        fgets(cmdline, sizeof(cmdline), fp);
        fclose(fp);
        if (strcmp(packageName, cmdline) == 0)
        {
          return id;
        }
      }
    }
  }
  closedir(dir);
  puts("获取游戏PID失败");
  exit(0);
  return -1;
}

long int get_module_base(int pid, const char *module_name)
{
  FILE *fp;
  long addr = 0;
  char *pch;
  char filename[64];
  char line[1024];
  snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
  fp = fopen(filename, "r");
  if (fp != NULL)
  {
    while (fgets(line, sizeof(line), fp))
    {
      if (strstr(line, module_name))
      {
        pch = strtok(line, "-");
        addr = strtoul(pch, NULL, 16);
        if (addr == 0x8000)
        addr = 0;
        break;
      }
    }
    fclose(fp);
  }
  return addr;
}


float getFloat(long int addr)
{
  float var = 0;
  vm_readv(addr,&var,4);
  return var;
}

int getDword(long int addr)
{
  int var = 0;
  vm_readv(addr,&var,4);
  return var;
}

long int getPtr64(long int addr)
{
  long int var = 0;
  vm_readv(addr,&var,8);
  return var;
}

void WriteFloat(long int addr, float value)
{
  vm_writev(addr,&value,4);
}

struct D3DXVECTOR4
{
  float X;
  float Y;
  float Z;
  float W;
};

struct D3DVector
{
  float X;
  float Y;
  float Z;
};

struct FTransform
{
  D3DXVECTOR4 Rotation;
  D3DVector Translation;
  D3DVector Scale3D;
};

struct D3DXMATRIX
{
  float _11;
  float _12;
  float _13;
  float _14;
  float _21;
  float _22;
  float _23;
  float _24;
  float _31;
  float _32;
  float _33;
  float _34;
  float _41;
  float _42;
  float _43;
  float _44;
};

struct D2DVector
{
  float X;
  float Y;
};

FTransform ReadFTransform(long int location);
D3DXMATRIX ToMatrixWithScale(D3DXVECTOR4 Rotation, D3DVector Translation, D3DVector Scale3D);
D3DVector D3dMatrixMultiply(D3DXMATRIX bonematrix, D3DXMATRIX actormatrix);
D3DVector getBoneXYZ(long int humanAddr, long int boneAddr, int Part);

// 获取骨骼3d坐标
D3DVector D3dMatrixMultiply(D3DXMATRIX bonematrix, D3DXMATRIX actormatrix)
{
  D3DVector result;
  result.X =
  bonematrix._41 * actormatrix._11 + bonematrix._42 * actormatrix._21 +
  bonematrix._43 * actormatrix._31 + bonematrix._44 * actormatrix._41;
  result.Y =
  bonematrix._41 * actormatrix._12 + bonematrix._42 * actormatrix._22 +
  bonematrix._43 * actormatrix._32 + bonematrix._44 * actormatrix._42;
  result.Z =
  bonematrix._41 * actormatrix._13 + bonematrix._42 * actormatrix._23 +
  bonematrix._43 * actormatrix._33 + bonematrix._44 * actormatrix._43;
  return result;
}

D3DXMATRIX ToMatrixWithScale(D3DXVECTOR4 Rotation, D3DVector Translation, D3DVector Scale3D)
{
  D3DXMATRIX M;
  float X2, Y2, Z2, xX2, Yy2, Zz2, Zy2, Wx2, Xy2, Wz2, Zx2, Wy2;
  M._41 = Translation.X;
  M._42 = Translation.Y;
  M._43 = Translation.Z;
  X2 = Rotation.X + Rotation.X;
  Y2 = Rotation.Y + Rotation.Y;
  Z2 = Rotation.Z + Rotation.Z;
  xX2 = Rotation.X * X2;
  Yy2 = Rotation.Y * Y2;
  Zz2 = Rotation.Z * Z2;
  M._11 = (1 - (Yy2 + Zz2)) * Scale3D.X;
  M._22 = (1 - (xX2 + Zz2)) * Scale3D.Y;
  M._33 = (1 - (xX2 + Yy2)) * Scale3D.Z;
  Zy2 = Rotation.Y * Z2;
  Wx2 = Rotation.W * X2;
  M._32 = (Zy2 - Wx2) * Scale3D.Z;
  M._23 = (Zy2 + Wx2) * Scale3D.Y;
  Xy2 = Rotation.X * Y2;
  Wz2 = Rotation.W * Z2;
  M._21 = (Xy2 - Wz2) * Scale3D.Y;
  M._12 = (Xy2 + Wz2) * Scale3D.X;
  Zx2 = Rotation.X * Z2;
  Wy2 = Rotation.W * Y2;
  M._31 = (Zx2 + Wy2) * Scale3D.Z;
  M._13 = (Zx2 - Wy2) * Scale3D.X;
  M._14 = 0;
  M._24 = 0;
  M._34 = 0;
  M._44 = 1;
  return M;
}

D3DVector getBoneXYZ(long int humanAddr, long int boneAddr, int Part)
{
  // 获取Bone数据
  FTransform Bone = ReadFTransform(boneAddr + Part * 48);
  // 获取Actor数据
  FTransform Actor = ReadFTransform(humanAddr);
  D3DXMATRIX Bone_Matrix = ToMatrixWithScale(Bone.Rotation, Bone.Translation, Bone.Scale3D);
  D3DXMATRIX Component_ToWorld_Matrix =
  ToMatrixWithScale(Actor.Rotation, Actor.Translation, Actor.Scale3D);
  D3DVector result = D3dMatrixMultiply(Bone_Matrix, Component_ToWorld_Matrix);
  return result;
}

FTransform ReadFTransform(long int address)
{
  FTransform Result;
  Result.Rotation.X = getFloat(address); // Rotation_X
  Result.Rotation.Y = getFloat(address + 4); // Rotation_y
  Result.Rotation.Z = getFloat(address + 8); // Rotation_z
  Result.Rotation.W = getFloat(address + 12); // Rotation_w
  Result.Translation.X = getFloat(address + 16); // /Translation_X
  Result.Translation.Y = getFloat(address + 20); // Translation_y
  Result.Translation.Z = getFloat(address + 24); // Translation_z
  Result.Scale3D.X = getFloat(address + 32);; // Scale_X
  Result.Scale3D.Y = getFloat(address + 36);; // Scale_y
  Result.Scale3D.Z = getFloat(address + 40);; // Scale_z
  return Result;
}

void getAimDistancelocation(float zx, float zy, float zz, float dx, float dy, float dz, float *arr)
{
  float x = zx - dx;
  float y = zy - dy;
  float z = zz - dz;
  float Distance = (float)sqrt(x * x + y * y);
  arr[0] = (float)atan2(y, x) * RadianSystem;
  arr[0] = arr[0] < 0 ? arr[0] + 360.0 : arr[0];
  arr[1] = (float)atan2(z, Distance) * RadianSystem;
}