//晚离Make 裸奔读写
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
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
#define PI 3.141592653589793238

#if defined(__arm__)
int process_vm_readv_syscallll = 376;
int process_vm_writev_syscallll = 377;
#elif defined(__aarch64__)
int process_vm_readv_syscallll = 270;
int process_vm_writev_syscallll = 271;
#elif defined(__i386__)
int process_vm_readv_syscallll = 347;
int process_vm_writev_syscallll = 348;
#else
int process_vm_readv_syscallll = 310;
int process_vm_writev_syscallll = 311;
#endif
#ifndef __UTF_H__
#define __UTF_H__
#define FALSE 0
#define TRUE 1
#define halfShift 10
#define UNI_SUR_HIGH_START (UTF32)0xD800
#define UNI_SUR_HIGH_END (UTF32)0xDBFF
#define UNI_SUR_LOW_START (UTF32)0xDC00
#define UNI_SUR_LOW_END (UTF32)0xDFFF
	// Some fundamental constants
#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD
#define UNI_MAX_BMP (UTF32)0x0000FFFF
#define UNI_MAX_UTF16 (UTF32)0x0010FFFF
#define UNI_MAX_UTF32 (UTF32)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 (UTF32)0x0010FFFF

typedef unsigned char boolean;
typedef unsigned int CharType;
typedef char UTF8;
typedef unsigned short UTF16;
typedef unsigned int UTF32;




int ipiddd,handleee,scwqqq;;
float matrixxx[50], pxxx, pyyy, angleee;

static const UTF32 halfMask = 0x3FFUL;
static const UTF32 halfBase = 0x0010000UL;
static const UTF8 firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

typedef enum
{
	strictConversion = 0,
	lenientConversion
}
ConversionFlags;
typedef enum
{
	conversionOK,				// conversion successful
	sourceExhausted,			// partial character in source,but hit end
	targetExhausted,			// insuff. room in target for conversion
	sourceIllegal,				// source sequence is illegal/malformed
	conversionFailed
}
ConversionResult;
#endif

int Open_Memoryyy()
{
	char lj[64];
	if(ipiddd == -1)
	{}
	sprintf(lj, "/proc/%d/mem", ipiddd);
	handleee = open(lj, O_RDWR);
	if (handleee == 0)
	{
		puts("获取mem失败!");
		exit(1);
	}

}


ssize_t process_vvv(pid_t __pid, const struct iovec* __local_iov, unsigned long __local_iov_count,
                  const struct iovec* __remote_iov, unsigned long __remote_iov_count, unsigned long __flags, bool iswrite) {
    return syscall((iswrite ? process_vm_writev_syscallll : process_vm_readv_syscallll), __pid, __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}

bool pvm(int ipiddd , void* address, void* buffer, size_t size, bool iswrite) {
    struct iovec local[1];
    struct iovec remote[1];

    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;

    if (ipiddd < 0) {
        return false;
    }

    ssize_t bytes = process_vvv(ipiddd, local, 1, remote, 1, 0, iswrite);
    return bytes == size;
}
bool vm_readv(long int address, void *buffer, size_t size) {
	return pvm(ipiddd, reinterpret_cast<void *>(address), buffer, size, false);
}

//write   入
bool vm_writev(long int address, void *buffer, size_t size) {
	return pvm(ipiddd, reinterpret_cast<void *>(address), buffer, size, true);
}


int remoteSwitch()
{
    FILE *fp = fopen("/sdcard/x", "r");
	FILE *fp1 = fopen("/sdcard/y", "r");
	if (fp == NULL || fp1 == NULL)	// 如果没有读取到分辨率文件,则设置以下分辨
	{
		pxxx = 1080.0;
		pyyy = 2340.0;
	}
	else
	{
		fscanf(fp, "%f", &pxxx);
		fscanf(fp1, "%f", &pyyy);
		if (pyyy > pxxx)
		{
			float t = pxxx;
			pxxx = pyyy;
			pyyy = t;
		}
		pyyy = pyyy / 2.0;
		pxxx = pxxx / 2.0;
		fclose(fp);
		fclose(fp1);
	}
}

typedef char PACKAGENAME;
int getPIDDD(const char *packageName)
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
	printf("获取游戏PID失败");
	exit(0);
	return -1;
}

