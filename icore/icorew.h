
#ifndef ICOREW_H
#define ICOREW_H  1

#include <icore.h>
#include <gcore.h>



#define CHK_D(s)   if(s==NULL) {  fatal("null stack data",__FILE__, __LINE__, 0 ); return ICORE_NULL;	}

icore_statistics *
c_public_set(icore_statistics *ptr, uint32_t seid);

extern
icore_class   *task[MAX_TABLE+3];



#endif


