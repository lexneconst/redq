#include "micropw.h"


icuint32     microp_init(icore_class *cls)
{
	return 0;
}

/*
void* thread_procw(void *arg)
{
      icore_class  *cls = (icore_class*) arg;
      
	  do{
	  	printf("Thread (%s)(%s) (%d) \n", __FILE__, __FUNCTION__, __LINE__);
	  	sleep(1);
	  }	while(1);
	  
	  return NULL;
}
*/

icuint32     microp_run(icore_class *cls)
{
	int err;
	pthread_t  threadid;
	
	
	err = pthread_create(&threadid, NULL, &thread_procl, (void*)cls);
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
	return 0;	
}


icuint32     microp_fini(icore_class *cls)
{
	return 0;	
}

#ifdef WIN32_H


HWAVEIN       WaveInHandle;
WAVEHDR       WaveHeader[2];
BOOL          InRecord=FALSE;
BOOL          InStart=TRUE;
unsigned char DoneAll;

unsigned int   wstart = 0;

void _kmgcsp(const char *message, const char *func, unsigned int line)
{
   #ifndef __INFO
   printf("[%s]=%6d, %12s (%16s)\n", __FUNCTION__, line, func, message);
   #endif
   return ;
}




uint32_t _libmgc_surW(libssc_t *ynl, 
         const c_int8_t *buf, uint32_t len, libbuff_t *data)
{ 
   uint32_t        i,j,count1=0,count2=0;//,icount=0;
   uint32_t         ret=0,pt1=0,pt2=0;
   //char                astr,bstr;
   //char                *sbuf = NULL;
   //char                *ptr=NULL;
   //struct _mgc_point_t    zpy;
   uint8_t                 ndata[LIBMGC_POINT];
   
   //uint8_t                 zstr[4];
   //uint32_t                zbuf[SSC_STREAM_MAX];

do{
          
   if(data==NULL){
      fatal("stack memory failed.,", __FILE__,  __LINE__, 0);
      break;
   }
   if(data->buffer == NULL)
   {
       fatal("stack memory failed.,", __FILE__, __LINE__, 0);
       break;
   }
   if(ynl==NULL){
       fatal("stack memory failed.,", __FILE__, __LINE__, 0);
       break;
   }
   //printf("<%s> %d \n", __FUNCTION__, ret); 
   //cmemset(zbuf,0 ,sizeof(zbuf));
   ndata[0]=0;
   ndata[1]=0;
   
   for(i=0;i<len;i+= LIBMGC_POINT){
      //for(j=0;j<LIBMGC_POINT;j++){
      //  astr=ndata[j]  = buf[i];
        //#ifndef __NOSIE__
        ndata[1]  = 0.333* buf[i] + (1.0-0.333)* ndata[0];
        //#endif
      //}
      //zbuf[ count ] = ndata[0];  
      /*#ifndef LOW_NOISE
      bstr=sbuf[ count ] = buf[i];                
      astr=sbuf[ count ] = 0.333 * 
          buf[ i ] + ( 1.0 - 0.333 ) * sbuf[ count-1 ];
      #else
          sbuf[ count ] = buf[i]; 
      #endif
     // */
      //ret = mgc_check(ndata[0]);
     // if(ret) {
      	 //break;
	  //}
      //#ifndef DEBUG_H
	  if(/*abs(buf[i]) >= 0 && abs(buf[i]) <= LIBPUBSIZEA &&*/
	     abs(ndata[1]) >= LIBPUBSIZEW && abs(ndata[1]) <= LIBPUBSIZEA ){
          //printf("%3d ", ndata[0]);
		  //if(count > (len /128)){
		  	  //printf("=======================================\n");
		  	// printf("ID( %d ) LEN ( %d ) %d \n", i, len, len/128);
		  	  //printf("=======================================\n");
		      //ret = 1;
           //   break;
		  //}
          count1+=1;
		  //printf("%5d %5d ", (uint32_t)zbuf[count], LIBPUBSIZE);
          //printf("%d ", zbuf[count]);
          //if(icount==0){
             //zpt->cxstart = i;
          //}else{
             //zpt->cxfinal = i;
          //}
          //icount += 1;
      //}//else 
      //if(/*buf[i] <= 0 && */ndata[0] <= LIBPUBSIZEW){
      	//  count2+=1;
	  }else{
	  	  //printf("%d ", ndata[0]);
	  	  count2+=1;
	  }
      //#endif
      ndata[0] = ndata[1];
   } 
   
   if(count1<= 0) count1++;
   if(count2<= 0) count2++;
   
   pt1 = percent(len,count1);
   pt2 = percent(len,count2);
   
   #ifdef DEBUG_H
   printf("<%s> %d Percent %6d, %6d - %6d | (%3d) (%3d) (%3d)\n", __FUNCTION__, ret, 
    	  count1,
    	  count2,
    	  count1+count2,
		  pt1,
		  pt2,
		  pt1+pt2
		  );
   #endif
   if(pt1 >= 45 && pt2 <= 55 ){
   	    #ifdef DEBUG_H
         printf("<%s> %d Percent %6d, %6d - %6d | (%3d) (%3d) (%3d)\n", __FUNCTION__, ret, 
    	  count1,
    	  count2,
    	  count1+count2,
		  pt1,
		  pt2,
		  pt1+pt2
		  );
        #endif
   
   	    ret=1;
   }
   ///printf("\n");
    
   if(ret){
    
    
     if(ynl->check == 0){   
        #ifdef __INFO_SPD   
        printf("-------------------------------------\n");      
        #endif
     }
     #ifdef __INFO_SPD
     printf("  +MGC_SEEK1[%d] LIBMGC_POINT[%d] SSC[%d][%d]\n", 
         MGC_SEEK1,
         LIBMGC_POINT,
         SSC_STREAM_MAX, 
         SSC_STREAM_MAX*LIBMGC_POINT
         );   
     #endif
     
     if(ynl->check == 0){
        _kmgcsp(" INIT ", __FUNCTION__, __LINE__);
        data->block =1;
        
        cmemset(data->buffer, 0, len);
        cmemcpy(data->buffer, buf, len);
        
        data->length = len;
        ynl->block=0;
        
        ynl->block+= 1;      
        #ifdef __SS_DBG_
        printf("[%s]DBG_HANDLE =INIT(%d) \n",__FUNCTION__, 
                             SSC_STREAM_MAX*ynl->block); 
        #endif
        ynl->check = 1;
     }else{
        _kmgcsp(" UPDATE ", __FUNCTION__, __LINE__);
        
        data->block += 1;
        cmemcpy(data->buffer+data->length, buf, len);
        data->length += len;
        
        #ifdef __SS_DBG_
        printf("[%s]DBG_HANDLE =UPDATE(%d) \n",__FUNCTION__, 
                             SSC_STREAM_MAX*ynl->block); 
        #endif
        ynl->block += 1;
        if(ynl->block >=  SSC_STREAM_BLOCK-1)
        {
            _kmgcsp(" NEWLINE ", __FUNCTION__, __LINE__);
            goto NEWLINE;
        }
        ynl->check = 2;
     }    
   }else{
     NEWLINE:
     if(ynl->check == 1 || ynl->check == 2){
        _kmgcsp(" FINAL ", __FUNCTION__, __LINE__);
        //data->block += 1;
        cmemcpy(data->buffer+data->length, buf, len);
        data->length += len;
        
        if(data->length <=0 || data->length > MGC_SEEK1*SSC_STREAM_BLOCK ) data->length = MGC_SEEK1 * data->block ;
        
        #ifdef __INFO_SPD
        printf("-------------------------------------\n");        
        #endif
        ynl->check = 0;
        ynl->length = SSC_STREAM_MAX*ynl->block;
        
         
        if(ynl->block >= 2) ret = 2;
        
     }else{
       #ifdef __INFO_SPD
        printf("  -MGC_SEEK1[%d] LIBMGC_POINT[%d] SSC[%d][%d]\n", 
         MGC_SEEK1,
         LIBMGC_POINT,
         SSC_STREAM_MAX, 
         SSC_STREAM_MAX*LIBMGC_POINT
         );  
       #endif
     }
   }
   
}while(0);
 
   //if(sbuf!=NULL)
   //  free_t(sbuf), sbuf=NULL;
   //printf("<%s> %d %x\n", __FUNCTION__, ret, LIBPUBSIZE);
     
   return ret;
}


