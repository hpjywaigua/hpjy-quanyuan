//本读写通过打开mem，实现外部可修改xa内存地址，用syscall系统调用修复了open以及pread64及write64等函数会被游戏检测到并闪退的问题，且8192也不会变0，也就是说使用此读写，可修改xa内存地址且游戏不会检测到，不会闪退，不会10分钟！
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

#if defined(__arm__)
int process_vm_readv_syscall = 376;
int process_vm_write64_syscall = 377;
#elif defined(__aarch64__)
//int process_vm_readv_syscall = 270;
int process_vm_write64_syscall = 271;
#elif defined(__i386__)
int process_vm_readv_syscall = 347;
int process_vm_write64_syscall = 348;
#else
int process_vm_readv_syscall = 310;
int process_vm_write64_syscall = 311;
#endif

//坐标计算宏
#define MaxPlayerCount 30
#define MaxVehicleCount 20
#define MaxItemsCount 20
#define MaxGrenadeCount 20
#define MaxAirBoxCount 20

	// 文字转码
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

//公共变量区域
int 进程号,处理内存,scwq;;



static const UTF32 halfMask = 0x3FFUL;
static const UTF32 halfBase = 0x0010000UL;
static const UTF8 firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
static const UTF32 offsetsFromUTF8[6] =
	{ 0x00000000UL, 0x00003080UL, 0x000E2080UL, 0x03C82080UL, 0xFA082080UL, 0x82082080UL };
static const char trailingBytesForUTF8[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5
};

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


/*下面是函数区域*/

//Open_Memory Function
int 内存()
{
	char lj[64];
	if(进程号 == -1)
	{exit(1);}
	sprintf(lj, "/proc/%d/mem", 进程号);
	处理内存 = syscall(__NR_openat,AT_FDCWD,lj,O_RDWR,0777);
	printf("%d",处理内存);
		if (处理内存 == 0)
	{
		puts("获取mem失败!");
		exit(1);
	}

}
typedef char PACKAGENAME;
int 进程(const char *packageName)
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

long int 模块(int pid, const char *module_name)
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

float 浮点(long int addr)
{
	float var = 0;
	syscall(SYS_pread64,处理内存,&var,8,addr);
	return var;
}

 int 整数(long int addr)
{
	int var = 0;
	syscall(SYS_pread64,处理内存,&var,8,addr);
	return var;
}

long int 指针1(long int addr)
{
	long int var = 0;
	syscall(SYS_pread64,处理内存,&var,8,addr);
	return var;
}

long int 指针2(long int addr)
{
	long int var = 0;
	syscall(SYS_pread64,处理内存,&var,4,addr);
	return var;
}

int 写入整数(long int addr, int value)
{
	syscall(SYS_pwrite64,处理内存,&value,4,addr);
	return 0;
}

// F类型
float 写入浮点(long int addr, float value)
{
	syscall(SYS_pwrite64,处理内存,&value,4,addr);
	return 0;
}

/*自定义读，写字节类型*/
template < typename T > T Read(uintptr_t addr)
	{
		T buf;
		syscall(SYS_pread64,处理内存,&buf, sizeof(buf),addr);
			return buf;
	}
	