long int get_module_baseee(int pid, const char *module_name)
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
bool isVirtualAddress(long int address) {

    int pageSize = getpagesize();
    unsigned long v_pageIndex = address / pageSize;            //虚地址页数
    unsigned long v_offset = v_pageIndex * sizeof(uint64_t);  //计算在/proc/pid/page_map文件中的偏移量
    unsigned long page_offset = address % pageSize;
    uint64_t item = 0;
 vm_readv(v_offset, &item, sizeof(uint64_t));
// pread(pagemap_fd, &item, sizeof(uint64_t), v_offset);
 if ((((uint64_t) 1 << 63) & item) != 0)
  {
   return false;
  } 
  else
  {
   return true;
  }
  return item;
   }   
float getFloattt(long int addr)
 {
  if (!isVirtualAddress(addr) || addr <= 0)
  {
   return 0;
  }
  else
  {
   float var[1] = { 0 };
   vm_readv(addr, var, 8);
   return var[0];
  }
 }


int getDworddd(long int addr)
 {
if (!isVirtualAddress(addr) || addr <= 0)
  {
   return 0;
  }
  else
  {
   long int var[1] = { 0 };
   vm_readv(addr, var, 8);
   return var[0];
  }
 }

long int getZZ_64(long int addr)
{
	long int var = 0;
	pread64(handleee, &var, 8, addr);
	return var;
}

long int getZZ_32(long int addr)
{
	long int var = 0;
	pread64(handleee, &var, 4, addr);
	return var;
}

struct Vector2AAA {
	float X;
	float Y;

	Vector2AAA() {
		this->X = 0;
		this->Y = 0;
	}

	Vector2AAA(float x, float y) {
		this->X = x;
		this->Y = y;
	}
};

struct Vector3AAA {
	float X;
	float Y;
	float Z;

	Vector3AAA() {
		this->X = 0;
		this->Y = 0;
		this->Z = 0;
	}

	Vector3AAA(float x, float y, float z) {
		this->X = x;
		this->Y = y;
		this->Z = z;
	}

};




struct FMatrixxx {
	float M[4][4];
};

struct Quattt {
	float X;
	float Y;
	float Z;
	float W;
};

struct FTransform111 {
	Quattt Rotation;
	Vector3AAA Translation;
	float chunk;
	Vector3AAA Scale3D;
};

struct MAPS {
	long int addr;
	long int taddr;
	int type;
	struct MAPS *next;
};

struct D3DXVECTOR444
{
	float X;
	float Y;
	float Z;
	float W;
};

struct D3DVectorrr
{
	float X;
	float Y;
	float Z;
};

struct FTransformmm
{
	D3DXVECTOR444 Rotation;
	D3DVectorrr Translation;
	D3DVectorrr Scale3D;
};

struct D3DXMATRIXXX
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

struct D2DVectorrr
{
	float X;
	float Y;
};

float get_3D_Distanceee(float Self_x, float Self_y, float Self_z, float Object_x, float Object_y,
					  float Object_z)
{
	float x, y, z;
	x = Self_x - Object_x;
	y = Self_y - Object_y;
	z = Self_z - Object_z;
	// 求平方根
	return (float)(sqrt(x * x + y * y + z * z));
}

FTransformmm ReadFTransformmm(long int location);
D3DXMATRIXXX ToMatrixWithScale(D3DXVECTOR444 Rotation, D3DVectorrr Translation, D3DVectorrr Scale3D);
D3DXMATRIXXX D3DXMatrixMultiply(D3DXMATRIXXX Pm1, D3DXMATRIXXX Pm2);
D2DVectorrr rotateCoorddd(float angleee, float objectRadar_x, float objectRadar_y);
D3DVectorrr D3dMatrixMultiply(D3DXMATRIXXX bonematrix, D3DXMATRIXXX actormatrix);
D3DVectorrr getBoneXYZZZ(long int humanAddr, long int boneAddr, int Part);
D2DVectorrr getBoneCoordinate(long int humanAddr, long int boneAddr, int Part);