//UPBUFFER_T      *r_stream=NULL;
//unsigned int nstart = 0;

unsigned int _libmgc_kstart( void )
{
      //_mgc_begin = 0;
      //_mgc_final = 0;
do{
    
     // r_stream = (UPBUFFER_T*) cmalloc(sizeof(UPBUFFER_T));
      //if(r_stream==NULL){
      //   fatal("allocate memory failed.,", __FILE__, __LINE__, 0);
      //   break;
      //}
      //r_stream->block  = 0;
     // r_stream->length = MGC_SEEK1 * SSC_STREAM_BLOCK * 2 /*64*/;
     // r_stream->buffer = (int8_t *) cmalloc( r_stream->length +1 ); 
     // if(r_stream->buffer==NULL){
     //    fatal("allocate memory failed.,", __FILE__, __LINE__, 0);
     //    break;
     // }
      
      //rtdata->next = rtdata;   
}while(0);
     
      return 1;
}

unsigned int _libmgc_kend( void)
{
    //if(r_stream!=NULL){
    //   CLEAN(r_stream->buffer);
    //   r_stream->length=r_stream->block=0;
    //}
    
   // CLEAN(r_stream);

             
    return 0;
}
uint32_t  _libmgc_mainW(const c_int8_t *data, uint32_t length, libssv_t **ppv)
{
     //struct _mgc_point_t      zpt;
     unsigned int             ret=0;
     unsigned int             nstate=0;
     libssv_t                 *pnv=(libssv_t *)(*ppv);


do{
        
     if(pnv == NULL){
        fatal("stack memory failed.,", __FILE__, __LINE__, 0);
        break;
     }//
     if(data==NULL){
        fatal("buffer memory failed.,", __FILE__, __LINE__, 0);
        break;
     }
     if(length <=0){
        fatal("size of memory ", __FILE__, __LINE__, 0);
        break;
     }
     //if(!nstart){ nstart = 1; goto END; }   // start detect stream   
    
     #ifdef INFO_SDBG_
       printf("STATE CALLBACK [%s]#%d\n",__FUNCTION__, __LINE__);
     #endif     
     
        
     nstate = _libmgc_surW(pnv->xssv, data, length,  pnv->data);
     if(nstate == 2){ // true +
         //memcpy((*ppv), rtdata , sizeof(libssc_t )); 
         //_libmgc_copyW(rtdata,&pnv->temp);
         if(pnv->data!=NULL){
             //memcpy(pnv->data, r_stream, sizeof(UPBUFFER_T));
             if(pnv->data->buffer == NULL){
                 
                 fatal("stack memory failed.,", __FILE__, __LINE__, 0);
                 
             }else{
                 //cmemcpy(pnv->data->buffer, r_stream->buffer, r_stream->length);
                 //pnv->data->length = r_stream->length;
                 //cmemset(data, 0, length);
                 //printf(">> OK [%d]= [%d] length(%d)\n",MGC_SEEK1, pnv->data->length, r_stream->length);
             } 
         }else{
            fatal("stack memory failed.,", __FILE__, __LINE__,0 );
         }
         //_libmgc_packW(pnv);
         
         *ppv = pnv;
         
         ret = 1;
         break;
     }else 
     if(nstate == 1){ // true 
         //printf("WAIT [%s] \n", __FUNCTION__);
         ret = 0;
         break;
     }else{
        //*ppv = NULL;
        ret = 0;
        break;
     }
  
}while(0);
     #ifdef _SP_DBG
     printf("Return [%d] \n", ret);
     #endif
     
     return ret;
}


