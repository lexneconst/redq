#include "rmsvw.h"

vss_task *task=NULL;

void * RUNAPI _vss_init(uint32_t addr)
{
	uint32_t   i=0;
    vss_node   *next, *prev;
    
    #ifdef DEBUG_H
    printf("<%s> init \n", __FUNCTION__);
    #endif
    
do{

	task = (vss_task *) cmalloc( sizeof(vss_task));
	if(task==NULL){
       fatal("allocate memory ", __FILE__, __LINE__, 0);
	   break;
	}	
	
	task->task = (vss_node *) cmalloc( sizeof(vss_node));
	if(task->task==NULL){
       fatal("allocate memory ", __FILE__, __LINE__, 0);
	   break;
	}
	
	task->count = 0;
	task->xpos = 0;
	task->ypos = 0;
	
	task->blocks = 0;
	
	task->task->first= NULL;
	task->task->next = NULL;
	task->task->status= DISABLE;
	task->task->seid = 0;
	task->task->usrid= 0;
	
	task->task->number= 0;
	task->task->length = MGC_SEEK1 * SSC_STREAM_BLOCK * 64;
	
    task->task->len_old = task->task->length;
	task->task->buffer = (int8_t*) cmalloc( task->task->length);
	if(task->task->buffer==NULL){
		fatal("allocate memory ", __FILE__, __LINE__, 0);
		break;
	}
	
	prev = task->task;
	for(i=0;i<=VSS_MAX;i++){
		
		next = (vss_node*) cmalloc(sizeof(vss_node));
		if(next==NULL){
			fatal("allocate memory failed", __FILE__, __LINE__, 0);
			break;
		}
		
		next->status = DISABLE;
		
		next->number = prev->number+1;
		next->seid = 0;  // default id
		next->usrid= next->number;  
		
		next->length = prev->length;
        next->len_old = next->length;
        if(next->length <= 0){
        	next->length = MGC_SEEK1 * SSC_STREAM_BLOCK * 64;
		}
		next->buffer = (int8_t*) cmalloc( next->length );
		if(next->buffer==NULL){
			fatal("allocate memory ", __FILE__, __LINE__, 0);
			break;
		}
		
		next->first= NULL;
		next->next = NULL;
		
		prev->next = next;
		prev = next;
	}
	
}while(0);
	
	return VSS_SUCCESS;
}
void * RUNAPI _vss_fini(uint32_t addr)
{
	vss_node  *tmp=NULL, *node=task->task;
	
	#ifdef DEBUG_H
	printf("<%s> fini \n", __FUNCTION__);
	#endif
	
	while(node) {
       tmp = node->next;
	   if(node->first!=NULL) cfree(node->first);
	   
	   CFREE(node->buffer);
	   node->length = 0;
	   
       cfree(node);
       node = tmp;
    }
	
	return VSS_SUCCESS;
}
void * RUNAPI _vss_count(uint32_t *count)
{
	#ifdef DEBUG_H
	printf("<%s> count \n", __FUNCTION__);	
	#endif
	
	*count = task->count;
	
	return VSS_SUCCESS;
}

void _vss_setblocks(uint32_t bset)
{
	uint32_t i=0;
	vss_node *next=NULL,*prev=NULL;
	
	prev = task->task;
	for(i=0;i<=VSS_MAX;i++){
		if(prev == NULL){
			break;
		}
		
		next = prev->next;
		
		if(next ==NULL){
			break;
		}
		
		if(next!=NULL && prev->status == DISABLE || prev->status == ENABLE && next->status == ENABLE){
		   // move struct and string
		  cmemcpy(prev, next, sizeof(vss_node));
		  cstrcpy(prev->buffer, next->buffer);
		  prev->length = next->length;
		  prev->len_old= next->len_old;
		  prev->seid   = next->seid;
		  prev->usrid  = next->usrid;
		  prev->status = ENABLE;
		  
		  //next->status = DISABLE;
		}else
		if(next != NULL && next->status == DISABLE){
			
			break;
		}
		prev = next;
	}
	
	task->blocks = bset;
}
uint32_t _vss_getblocks(void)
{
	uint32_t i=0;
	vss_node *next=NULL,*prev=NULL;
	uint32_t  curr = task->curr_max;
	
	curr = 0;
do{
	next = task->task;
	for(i=0;i<=VSS_MAX;i++){
		if(next == NULL){
			break;
		}
		if(task->curr_max >= i || task->curr_max <= i && next->status == ENABLE){
			curr = i;
			break;
		}
		prev = next->next;
		next = prev;
	}
	
}while(0);

	task->curr_max = curr;
	
	return curr;
}

uint32_t _vss_findblocks(void)
{
	uint32_t i=0;
	vss_node *next=NULL,*prev=NULL;
	
	task->find_max = 0;
do{
	if(task->count >= VSS_MAX){
		task->find_max = VSS_MAX;
		break;
	}
	next = task->task;
	for(i=0;i<=VSS_MAX;i++){
		if(next == NULL){
			break;
		}
		if(next!=NULL && next->status == DISABLE){
			task->find_max = i;
			break;
		}
		prev = next->next;
		next = prev;
	}
}while(0);

	return task->find_max;
}

