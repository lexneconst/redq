#ifndef _ICORE_H_
#define _ICORE_H_

#include <itypedef.h>




#define icuint8    unsigned char
#define icuint16   unsigned short
#define icuint32   unsigned int   

typedef unsigned char *POINTER;
/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;
/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;

typedef unsigned long long u_int64_t;
typedef unsigned long u_int32_t;
typedef unsigned int  u_int16_t;
typedef unsigned char u_int8_t;

typedef unsigned char u_char;
typedef unsigned int  u_int;
typedef unsigned long u_long;


#define icdata     void *

#define c_int8_t   char 

#define ICORENAME_STR    "ICORE"
#define ICOREVERS_STR    "1.10"

#define ICORESERIAL_D    0x0f0d03a0

#define ICORE_SUCCESS   1
#define ICORE_ACTIVE    2
#define ICORE_ERROR     10
#define ICORE_NULL      0

typedef enum _icore_status icore_status;

enum _icore_status{
	ICORENULL=0,
	ICOREINIT=1,
	ICORERUN=2,
	ICOREFINI=3,
	ICOREERR=4,
	ICOREACT=5,
	ICORELOAD=6,
	ICOREFAILED=7,
	ICOREPROCESS=8,
	ICORECOMPLETE=9,
	ICORESUCCESS=10,
	ICOREENABLE=11,
	ICOREDISABLE=12,
};

enum _icore_flags
{
    CREAD,
	CWRITE,
	CMESSAGE
};

typedef struct _icore_class      icore_class;
typedef struct _icore_public     icore_public;
typedef struct _icore_read       icore_read;
typedef struct _icore_statistics icore_statistics;
typedef struct icore_class *     icore_classw;
typedef struct icore_public*     icore_publicw;
typedef struct icore_read  *     icore_readw;
typedef struct icore_statistics* icore_statisticsw;

struct _icore_statistics
{
	struct n {
	    uint32_t   c00;
		uint32_t   c01;
		uint32_t   c02;
		uint32_t   c03;
		uint32_t   c04;
		uint32_t   c05;
		uint32_t   c06;
		uint32_t   c07;
		uint32_t   c08;
		uint32_t   c09;
		uint32_t   c10;
		uint32_t   c11;
		uint32_t   c12;
		uint32_t   c13;
		uint32_t   c14;
		uint32_t   c15;
		uint32_t   c16;
		uint32_t   c17;
		uint32_t   c18;
		uint32_t   c19;
		uint32_t   c20;
		uint32_t   c21;
		uint32_t   c22;
		uint32_t   c23;
		uint32_t   c24;
		uint32_t   c25;
		uint32_t   c26;
		uint32_t   c27;
		uint32_t   c28;
		uint32_t   c29;
		uint32_t   c30;
		uint32_t   c31;
		uint32_t   c32;
		uint32_t   c33;
		uint32_t   c34;
		uint32_t   c35;
		uint32_t   c36;
		uint32_t   c37;
		uint32_t   c38;
		uint32_t   c39;
		uint32_t   c40;
		uint32_t   c41;
		uint32_t   c42;
		uint32_t   c43;
		uint32_t   c44;
		uint32_t   c45;
		uint32_t   c46;
		uint32_t   c47;
		uint32_t   c48;
		uint32_t   c49;
		uint32_t   c50;
		uint32_t   c51;
		uint32_t   c52;
		uint32_t   c53;
		uint32_t   c54;
		uint32_t   c55;
		uint32_t   c56;
		uint32_t   c57;
		uint32_t   c58;
		uint32_t   c59;
		uint32_t   c60;
		uint32_t   c61;
		uint32_t   c62;
		uint32_t   c63;
		uint32_t   c64;
		uint32_t   c65;
		uint32_t   c66;
		uint32_t   c67;
		uint32_t   c68;
		uint32_t   c69;
		uint32_t   c70;
		uint32_t   c71;
		uint32_t   c72;
		uint32_t   c73;
		uint32_t   c74;
		uint32_t   c75;
		uint32_t   c76;
		uint32_t   c77;
		uint32_t   c78;
		uint32_t   c79;
		uint32_t   c80;
		uint32_t   c81;
		uint32_t   c82;
		uint32_t   c83;
		uint32_t   c84;
		uint32_t   c85;
		uint32_t   c86;
		uint32_t   c87;
		uint32_t   c88;
		uint32_t   c89;
		uint32_t   c90;
		uint32_t   c91;
		uint32_t   c92;
		uint32_t   c93;
		uint32_t   c94;
		uint32_t   c95;
		uint32_t   c96;
		uint32_t   c97;
		uint32_t   c98;
		uint32_t   c99;
			
	}cc;
}PACKED;

