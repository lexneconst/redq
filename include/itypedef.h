
#ifndef _ITYPEDEF_H
#define _ITYPEDEF_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef RMSV_C
#define int8_t     char
#define int16_t    short
#define int32_t    int

#define uint8_t    unsigned char
#define uint16_t   unsigned short
#define uint32_t   unsigned int
#endif

#define ENABLE   2
#define DISABLE  1

#ifndef MAX_THREAD    
#define MAX_THREAD    320
#endif

#ifndef MAX_CORE
#define MAX_CORE      20000
#endif

#ifndef MAX_TIMEOUT
#define MAX_TIMEOUT   10
#endif

#ifndef CMAX_PATH
#define CMAX_PATH 256
#endif

#ifndef MAX_TABLE
#define MAX_TABLE    99
#endif

#ifndef MAX_TABLEA
#define MAX_TABLEA   10
#endif

#ifdef WIN32_H
#include <windows.h>
#include <mmsystem.h>

#ifdef BUILDING_DLL
#define ICORE_DLL __declspec(dllexport)
#else
#define ICORE_DLL __declspec(dllimport)
#endif
#else
#define ICORE_DLL  extern
#endif

#ifdef WIN32_H
#ifdef RUNTIME_DLL
#define RUNC_DLL __declspec(dllexport)
#else
#define RUNC_DLL __declspec(dllimport)
#endif

#ifndef RUNAPI
#define RUNAPI  __cdecl
#endif

#else
#define RUNAPI
#define RUNC_DLL  extern
#endif



#ifndef PACKED
#define PACKED __attribute__((packed))
#endif



struct _dat_node;

#endif

