#ifndef _MICROPW_H_
#define _MICROPW_H_

#include <icore.h>

#ifndef CLEAN
#define CLEAN(x) if(x!=NULL) cfree(x),x=NULL;
#endif

void* thread_procl(void *arg);


DWORD WINAPI waveInProc(LPVOID arg);

#endif