struct _icore_public
{
	struct c {
	    uint8_t   c00;
		uint8_t   c01;
		uint8_t   c02;
		uint8_t   c03;
		uint8_t   c04;
		uint8_t   c05;
		uint8_t   c06;
		uint8_t   c07;
		uint8_t   c08;
		uint8_t   c09;
		uint8_t   c10;
		uint8_t   c11;
		uint8_t   c12;
		uint8_t   c13;
		uint8_t   c14;
		uint8_t   c15;
		uint8_t   c16;
		uint8_t   c17;
		uint8_t   c18;
		uint8_t   c19;
		uint8_t   c20;
		uint8_t   c21;
		uint8_t   c22;
		uint8_t   c23;
		uint8_t   c24;
		uint8_t   c25;
		uint8_t   c26;
		uint8_t   c27;
		uint8_t   c28;
		uint8_t   c29;
		uint8_t   c30;
		uint8_t   c31;
		uint8_t   c32;
		uint8_t   c33;
		uint8_t   c34;
		uint8_t   c35;
		uint8_t   c36;
		uint8_t   c37;
		uint8_t   c38;
		uint8_t   c39;
		uint8_t   c40;
		uint8_t   c41;
		uint8_t   c42;
		uint8_t   c43;
		uint8_t   c44;
		uint8_t   c45;
		uint8_t   c46;
		uint8_t   c47;
		uint8_t   c48;
		uint8_t   c49;
		uint8_t   c50;
		uint8_t   c51;
		uint8_t   c52;
		uint8_t   c53;
		uint8_t   c54;
		uint8_t   c55;
		uint8_t   c56;
		uint8_t   c57;
		uint8_t   c58;
		uint8_t   c59;
		uint8_t   c60;
		uint8_t   c61;
		uint8_t   c62;
		uint8_t   c63;
		uint8_t   c64;
		uint8_t   c65;
		uint8_t   c66;
		uint8_t   c67;
		uint8_t   c68;
		uint8_t   c69;
		uint8_t   c70;
		uint8_t   c71;
		uint8_t   c72;
		uint8_t   c73;
		uint8_t   c74;
		uint8_t   c75;
		uint8_t   c76;
		uint8_t   c77;
		uint8_t   c78;
		uint8_t   c79;
		uint8_t   c80;
		uint8_t   c81;
		uint8_t   c82;
		uint8_t   c83;
		uint8_t   c84;
		uint8_t   c85;
		uint8_t   c86;
		uint8_t   c87;
		uint8_t   c88;
		uint8_t   c89;
		uint8_t   c90;
		uint8_t   c91;
		uint8_t   c92;
		uint8_t   c93;
		uint8_t   c94;
		uint8_t   c95;
		uint8_t   c96;
		uint8_t   c97;
		uint8_t   c98;
		uint8_t   c99;
			
	}cc;
	
	uint8_t    d[CMAX_PATH];	
	
	struct a {
	
    	uint8_t  c_hour;
    	uint8_t  c_min;
		uint8_t  c_sec;
    	uint8_t  c_day;
    	uint8_t  c_mon;
    	uint16_t c_year;
        
		uint32_t c_lasttime; // last time current
		uint32_t c_result;  // search time 	
	}tc;
	
	struct _dat_node *message;
}PACKED;

struct _icore_read
{
     c_int8_t     *buffer;
	 uint32_t      length;	
}PACKED;


struct _icore_class
{
     char         *name;
	 char         *version;
	 icuint32      serial;	
	 
	 icuint32      session;
	 
	 icore_status  status;
	 uint32_t      code;
	 
	 icuint32      (*c_init)(icore_class *data);
	 icuint32      (*c_run)(icore_class *data);
	 icuint32      (*c_fini)(icore_class *data);
	 
	 icuint32      (*c_active)(icore_class *data);
	 icuint32      (*c_core)(icore_class *data);
	 icuint32      (*c_public)(icore_class *cls, icore_class *dst, icore_class *src);
	 
	 icuint32      (*c_microp)(icore_class *data, icore_readw       stream);

	 icuint32      (*c_data)(icore_class *, icore_readw stream);
	 icuint32      (*c_control)(icore_class *data, enum _icore_flags flags, uint32_t sessionid, void **param);
	 
	 icore_public     *b_set;
	 icore_statistics *g_set;
	 
	 struct _yss_time   {
	 	    uint32_t   min;
	 	    uint32_t   sec;
	 	    uint32_t   utime;
	 	    uint32_t   number;
	 }tm;
	 
	 struct _icore_class   *next;
}PACKED;


ICORE_DLL icuint32 icore_init(icore_class *s);
ICORE_DLL icuint32 icore_run(icore_class *s); 
ICORE_DLL icuint32 icore_fini(icore_class *s);