// 获取骨骼2d
D2DVectorrr getBoneCoordinate(long int humanAddr, long int boneAddr, int Part)
{
	D2DVectorrr result;
	// 获取Bone数据
	FTransformmm Boneee = ReadFTransformmm(boneAddr + Part * 48);
	// 获取Actor数据
	FTransformmm Actorrr = ReadFTransformmm(humanAddr);
	D3DXMATRIXXX Bone_Matrix = ToMatrixWithScale(Boneee.Rotation, Boneee.Translation, Boneee.Scale3D);
	D3DXMATRIXXX Component_ToWorld_Matrix =
		ToMatrixWithScale(Actorrr.Rotation, Actorrr.Translation, Actorrr.Scale3D);
	D3DXMATRIXXX New_Matrix = D3DXMatrixMultiply(Bone_Matrix, Component_ToWorld_Matrix);
	float ViewW =
		matrixxx[3] * New_Matrix._41 + matrixxx[7] * New_Matrix._42 + matrixxx[11] * New_Matrix._43 +
		matrixxx[15];
	if (ViewW >= 0.01)
	{
		ViewW = 1 / ViewW;
		result.X =
			pxxx + (matrixxx[0] * New_Matrix._41 + matrixxx[4] * New_Matrix._42 +
				  matrixxx[8] * New_Matrix._43 + matrixxx[12]) * ViewW * pxxx;
		result.Y =
			pyyy - (matrixxx[1] * New_Matrix._41 + matrixxx[5] * New_Matrix._42 +
				  matrixxx[9] * New_Matrix._43 + matrixxx[13]) * ViewW * pyyy;
	}
	return result;
}

D3DVectorrr getBoneXYZZZ(long int humanAddr, long int boneAddr, int Part)
{
	// 获取Bone数据
	FTransformmm Boneee = ReadFTransformmm(boneAddr + Part * 48);
	// 获取Actor数据
	FTransformmm Actorrr = ReadFTransformmm(humanAddr);
	D3DXMATRIXXX Bone_Matrix = ToMatrixWithScale(Boneee.Rotation, Boneee.Translation, Boneee.Scale3D);
	D3DXMATRIXXX Component_ToWorld_Matrix =
		ToMatrixWithScale(Actorrr.Rotation, Actorrr.Translation, Actorrr.Scale3D);
	D3DVectorrr result = D3dMatrixMultiply(Bone_Matrix, Component_ToWorld_Matrix);
	return result;
}

