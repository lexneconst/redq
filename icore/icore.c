
#include "icorew.h"


icore_class   *task[MAX_TABLE+3];

#define G_SET_C(s, x) \
	if(s!=NULL){ \
	  if(s->g_set!=NULL){ \
	     s->g_set->cc.x = 0; \
	  }else{ \
	   fatal("stack pointer ", __FILE__, __LINE__, 0); \
      } \
	}else{ \
	  fatal("stack failed", __FILE__, __LINE__, 0); \
    }
    
#define RESET_CC(s) \
		G_SET_C(s,c00); \
		G_SET_C(s,c01); \
		G_SET_C(s,c02); \
		G_SET_C(s,c03); \
		G_SET_C(s,c04); \
		G_SET_C(s,c05); \
		G_SET_C(s,c06); \
		G_SET_C(s,c07); \
		G_SET_C(s,c08); \
		G_SET_C(s,c09); \
		G_SET_C(s,c10); \
		G_SET_C(s,c11); \
		G_SET_C(s,c12); \
		G_SET_C(s,c13); \
		G_SET_C(s,c14); \
		G_SET_C(s,c15); \
		G_SET_C(s,c16); \
		G_SET_C(s,c17); \
		G_SET_C(s,c18); \
		G_SET_C(s,c19); \
		G_SET_C(s,c20); \
		G_SET_C(s,c21); \
		G_SET_C(s,c22); \
		G_SET_C(s,c23); \
		G_SET_C(s,c24); \
		G_SET_C(s,c25); \
		G_SET_C(s,c26); \
		G_SET_C(s,c27); \
		G_SET_C(s,c28); \
		G_SET_C(s,c29); \
		G_SET_C(s,c30); \
		G_SET_C(s,c31); \
		G_SET_C(s,c32); \
		G_SET_C(s,c33); \
		G_SET_C(s,c34); \
		G_SET_C(s,c35); \
		G_SET_C(s,c36); \
		G_SET_C(s,c37); \
		G_SET_C(s,c38); \
		G_SET_C(s,c39); \
		G_SET_C(s,c40); \
		G_SET_C(s,c41); \
		G_SET_C(s,c42); \
		G_SET_C(s,c43); \
		G_SET_C(s,c44); \
		G_SET_C(s,c45); \
		G_SET_C(s,c46); \
		G_SET_C(s,c47); \
		G_SET_C(s,c48); \
		G_SET_C(s,c49); \
		G_SET_C(s,c50); \
		G_SET_C(s,c51); \
		G_SET_C(s,c52); \
		G_SET_C(s,c53); \
		G_SET_C(s,c54); \
		G_SET_C(s,c55); \
		G_SET_C(s,c56); \
		G_SET_C(s,c57); \
		G_SET_C(s,c58); \
		G_SET_C(s,c59); \
		G_SET_C(s,c60); \
		G_SET_C(s,c61); \
		G_SET_C(s,c62); \
		G_SET_C(s,c63); \
		G_SET_C(s,c64); \
		G_SET_C(s,c65); \
		G_SET_C(s,c66); \
		G_SET_C(s,c67); \
		G_SET_C(s,c68); \
		G_SET_C(s,c69); \
		G_SET_C(s,c70); \
		G_SET_C(s,c71); \
		G_SET_C(s,c72); \
		G_SET_C(s,c73); \
		G_SET_C(s,c74); \
		G_SET_C(s,c75); \
		G_SET_C(s,c76); \
		G_SET_C(s,c77); \
		G_SET_C(s,c78); \
		G_SET_C(s,c79); \
		G_SET_C(s,c80); \
		G_SET_C(s,c81); \
		G_SET_C(s,c82); \
		G_SET_C(s,c83); \
		G_SET_C(s,c84); \
		G_SET_C(s,c85); \
		G_SET_C(s,c86); \
		G_SET_C(s,c87); \
		G_SET_C(s,c88); \
		G_SET_C(s,c89); \
		G_SET_C(s,c90); \
		G_SET_C(s,c91); \
		G_SET_C(s,c92); \
		G_SET_C(s,c93); \
		G_SET_C(s,c94); \
		G_SET_C(s,c95); \
		G_SET_C(s,c96); \
		G_SET_C(s,c97); \
		G_SET_C(s,c98); \
		G_SET_C(s,c99); 
		
		
