
#include "redq.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */




icore_public  b_set={
    0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
    NULL,
    { 0,0,0,0,0,0, 0,0},
    {NULL, NULL, NULL, NULL}
};

icore_class cls ={
    ICORENAME_STR,
    ICOREVERS_STR,
    ICORESERIAL_D,
    0,
    ICORENULL,
    0,
    icore_init,
    icore_run,
    icore_fini,
    icore_active,
    icore_core,
    icore_public_t,
    icore_microp,
    vsa_thread_core,
    icore_control,
    (icore_publicw)&b_set,
    NULL
};


int main(int argc, char *argv[]) {
	
	//cinit(0xffffffff);
	icore_init(&cls);
	vsa_thread_init(&cls, argc, argv);
	microp_init(&cls);
	//mgc_core_init(&cls);
	microp_run(&cls);
	do{
		//printf("loop red quees my data\n");
		sleep(1);
	}while(icore_run(&cls)>0);
	
	microp_fini(&cls);
	vsa_thread_fini(&cls);
	icore_fini(&cls);
	
	return 0;
}