FTransformmm ReadFTransformmm(long int address)
{
	FTransformmm Result;
	Result.Rotation.X = getFloattt(address);	// Rotation_X 
	Result.Rotation.Y = getFloattt(address + 4);	// Rotation_y
	Result.Rotation.Z = getFloattt(address + 8);	// Rotation_z
	Result.Rotation.W = getFloattt(address + 12);	// Rotation_w
	Result.Translation.X = getFloattt(address + 16);	// /Translation_X
	Result.Translation.Y = getFloattt(address + 20);	// Translation_y
	Result.Translation.Z = getFloattt(address + 24);	// Translation_z
	Result.Scale3D.X = getFloattt(address + 32);;	// Scale_X
	Result.Scale3D.Y = getFloattt(address + 36);;	// Scale_y
	Result.Scale3D.Z = getFloattt(address + 40);;	// Scale_z
	return Result;
}
// 获取骨骼3d坐标
D3DVectorrr D3dMatrixMultiply(D3DXMATRIXXX bonematrix, D3DXMATRIXXX actormatrix)
{
	D3DVectorrr result;
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
// 计算旋转坐标
D2DVectorrr rotateCoorddd(float angleee, float objectRadar_x, float objectRadar_y)
{
	D2DVectorrr radarCoordinate;
	float s = sin(angleee * PI / 180);
	float c = cos(angleee * PI / 180);
	radarCoordinate.X = objectRadar_x * c + objectRadar_y * s;
	radarCoordinate.Y = -objectRadar_x * s + objectRadar_y * c;
	return radarCoordinate;
}

D3DXMATRIXXX ToMatrixWithScale(D3DXVECTOR444 Rotation, D3DVectorrr Translation, D3DVectorrr Scale3D)
{
	D3DXMATRIXXX M;
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

D3DXMATRIXXX D3DXMatrixMultiply(D3DXMATRIXXX Pm1, D3DXMATRIXXX Pm2)
{
	D3DXMATRIXXX Pout;
	Pout._11 = Pm1._11 * Pm2._11 + Pm1._12 * Pm2._21 + Pm1._13 * Pm2._31 + Pm1._14 * Pm2._41;
	Pout._12 = Pm1._11 * Pm2._12 + Pm1._12 * Pm2._22 + Pm1._13 * Pm2._32 + Pm1._14 * Pm2._42;
	Pout._13 = Pm1._11 * Pm2._13 + Pm1._12 * Pm2._23 + Pm1._13 * Pm2._33 + Pm1._14 * Pm2._43;
	Pout._14 = Pm1._11 * Pm2._14 + Pm1._12 * Pm2._24 + Pm1._13 * Pm2._34 + Pm1._14 * Pm2._44;
	Pout._21 = Pm1._21 * Pm2._11 + Pm1._22 * Pm2._21 + Pm1._23 * Pm2._31 + Pm1._24 * Pm2._41;
	Pout._22 = Pm1._21 * Pm2._12 + Pm1._22 * Pm2._22 + Pm1._23 * Pm2._32 + Pm1._24 * Pm2._42;
	Pout._23 = Pm1._21 * Pm2._13 + Pm1._22 * Pm2._23 + Pm1._23 * Pm2._33 + Pm1._24 * Pm2._43;
	Pout._24 = Pm1._21 * Pm2._14 + Pm1._22 * Pm2._24 + Pm1._23 * Pm2._34 + Pm1._24 * Pm2._44;
	Pout._31 = Pm1._31 * Pm2._11 + Pm1._32 * Pm2._21 + Pm1._33 * Pm2._31 + Pm1._34 * Pm2._41;
	Pout._32 = Pm1._31 * Pm2._12 + Pm1._32 * Pm2._22 + Pm1._33 * Pm2._32 + Pm1._34 * Pm2._42;
	Pout._33 = Pm1._31 * Pm2._13 + Pm1._32 * Pm2._23 + Pm1._33 * Pm2._33 + Pm1._34 * Pm2._43;
	Pout._34 = Pm1._31 * Pm2._14 + Pm1._32 * Pm2._24 + Pm1._33 * Pm2._34 + Pm1._34 * Pm2._44;
	Pout._41 = Pm1._41 * Pm2._11 + Pm1._42 * Pm2._21 + Pm1._43 * Pm2._31 + Pm1._44 * Pm2._41;
	Pout._42 = Pm1._41 * Pm2._12 + Pm1._42 * Pm2._22 + Pm1._43 * Pm2._32 + Pm1._44 * Pm2._42;
	Pout._43 = Pm1._41 * Pm2._13 + Pm1._42 * Pm2._23 + Pm1._43 * Pm2._33 + Pm1._44 * Pm2._43;
	Pout._44 = Pm1._41 * Pm2._14 + Pm1._42 * Pm2._24 + Pm1._43 * Pm2._34 + Pm1._44 * Pm2._44;
	return Pout;
}

typedef struct MAPS *PMAPS;

#define LEN sizeof(struct MAPS)


/* ------ 相关数据计算 ------ */
/*人物骨骼*/
FMatrixxx TransformToMatrix(FTransform111 transform) {
	FMatrixxx matrixxx;
	matrixxx.M[3][0] = transform.Translation.X;
	matrixxx.M[3][1] = transform.Translation.Y;
	matrixxx.M[3][2] = transform.Translation.Z;
	float x2 = transform.Rotation.X + transform.Rotation.X;
	float y2 = transform.Rotation.Y + transform.Rotation.Y;
	float z2 = transform.Rotation.Z + transform.Rotation.Z;
	float xx2 = transform.Rotation.X * x2;
	float yy2 = transform.Rotation.Y * y2;
	float zz2 = transform.Rotation.Z * z2;
	matrixxx.M[0][0] = (1 - (yy2 + zz2)) * transform.Scale3D.X;
	matrixxx.M[1][1] = (1 - (xx2 + zz2)) * transform.Scale3D.Y;
	matrixxx.M[2][2] = (1 - (xx2 + yy2)) * transform.Scale3D.Z;
	float yz2 = transform.Rotation.Y * z2;
	float wx2 = transform.Rotation.W * x2;
	matrixxx.M[2][1] = (yz2 - wx2) * transform.Scale3D.Z;
	matrixxx.M[1][2] = (yz2 + wx2) * transform.Scale3D.Y;
	float xy2 = transform.Rotation.X * y2;
	float wz2 = transform.Rotation.W * z2;
	matrixxx.M[1][0] = (xy2 - wz2) * transform.Scale3D.Y;
	matrixxx.M[0][1] = (xy2 + wz2) * transform.Scale3D.X;
	float xz2 = transform.Rotation.X * z2;
	float wy2 = transform.Rotation.W * y2;
	matrixxx.M[2][0] = (xz2 + wy2) * transform.Scale3D.Z;
	matrixxx.M[0][2] = (xz2 - wy2) * transform.Scale3D.X;
	matrixxx.M[0][3] = 0;
	matrixxx.M[1][3] = 0;
	matrixxx.M[2][3] = 0;
	matrixxx.M[3][3] = 1;
	return matrixxx;
}

/* 计算3D坐标距离 */
float getDistance(float zx, float zy, float zz, float dx, float dy, float dz) {
	float x,y,z;
	x = zx - dx;
	y = zy - dy;
	z = zz - dz;
	return (float)(sqrt(x * x + y * y + z * z));
}

Vector2AAA WorldToScreen(Vector3AAA obj , float matrixxx[16]) {
	float ViewW = matrixxx[3] * obj.X + matrixxx[7] * obj.Y + matrixxx[11] * obj.Z + matrixxx[15];
	float x = pxxx + (matrixxx[0] * obj.X + matrixxx[4] * obj.Y + matrixxx[8] * obj.Z + matrixxx[12]) / ViewW * pxxx;
	float y = pyyy - (matrixxx[1] * obj.X + matrixxx[5] * obj.Y + matrixxx[9] * obj.Z + matrixxx[13]) / ViewW * pyyy;
	return Vector2AAA(x, y);
}

Vector2AAA WorldToScreen2(Vector3AAA obj , float matrixxx[16]) {
	float ViewW = matrixxx[3] * obj.X + matrixxx[7] * obj.Y + matrixxx[11] * obj.Z + matrixxx[15];
	if (ViewW < 0.01) {
		return Vector2AAA(3100, 3100);
	}
	float x = pxxx + (matrixxx[0] * obj.X + matrixxx[4] * obj.Y + matrixxx[8] * obj.Z + matrixxx[12]) / ViewW * pxxx;
	float y = pyyy - (matrixxx[1] * obj.X + matrixxx[5] * obj.Y + matrixxx[9] * obj.Z + matrixxx[13]) / ViewW * pyyy;
	return Vector2AAA(x,y);
}

Vector3AAA MarixToVector(FMatrixxx matrixxx) {
	return Vector3AAA(matrixxx.M[3][0], matrixxx.M[3][1], matrixxx.M[3][2]);
}

FMatrixxx MatrixMulti(FMatrixxx m1, FMatrixxx m2) {
	FMatrixxx matrixxx = FMatrixxx();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				matrixxx.M[i][j] += m1.M[i][k] * m2.M[k][j];
			}
		}
	}
	return matrixxx;
}