icuint32 icore_init(icore_class *s)
{
	icore_class  *next, *prev = s;
	uint32_t      i;
	//printf("initialize project <%s>(%d)\n", __FUNCTION__, __LINE__);
	CHK_D(s);
	//printf("initialize project <%s>(%d)\n", __FUNCTION__, __LINE__);
	
	if(s->g_set == NULL)
	{
		s->g_set = (icore_statistics* ) malloc( sizeof(icore_statistics));
		if(s->g_set == NULL){
			fatal("allocate memory ", __FILE__, __LINE__, 0);
			return ICORE_ERROR;
		}
		cmemset(s->g_set, 0, sizeof(icore_statistics));
		
	    RESET_CC(s);
	}
	//printf("initialize project <%s>(%d)\n", __FUNCTION__, __LINE__);
	for(i=0;i<=MAX_TABLE+2;i++){
		
	 	next = (icore_class *)cmalloc( sizeof(icore_class) );
		if(next == NULL){
			fatal("allocate memory ", __FILE__, __LINE__, 0);
			break;
		} 
		cmemset(next, 0 , sizeof(icore_class) );
		next->session = prev->session;
		if(next->session == 0){
		  next->session = prev->session+1;
		}else{
		  next->session += 1;
		}
		
	 	next->status  = ICOREINIT;
	 	next->c_active= prev->c_active;
	 	next->c_core  = prev->c_core;
	 	next->c_public = prev->c_public;
	 	
	 	next->c_data  = prev->c_data;
	 	next->c_microp = prev->c_microp;
	 	
	 	next->c_control= prev->c_control;
	 	
	 	next->b_set = (icore_public *) cmalloc( sizeof(icore_public));
	 	if(next->b_set==NULL){
	 		fatal("allocate memory ", __FILE__, __LINE__, 0);
			break;
	 	}
	 	next->b_set->message = (struct _dat_node *) cmalloc( sizeof(struct _dat_node));
	 	if(next->b_set->message == NULL){
	 		fatal("allcate memory ", __FILE__, __LINE__, 0);
	 		break;
		 }
	 	cmemset(next->b_set, 0, sizeof(icore_public));	 	
	 	//next->c_active(next);
	 	
	 	next->next = NULL;
		//printf("addon process core (%d) \n", next->session); 
		
		task[i] = (icore_class *) cmalloc( sizeof( icore_class));
		if(task [i]==NULL){
			fatal("allocate memory", __FILE__, __LINE__, 0);
			break;
		}
		//cmemset(task[i],0, sizeof(icore_class));
		//task[i] = next;
		task[i]->session = prev->session += 1;
	 	task[i]->status  = ICOREINIT;
	 	task[i]->c_active= prev->c_active;
	 	task[i]->c_core  = prev->c_core;
	 	task[i]->c_public = prev->c_public;
	 	task[i]->c_data  = prev->c_data;
	 	task[i]->c_microp = prev->c_microp;
	 	task[i]->c_control= prev->c_control;
		task[i]->b_set = (icore_public *) cmalloc( sizeof(icore_public));
	 	if(task[i]->b_set==NULL){
	 		fatal("allocate memory ", __FILE__, __LINE__, 0);
			break;
	 	}
	 	cmemset(task[i]->b_set, 0, sizeof(icore_public));	 
	 	
	 	task[i]->b_set->message = (struct _dat_node *) cmalloc( sizeof(struct _dat_node));
	 	if(task[i]->b_set->message == NULL){
	 		fatal("allcate memory ", __FILE__, __LINE__, 0);
	 		break;
		 }
	 	cmemset(task[i]->b_set->message, 0, sizeof(struct _dat_node ));	 
	 	
	 	//next->c_active(task[i]);
	 	
		task[i]->next = next;
		
		
		prev->next = next;
		prev = next;
	}
	for(i=0;i<=MAX_TABLE+2;i++){
	    if(task[i] == NULL){
	    	printf("stack memory %d %s %d\n", i,__FILE__, __LINE__, 0);
	    	break;
		}
		if(task[i]->b_set==NULL){
			printf("stack memory %d %s %d\n", i,__FILE__, __LINE__, 0);
			break;
		}
		if(task[i]->b_set->message==NULL){
			printf("stack memory %d %s %d\n", i, __FILE__, __LINE__, 0);
			break;
		}
	}
	
	next = s;
	for(i=0;i<MAX_TABLE+2;i++){
		if(next==NULL){
			fatal("stack null", __FILE__, __LINE__, 0);
			break;
		}
		#ifdef DEBUG_H
		printf("> P(%d) C(%d) (%x)\n", next->session, task[i]->session, next->c_active);
		#endif
		
		if(task[i] != NULL && task[i]->c_active!=NULL){
			//printf("load active <%x> \n", task[i]->c_active);
			
			task[i]->c_active(next);
			
			prev = task[i]->next;
			if(prev !=NULL && prev->c_active!=NULL)
			   prev->c_active(next);
		}
		//	cprint("stack memory failed");
		
		if(next != NULL && next->c_active!=NULL){
		   next->c_active(next);
		}
		
		prev = next->next;
		next = prev;
	}
	
 	return 0;
}


icuint32 icore_run(icore_class *s)
{
	CHK_D(s);
	
	do{
	
	//cprint("run project");
	if(s!=NULL)
	   if(s->c_core!=NULL)
	       s->c_core(s);
	    else
	       fatal("stack pointer ", __FILE__, __LINE__, 0);
	else
	   fatal("stack pointer ", __FILE__, __LINE__, 0);	
    }while(0);
    
	return ICORE_SUCCESS;
}


icuint32 icore_fini(icore_class *s)
{
	icore_class *tmp=NULL, *node=s;
	
	CHK_D(s);
	while(node) {
       tmp = node->next;
       cfree(node);
       node = tmp;
    }
	cprint("close project");
	return 0;
}




/* ------------------------------------------------------------------------------------------------------------















------------------------------------------------------------------------------------------------------------------*/
#define LOOP(a, x) i=3; \
	do{ \
	  if(a->cc.x<100) break;\
	  if(a->cc.x>=100) a->cc.x = a->cc.x%10; i--; \
	  if(i==0) if(a->cc.x >= 100) a->cc.x = 0; \
	  --i; \
	}while(i>0);
	
#define ACT_CC(a, x) a->cc.x = crand()%100; LOOP(a,x)

