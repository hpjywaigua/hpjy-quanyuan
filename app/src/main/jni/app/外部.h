//by TKApple
//本读写通过打开mem，实现外部可修改xa内存地址，用syscall系统调用修复了open以及pread64及write64等函数会被游戏检测到并闪退的问题
//使用此读写，可修改xa内存地址且游戏不会检测到，不会闪退，不会10分钟！

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <string.h>
#include <math.h>

//公共变量区域
int ipid,handle;

//下面是函数区域

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
	exit(1);
	return -1;
}

int getHandle(int pid)
{
	char lj[64];
	sprintf(lj, "/proc/%d/mem", pid);
	handle = syscall(__NR_openat,AT_FDCWD,lj,O_RDWR,0777);					
	if (handle != 0)
	{
	printf("Handle=%d",handle);
	return handle;
	}					
	puts("获取游戏Handle失败");
	exit(1);
	return -1;
}


// 获取模块
long getModuleBase(int PID,char*module_name)
{
	char*phgsr;
	char jjjj_N[64];
	long startaddr = 0;
	char path[256],line[1024];
	bool bssOF = false,LastIsSo = false;
	strcpy(jjjj_N,module_name);
	phgsr = strtok(jjjj_N,":");
	module_name = phgsr;
	phgsr = strtok(NULL,":");
	if(phgsr)
	{
		if(strcmp(phgsr,"bss")==0)
		{
			bssOF = true;
		}
	}
	sprintf(path,"/proc/%d/maps",PID);
	FILE*p = fopen(path,"r");
	if(p)
	{
		while(fgets(line,sizeof(line),p))
		{
			if(LastIsSo)
			{
				if(strstr(line,"[anon:.bss]")!=NULL)
				{
					sscanf(line,"%lx-%*lx",&startaddr);
					break;
				}
				else
				{
					LastIsSo = false;
				}
			}
			if(strstr(line,module_name)!=NULL)
			{
				if(!bssOF)
				{
					sscanf(line,"%lx-%*lx",&startaddr);
					break;
				}
				else
				{
					LastIsSo = true;
				}
			}
		}
		fclose(p);
	}
	return startaddr;
}


float getFloat(long addr)
{
	float var = 0;
	syscall(SYS_pread64,handle,&var,4,addr);
	return var;
}

 int getDword(long addr)
{
	int var = 0;
	syscall(SYS_pread64,handle,&var,4,addr);
	return var;
}

long getZZ(long addr)
{
	long var = 0;
	syscall(SYS_pread64,handle,&var,8,addr);
	return var;
}

int WriteAddress_DWORD(long addr, int value)
{
	syscall(SYS_pwrite64,handle,&value,4,addr);
	return 0;
}

// F类型
float WriteAddress_FLOAT(long addr, float value)
{
	syscall(SYS_pwrite64,handle,&value,4,addr);
	return 0;
}


/*自定义读，写字节类型*/
template < typename T > T Read(uintptr_t addr)
	{
		T buf;
		syscall(SYS_pread64,handle,&buf, sizeof(buf),addr);
			return buf;
	}
	