FTransform111 getBone(long int addr) {
	FTransform111 transform;
	vm_readv(addr, &transform, 4 * 11);
	return transform;
}
float getQuasicentered(float pxxx, float pyyy, float r_x, float r_y)
{
	float centerDist = sqrt(pow(r_x - pxxx, 2) + pow(r_y - pyyy, 2));//自瞄距离
	
	return centerDist;
}

float MatrixValue[16] = { 0 };
long int zmAddr;
long int zmAddrx, zmAddry;
int khhh, kjjj;
float Myz, Myx, Myy, cx, cy, cz;
double pfg;
double XGX, XGY;
double vax = 180 / PI, vay = 180 / PI;
float JD, XGX1, XGY1, JD1;
float ab;
long int aimatRecord = 0;
float DropMMM;
D2DVectorrr getPointingAngle(long int oneself, long int ObjectAddrPointer, float object_x,
						   float object_y, float object_z, float Self_x, float Self_y,
						   float Self_z)
{
	D2DVectorrr PointingAngle;
	
int	maam = getPIDDD("com.tencent.tmgp.pubgmhd");
	char snslalal[] = "libUE4.so";	// libUE4模块
	long int libbwnemem = get_module_baseee(maam, snslalal);	// 获取libUE4入口模块地址
	long int oneselfmsmsms = getZZ_64(getZZ_64(getZZ_64(getZZ_64(getZZ_64(libbwnemem + 0xBC3F348) + 0x98) + 0x78) + 0x30) + 0x2690);	// 自身对象	
	   float bulletVelocity = getDworddd(oneselfmsmsms + 0x16FC);
	   
	   
	   float ypjl = get_3D_Distanceee(object_x, object_y,object_z, Self_x, Self_y, Self_z);//世界距离
	   float xtime = ypjl / bulletVelocity; 
	   float ytime = ypjl / bulletVelocity; //子弹到达时间
	   DropMMM = 500.0f * ytime * ytime;
	 
	float zbcx = object_x - Self_x;
	float zbcy = object_y - Self_y;
	float zbcz = object_z - Self_z;
	PointingAngle.X = atan2(zbcy, zbcx) * 180 / PI;
	PointingAngle.Y = atan2(zbcz, sqrt(zbcx * zbcx + zbcy * zbcy)+DropMMM) * 180 / PI;
	return PointingAngle;
}