ICORE_STATIC
icuint32     icore_active(icore_class *data)
{
	struct _icore_public  *ptr=NULL;
	uint32_t               i=0;
	
	CHK_D(data);
	
	#ifdef DEBUG_H
	printf("-> %s [%x] ID= [%d]\n", __FUNCTION__, data, data->session);
	#endif
	
	ptr= (struct _icore_public  *)data->b_set;
	    
	if(ptr!=NULL){
		//srand(time(NULL));
		//for(i=0;i<MAX_TABLEA;i++){
		   ACT_CC(ptr, c00);
		   ACT_CC(ptr, c01);
		   ACT_CC(ptr, c02);
		   ACT_CC(ptr, c03);
		   ACT_CC(ptr, c04);
		   ACT_CC(ptr, c05);
		   ACT_CC(ptr, c06);
		   ACT_CC(ptr, c07);
		   ACT_CC(ptr, c08);
		   ACT_CC(ptr, c09);
		   ACT_CC(ptr, c10);
		   ACT_CC(ptr, c11);
		   ACT_CC(ptr, c12);
		   ACT_CC(ptr, c13);
		   ACT_CC(ptr, c14);
		   ACT_CC(ptr, c15);
		   ACT_CC(ptr, c16);
		   ACT_CC(ptr, c17);
		   ACT_CC(ptr, c18);
		   ACT_CC(ptr, c19);
		   ACT_CC(ptr, c20);
		   ACT_CC(ptr, c21);
		   ACT_CC(ptr, c22);
		   ACT_CC(ptr, c23);
		   ACT_CC(ptr, c24);
		   ACT_CC(ptr, c25);
		   ACT_CC(ptr, c26);
		   ACT_CC(ptr, c27);
		   ACT_CC(ptr, c28);
		   ACT_CC(ptr, c29);
		   ACT_CC(ptr, c30);
		   ACT_CC(ptr, c31);
		   ACT_CC(ptr, c32);
		   ACT_CC(ptr, c33);
		   ACT_CC(ptr, c34);
		   ACT_CC(ptr, c35);
		   ACT_CC(ptr, c36);
		   ACT_CC(ptr, c37);
		   ACT_CC(ptr, c38);
		   ACT_CC(ptr, c39);
		   ACT_CC(ptr, c40);
		   ACT_CC(ptr, c41);
		   ACT_CC(ptr, c42);
		   ACT_CC(ptr, c43);
		   ACT_CC(ptr, c44);
		   ACT_CC(ptr, c45);
		   ACT_CC(ptr, c46);
		   ACT_CC(ptr, c47);
		   ACT_CC(ptr, c48);
		   ACT_CC(ptr, c49);
		   ACT_CC(ptr, c50);
		   ACT_CC(ptr, c51);
		   ACT_CC(ptr, c52);
		   ACT_CC(ptr, c53);
		   ACT_CC(ptr, c54);
		   ACT_CC(ptr, c55);
		   ACT_CC(ptr, c56);
		   ACT_CC(ptr, c57);
		   ACT_CC(ptr, c58);
		   ACT_CC(ptr, c59);
		   ACT_CC(ptr, c60);
		   ACT_CC(ptr, c61);
		   ACT_CC(ptr, c62);
		   ACT_CC(ptr, c63);
		   ACT_CC(ptr, c64);
		   ACT_CC(ptr, c65);
		   ACT_CC(ptr, c66);
		   ACT_CC(ptr, c67);
		   ACT_CC(ptr, c68);
		   ACT_CC(ptr, c69);
		   ACT_CC(ptr, c70);
		   ACT_CC(ptr, c71);
		   ACT_CC(ptr, c72);
		   ACT_CC(ptr, c73);
		   ACT_CC(ptr, c74);
		   ACT_CC(ptr, c75);
		   ACT_CC(ptr, c76);
		   ACT_CC(ptr, c77);
		   ACT_CC(ptr, c78);
		   ACT_CC(ptr, c79);
		   ACT_CC(ptr, c80);
		   ACT_CC(ptr, c81);
		   ACT_CC(ptr, c82);
		   ACT_CC(ptr, c83);
		   ACT_CC(ptr, c84);
		   ACT_CC(ptr, c85);
		   ACT_CC(ptr, c86);
		   ACT_CC(ptr, c87);
		   ACT_CC(ptr, c88);
		   ACT_CC(ptr, c89);
		   ACT_CC(ptr, c90);
		   ACT_CC(ptr, c91);
		   ACT_CC(ptr, c92);
		   ACT_CC(ptr, c93);
		   ACT_CC(ptr, c94);
		   ACT_CC(ptr, c95);
		   ACT_CC(ptr, c96);
		   ACT_CC(ptr, c97);
		   ACT_CC(ptr, c98);
		   ACT_CC(ptr, c99);
		   
	      
		//}
	    
		data->b_set = (icore_public *)ptr;
	}
	
	return 0;
}

#define PROC_CC(q, a, x, v) \
   if(task[a->cc.x]!=NULL && task[a->cc.x]->c_public!=NULL && a->cc.x<= 99){ \
      int i=0; \
	  /*printf("__stack__ (%d,%d, %x) \n", v, a->cc.x , task[a->cc.x]->c_public ); */\
	  for(i=0;i<10;i++){ \
	  } \
	  task[a->cc.x]->c_public(q, next, task[a->cc.x]);  \
    }else{ \
       fatal("stack pointer", __FILE__, __LINE__, 0); \
	}


#define PRINT_CC(c, x) \
	printf("%3d ", c->cc.x);
	
