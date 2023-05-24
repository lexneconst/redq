
#include "gcorew.h"

thread_core   *task=NULL;




int delay(uint32_t s)
{
	uint32_t i=0;
	for(i=0;i<s*100000;i++){
		
	}
}


void *match_buffer(struct _dat_node **ptr, const struct _dat_node *str, const int8_t *dst, uint32_t o_len, const int8_t *src, uint32_t len){
	uint32_t i=0;
	struct _dat_node *prev = (struct _dat_node *) *ptr;
	struct _dat_node next;
	
	cmemset(&next, 0, sizeof(struct _dat_node));
	//printf(@"%s :::: MGC(%d) %d, %d = (%x-%x)\n" , __FUNCTION__, MGC_SEEK1, o_len, len, prev, str );
do{
	if(prev==NULL || str == NULL){
		
		break;
	}
    if(dst==NULL){
    	
    	break;
	}	
	if(src == NULL){
		
		break;
	}
	if(len <= 0 || o_len <= 0){
		
		break;
	}
	cstrcpy(next.message, str->message);
	cstrcpy(next.filedev, str->filedev);
	cstrcpy(next.signdev, str->signdev);
	next.current = 0;
	next.maximum = o_len;
	next.ctime   = str->ctime;
	next.len_old = str->len_old;
	next.length  = str->length;
	
	#ifdef DEBUG_H
	printf("@%s :::: MGC(%d) %d, %d = (%x-%x)\n" , __FUNCTION__, MGC_SEEK1, o_len, len, prev, str );
	#endif
	
	for(i=0;i<o_len;i+=8){
		if(i>=len || i>= o_len){
		    break;
		} 
		//if(dst[i] == 0x0 || src[i] == 0x0){
		//	break;
		//}
		if(abs(dst[i]) >= abs(src[i])-15 && abs(dst[i]) <= abs(src[i]) + 15 ){
			next.current += 1;
		}
	}
	
	next.percent = percent(next.maximum, next.current);
	if(next.percent > prev->percent && next.percent >= 20){
	    cstrcpy(prev->message, next.message);
	    cstrcpy(prev->filedev, next.filedev);
	    cstrcpy(prev->signdev, next.signdev);
	    prev->length = next.length;
	    prev->len_old= next.len_old;
		prev->maximum= next.maximum;
		prev->current= next.current;
		prev->ctime  = next.ctime;
		prev->percent = next.percent;
		//printf("#%s :::: MGC(%d) %d, %d = (%x-%x)\n" , __FUNCTION__, MGC_SEEK1, o_len, len, prev, str );
	}
}while(0);
    *ptr = prev;
    
	return NULL;
}



