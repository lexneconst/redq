
#include "rmsvw.h"

float  percent(uint32_t total_marks, uint32_t scored)
{
	float  per=0;
	do{
		if(total_marks==0){
		   fatal("number line ", __FILE__, __LINE__, 0);
		   break;	
		} 
		if(scored==0){
		   fatal("number line", __FILE__, __LINE__, 0);
		    break;
		}
		
		per = (float)scored / total_marks * 100.0;
		
	}while(0);
	return per;
}