float WriteFloat(long int addr, float value)
{
 vm_writev(addr, &value, 4);
  
}

int getProcessIDDD(const char *packageName)
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
	return -1;
}//过缺页
int mem_addr_virtophyyy(unsigned long vaddr)
{
	int fh=1;
    int pageSize = getpagesize();

    //虚拟地址相对于0X0经过的页面数
    unsigned long v_pageIndex = vaddr / pageSize;
    //在pagemap文件中的偏移量
    unsigned long pfn_item_offset = v_pageIndex * sizeof(uint64_t);

    //页内偏移量
    unsigned long page_offset = vaddr % pageSize;   
    uint64_t item = 0;
    char filename[32];
	snprintf(filename, sizeof(filename), "/proc/%d/pagemap",getProcessIDDD);  
	 int fd = open(filename,O_RDONLY);
    if(fd < 0)
    {
      //  printf("open failed\n");
        return 0;
    }

    if(lseek(fd,pfn_item_offset,SEEK_SET) < 0)
    {
        printf("lseek failed\n");
        return 0;
    }

     if(read(fd, &item, sizeof(uint64_t)) != sizeof(uint64_t))
     {
         printf("read failed\n");
         return 0;
     }

     if (0==(item & (((uint64_t)1)<<63)))
     {
     	fh=0;
         printf("page is not present");
         
     }
     close(fd);
     return fh;
}