void* thread_procw(void *arg)
{
	 thread_core  *core = (thread_core*) arg;
	 uint32_t     i=0,j=0;
	 uint32_t     threadid=0;
	 uint32_t     coreid=0;
	 uint32_t     old_thread_count=0,task_count=0;
	 uint32_t     seid=0, usrid=0, count=0;
	 struct _thread_ex  *next, *prev;
	 vss_node     *ptr=NULL;
	  
do{
	 ptr =(vss_node*) cmalloc(sizeof(vss_node));
	 if(ptr==NULL){
	 	fatal("allocate memory ", __FILE__, __LINE__, 0);
	 	break;
	 }
	 ptr->length = MGC_SEEK1 * SSC_STREAM_BLOCK * 2 /*64*/;
	 ptr->buffer = (int8_t*) cmalloc( ptr->length );
	 if(ptr->buffer == NULL){
	 	fatal("allcoate memory", __FILE__, __LINE__, 0);
	 	break;
	 }
	 #ifdef DEBUG_H
	 printf("thread process *(%5d) (%5d) (%5d)\n", core->threadid, core->thread_count , core->baseid);
     #endif

	if(core==NULL){
		fatal("stack memory ", __FILE__, __LINE__, 0);
		break;
	}
	coreid = core->baseid;
    core->number = 0;
    old_thread_count = task->thread_count;
    
	task->node[coreid]->number = 0;
    
	do{
		do{
		 if(task == NULL){
		 	fatal("Stack pointer ", __FILE__, __LINE__, 0);
		 	break;
		 }
	     if(task->status == ENABLE){
		     
		 #ifdef DEBUG_H
		 printf("### thread process *(%5d) (%5d) (%5d) (%d)\n", task->threadid , core->thread_count , coreid, task->max_core[coreid]);
		 #endif
		
		 if(task->node[coreid] == NULL){
		 	fatal("stack pointer", __FILE__, __LINE__, 0);
		 	break;
		 } 
		 task->node[coreid]->coreid = coreid;
         task->thread_count = old_thread_count;
          
         ptr->first=NULL;
		 ptr->next=NULL;
		 ptr->status=DISABLE;
		 ptr->seid=coreid;
		 ptr->usrid=task->count;
		 ptr->number = 0;
		 //ptr->buffer;
		 //ptr->length;
		 #ifdef DEBUG_H
		 printf("TSCORE :: thread core (%d) \n", __LINE__);
		 #endif
		 _vss_read(&ptr, task->count);
         
		 #ifdef DEBUG_H 
		 printf("state (%d) seid (%d) usr(%d) length: (%d), old (%d) , (%x)\n", 
		    ptr->status, ptr->seid, ptr->usrid, ptr->length, ptr->len_old, ptr->buffer
		  ); 
		 #endif
		 count = 0;
		 //printf("-------------------------------------------------------\n");
		 //printf("COREID (%d) COUNT (%d) \n", coreid, task->max_core[coreid]);
		 next = NULL;
		 prev = NULL;
         for(i=0;i<task->max_core[coreid];i++){
            
            if(ptr != NULL && ptr->length <= ptr->len_old && task->base[coreid] !=NULL){
                
                	if(count<=0) next = task->base[coreid];
                   // if(task->base[coreid]->stream!=NULL ){
					   //match_buffer(ptr->buffer, task->base[coreid]->stream->buffer, ptr->length );  
					if(next!=NULL){
					   
					   if(next->stream!=NULL && next->message!=NULL && ptr->status == ENABLE){
					   	    //printf("TSCORE :: thread core (%d) \n", __LINE__);
					   	    _vss_count(&task_count);
					   	    //printf("===================================================\n");
					   	    #ifndef DEBUG_H
							printf("$COREID (%d,%d)*(%d) MAX_LIST(%d) (%d) (%x)\n", 
							   task_count, task->count , coreid, task->max_core[coreid], i, next);
							#endif
							match_buffer(&task->base[coreid]->message, 
							   next->message,
							   ptr->buffer, ptr->length, 
							   next->stream->buffer, next->stream->length );
					        
					   }else{
					   	   #ifndef DEBUG_H
					   	   printf("+memory coreid(%d) list(%d) (%x) (%x) (%x) S(%d)\n", 
							   coreid, i, next->stream, task->base[coreid]->message, next->message, ptr->status);
					       #endif
					   }
					}else{
						#ifndef DEBUG_H
					    printf("#memory coreid(%d) list(%d) \n", coreid, i);
					    #endif
						break;	
					}    	
				    count += 1;
						   
					prev = next->next;
					next = prev;   
			      /// }
			
					
		    }else{
		    	#ifndef DEBUG_H
				 printf("@memory coreid(%d) list(%d) (%x) (%x) (%d) (%d)\n", 
							   coreid, i, task->base[coreid], ptr, ptr->length, ptr->len_old);
				#endif
		    	//fatal("stack memory ", __FILE__, __LINE__, 0);
			} 	
		 }
		 
		 //printf("TSCORE :: thread core (%d) \n", __LINE__);
		 //printf("=============================================\n");
		 //printf("MAX core (%d) \n", task->max_core);
		 //printf("=============================================\n");    
	     if(task->number >= task->thread_count-1 /* && task->node[coreid]->number == 2*/){
		   
		   //printf("###### (%d), %d = %d \n", task->number, task->thread_count, task->node[coreid]->number);
		   ptr->first=NULL;
		   ptr->next=NULL;
		   ptr->status=DISABLE;
		   ptr->seid=coreid;
		   ptr->usrid=ptr->usrid;
		   
		   ptr->number= 0;
		   
		   ptr->buffer=0;
		   ptr->length=0;
		   
		   _vss_free(ptr);
		   //printf("TSCORE :: thread core (%d) \n", __LINE__); 
		   
		   task->number = 0;
		   
		   if(task->count > 0){ 
		      task->count -= 1; 
		      if(task->count==0)
		         task->status = DISABLE;
		   }else{
		   	  task->status = DISABLE;
		   } 
		   task->node[coreid]->number = 0;
		   
		   #ifndef DEBUG_H
		   printf("Thread (%5d,%5d)=%5d,%7d Complete %s = SEID(%d),SEC(%d)\n", 
		   task->thread_count, task->number, task->threadid ,  task->max_core[coreid],
		   __FUNCTION__, task->core[task->threadid]->session ,task->core[task->threadid]->tm.utime);
		   #endif
		   
		   //task->core[]
		   if(task->core[task->threadid]->c_control!=NULL){
	          task->core[task->threadid]->c_control(task->core[task->threadid],
			      CWRITE, task->core[task->threadid]->session, (void **)ICORECOMPLETE );
	         if(task->base[coreid]->message->percent > 0){
	         	task->core[task->threadid]->c_control(task->core[task->threadid],
			      CMESSAGE, task->core[task->threadid]->session, (void **)task->base[coreid]->message );
			 }
		   } 
	       break;
		 }else{
		   task->node[coreid]->number = 1;
		   if(task->node[coreid]->number == 1)
		   {
		        task->node[coreid]->number += 1;
		     
		   }
		   
		   task->number += 1;
		   	
	      }
	    }else{
	    	#ifdef DEBUG_H
	    	printf("WARNING :: thread sleep (%d) \n", __LINE__);
	    	#endif
	    	break;
		}
	    sleep(1);
	  }while(1);
	    
	  sleep(1);
	}while(1);
	
}while(0);
	 
	 if(ptr!=NULL)
	  CFREE(ptr->buffer);
	 CFREE(ptr);
	 
	 return NULL;
}