ICORE_STATIC
icuint32     icore_core(icore_class *data)
{	
	icore_class  *next, *prev = NULL;
	uint32_t      i,l;
	int           sec=0, min=0, utime=0;
	struct _icore_public  *ptr=NULL;
	
	CHK_D(data);

	get_time_start(&min, &sec);
	#ifdef DEBUG_H
	printf("++++++++++++++++++++++++ Core + time %d,%d\n", min, sec);
	#endif
	
	RESET_CC(data);
	//printf("RESET_CC Continue <%s> <%d>\n", __FUNCTION__, __LINE__);	

	next = data;
	for(i=0;i<MAX_TABLE;i++){
		if(i > 99 ) break;
		if(next==NULL){
			fatal("stack null", __FILE__, __LINE__, 0);
			break;
		}
		if(next->status == ICOREDISABLE){
			//prev = next->next;
		    //next = prev;
		    //printf("$$$$$$$$$$$$$$$$ Continue <%s> <%d>\n", __FUNCTION__, __LINE__);
			//continue;
		}else
		if(next->status == ICOREINIT){
			
			//printf("$$$$$$$$$$$$$$$$ Continue <%s> <%d>\n", __FUNCTION__, __LINE__);
		}else
		if(next->status == ICOREFAILED)
		{
			//printf("$$$$$$$$$$$$$$$$ Continue <%s> <%d>\n", __FUNCTION__, __LINE__);
		}else{
		    //printf("$$$$$$$$$$$$$$$$ Continue <%s> <%d>\n", __FUNCTION__, __LINE__);
		}
		
		//printf("$$$ Continue <%s> <%d> ID <%d> ST<%d>\n",
		//   __FUNCTION__, __LINE__, i, next->session );
		
		if(next->status == ICOREENABLE){
		  ptr= (struct _icore_public  *)task[i]->b_set;
	    }else{
	      ptr= (struct _icore_public  *)next->b_set;
	    }
		//printf("> %x \n", data);
		//for(l=0;l<MAX_TABLEA;l++){
		PROC_CC(data, ptr, c00, 0);
		PROC_CC(data, ptr, c01, 1);
		PROC_CC(data, ptr, c02, 2);
		PROC_CC(data, ptr, c03, 3);
		PROC_CC(data, ptr, c04, 4);
		PROC_CC(data, ptr, c05, 5);
		PROC_CC(data, ptr, c06, 6);
		PROC_CC(data, ptr, c07, 7);
		PROC_CC(data, ptr, c08, 8);
		PROC_CC(data, ptr, c09, 9);
		PROC_CC(data, ptr, c10, 10);
		PROC_CC(data, ptr, c11, 11);
		PROC_CC(data, ptr, c12, 12);
		PROC_CC(data, ptr, c13, 13);
		PROC_CC(data, ptr, c14, 14);
		PROC_CC(data, ptr, c15, 15);
		PROC_CC(data, ptr, c16, 16);
		PROC_CC(data, ptr, c17, 17);
		PROC_CC(data, ptr, c18, 18);
		PROC_CC(data, ptr, c19, 19);
		PROC_CC(data, ptr, c20, 20);
		PROC_CC(data, ptr, c21, 21);
		PROC_CC(data, ptr, c22, 22);
		PROC_CC(data, ptr, c23, 23);
		PROC_CC(data, ptr, c24, 24);
		PROC_CC(data, ptr, c25, 25);
		PROC_CC(data, ptr, c26, 26);
		PROC_CC(data, ptr, c27, 27);
		PROC_CC(data, ptr, c28, 28);
		PROC_CC(data, ptr, c29, 29);
		PROC_CC(data, ptr, c30, 30);
		PROC_CC(data, ptr, c31, 31);
		PROC_CC(data, ptr, c32, 32);
		PROC_CC(data, ptr, c33, 33);
		PROC_CC(data, ptr, c34, 34);
		PROC_CC(data, ptr, c35, 35);
		PROC_CC(data, ptr, c36, 36);
		PROC_CC(data, ptr, c37, 37);
		PROC_CC(data, ptr, c38, 38);
		PROC_CC(data, ptr, c39, 39);
		PROC_CC(data, ptr, c40, 40);
		PROC_CC(data, ptr, c41, 41);
		PROC_CC(data, ptr, c42, 42);
		PROC_CC(data, ptr, c43, 43);
		PROC_CC(data, ptr, c44, 44);
		PROC_CC(data, ptr, c45, 45);
		PROC_CC(data, ptr, c46, 46);
		PROC_CC(data, ptr, c47, 47);
		PROC_CC(data, ptr, c48, 48);
		PROC_CC(data, ptr, c49, 49);
		PROC_CC(data, ptr, c50, 50);
		PROC_CC(data, ptr, c51, 51);
		PROC_CC(data, ptr, c52, 52);
		PROC_CC(data, ptr, c53, 53);
		PROC_CC(data, ptr, c54, 54);
		PROC_CC(data, ptr, c55, 55);
		PROC_CC(data, ptr, c56, 56);
		PROC_CC(data, ptr, c57, 57);
		PROC_CC(data, ptr, c58, 58);
		PROC_CC(data, ptr, c59, 59);
		PROC_CC(data, ptr, c60, 60);
		PROC_CC(data, ptr, c61, 61);
		PROC_CC(data, ptr, c62, 62);
		PROC_CC(data, ptr, c63, 63);
		PROC_CC(data, ptr, c64, 64);
		PROC_CC(data, ptr, c65, 65);
		PROC_CC(data, ptr, c66, 66);
		PROC_CC(data, ptr, c67, 67);
		PROC_CC(data, ptr, c68, 68);
		PROC_CC(data, ptr, c69, 69);
		PROC_CC(data, ptr, c70, 70);
		PROC_CC(data, ptr, c71, 71);
		PROC_CC(data, ptr, c72, 72);
		PROC_CC(data, ptr, c73, 73);
		PROC_CC(data, ptr, c74, 74);
		PROC_CC(data, ptr, c75, 75);
		PROC_CC(data, ptr, c76, 76);
		PROC_CC(data, ptr, c77, 77);
		PROC_CC(data, ptr, c78, 78);
		PROC_CC(data, ptr, c79, 79);
		PROC_CC(data, ptr, c80, 80);
		PROC_CC(data, ptr, c81, 81);
		PROC_CC(data, ptr, c82, 82);
		PROC_CC(data, ptr, c83, 83);
		PROC_CC(data, ptr, c84, 84);
		PROC_CC(data, ptr, c85, 85);
		PROC_CC(data, ptr, c86, 86);
		PROC_CC(data, ptr, c87, 87);
		PROC_CC(data, ptr, c88, 88);
		PROC_CC(data, ptr, c89, 89);
		PROC_CC(data, ptr, c90, 90);
		PROC_CC(data, ptr, c91, 91);
		PROC_CC(data, ptr, c92, 92);
		PROC_CC(data, ptr, c93, 93);
		PROC_CC(data, ptr, c94, 94);
		PROC_CC(data, ptr, c95, 95);
		PROC_CC(data, ptr, c96, 96);
		PROC_CC(data, ptr, c97, 97);
		PROC_CC(data, ptr, c98, 98);
		PROC_CC(data, ptr, c99, 99);				
		//if(task[ptr->c[l]]!=NULL && task[ptr->c[l]]->c_public!=NULL){ task[ptr->c[l]]->c_public(next, task[ptr->c[l]]); }
		//}
		next->status = ICOREENABLE;
		
		if(next==NULL) break;
		
	    prev = next->next;
		next = prev;
	}
	//printf(">>>>>>>>>>>>>>>>> [ %s ]\n", __FUNCTION__);
	#ifdef DEBUG_H
	//sleep(2);
	    PRINT_CC(data->g_set,c00);
		PRINT_CC(data->g_set,c01);
		PRINT_CC(data->g_set,c02);
		PRINT_CC(data->g_set,c03);
		PRINT_CC(data->g_set,c04);
		PRINT_CC(data->g_set,c05);
		PRINT_CC(data->g_set,c06);
		PRINT_CC(data->g_set,c07);
		PRINT_CC(data->g_set,c08);
		PRINT_CC(data->g_set,c09);
		PRINT_CC(data->g_set,c10);
		PRINT_CC(data->g_set,c11);
		PRINT_CC(data->g_set,c12);
		PRINT_CC(data->g_set,c13);
		PRINT_CC(data->g_set,c14);
		PRINT_CC(data->g_set,c15);
		PRINT_CC(data->g_set,c16);
		PRINT_CC(data->g_set,c17);
		PRINT_CC(data->g_set,c18);
		PRINT_CC(data->g_set,c19);
		PRINT_CC(data->g_set,c20);
		PRINT_CC(data->g_set,c21);
		PRINT_CC(data->g_set,c22);
		PRINT_CC(data->g_set,c23);
		PRINT_CC(data->g_set,c24);
		PRINT_CC(data->g_set,c25);
		PRINT_CC(data->g_set,c26);
		PRINT_CC(data->g_set,c27);
		PRINT_CC(data->g_set,c28);
		PRINT_CC(data->g_set,c29);
		PRINT_CC(data->g_set,c30);
		PRINT_CC(data->g_set,c31);
		PRINT_CC(data->g_set,c32);
		PRINT_CC(data->g_set,c33);
		PRINT_CC(data->g_set,c34);
		PRINT_CC(data->g_set,c35);
		PRINT_CC(data->g_set,c36);
		PRINT_CC(data->g_set,c37);
		PRINT_CC(data->g_set,c38);
		PRINT_CC(data->g_set,c39);
		PRINT_CC(data->g_set,c40);
		PRINT_CC(data->g_set,c41);
		PRINT_CC(data->g_set,c42);
		PRINT_CC(data->g_set,c42);
		PRINT_CC(data->g_set,c44);
		PRINT_CC(data->g_set,c45);
		PRINT_CC(data->g_set,c46);
		PRINT_CC(data->g_set,c47);
		PRINT_CC(data->g_set,c48);
		PRINT_CC(data->g_set,c49);
		PRINT_CC(data->g_set,c50);
		PRINT_CC(data->g_set,c51);
		PRINT_CC(data->g_set,c52);
		PRINT_CC(data->g_set,c53);
		PRINT_CC(data->g_set,c54);
		PRINT_CC(data->g_set,c55);
		PRINT_CC(data->g_set,c56);
		PRINT_CC(data->g_set,c57);
		PRINT_CC(data->g_set,c58);
		PRINT_CC(data->g_set,c59);
		PRINT_CC(data->g_set,c60);
		PRINT_CC(data->g_set,c61);
		PRINT_CC(data->g_set,c62);
		PRINT_CC(data->g_set,c63);
		PRINT_CC(data->g_set,c64);
		PRINT_CC(data->g_set,c65);
		PRINT_CC(data->g_set,c66);
		PRINT_CC(data->g_set,c67);
		PRINT_CC(data->g_set,c68);
		PRINT_CC(data->g_set,c69);
		PRINT_CC(data->g_set,c70);
		PRINT_CC(data->g_set,c71);
		PRINT_CC(data->g_set,c72);
		PRINT_CC(data->g_set,c73);
		PRINT_CC(data->g_set,c74);
		PRINT_CC(data->g_set,c75);
		PRINT_CC(data->g_set,c76);
		PRINT_CC(data->g_set,c77);
		PRINT_CC(data->g_set,c78);
		PRINT_CC(data->g_set,c79);
		PRINT_CC(data->g_set,c80);
		PRINT_CC(data->g_set,c81);
		PRINT_CC(data->g_set,c82);
		PRINT_CC(data->g_set,c83);
		PRINT_CC(data->g_set,c84);
		PRINT_CC(data->g_set,c85);
		PRINT_CC(data->g_set,c86);
		PRINT_CC(data->g_set,c87);
		PRINT_CC(data->g_set,c88);
		PRINT_CC(data->g_set,c89);
		PRINT_CC(data->g_set,c90);
		PRINT_CC(data->g_set,c91);
		PRINT_CC(data->g_set,c92);
		PRINT_CC(data->g_set,c93);
		PRINT_CC(data->g_set,c94);
		PRINT_CC(data->g_set,c95);
		PRINT_CC(data->g_set,c96);
		PRINT_CC(data->g_set,c97);
		PRINT_CC(data->g_set,c98);
		PRINT_CC(data->g_set,c99);
	
		printf("\n");
	#endif
	//printf(":::::::::::::::::: [ %s ]\n", __FUNCTION__);
	utime = (uint32_t) get_time_final(min,sec, &data->tm.min, &data->tm.sec);
	printf("Core (%d) + data (%d,%d) %d \n", data->status ,data->tm.min, data->tm.sec, utime);
	
	//icore_microp(data, NULL);
	
	return 0;
}