#ifndef ICORE_STATIC
#define ICORE_STATIC  
#endif

ICORE_DLL 
ICORE_STATIC
icuint32     icore_active(icore_class *data);
ICORE_DLL
ICORE_STATIC
icuint32     icore_core(icore_class *data);
ICORE_DLL
ICORE_STATIC
icuint32     icore_public_t(icore_class *cls, icore_class *dst, icore_class *src);

ICORE_DLL
icuint32     icore_microp(icore_class *data, icore_readw       stream);

ICORE_DLL
icuint32     icore_control(icore_class *data, enum _icore_flags flags, uint32_t sessionid, void **param);


#ifndef MGC_SEEK1
#define MGC_SEEK1     32768/4
//#define MGC_SEEK1     24576
#endif

#define SSC_STREAM_MAX       1024*2
#define SSC_STREAM_BLOCK     8
#define SSVN_TSIZE   (SSC_STREAM_MAX*SSC_STREAM_BLOCK)

#define LIBMGC_POINT         1 // 32/2

#define EXT_C(x)        x

#define LIBPUBSIZEA     EXT_C(245) // +32 118
#define LIBPUBSIZEW     EXT_C(25)

/*struct _mgc_point_t 
{
   unsigned int cxstart;
   unsigned int cxfinal;
   
   unsigned int cystart;
   unsigned int cyfinal;
}PACKED mgc_point_t;
typedef struct _libcmdf_t
{
   unsigned int   zday;
   unsigned int   zmonth;
   unsigned int   zyear;
   
   unsigned int   zhour;
   unsigned int   zminute;
   unsigned int   zsecond;
   
   unsigned int   zmilli;
}PACKED libcmdf_t;
*/
typedef struct _libbuff_t
{
    c_int8_t     *buffer;
    
    uint32_t     length;
    uint32_t     block;
    
}PACKED libbuff_t;

typedef struct _libssc_t
{
    unsigned int usdr;   // queryid
    unsigned int ustc;   // thread count
    
    unsigned int zbool; // true    
    unsigned int block;  // block count
    unsigned int length; // size position
    
    unsigned int check;
   // unsigned int zback;
   // unsigned int zsize;
    
    unsigned int start;
    unsigned int stop;
    unsigned int usize;
    
    unsigned int zax,zbx;
    unsigned int zay,zby;
    
    unsigned short zcdate;
    unsigned short zctime; 
    unsigned short zmdate;
    unsigned short zmtime;   
    
    //unsigned char stream[SSVN_TSIZE]; 
    
	unsigned long stream[SSVN_TSIZE];
	
    unsigned int size;
    
    struct _libssc_t *next;
}PACKED libssc_t;

typedef struct _libssv_t
{
   unsigned int       uid;
    
   struct _libssc_t *xssv;

   libbuff_t        *data;
   
   struct _libssv_t *next;   
}PACKED libssv_t;

ICORE_DLL
icuint32     microp_init(icore_class *cls);
ICORE_DLL
icuint32     microp_run(icore_class *cls);
ICORE_DLL
icuint32     microp_fini(icore_class *cls);


RUNC_DLL void cprint(char *message);
RUNC_DLL void fatal(char *message,char *file,uint32_t line, uint32_t statusid);
RUNC_DLL int crand(void);

RUNC_DLL void *cmemmove (void *dest, const void *src, size_t n);;
RUNC_DLL int cmemcmp (const void *s1, const void *s2, size_t n);
RUNC_DLL void *cmemcpy (void *dest, const void *src, size_t count);
RUNC_DLL void *cmemset (void *s, int c, size_t n);
RUNC_DLL char *cstrcpy (char *dest, const char *src);
RUNC_DLL char *cstrcat (char *dest, const char *src);
RUNC_DLL char * cstrncat (char *dest, const char *src, int c);
RUNC_DLL int cstrcmp (const char *s1, const char *s2);
RUNC_DLL int cstrncmp (const char *s1, const char *s2, size_t n);
RUNC_DLL size_t cstrlen (const char *s);

ICORE_DLL void     RUNAPI get_time_start(uint32_t *min, uint32_t *sec);
ICORE_DLL uint32_t RUNAPI get_time_final(uint32_t min, uint32_t sec, uint32_t *lmin, uint32_t *lsec);

#define get_time_start(a,b) get_time_start(a,b)
#define get_time_final(a,b,c,d) get_time_final(a,b,c,d)

RUNC_DLL uint32_t cinit(uint32_t addr);
RUNC_DLL void * cmalloc (uint32_t size);
RUNC_DLL void   cfree (void *ptr);
RUNC_DLL void * crealloc (void *ptr, uint32_t size);

RUNC_DLL float  percent(uint32_t total_marks, uint32_t scored);

#endif