void vsa_thread_run(thread_core *cls)
{
	int        err;
	uint32_t   i=0,j=0;
    struct _icore_class   *ptr;
	
	#ifdef  DEBUG_H
	printf("<%s> system core (%x)\n", __FUNCTION__ , ptr );
	#endif
	
	//cls->core[cls->threadid]->tm.min    = 0;
	//cls->core[cls->threadid]->tm.sec    = 0;
	//cls->core[cls->threadid]->tm.utime  = 0;	
	//cls->core[cls->threadid]->tm.number = 0;
	
	//get_time_start(&cls->core[cls->threadid]->tm.min, &cls->core[cls->threadid]->tm.sec );
	#ifdef DEBUG_H
	  printf("Thread start (%d)(%d) + data (%d,%d) \n", 
	   cls->core[cls->threadid]->session,
	   cls->core[cls->threadid]->status ,
	   cls->core[cls->threadid]->tm.min, 
	   cls->core[cls->threadid]->tm.sec);
    #endif
    
    #ifdef DEBUG_H
	printf("<%s> system core (%d)\n", __FUNCTION__ , cls->thread_count );
    #endif
    
    for(i=0;i<cls->thread_count;i++){
       
       cls->node[i] = (struct _thread_node *) cmalloc(sizeof(struct _thread_node));
       if(cls->node[i]== NULL){
       	  fatal("allocate memory ", __FILE__, __LINE__, 0);
       	  break;
	   }
	   
	   delay(10);
    	
	   cls->baseid = i;
	   err = pthread_create(&cls->uid[i], NULL, &thread_procw, (void*)cls);
	   if (err != 0)
 	   {
	    #ifdef DEBUG_H
        printf("can't create thread :[%s]\n", strerror(err));
        #endif
       }else{
   	    #ifdef DEBUG_H
        printf("Thread created successfully\n");
        #endif
	   }
    }
    
    //if(task->core[task->threadid]->c_control!=NULL)
	//   task->core[task->threadid]->c_control(ptr, CWRITE, ptr->session, (void **)ICORECOMPLETE );
    //sleep(1);
}
icuint32   vsa_thread_setup(thread_core  *cls, char *cfg)
{
	uint32_t   i,j;
	struct _thread_ex  *next, *prev;
	uint32_t   load=0,max_core=0, max_count=0, count=0;
	uint32_t   end_point = 0;
	intptr_t   handle=-1;
	char       file[MAX_PATH],szfile[MAX_PATH];
	
do{
	if(cls == NULL){
		fatal("stack memory ", __FILE__, __LINE__, 0);
		break;
	}	
	cls->thread_count = 0;
	
	//printf("CONFIG (%s) \n", cfg);
	load = _vsc_getfilecount(cfg);
	max_count = load;
	
	if(!load){
	   cls->max_core[0] = MAX_CORE;
	   max_core      = MAX_CORE*MAX_THREAD;
	}else{
	   cls->max_core[0] = load;
	   max_core      = load;
	}
	
	for(i=0;i<MAX_THREAD;i++)
	{
		//printf("==================[%d]===================\n", __LINE__);
		if(cls->base[i]==NULL){
			fatal("segment failed", __FILE__, __LINE__, 0);
			break;
		}
		//printf("==================[%d]===================\n", __LINE__);
		//printf("COUNT (%d) MAX (%d) - (%d) THREADC (%d) \n",count, max_count, end_point, cls->thread_count);
		if(count >= max_count || end_point){
		    //printf("@!End (%d) Point <%s> (%d,%d) (%d)\n", 
			// __LINE__, __FUNCTION__, count, max_count, cls->thread_count);
		    _vsc_closefile(handle);
		    break;
		}
		//printf("==================[%d]===================\n", __LINE__);
		if(count >= cls->max_core[i] && count == max_core){
		    //printf("End Point <%s> \n", __FUNCTION__);
		    break;
		}
		//printf("==================[%d]===================\n", __LINE__);
		//printf("CORE MATCH (%d,%d) \n", cls->max_core,MAX_CORE *i+1);
		if(max_core >= MAX_CORE){
			#ifdef DEBUG_H
			printf("@@max core loop %s=%d\n", __FUNCTION__, __LINE__);
			#endif
			cls->max_core[i] = MAX_CORE;
			load-= MAX_CORE;
			cls->base[i]->status = ENABLE;
			max_core -= MAX_CORE;
			
		}else{
			#ifdef DEBUG_H
			printf("$$max core loop %s=%d\n", __FUNCTION__, __LINE__);
			#endif
			cls->max_core[i] = load;
			max_core -= load;
			cls->base[i+1]->status = DISABLE;
			//if(max_core == 0) break;
			
		}
		//printf("==================[%d]===================\n", __LINE__);
		//printf("-COUNT (%d) MAX (%d)- (%d) THREADC (%d) \n",count, max_count, end_point, cls->thread_count);
		
		//printf("+COUNT (%d) MAX (%d) THREADC (%d) \n",count, max_count, cls->thread_count);
		//if(max_core == 0) break;
		//printf("==================[%d]===================\n", __LINE__);
	    prev = (thread_ex *) task->base[i];
	    prev->stream = (icore_read*) cmalloc(sizeof(icore_read));
	    if(prev->stream==NULL){
	    	fatal("allocate memory ", __FILE__, __LINE__, 0);
	    	break;
		}
		//printf("==================[%d]===================\n", __LINE__);
		prev->message = (struct _dat_node*) cmalloc( sizeof(struct _dat_node));
		if(prev->message==NULL){
			fatal("allocate memory ", __FILE__, __LINE__, 0);
			break;
		}
	    //printf("==================[%d]===================\n", __LINE__);		
		if(count >= max_count || end_point){
		    //printf("End (%d) Point <%s> \n", __LINE__, __FUNCTION__);
		    _vsc_closefile(handle);
		    end_point = 100;
		    break;
		}
		
		
		cls->thread_count = i;
		//printf("==================[%d]===================\n", __LINE__);
	    //printf("+MAX (%d) (%d) (%x) \n", cls->max_core, count, prev);
		for(j=0;j<cls->max_core[i];j++){
			//printf("cls->max_core (%d) \n", cls->max_core);
		    
		    if(count >= max_count  ){
		    	//printf("End (%d) Point <%s> (%d,%d) (%d) \n", __LINE__, __FUNCTION__, count, max_count, end_point);
		    	_vsc_closefile(handle);
		    	end_point = 100;
		    	handle = -1;
		    	break;
		    }
			//printf("==================[%d]===================\n", __LINE__);
		    if(count == 0){
		    	//printf("==================[%d]===================\n", __LINE__);
				handle = _vsc_firstfile(cfg, file);
				sprintf(szfile, "%s/%s\0", cfg, file);
				//printf("==================[%d]===================\n", __LINE__);
		    }else{
		    	//printf("==================[%d]===================\n", __LINE__);
				_vsc_nextfile(handle, file);
				sprintf(szfile, "%s/%s\0", cfg, file);
				//printf("==================[%d]===================\n", __LINE__);
		    }
		    if(!count) {
		       //printf("==================[%d]===================\n", __LINE__);	
		       //printf("%s", szfile);
		       dat_getfile(szfile, prev->stream, prev->message);
		       
			   if(count+1 >= max_count ){
			     _vsc_closefile(handle);
			     handle = -1;
			     break;
		       }
		       
		       count = i+j+1;
			   //printf("==================[%d]===================\n", __LINE__);	
		       //break;
		       //continue;
		    }
		    //printf("==================[%d]===================\n", __LINE__);
			count=i+j;
			
			next = (thread_ex *) cmalloc (sizeof(thread_ex));
			if(next ==NULL){
				fatal("allocate memory", __FILE__, __LINE__, 0);
				break;
			}
			cmemset(next, 0, sizeof(thread_ex));
		    //printf("==================[%d]===================\n", __LINE__);	
		    next->stream = (icore_read *) cmalloc( sizeof(icore_read));
		    if(next->stream==NULL){
		    	fatal("allocate memory", __FILE__, __LINE__, 0);
		    	break;
			}
			//printf("==================[%d]===================\n", __LINE__);
			next->message = (struct _dat_node*) cmalloc( sizeof(struct _dat_node) );
			if(next->message ==NULL){
				fatal("allocate memory", __FILE__, __LINE__, 0);
				break;
			}
		    //printf("ADDRESS [%x] [%x] \n", task->base[i]->message, next->message);
			//printf("FILE,%d(%s)\n", i+j, szfile);
			dat_getfile(szfile, next->stream, next->message);
			//printf("- PREV (%8x) NEXT (%8x) \n", prev, next);
			next->session = i+j;
			next->status = ENABLE;
			
			next->swap = 0xfff;
			
			prev->next = next;
		    prev = next;
		    
		}
		//task->base[i] = prev;
		//printf("==================[%d]===================\n", __LINE__);	
	    //printf("@MAX (%d) (%d) (%x) \n", cls->max_core, count, prev);
	}
	//prev= NULL;
	count = 0;
	for(i=0;i<MAX_THREAD;i++){
		
		if(count >= max_count) break;
		//printf("%s = %d - %d \n", __FUNCTION__, __LINE__, i);
		prev = task->base[i];
		
		for(j=0;j<cls->max_core[i];j++){
			//delay(1);
			if(count >= max_count) break;
			//printf("%s = %d - %d \n", __FUNCTION__, __LINE__, i);
		    
			if(prev == NULL){
		    	//printf("END %s = %d, COUNT(%d, %d)\n", __FUNCTION__, __LINE__, i, cls->max_core[i]);
		    	break;
			}
			count = i+j;
			//printf("COUNT (%d) List(%d) - (%x) \n", i, j, next);
		    if(task->base[i]==NULL){
		    	printf("$stack :: Id(%d) Line(%d) \n", i, j); 
		    	break;
			}
			//printf("%s = %d - %d \n", __FUNCTION__, __LINE__, i);
			if(task->base[i]->stream==NULL){
				 //task->base[i]->stream = (icore_read*) cmalloc( sizeof(icore_read));
				 //if(task->base[i]->stream==NULL){
				  printf("@stack :: Id(%d) Line(%d) \n", i, j); 
				  break;
				//}
				
			}
			//printf("%s = %d - %d \n", __FUNCTION__, __LINE__, i);
			if(prev->stream==NULL){
				printf("#stack :: Id(%d) Line(%d) \n", i, j); 
				break;
			}
			//printf("%s = %d - %d \n", __FUNCTION__, __LINE__, i);
			if(prev->message == NULL){
				printf("$stack :: ID(%d) Line(%d) \n", i, j);
				break;
			}
			//printf("++PREV:: (%8x) NEXT:: (%8x) \n", prev, next);
			//printf("%s = %d - %d \n", __FUNCTION__, __LINE__, i);
			next = prev->next;
		    prev = next;
		    //printf("%s = %d - %d \n", __FUNCTION__, __LINE__, i);
		}
		
	}
	
	//printf("Load Setup Complete (%s) (%d) \n", __FUNCTION__ , cls->thread_count );
}while(0);
     
    cls->count = 0;
    
    vsa_thread_run(cls);

	return 0;
}