void * RUNAPI _vss_update (vss_node *ptr)
{
	uint32_t i=0;
	uint32_t blocks = 0;
	vss_node *next,*prev;
	
	#ifdef DEBUG_H
	printf("<%s> update seid(%d) usrid(%d)\n", __FUNCTION__, ptr->seid, ptr->usrid);	
	#endif
	
	next = task->task;
	for(i=0;i<=VSS_MAX;i++){
		if(next == NULL){
			
			break;
		}
		//blocks = _vss_getblocks();
		blocks = _vss_findblocks();
		
		#ifdef DEBUG_H
		printf("+BLOCKS(%2d,%2d-%2d) = %2d,%2d \n", 
		      task->blocks, ptr->usrid, task->count, 
		      ((task->blocks - ptr->usrid) + task->count ) - blocks,
		      blocks
		    );
		#endif
		
		if(blocks == i && next->status==DISABLE){
			//blocks += 1;
			
			next->seid = ptr->seid;
			//printf("L: %d-%d \n", next->length, ptr->length);
			if(ptr->length > next->len_old){
				fatal("stak overflow", __FILE__, __LINE__, 0);
				next->status = DISABLE;
				break;
			}
			next->status = ENABLE;
			next->length = ptr->length;
			next->len_old= ptr->len_old;
			//printf("A: [%d-%d]\n", next->length, next->len_old);
			//cmemset(next->buffer,0, next->length);
			//printf("B: [%d-%d]\n", next->length, next->len_old);
			if(next->buffer!=NULL){
			    cmemcpy(next->buffer, ptr->buffer, next->length );
		    }else{
		    	next->buffer = (int8_t*) crealloc(next->buffer, next->len_old );
		    	if(next->buffer == NULL){
		    		fatal("allocate memory ", __FILE__, __LINE__, 0);
		    		break;
				}
		    	fatal("segment memory ", __FILE__, __LINE__, 0);
		    	break;
			}
			//printf("C: [%d-%d]\n", next->length, next->len_old);
			
			break;
		}
		
		prev = next->next;
		next = prev;
	}
	if(task->count<VSS_MAX){	
 	   task->count += 1;
    }
	
	return VSS_SUCCESS;
}
void * RUNAPI _vss_read (vss_node **ptr, uint32_t seid)
{
	uint32_t i=0;
	uint32_t blocks=0;
	
	vss_node *next,*prev;
    vss_node *str = (vss_node*) *ptr;
	
	#ifdef DEBUG_H
	printf("<%s> read (%x) seid (%d)\n", __FUNCTION__, str, seid);	
	#endif
	
do{
	if(str == NULL){
		fatal("segment memory ", __FILE__, __LINE__, 0);
		break;
	}
	
	next = task->task;
	
	for(i=0;i<=VSS_MAX;i++){
	    if(next==NULL || i >= VSS_MAX){
	    	break;
		}
		#ifdef DEBUG_H
		printf("%s :: seid(%d)(%d) usrid(%d) \n", __FUNCTION__, next->seid, seid, next->usrid );
	    #endif
	    
	    #ifdef DEBUG_H
	    printf("TASK_BLOCK(%d) \n", task->blocks );
	    #endif
		
		blocks = _vss_getblocks ();
	    #ifdef DEBUG_H
		printf("-BLOCKS(%2d,%2d-%2d) = %2d,%2d \n", 
		      task->blocks, str->usrid, task->count, 
		      ((task->blocks - str->usrid) + task->count ) - blocks,
		      blocks
		);
		#endif
		
		if(blocks  == i  && ptr!=NULL && next!=NULL && next->status==ENABLE){
	       
			str->first = next;
			str->next  = NULL;
			
			str->status = next->status ? next->status : ENABLE;
	    	str->seid   = next->seid;
	    	str->usrid  = next->usrid;
	    	
	    	str->length = next->length;
	    	str->len_old= next->len_old;
	    	str->number = next->number;
	    	
	    	if(str->buffer!=NULL){
	    		cmemcpy(str->buffer, next->buffer, next->length);
			}else{
				str->buffer = (int8_t*) crealloc(str->buffer, next->len_old );
				if(str->buffer==NULL){
					fatal("allocate memory ", __FILE__, __LINE__, 0);
				}else{
					cmemcpy(str->buffer, next->buffer, next->length);
				}
			}
			
	    	#ifdef DEBUG_H
	    	 printf("%s ::: LEN(%d) pointer(%x) \n", 
			   __FUNCTION__,
			   next->length,
			   next->buffer);
	    	#endif
	    	
	    	break;
		}
		
	    prev = next->next;
		next = prev;
	}
	
}while(0);
	
	if(str->status == DISABLE){
       printf("+SMEM (%s=%d) - (%d, %d) \n", __FILE__, __LINE__, blocks, str->status );
	  //str->status = ENABLE;
    }
	*ptr = (vss_node *) str;
	
	return VSS_SUCCESS;
}
void * RUNAPI  _vss_free (vss_node *ptr) // free chiled
{
	uint32_t i=0;
	vss_node *next,*prev;
	
	#ifdef DEBUG_H
	printf("<%s> free \n", __FUNCTION__);	
	#endif
	
	next = task->task;
	for(i=0;i<VSS_MAX;i++){
		if(next==NULL){
			break;
		}
		if(task->count == i){
	        
			_vss_setblocks(0);
			
	        next->status = DISABLE;
	        
	        break;
		}
	    
		prev = next->next;
		next = prev;
	}
	
	if(task->count>0) task->count-=1;
	
	return VSS_SUCCESS;
}

uint32_t  RUNAPI _vss_max(void)
{
	return VSS_MAX;
}