BOOL SetInputDevice (unsigned int inDev)
{
    HMIXER hMixer   = NULL;
    int    inDevIdx = -1;

	if (mixerOpen(&hMixer, 0, 0, 0, MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR)
	{
        return FALSE;
	}

	// get dwLineID
	MIXERLINE mxl;
	mxl.cbStruct        = sizeof(MIXERLINE);
	mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;

	if (mixerGetLineInfo((HMIXEROBJ)hMixer, &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
	{
        mixerClose (hMixer);
		return FALSE;
	}

	// get dwControlID
	MIXERCONTROL      mxc;
	MIXERLINECONTROLS mxlc;
    DWORD             dwControlType = MIXERCONTROL_CONTROLTYPE_MIXER;

	mxlc.cbStruct      = sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID      = mxl.dwLineID;
	mxlc.dwControlType = dwControlType;
	mxlc.cControls     = 0;
	mxlc.cbmxctrl      = sizeof(MIXERCONTROL);
	mxlc.pamxctrl      = &mxc;

	if (mixerGetLineControls((HMIXEROBJ)hMixer, &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR)
	{
		// no mixer, try MUX
		dwControlType      = MIXERCONTROL_CONTROLTYPE_MUX;
		mxlc.cbStruct      = sizeof(MIXERLINECONTROLS);
		mxlc.dwLineID      = mxl.dwLineID;
		mxlc.dwControlType = dwControlType;
		mxlc.cControls     = 0;
		mxlc.cbmxctrl      = sizeof(MIXERCONTROL);
		mxlc.pamxctrl      = &mxc;
		if (mixerGetLineControls((HMIXEROBJ)hMixer, &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE) != MMSYSERR_NOERROR)
		{
            mixerClose (hMixer);
			return FALSE;
		}
	}

	if (mxc.cMultipleItems <= 0)
	{
        mixerClose (hMixer);
		return FALSE;
	}

	// get the index of the inDevice from available controls
	MIXERCONTROLDETAILS_LISTTEXT*  pmxcdSelectText =
      (MIXERCONTROLDETAILS_LISTTEXT*)cmalloc( sizeof( MIXERCONTROLDETAILS_LISTTEXT ));
    //memcpy(pmxcdSelectText,  
    //       MIXERCONTROLDETAILS_LISTTEXT[mxc.cMultipleItems], 
    //       sizeof(MIXERCONTROLDETAILS_LISTTEXT ) );

	if (pmxcdSelectText != NULL)
	{
		MIXERCONTROLDETAILS mxcd;

		mxcd.cbStruct       = sizeof(MIXERCONTROLDETAILS);
		mxcd.dwControlID    = mxc.dwControlID;
		mxcd.cChannels      = 1;
		mxcd.cMultipleItems = mxc.cMultipleItems;
		mxcd.cbDetails      = sizeof(MIXERCONTROLDETAILS_LISTTEXT);
		mxcd.paDetails      = pmxcdSelectText;

		if (mixerGetControlDetails ((HMIXEROBJ)hMixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_LISTTEXT) == MMSYSERR_NOERROR)
		{
			// determine which controls the inputDevice source line
            DWORD dwi;
			for (dwi = 0; dwi < mxc.cMultipleItems; dwi++)
			{
				// get the line information
				MIXERLINE mxl;
				mxl.cbStruct = sizeof(MIXERLINE);
				mxl.dwLineID = pmxcdSelectText[dwi].dwParam1;
				if (mixerGetLineInfo ((HMIXEROBJ)hMixer, &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_LINEID) == MMSYSERR_NOERROR && mxl.dwComponentType == inDev)
				{
					// found, dwi is the index.
					inDevIdx = dwi;
//					break;
				}
			}

		}
        
        //if(pmxcdSelectText)
		 CLEAN(pmxcdSelectText);//,pmxcdSelectText=NULL;
	}

    if (inDevIdx < 0)
    {
        mixerClose (hMixer);
        return FALSE;
    }

	// get all the values first
    MIXERCONTROLDETAILS_BOOLEAN* pmxcdSelectValue = 
      (MIXERCONTROLDETAILS_BOOLEAN*)cmalloc( sizeof(MIXERCONTROLDETAILS_BOOLEAN)  );
    //memcpy(pmxcdSelectValue, MIXERCONTROLDETAILS_BOOLEAN[mxc.cMultipleItems], 
    //   sizeof(MIXERCONTROLDETAILS_BOOLEAN));
	if (pmxcdSelectValue != NULL)
	{
		MIXERCONTROLDETAILS mxcd;
		mxcd.cbStruct       = sizeof(MIXERCONTROLDETAILS);
		mxcd.dwControlID    = mxc.dwControlID;
		mxcd.cChannels      = 1;
		mxcd.cMultipleItems = mxc.cMultipleItems;
		mxcd.cbDetails      = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
		mxcd.paDetails      = pmxcdSelectValue;
		if (mixerGetControlDetails((HMIXEROBJ)hMixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE) == MMSYSERR_NOERROR)
		{
			// ASSERT(m_dwControlType == MIXERCONTROL_CONTROLTYPE_MIXER || m_dwControlType == MIXERCONTROL_CONTROLTYPE_MUX);

			// MUX restricts the line selection to one source line at a time.
			if (dwControlType == MIXERCONTROL_CONTROLTYPE_MUX)
			{
				ZeroMemory(pmxcdSelectValue, mxc.cMultipleItems * sizeof(MIXERCONTROLDETAILS_BOOLEAN));
			}

			// Turn on this input device
            pmxcdSelectValue[inDevIdx].fValue = 0x1;

			mxcd.cbStruct       = sizeof(MIXERCONTROLDETAILS);
			mxcd.dwControlID    = mxc.dwControlID;
			mxcd.cChannels      = 1;
			mxcd.cMultipleItems = mxc.cMultipleItems;
			mxcd.cbDetails      = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
			mxcd.paDetails      = pmxcdSelectValue;
			if (mixerSetControlDetails ((HMIXEROBJ)hMixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
			{
                //if(pmxcdSelectValue) 
                 CLEAN(pmxcdSelectValue);//,pmxcdSelectValue=NULL;
                 
                mixerClose (hMixer);
                return FALSE;
			}
		}

		//if(pmxcdSelectValue)
          CLEAN(pmxcdSelectValue);//,pmxcdSelectValue=NULL;
	}

    mixerClose (hMixer);
    return TRUE;
}

void* thread_procl(void *arg){
	
    MMRESULT     err;
	WAVEFORMATEX waveFormat;
	WAVEINCAPSA  wcaps;
	HANDLE       waveInThread;
	DWORD        num,i;
	TCHAR        buf[MAX_PATH];
	int          result;
	pthread_t   threadid;
	//SOCALLBACK_T *cb = (SOCALLBACK_T*) ptr;
    
do{
    
    
    /*
    args->fsinit = wave_fsinit;
    if(args->fsinit == NULL){
        fatal("stack memory failed.,", __FILE__, __FUNCTION__, __LINE__);
        break;
    }
    args->fsplay = wave_fsplay;
    if(args->fsplay == NULL){
        fatal("stack memory failed.,", __FILE__, __FUNCTION__, __LINE__);
        break;
    }
    args->fsfree = wave_fsfree;
    if(args->fsfree == NULL){
        fatal("stack memory failed.,", __FILE__, __FUNCTION__, __LINE__);
        break;
    } 
    */
    
    
    //if(cb != NULL){
	//   
    //   memcpy(args, cb, sizeof(SOCALLBACK_T));
	   
	//}else
	//  fatal("callback function", __FILE__, __FUNCTION__, __LINE__);

	
	/*
	result = pthread_create(&threadid, NULL, &thread_procw, (void*)arg);
	   if (result != 0)
 	   {
	    #ifdef DEBUG_H
        printf("can't create thread :[%s]\n", strerror(result));
        #endif
       }else{
   	    #ifdef DEBUG_H
        printf("Thread created successfully\n");
        #endif
	   }
	   */
   waveInThread=CreateThread(NULL,0,
	        (LPTHREAD_START_ROUTINE)waveInProc,(void *)arg,0,(PDWORD)&err);
	if(waveInThread==NULL ){
		fatal("Can't create WAVE recording thread X 0x00 ", 
           __FILE__,  __LINE__, 0); //,(long)GetLastError()
		return 0;
 	}
	//CloseHandle(waveInThread);
	
	num = waveInGetNumDevs(); 
    for( i=0;i<num;i++){
        waveInGetDevCapsA(i, &wcaps, sizeof(wcaps));
                      
        sprintf(buf,"caps ID %d %s ", i, wcaps.szPname); 
        //printf("[DEV] %s\n", buf); 
                     
    }
     //MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT
     
   if (SetInputDevice (MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE) == FALSE)
        {
        //printf("MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE> %s \n", __FUNCTION__);
		if (SetInputDevice (MIXERLINE_COMPONENTTYPE_SRC_ANALOG) == FALSE)
				{
			  //printf("MIXERLINE_COMPONENTTYPE_SRC_ANALOG> %s \n", __FUNCTION__);		
			 if (SetInputDevice (MIXERLINE_COMPONENTTYPE_SRC_LAST) == FALSE)
				{
					//printf("MIXERLINE_COMPONENTTYPE_SRC_LAST> %s \n", __FUNCTION__);
					//if(SetInputDevice(MIXERLINE_COMPONENTTYPE_SRC_DIGITAL )==FALSE){
					//	printf("MIXERLINE_COMPONENTTYPE_SRC_DIGITAL> %s \n", __FUNCTION__);
					//	if(SetInputDevice(MIXERLINE_COMPONENTTYPE_SRC_LINE )==FALSE){
					//	    printf("MIXERLINE_COMPONENTTYPE_SRC_LINE> %s \n", __FUNCTION__);
						
					//    }
				    //}
                                
				 }
		}
    }
                                   
	ZeroMemory(&WaveHeader[0],sizeof(WAVEHDR)*2);
	
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels  = 2;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nBlockAlign    = waveFormat.nChannels*(waveFormat.wBitsPerSample/8);
	waveFormat.nAvgBytesPerSec= waveFormat.nSamplesPerSec*waveFormat.nBlockAlign;
	waveFormat.cbSize     = 0;
	
	err = waveInOpen(&WaveInHandle,WAVE_MAPPER, &waveFormat, (DWORD)err, 0, CALLBACK_THREAD );
	if(err){
		fatal("Can't open WAVE In Device ", 
          __FILE__, __LINE__, 0);
		return NULL;
	}else{
		
		//printf("[WAVE] size [%d] [%d] \n", 
        //  waveFormat.nAvgBytesPerSec<<1, waveFormat.nAvgBytesPerSec );
		
		WaveHeader[1].dwBufferLength = 
           WaveHeader[0].dwBufferLength = 
               MGC_SEEK1 ; //120000
              //waveFormat.nAvgBytesPerSec<<1;
		if(!(WaveHeader[0].lpData=(char *) cmalloc(WaveHeader[0].dwBufferLength *2) /*VirtualAlloc(0,WaveHeader[0].dwBufferLength *2, MEM_COMMIT, PAGE_READWRITE)*/))
	    {
	    	fatal("Error: Can't allocate memory for WAVE buffer!\n",__FILE__,  __LINE__, 0);
	    	
	    }else{
	    	
	    	WaveHeader[1].lpData = WaveHeader[0].lpData+WaveHeader[0].dwBufferLength;
	    	if((err=waveInPrepareHeader(WaveInHandle,&WaveHeader[0],sizeof(WAVEHDR)))){
	           		
	    		fatal("Error: preparing WAVEHDR 1!", __FILE__,  __LINE__, 0 );
	    	}else{
	    		
	    		if((err=waveInPrepareHeader(WaveInHandle,&WaveHeader[1],sizeof(WAVEHDR)))){
	    	       fatal("Error: preparing WAVEHDR 2!", __FILE__,  __LINE__ , 0);	
					 	
	    		}else{
	    			
	    			if((err=waveInAddBuffer(WaveInHandle,&WaveHeader[0],sizeof(WAVEHDR))))
                    {
                    	fatal("Error: queueing WAVEHDR 1! ",__FILE__, __LINE__, 0);
                    }else{
                    	
                    	
                    	if((err=waveInAddBuffer(WaveInHandle, &WaveHeader[1],sizeof(WAVEHDR)))){
                    		
                    		  fatal("Error: queueing WAVEHDR 2! ",__FILE__,  __LINE__, 0);
							  DoneAll =1;
                    		  goto abort;
                    	}else{
                    		
                    		InRecord = TRUE;
                    		if((err=waveInStart(WaveInHandle))){
                    			fatal("Error: starting record! ",  __FILE__, __LINE__, 0);
                    		}else{
                    			//printf("++++++++++++++ Recording has started. \n");
                    			while(InRecord){
                    				
                                    sleep(1);
                    			}
                    			
                    		}
                    		abort:
                    			InRecord = FALSE;
                    		
                    		waveInReset(WaveInHandle);
                    		
                    		while(DoneAll < 2) Sleep(100);
                    	}
                    }    			
                    
                    if((err=waveInPrepareHeader(WaveInHandle,&WaveHeader[1],sizeof(WAVEHDR)))){
                    	fatal("Error: unpreparing WAVEHDR 2! ", __FILE__, __LINE__, 0);
                    }
	    		}
	    	    if((err=waveInPrepareHeader(WaveInHandle,&WaveHeader[0],sizeof(WAVEHDR)))){
	    	    	fatal("Error: unpreparing WAVEHDR 1! ", __FILE__, __LINE__, 0);
	    	    }
	    	}
	    }
	
	}
	
	do{
		err= waveInClose(WaveInHandle);
		if(err){
			fatal("Error: Can't close WAVE In Device ", __FILE__,  __LINE__, 0);
		}
	}while(err);
	
	CLEAN(WaveHeader[0].lpData);
	//if(WaveHeader[0].lpData) 
	  //VirtualFree(WaveHeader[0].lpData, 0, MEM_RELEASE);
	
	SetInputDevice (MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE);  
	
}while(0);

	return ERROR_SUCCESS;
}

DWORD WINAPI waveInProc(LPVOID arg)
{
	MSG   msg;
	uint32_t      length=0;
	char          *pbuffer=NULL;
	//libssc_t     *uigen=NULL;
    icore_class  *args = (icore_class*) arg;
    unsigned int  ustate=0;
    libssv_t     *ssv=NULL;
    //libcmdf_t     nsyst;
    //libcmdf_t    *qdate=NULL, *qtime=NULL;
    SYSTEMTIME    systime;
    
do{
    
    if(args ==NULL){
       fatal("import thread paramters", __FILE__,  __LINE__, 0);
       break;
    }
    
    ssv = (libssv_t *) cmalloc (sizeof(libssv_t));
    if(ssv == NULL){
       fatal("allocate memory failed.,", __FILE__,  __LINE__, 0);
       break;
    }
    cmemset(ssv, 0, sizeof(libssv_t)); 
    
    #ifdef __STN_BACK_H
    ssv->buffer = (char **) cmalloc(512);
    if(ssv->buffer == NULL){
       fatal("allocate memory failed.,", __FILE__,  __LINE__, 0);
       break;
    }
    #endif
    
    ssv->xssv = (libssc_t *) cmalloc (sizeof(libssc_t));
    if(ssv->xssv  == NULL){
       fatal("allocate memory failed.,", __FILE__, __LINE__, 0);
       break;
    }
    
    ssv->data = (libbuff_t*) cmalloc (sizeof(libbuff_t));
    if(ssv->data == NULL){
        fatal("allocate memory failed.,", __FILE__,  __LINE__, 0);
        break;
    }
    ssv->data->length = MGC_SEEK1 * SSC_STREAM_BLOCK * 2;
    ssv->data->buffer = (c_int8_t *) cmalloc ( ssv->data->length );
    if(ssv->data->buffer==NULL){
        fatal("allocate memory failed.,", __FILE__, __LINE__, 0);
        break;
    }
    ssv->xssv->check=0;
    
    //if(args->fsinit!=NULL) args->fsinit();
    //printf("Initialized %s = %d \n", __FUNCTION__, __LINE__);
    _libmgc_kstart();
    
	while(GetMessage(&msg,0,0,0)==1){
		switch(msg.message){
			
			case MM_WIM_DATA:
			{
				if(((WAVEHDR *)msg.lParam)->dwBytesRecorded)
				{
					 if(!wstart){ wstart=1; goto SKIP; } // detected stream 
                      
                      /*
                      if(args->fsplay!=NULL){ 
                         //printf(" Update Player [%s] \n", __FUNCTION__);                     
                        args->fsplay( (char *) ((WAVEHDR *)msg.lParam)->lpData ,
                         (unsigned int)((WAVEHDR *)msg.lParam)->dwBytesRecorded );
                         //printf(" Update Player [%s] \n", __FUNCTION__);
                      }else{
                         fatal("invalid function ", __FILE__, __FUNCTION__, __LINE__);
                      }
                      */
                      //printf("LENGTH: %d \n", length); 
					 
                      
                     
                     pbuffer = (char *) ((WAVEHDR *)msg.lParam)->lpData;
					 length  = (uint32_t) ((WAVEHDR *)msg.lParam)->dwBytesRecorded;
					 
					 
                     if((ustate=_libmgc_mainW( pbuffer , length , &ssv))==1){
                        if(ssv !=NULL){
                        
                        #ifdef __DBG_HH_ 
                        printf("[%8d] [%8d] [%8d] \n", 
                             uigen->block,
                             uigen->size, 
                             uigen->length );
                         #endif
                        // _lib_private_r( ssv, NULL );
                        
                         
                         if(ssv!=NULL){
                           if(ssv->xssv!=NULL && ssv->data!=NULL){
                                 
                               //if(args->fstream!=NULL){
                                 if(ssv->data->length>0 && ssv->data->buffer!=NULL){
								   if(args->c_microp!=NULL ){
									  icore_read ptr = {
									       ssv->data->buffer,
									       ssv->data->length
									  };
								      args->c_microp(args, (icore_readw)&ptr);
								   }
                                  // printf("################ Microphone (%s)=(%d) \n", __FILE__, __LINE__);
                                   //args->fstream(ssv->data->buffer,
                                   //     ssv->data->length); 
                                   //if(args->fstream != NULL)
                                    //  args->fstream = NULL;
                                 } else{
                                    fatal("stack memory failed.,", 
                                       __FILE__,  __LINE__, 0);
                                 }
							   //}else{
                              //    fatal("function arguments ", 
                                //     __FILE__, __LINE__, 0);
                              // }
                              //PostThreadMessageA(args->surmicro, WM_USER,
                              //   (WPARAM)  0 ,
                              //   (LPARAM) (libssc_t *)ssv->temp );
                           }else{
                              fatal("segment memory failed.,", 
                                __FILE__,  __LINE__, 0);
                            }
                         }else{
                            fatal("segment memory failed.,", 
                                __FILE__,  __LINE__, 0);
                         }
                         
         
                         #ifdef __DBG_HH_ 
                          printf("[%8d=d] [%8d] [%8d] \n", 
                             uigen->block, 
                             ssv->block, ssv->size, ssv->length);
                         #endif  
                           
                         // if(uigen !=NULL)
                          //  free_t(uigen), uigen=NULL;
                          
                        }else{
                           fatal("query stream line ", __FILE__,  __LINE__, 0);
                        }
                     }else{
                        #ifdef __DBG_HR
                        if(ustate!=0)
                          printf("[stream] <%d> %s@%d\n", ustate,  __FUNCTION__, __LINE__); 
                        #endif
                     }
                       
				}
				
				SKIP:
				
				if(InRecord){
					waveInAddBuffer(WaveInHandle,(WAVEHDR*)msg.lParam,sizeof(WAVEHDR));
				}
			}
			case MM_WIM_OPEN:
			{
				//printf("WM_WIN_OPEN ___%s___%d \n", __FUNCTION__, __LINE__);
				DoneAll = 0;
				continue;
			}
			case MM_WIM_CLOSE:
			{
				break;
			}
		}
	}
  
}while(0);
  //END:
    //if(args->fsfree!=NULL) args->fsfree();
    
	_libmgc_kend();
	
	if(ssv!=NULL)
	{
	  #ifdef __STN_BACK_H
	    CLEAN(ssv->buffer);
	  #endif
    }
    
    if(ssv!= NULL){
      if(ssv->data!=NULL){
         
         CLEAN(ssv->data->buffer);
         ssv->data->length = 0;
         
      }         
      CLEAN(ssv->data);
      CLEAN(ssv->xssv);
      CLEAN(ssv);
    }
    CLEAN(args);
	
	return 0;
}

#endif