ICORE_STATIC
icuint32     icore_public_t(icore_class *cls, icore_class *dst, icore_class *src)
{
	uint32_t   i=0;
	struct _icore_public  *ptr=NULL;
			

 do{
	if(dst!=NULL && src!=NULL && dst->c_active!=NULL && src->c_active!=NULL){
		ptr= (struct _icore_public  *)src->b_set;
		
		if(cls == NULL){
			fatal("segment failed", __FILE__, __LINE__, 0);
			return ICORE_ERROR;
		}
		if(cls->g_set==NULL){
			fatal("segment failed", __FILE__, __LINE__, 0);
			return ICORE_ERROR;
		}
		if(src->session > MAX_TABLE ){
			src->session = 0;
		}
		//printf("__STACK__ (%d) %d, %d ,%d \n", __LINE__, cls->session, dst->session, src->session);
		cls->g_set = c_public_set(cls->g_set, src->session);
		if(cls->g_set == NULL){
			fatal("segment failed", __FILE__, __LINE__, 0);
			return ICORE_ERROR;
		}
		//printf("__STACK__ (%d) %d, %d ,%d \n", __LINE__, cls->session, dst->session, src->session);
		//printf("================ Process <%s><%d> \n", __FUNCTION__, __LINE__);
		if(src->status == ICORECOMPLETE){
			//printf("================ Process <%s><%d> \n", __FUNCTION__, __LINE__);
			 src->tm.utime = get_time_final(src->tm.min,src->tm.sec, &dst->tm.min, &dst->tm.sec );
			 // run process core table data
			 printf("@@@@@@@@@@@@@@@ COMPLETE (%d):  ID (%5d,%5d) \n", 
			   src->tm.utime , src->session, src->status ) ;
		     src->status = ICORELOAD;
		     //printf("================ Process <%s><%d> \n", __FUNCTION__, __LINE__);
		}else
		if(src->status == ICOREPROCESS){
			//printf("================ Process <%s><%d> \n", __FUNCTION__, __LINE__);
			src->tm.utime = get_time_final(src->tm.min,src->tm.sec, &dst->tm.min, &dst->tm.sec );
			if(src->tm.utime >= MAX_TIMEOUT){
				
				src->status = ICOREFAILED;
				printf("############### TIMEOUT (%d): ID (%5d,%5d), %s %d \n",
				 src->tm.utime ,
				 src->session, src->status ,__FILE__, __LINE__);
				
				//fatal("data time out ", __FILE__, __LINE__, 0);
		    }
		    //printf("================ Process <%s><%d> \n", __FUNCTION__, __LINE__);
		}
		//printf("================ Process <%s><%d> SEID(%d) \n", __FUNCTION__, __LINE__, src->session);
	    //printf("__STACK__ (%d) %d, %d ,%d \n", __LINE__, cls->session, dst->session, src->session);
		if(task[src->session]!=NULL && src->session < 100){
		   task[src->session]->status = src->status;
		}else{
			fatal("task control session", __FILE__, __LINE__, 0);
		}
		//printf("__STACK__ (%d) %d, %d ,%d \n", __LINE__, cls->session, dst->session, src->session);   
		//printf("================ Process <%s><%d> \n", __FUNCTION__, __LINE__);  
	    if(dst->c_active!=NULL) dst->c_active(src);
	    if(src->c_active!=NULL) src->c_active(dst);
	    //printf("__STACK__ (%d) %d, %d ,%d \n", __LINE__, cls->session, dst->session, src->session);
	    //printf("================ Process <%s><%d> \n", __FUNCTION__, __LINE__);
    }else{
    	fatal("stack memory ", __FILE__, __LINE__, 0);
    	
    	return ICORE_ERROR;
	}
  }while(0);
	return ICORE_SUCCESS;
}
	 