icuint32   vsa_thread_init(icore_class *ptr, int argc, char **argv)
{
	uint32_t       i=0;
	
	char           szdir[MAX_PATH];
	getcwd(szdir, sizeof(szdir));
	if(szdir[cstrlen(szdir)-1] != '\\'){  cstrcat(szdir, "\\"); }
	cstrcat(szdir, "data");
	for(i=0;i<cstrlen(szdir);i++){
		if(szdir[i] == '\\')  szdir[i] = '/';
	}
	//printf("%d> %s - (%s)\n", argc, argv[0], szdir);
	
	task = (thread_core *) cmalloc( sizeof(thread_core));
	if(task == NULL){
		fatal("allocate memory failed", __FILE__, __LINE__, 0);
		return 0;
	}
	
	//printf("%s :: %d \n", __FUNCTION__, __LINE__);
do{
	if(ptr==NULL) break;
	
	task->core[ptr->session] = ptr;
	
	if(task == NULL){
		fatal("allocate memory", __FILE__, __LINE__, 0);
		break;
	}
//	printf("vsa initalized <%s>\n", __FILE__);
	
	for(i=0;i<MAX_THREAD;i++){
	    task->base[i] = (thread_ex*) cmalloc(sizeof(thread_ex));
	    if(task->base[i]==NULL){
	    	fatal("allocate memory ", __FILE__, __LINE__, 0);
	    	break;
	    }
	    
	    
	    
	}
	
	for(i=0;i<MAX_THREAD;i++){
	    task->core[i] = (icore_class *) cmalloc(sizeof(icore_class));
	    if(task->core[i]==NULL){
	    	fatal("allocate memory ", __FILE__, __LINE__, 0);
	    	break;
	    }
	}
	
	
	if(cstrlen(szdir)<=0){
	  vsa_thread_setup(task, CDATA_DIR);
	}else{
	  vsa_thread_setup(task, szdir);
	}
	
	_vss_init(0x000000ff);
	
	break;
	
}while(1);
	return 0;
}

