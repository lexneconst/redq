#ifndef GCORE_H
#define GCORE_H

#include <itypedef.h>


typedef struct _thread_core thread_core;
typedef struct _thread_ex   thread_ex;

typedef struct _dat_node    dat_node;

#define CFREE(x) if(x!=NULL) cfree(x),x=NULL;
#define CLEAN(x) CFREE(x)

struct _thread_ex
{
	uint8_t              status;
	uint32_t             session;   
	
	struct _icore_read  *stream;
	struct _dat_node    *message;
	
	uint32_t             swap;
	
	struct _thread_ex   *next;	
}PACKED;

struct _thread_node
{
    uint32_t           coreid;
	uint32_t           number;	
}PACKED;

struct _thread_core  
{
	uint32_t           threadid;
	
	uint32_t           baseid;
	uint32_t           number;
	uint32_t           status;
	uint32_t           count;
	
	uint32_t           thread_count;
	//struct _icore_read  *stream;
	
	icore_class          *core[MAX_TABLE];
	
	struct _thread_node *node[MAX_THREAD];
	uint32_t             max_core[MAX_THREAD];
	struct _thread_ex   *base[MAX_THREAD];
	
	pthread_t            uid[MAX_THREAD];
}PACKED;



ICORE_DLL
icuint32   vsa_thread_init(icore_class *ptr, int argc, char **argv);
ICORE_DLL
icuint32   vsa_thread_core(icore_class *, struct _icore_read *stream);
ICORE_DLL
icuint32   vsa_thread_fini(icore_class *ptr);

#define VSS_MAX     16
#define EXTD_C(x)   (void *)x

#define VSS_COMPELTE EXTD_C(0x1)
#define VSS_SUCCESS  EXTD_C(0xff)
#define VSS_FAILED   EXTD_C(0x00)

typedef struct _vss_node vss_node;
typedef vss_node *       vss_nodew;

typedef struct _vss_task vss_task;
typedef vss_task *       vss_taskw;

struct _vss_node  
{
	struct _vss_node  *first;
	struct _vss_node  *next;
	
	uint8_t    status;   // enable or disable
	uint32_t   seid;    // session id;
	uint32_t   usrid;   // compiler id
	
	uint32_t   number;
	
	int8_t    *buffer;
	uint32_t   length;
	uint32_t   len_old;
}PACKED;

struct _vss_task
{
	uint32_t     xpos;
	uint32_t     ypos;
	
	uint32_t     blocks;
	uint32_t     count;
	
	uint32_t     curr_max;
	uint32_t     find_max;
	uint32_t     mark_max;
	
	struct _vss_node  *task;
}PACKED;

RUNC_DLL void * RUNAPI _vss_init(uint32_t addr);
RUNC_DLL void * RUNAPI _vss_fini(uint32_t addr);
RUNC_DLL void * RUNAPI _vss_count(uint32_t *count);
RUNC_DLL void * RUNAPI _vss_update (vss_node *ptr);
RUNC_DLL void * RUNAPI _vss_read (vss_node **ptr, uint32_t seid);
RUNC_DLL void * RUNAPI _vss_free (vss_node *ptr); // free chiled
RUNC_DLL uint32_t  RUNAPI _vss_max(void);

#ifndef MAX_PATH
#define MAX_PATH  256
#endif

/* http://en.wikipedia.org/wiki/Attrib */
#define _A_NORMAL   0x00    /* Normal file.     */
#define _A_RDONLY   0x01    /* Read only file.  */
#define _A_HIDDEN   0x02    /* Hidden file.     */
#define _A_SYSTEM   0x04    /* System file.     */
#define _A_SUBDIR   0x10    /* Subdirectory.    */
#define _A_ARCH     0x20    /* Archive file.    */

struct _cfinddata_t {
    unsigned attrib;
    time_t time_create;
    time_t time_access;
    time_t time_write;
    off_t size;
    char name[260];
};

RUNC_DLL intptr_t _cfindfirst(const char* filespec, struct _cfinddata_t* fileinfo);
RUNC_DLL int _cfindnext(intptr_t handle, struct _cfinddata_t* fileinfo);
RUNC_DLL int _cfindclose(intptr_t handle);

RUNC_DLL uint32_t _vsc_getfilecount(const char *dir);
RUNC_DLL intptr_t _vsc_firstfile(const char *dir, char *szfile);
RUNC_DLL uint32_t _vsc_nextfile(intptr_t handle, char *szfile);
RUNC_DLL uint32_t _vsc_closefile(intptr_t handle);


struct _dat_node 
{
    char    filedev[64];
    char    signdev[16];
	char    message[256];
	
	time_t  ctime;
		
	uint32_t  length;
	uint32_t  len_old;
	
	uint32_t  wrap;
	
	uint32_t  percent;
	uint32_t  current;
	uint32_t  maximum;
	
	uint32_t  swap;
}PACKED ;

RUNC_DLL char *sign_digest(unsigned char *p,unsigned int len);
RUNC_DLL void *dat_setfile(const char *dir, icore_read *sdata, const char *message);
RUNC_DLL void *dat_getfile(const char *file, icore_read *sdata, struct _dat_node *msg);
RUNC_DLL void *dat_sethdr(const char *file, struct _dat_node *head);
RUNC_DLL void *dat_remove(const char *file);

#endif