#define G_ADD_C(a,x)  a->cc.x += 1; 
	 
icore_statistics *
c_public_set(icore_statistics *ptr, uint32_t seid)
{
	if(ptr == NULL){
	    fatal("stack pointer", __FILE__, __LINE__, 0);
	    return NULL;
	} 
	if(seid == 0){
		G_ADD_C(ptr,c00);
	}else 
	if(seid == 1){
		G_ADD_C(ptr,c01);
	}else 
	if(seid == 2){
		G_ADD_C(ptr,c02);
	}else 
	if(seid == 3){
		G_ADD_C(ptr,c03);
	}else 
	if(seid == 4){
		G_ADD_C(ptr,c04);
	}else 
	if(seid == 5){
		G_ADD_C(ptr,c05);
	}else 
	if(seid == 6){
		G_ADD_C(ptr,c06);
	}else 
	if(seid == 7){
		G_ADD_C(ptr,c07);
	}else 
	if(seid == 8){
		G_ADD_C(ptr,c08);
	}else 
	if(seid == 9){
		G_ADD_C(ptr,c09);
	}else 
	if(seid == 10){
		G_ADD_C(ptr,c10);
	}else 
	if(seid == 11){
		G_ADD_C(ptr,c11);
	}else 
	if(seid == 12){
		G_ADD_C(ptr,c12);
	}else 
	if(seid == 13){
		G_ADD_C(ptr,c13);
	}else 
	if(seid == 14){
		G_ADD_C(ptr,c14);
	}else 
	if(seid == 15){
		G_ADD_C(ptr,c15);
	}else 
	if(seid == 16){
		G_ADD_C(ptr,c16);
	}else 
	if(seid == 17){
		G_ADD_C(ptr,c17);
	}else 
	if(seid == 18){
		G_ADD_C(ptr,c18);
	}else 
	if(seid == 19){
		G_ADD_C(ptr,c19);
	}else 
	if(seid == 20){
		G_ADD_C(ptr,c20);
	}else 
	if(seid == 21){
		G_ADD_C(ptr,c21);
	}else 
	if(seid == 22){
		G_ADD_C(ptr,c22);
	}else 
	if(seid == 23){
		G_ADD_C(ptr,c23);
	}else 
	if(seid == 24){
		G_ADD_C(ptr,c24);
	}else 
	if(seid == 25){
		G_ADD_C(ptr,c25);
	}else 
	if(seid == 26){
		G_ADD_C(ptr,c26);
	}else 
	if(seid == 27){
		G_ADD_C(ptr,c27);
	}else 
	if(seid == 28){
		G_ADD_C(ptr,c28);
	}else 
	if(seid == 29){
		G_ADD_C(ptr,c29);
	}else 
	if(seid == 30){
		G_ADD_C(ptr,c30);
	}else 
	if(seid == 31){
		G_ADD_C(ptr,c31);
	}else 
	if(seid == 32){
		G_ADD_C(ptr,c32);
	}else 
	if(seid == 33){
		G_ADD_C(ptr,c33);
	}else 
	if(seid == 34){
		G_ADD_C(ptr,c34);
	}else 
	if(seid == 35){
		G_ADD_C(ptr,c35);
	}else 
	if(seid == 36){
		G_ADD_C(ptr,c36);
	}else 
	if(seid == 37){
		G_ADD_C(ptr,c37);
	}else 
	if(seid == 38){
		G_ADD_C(ptr,c38);
	}else 
	if(seid == 39){
		G_ADD_C(ptr,c39);
	}else 
	if(seid == 40){
		G_ADD_C(ptr,c40);
	}else 
	if(seid == 41){
		G_ADD_C(ptr,c41);
	}else 
	if(seid == 42){
		G_ADD_C(ptr,c42);
	}else 
	if(seid == 43){
		G_ADD_C(ptr,c43);
	}else 
	if(seid == 44){
		G_ADD_C(ptr,c44);
	}else 
	if(seid == 45){
		G_ADD_C(ptr,c45);
	}else 
	if(seid == 46){
		G_ADD_C(ptr,c46);
	}else 
	if(seid == 47){
		G_ADD_C(ptr,c47);
	}else 
	if(seid == 48){
		G_ADD_C(ptr,c48);
	}else 
	if(seid == 49){
		G_ADD_C(ptr,c49);
	}else 
	if(seid == 50){
		G_ADD_C(ptr,c50);
	}else 
	if(seid == 51){
		G_ADD_C(ptr,c51);
	}else 
	if(seid == 52){
		G_ADD_C(ptr,c52);
	}else 
	if(seid == 53){
		G_ADD_C(ptr,c53);
	}else 
	if(seid == 54){
		G_ADD_C(ptr,c54);
	}else 
	if(seid == 55){
		G_ADD_C(ptr,c55);
	}else 
	if(seid == 56){
		G_ADD_C(ptr,c56);
	}else 
	if(seid == 57){
		G_ADD_C(ptr,c57);
	}else 
	if(seid == 58){
		G_ADD_C(ptr,c58);
	}else 
	if(seid == 59){
		G_ADD_C(ptr,c59);
	}else 
	if(seid == 60){
		G_ADD_C(ptr,c60);
	}else 
	if(seid == 61){
		G_ADD_C(ptr,c61);
	}else 
	if(seid == 62){
		G_ADD_C(ptr,c62);
	}else 
	if(seid == 63){
		G_ADD_C(ptr,c63);
	}else 
	if(seid == 64){
		G_ADD_C(ptr,c64);
	}else 
	if(seid == 65){
		G_ADD_C(ptr,c65);
	}else 
	if(seid == 66){
		G_ADD_C(ptr,c66);
	}else 
	if(seid == 67){
		G_ADD_C(ptr,c67);
	}else 
	if(seid == 68){
		G_ADD_C(ptr,c68);
	}else 
	if(seid == 69){
		G_ADD_C(ptr,c69);
	}else 
	if(seid == 70){
		G_ADD_C(ptr,c70);
	}else 
	if(seid == 71){
		G_ADD_C(ptr,c71);
	}else 
	if(seid == 72){
		G_ADD_C(ptr,c72);
	}else 
	if(seid == 73){
		G_ADD_C(ptr,c73);
	}else 
	if(seid == 74){
		G_ADD_C(ptr,c74);
	}else 
	if(seid == 75){
		G_ADD_C(ptr,c75);
	}else 
	if(seid == 76){
		G_ADD_C(ptr,c76);
	}else 
	if(seid == 77){
		G_ADD_C(ptr,c77);
	}else 
	if(seid == 78){
		G_ADD_C(ptr,c78);
	}else 
	if(seid == 79){
		G_ADD_C(ptr,c79);
	}else 
	if(seid == 80){
		G_ADD_C(ptr,c80);
	}else 
	if(seid == 81){
		G_ADD_C(ptr,c81);
	}else 
	if(seid == 82){
		G_ADD_C(ptr,c82);
	}else 
	if(seid == 83){
		G_ADD_C(ptr,c83);
	}else 
	if(seid == 84){
		G_ADD_C(ptr,c84);
	}else 
	if(seid == 85){
		G_ADD_C(ptr,c85);
	}else 
	if(seid == 86){
		G_ADD_C(ptr,c86);
	}else 
	if(seid == 87){
		G_ADD_C(ptr,c87);
	}else 
	if(seid == 88){
		G_ADD_C(ptr,c88);
	}else 
	if(seid == 89){
		G_ADD_C(ptr,c89);
	}else 
	if(seid == 90){
		G_ADD_C(ptr,c90);
	}else 
	if(seid == 91){
		G_ADD_C(ptr,c91);
	}else 
	if(seid == 92){
		G_ADD_C(ptr,c92);
	}else 
	if(seid == 93){
		G_ADD_C(ptr,c93);
	}else 
	if(seid == 94){
		G_ADD_C(ptr,c94);
	}else 
	if(seid == 95){
		G_ADD_C(ptr,c95);
	}else 
	if(seid == 96){
		G_ADD_C(ptr,c96);
	}else 
	if(seid == 97){
		G_ADD_C(ptr,c97);
	}else 
	if(seid == 98){
		G_ADD_C(ptr,c98);
	}else 
	if(seid == 99){
		G_ADD_C(ptr,c99);
	}else{
		printf("set (%d) null pointer <%s><%d> \n",seid, __FILE__, __LINE__);
	}

	return ptr;
}
	 
