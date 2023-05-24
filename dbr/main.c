

#include "dbrw.h"


icuint32      add_icore_microp(icore_class *data, icore_read      * stream);

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
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    add_icore_microp,
    NULL,
    NULL,
    (icore_publicw)&b_set,
    NULL
};


int main(int argc, char *argv[]) {
	
	//cinit(0xffffffff);
	//icore_init(&cls);
	//vsa_thread_init(&cls);
	microp_init(&cls);
	//mgc_core_init(&cls);
	microp_run(&cls);
	do{
		//printf("loop red quees my data\n");
		sleep(1);
	}while(1);
	
	microp_fini(&cls);
	//vsa_thread_fini(&cls);
	//icore_fini(&cls);
	
	return 0;
}


	
icuint32      add_icore_microp(icore_class *data, icore_read      * stream)
{
	uint32_t       i=0;
	char           szdir[MAX_PATH];
	
	getcwd(szdir, sizeof(szdir));
	if(szdir[cstrlen(szdir)-1] != '\\'){  cstrcat(szdir, "\\"); }
	cstrcat(szdir, "data");
	for(i=0;i<cstrlen(szdir);i++){
		if(szdir[i] == '\\')  szdir[i] = '/';
	}
	
	//printf("stack initialize %s \n", __FUNCTION__);
	do{
	   if(stream==NULL){
	   	  fatal("stream allocate memory", __FILE__, __LINE__, 0);
	   	  break;
	   }	
	   
	   printf("%8x = %8x , %d \n", stream, stream->buffer, stream->length);
	   
	   dat_setfile(szdir, stream, "");
	}while(0);
	
	return 0;
}