icuint32   vsa_thread_core(icore_class *ptr, struct _icore_read *stream)
{
	uint32_t   i=0;
	#ifdef DEBUG_H
	printf("<%s> system core (%x)\n", __FUNCTION__ , ptr );
	#endif
	task->threadid= ptr->session;
	task->core[task->threadid] = ptr;
	ptr->status = ICOREPROCESS;
	task->core[task->threadid]->status = ptr->status;
	
	for(i=0;i<MAX_THREAD;i++){
	 if(task->core [i] == NULL) break;
	 task->core[i]->c_control = ptr->c_control;
	 if(task->core[i]->c_control==NULL){
		fatal("move stack null", __FILE__, __LINE__, 0);
	 }
	}
    #ifdef DEBUG_H
	printf("<%s> system core (%x)\n", __FUNCTION__ , ptr );
	#endif
		
	if(task->core[task->threadid]->c_control!=NULL)
	   task->core[task->threadid]->c_control(ptr, CWRITE, ptr->session, (void **)ICOREPROCESS );
	
	#ifdef DEBUG_H
	printf("<%s> system core (%x)\n", __FUNCTION__ , ptr );
	#endif
	
	do{
	  //printf("SESSION ID (%d) \n", ptr->session);
	  if(stream==NULL){
	     #ifdef DEBUG_H
		 fatal("stack null", __FILE__, __LINE__, 0);	
	     #endif
		 break;
	  }
	
	  //task->stream->length = stream->length;
	 // task->stream->buffer = (int8_t *) cmalloc (task->stream->length+2);
	  //if(task->stream->buffer == NULL){
	//	fatal("allocate memory failed", __FILE__, __LINE__, 0);
	//	break;
	  // }
	  // cstrcpy(task->stream->buffer, stream->buffer);
	   
	   struct _vss_node str = {
	   	   NULL,
	   	   NULL,
	   	   ENABLE,
	   	   ptr->session,
	   	   task->count,
	   	   0,
	   	   (int8_t*)stream->buffer,
	   	   stream->length,
	   	    MGC_SEEK1 * SSC_STREAM_BLOCK 
	   };
	   _vss_update(&str);
	   
	}while(0);
	
	#ifdef DEBUG_H
	printf("<%s> system core (%x) (%d)\n", __FUNCTION__ , ptr, ptr->session );
	#endif
	
	task->status = ENABLE;
	
	if(task->count < _vss_max() ) task->count += 1;
	//vsa_thread_run(task);
	//if(task->core[task->threadid]->c_control!=NULL)
	 //  task->core[task->threadid]->c_control(ptr, CWRITE, ptr->session, (void **)ICORECOMPLETE );
	   
	return 0;
}

icuint32   vsa_thread_fini(icore_class *ptr)
{
	_vss_fini(0x000000ff);
	return 0;
}