icuint32     icore_microp(icore_class *data, icore_readw       stream)
{
	uint32_t    i=0;
	uint32_t    cmin=0, csec=0;
	
	if(data == NULL)
	{
		fatal("segment failed", __FILE__, __LINE__, 0);
		return 0;
	}
	do{
		for(i=0;i<MAX_TABLE;i++){
			if(task[i]->status == ICOREPROCESS ){
				//task[i]->tm.utime = get_time_final(task[i]->tm.min,task[i]->tm.sec, &cmin, &csec );
			    //if(task[i]->tm.utime >= MAX_TIMEOUT ){
			    //    task[i]->status = ICOREFAILED;
			        //break;
				//}else{
				//printf("continue > %d \n", task[i]->status);
				 continue;
				//}
			}else
			if(task[i]->status == ICORECOMPLETE ){
			    continue;
			}
			//printf("task seid (%d) \n", task[i]->session );
	        if(task[i]->c_data!=NULL){
	          get_time_start(&task[i]->tm.min, &task[i]->tm.sec);
	          
	          task[i]->tm.utime = 0;
	          task[i]->c_data(task[i], stream);
	          break;
			}
		}
		
	}while(0);
	
	
	return 0;
}

icuint32     icore_control(icore_class *data, enum _icore_flags flags, uint32_t sessionid, void **param)
{
	#ifdef DEBUG_H
	printf("CTL : %d , FL(%d) SI(%d) \n", data, flags, sessionid);
	#endif
	
	if(flags == CREAD){
		if(sessionid>MAX_TABLE){
		   fatal("max table core", __FILE__, __LINE__, 0);
		} else{
			
			*param = (void*) task[sessionid]->status;
			//data->status = task[sessionid]->status;
			//printf(": %d \n", *param);
		}
	}else
	if(flags == CWRITE){
		if(sessionid>MAX_TABLE){
		   fatal("max table core", __FILE__, __LINE__, 0);
		} else{
			//printf("endline stack -\n", __FUNCTION__ );
			if(task[sessionid]!=NULL){
		       //printf("task status : %d , %d\n", task[sessionid]->status, param);
		       task[sessionid]->status =(enum _icore_status ) param;
		       if(data->status != task[sessionid]->status) data->status = task[sessionid]->status;
		    }else
		       fatal("task stack failed", __FILE__, __LINE__, 0);
		    //printf("endline stack +\n", __FUNCTION__ );
	    }
	}else 
	if(flags == CMESSAGE){
	   if(sessionid>MAX_TABLE){
		   fatal("max table core", __FILE__, __LINE__, 0);
		} else{
	        if(task[sessionid]!=NULL){
		       //printf("task1 status : %d , %x\n", task[sessionid]->status, param);
		       if(task[sessionid]->b_set->message!=NULL){
		          cmemcpy(task[sessionid]->b_set->message, (struct _dat_node*) param, sizeof(struct _dat_node ));
		       }else{
		       	  printf("ERR:%d - %d %x \n", __LINE__, sessionid, task[sessionid]->b_set->message );
			   }
			   //printf("task2 status : %d , %x\n", task[sessionid]->status, param);
		       if(data->status != task[sessionid]->status) {
			      if(data->b_set->message!=NULL){
				     cmemcpy(data->b_set->message,(struct _dat_node *)param, sizeof(struct _dat_node));	
			      }else{
			      	 printf("ERR:%d - %d %x \n", __LINE__, sessionid, data->b_set->message );
				  }
			   }
			   //printf("task3 status : %d , %x\n", task[sessionid]->status, param);
		    }else
		       fatal("task stack failed", __FILE__, __LINE__, 0);
	        
	        
        }
	}else{
		
	}
	
	return 0;
}
