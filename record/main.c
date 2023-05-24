
#include <windows.h>
#include <winuser.h>
#include <wingdi.h>
#include <mmsystem.h>
#include <commctrl.h>
#include "resource.h"

#include <icore.h>
#include <gcore.h>

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define IDM_START  1001
#define IDM_STOP   1002
#define IDM_PLAY   1003
#define IDM_STATUS 1004
#define IDM_INFO   1005
#define IDM_OPEN   1006
#define IDM_CDPLAY 1007
#define IDM_APPEND 1008 
#define IDM_TEXT   1009
#define IDM_FILEDEV 1010
#define IDM_SIGNDEV 1011
#define IDM_DELETE  1012
#define IDM_VIEW    1013

WAVEFORMATEX wformat;
WAVEHDR whdr[2]; 
WAVEHDR WaveHdr;
MMRESULT err,hr;
int num,len;
HPSTR pbuf;
WAVEINCAPSA wcaps;
char buf[MAX_PATH];   
static HWAVEIN hWaveIn;
static HWAVEOUT hWaveOut;
static BOOL inRecord;
static LPWAVEHDR phdr;

HMMIO mio;
MMCKINFO ckOut;
MMCKINFO ckOutRIFF;
MMIOINFO mminfo;

#define STAPI  __stdcall
BOOL SetInputDevice (unsigned int inDev);
extern BOOL STAPI st20_open(HMMIO *mio,LPSTR file,MMIOINFO mminfo);
extern BOOL STAPI st20_close(HMMIO mio, MMCKINFO ckOut,MMCKINFO ckOutRIFF);
extern BOOL STAPI st20_header(HMMIO mio,MMCKINFO *ckOutA ,MMCKINFO *ckOutRIFFA );
extern BOOL STAPI st20_write(HMMIO mio,unsigned char * buffer,LONG cb);
extern unsigned int STAPI stp_open(HMMIO *mio, LPSTR file);
extern unsigned int STAPI stp_read(HMMIO mio, HPSTR buf, LONG cb);
extern unsigned int STAPI stp_close(HMMIO mmio);
 
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

char szClassName[ ] = "_T(Record)";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;              
    MSG messages;         
    WNDCLASSEX wincl;      
    
    InitCommonControls();
    
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      
    wincl.style = CS_DBLCLKS;              
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDR_MAINICON));
    wincl.hIconSm = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDR_MAINICON));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                
    wincl.cbClsExtra = 0;                    
    wincl.cbWndExtra = 0;                   

    wincl.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);

    if (!RegisterClassEx (&wincl))
        return 0;

    hwnd = CreateWindowEx (
           0,                
           szClassName,      
           "Record",      
           WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU , 
           CW_USEDEFAULT,       
           CW_USEDEFAULT,     
           450,             
           350,               
           HWND_DESKTOP,  
           NULL,               
           hThisInstance,       
           NULL                
           );

    ShowWindow (hwnd, nFunsterStil);
    UpdateWindow(hwnd);
    
    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return messages.wParam;
}

#ifndef GWL_HINSTANCE
#define GWL_HINSTANCE  (-6)
#endif

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND wnd;
    RECT rect;
    HDC hdc,dc;
	HPEN hpen;
	static RECT			 drawArea;
    int    idx=0;
    static HDC           HdblDC        = NULL;
    static HBITMAP       HdblOldBitmap = NULL;
    
    unsigned int i;
    unsigned char ndata[2];
    wchar_t     lpszFile[MAX_PATH];
    wchar_t     szFile[MAX_PATH];
    
    OPENFILENAMEW ofn;
    icore_read    sdata;
    struct _dat_node msg;

	char         szfile[MAX_PATH];
	char         szData[MAX_PATH];
    ndata[0] = 0;
    
    switch (message)                  
    {
        case WM_CREATE:
             GetClientRect(hwnd, &rect);
             wnd = CreateWindowExA(0, WC_BUTTON, "START",
                   WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
                   5, rect.bottom - 25 , 60 , 20, 
                   hwnd,
                   (HMENU)IDM_START,
                   (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE ),
                   0);
             SendMessage(wnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT),
                 (LPARAM) MAKELPARAM(TRUE,0));
             
             wnd = CreateWindowExA(0, WC_BUTTON, "STOP",
                 WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
                 70 ,rect.bottom - 25, 60 , 20 ,
                 hwnd,
                 (HMENU) IDM_STOP,
                 (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                 0);
             SendMessage(wnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT),
                 (LPARAM) MAKELPARAM(TRUE,0));
             EnableWindow(wnd, FALSE);
                 
             wnd = CreateWindowExA(0, WC_BUTTON, "PLAY",
                 WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
                 135, rect.bottom - 25, 60 , 20,
                 hwnd,
                 (HMENU) IDM_PLAY,
                 (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                 0);
             SendMessage(wnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT),
                 (LPARAM) MAKELPARAM(TRUE,0));
             EnableWindow(wnd, FALSE);
             
             wnd = CreateWindowExA(0, WC_BUTTON, "OPEN",
                 WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
                 rect.right-70 , rect.bottom - 25, 60 , 20,
                 hwnd,
                 (HMENU) IDM_OPEN,
                 (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                 0);
             SendMessage(wnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT),
                 (LPARAM) MAKELPARAM(TRUE,0));
             EnableWindow(wnd, TRUE);
             
             wnd = CreateWindowExA(0, WC_BUTTON, "CDPLAY",
                 WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
                 rect.right-70 , rect.bottom - 45, 60 , 20,
                 hwnd,
                 (HMENU) IDM_CDPLAY,
                 (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                 0);
             SendMessage(wnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT),
                 (LPARAM) MAKELPARAM(TRUE,0));
             EnableWindow(wnd, FALSE);
             
             wnd = CreateWindowExA(0, WC_BUTTON, "APPEND",
                 WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
                 rect.right-70 , rect.bottom - 65, 60 , 20,
                 hwnd,
                 (HMENU) IDM_APPEND,
                 (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                 0);
             SendMessage(wnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT),
                 (LPARAM) MAKELPARAM(TRUE,0));
             EnableWindow(wnd, FALSE);
             
             wnd = CreateWindowExA(0, WC_BUTTON, "DELETE",
                 WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
                 rect.right-70 , rect.bottom - 85, 60 , 20,
                 hwnd,
                 (HMENU) IDM_DELETE,
                 (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                 0);
             SendMessage(wnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT),
                 (LPARAM) MAKELPARAM(TRUE,0));
             EnableWindow(wnd, FALSE);
             
             wnd = CreateWindowExA(0, WC_BUTTON, "VIEW",
                 WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
                 rect.right-70 , rect.bottom - 110, 60 , 20,
                 hwnd,
                 (HMENU) IDM_VIEW ,
                 (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                 0);
             SendMessage(wnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT),
                 (LPARAM) MAKELPARAM(TRUE,0));
             EnableWindow(wnd, FALSE);
             
             wnd = CreateWindowExA(0, WC_EDIT, "",
                 WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | ES_LEFT  ,
                 rect.left+5 , rect.bottom - 65, rect.right- 80 , 30,
                 hwnd,
                 (HMENU) IDM_TEXT,
                 (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                 0);
             SendMessage(wnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT),
                 (LPARAM) MAKELPARAM(TRUE,0));
             EnableWindow(wnd, TRUE);
             
             wnd = CreateWindowExA(
                 0,
                 WC_EDIT , "" ,
                 WS_CHILD | WS_BORDER | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP |
                   WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | ES_READONLY ,
                 5 , rect.bottom - 110 , rect.right - 80, 20,
                 hwnd,
                 (HMENU) IDM_FILEDEV ,
                 (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                 0);
             SendMessage(wnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT),
                 (LPARAM) MAKELPARAM(TRUE,0));
             
             wnd = CreateWindowExA(
                 0,
                 WC_EDIT , "" ,
                 WS_CHILD | WS_BORDER | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP |
                   WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL | ES_READONLY ,
                 5 , rect.bottom - 90 , rect.right - 80, 20,
                 hwnd,
                 (HMENU) IDM_SIGNDEV ,
                 (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                 0);
             SendMessage(wnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT),
                 (LPARAM) MAKELPARAM(TRUE,0));
             
             wnd = CreateWindowExA(
                 WS_EX_LEFT  | WS_EX_NOPARENTNOTIFY | WS_EX_LTRREADING,
                 WC_STATIC , ">Ready" ,
                 WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP |
                   WS_VISIBLE | SS_LEFT ,
                 5 , rect.bottom - 150 , rect.right - 10, 20,
                 hwnd,
                 (HMENU) IDM_STATUS ,
                 (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                 0);
             SendMessage(wnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT),
                 (LPARAM) MAKELPARAM(TRUE,0));
                 
             wnd = CreateWindowExA(
                 WS_EX_LEFT  | WS_EX_NOPARENTNOTIFY | WS_EX_LTRREADING,
                 WC_STATIC , ">Information" ,
                 WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP |
                   WS_VISIBLE | SS_LEFT ,
                 5 , rect.bottom - 130 , rect.right - 10, 20,
                 hwnd,
                 (HMENU) IDM_INFO ,
                 (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                 0);
             SendMessage(wnd, WM_SETFONT, (WPARAM) GetStockObject(DEFAULT_GUI_FONT),
                 (LPARAM) MAKELPARAM(TRUE,0));
                 
             GetClientRect (hwnd, &drawArea);

                // create offscreen work area.
             dc = GetDC (hwnd);
             HdblDC = CreateCompatibleDC (dc);
             HdblOldBitmap = CreateCompatibleBitmap (dc, drawArea.right, drawArea.bottom);
             HdblOldBitmap = (HBITMAP)SelectObject (HdblDC, HdblOldBitmap);
             ReleaseDC (hwnd, dc);
		       
        break;
        case WM_CTLCOLORSTATIC:
            hdc = (HDC) wParam;
            SetBkMode(hdc, TRANSPARENT);
            SetBkColor(hdc, (DWORD) GetStockObject( COLOR_WINDOW + 1));
        break;
        case WM_SIZE:{
			GetClientRect(hwnd,&rect);
			
			//wnd = GetDlgItem(hwnd,IDM_CTRLPLAY );
			//MoveWindow(wnd, 10, rect.bottom - 30, 60 , 25 , TRUE );
			//wnd = GetDlgItem(hwnd,IDM_EXIT );
			//MoveWindow(wnd, rect.right - 70, rect.bottom - 30, 60 , 25 , TRUE );
			
			DeleteObject (SelectObject (HdblDC, HdblOldBitmap));
            DeleteDC (HdblDC);
            
            HdblDC        = NULL;
            HdblOldBitmap = NULL;
			GetClientRect (hwnd, &drawArea);
                // create offscreen work area.
            dc = GetDC (hwnd);
            HdblDC = CreateCompatibleDC (dc);
            HdblOldBitmap = CreateCompatibleBitmap (dc, drawArea.right, drawArea.bottom-150 );
            HdblOldBitmap = (HBITMAP)SelectObject (HdblDC, HdblOldBitmap);
            ReleaseDC (hwnd, dc);
            //SendMessage(hwnd, WM_USER, (WPARAM) LOWORD(0),0 );
            
			break;
		}
        case WM_USER:
			
			switch(LOWORD(wParam)){
			    case 0:
			    	   GetDlgItemTextA(hwnd, IDM_FILEDEV, szfile, sizeof(szfile) );
                       cmemset(&msg,0, sizeof(struct _dat_node));
                       cmemset(&sdata, 0, sizeof(icore_read));
                        
						sdata.length = 0;
              	        dat_getfile(szfile, &sdata, &msg );
              	        
			    	    short int*   pSamples  = (short int*)sdata.buffer;
                        int sampleCnt = sdata.length/(sizeof(short int));
                          
					    hdc = GetDC (hwnd);

                        SetViewportOrgEx (hdc, 0, 0, NULL);

                        HBRUSH hbr = CreateSolidBrush (GetSysColor (COLOR_WINDOW ));
                        RECT     r = drawArea;
                        
                        GetClientRect(hwnd, &rect);
                        
                        r.right = rect.right;
				        r.bottom = rect.bottom -150;               
                        
						FillRect (HdblDC,  &r, hbr);

                        DeleteObject (hbr);                        

						float sampReduce = (float)(r.bottom / 4/*4*/) / (float)0x7fff;
                        float xStep      = (float)r.right / (float)(sampleCnt/2);
                        int   yPos       = -1;
                        float xPos       = xStep;                        

                        int yPosLeft   = (int)yPos;
                        int yPosRight  = r.bottom  - (r.bottom / 4/*4*/);                        
                        
                        hpen = CreatePen(PS_SOLID,1, RGB(0,0,0));
						SelectObject(hdc,hpen);
                        
                        MoveToEx (HdblDC, (int)((float)xPos-xStep), yPos, NULL);

                        for ( idx = 0; idx < sampleCnt; idx += 2)
					    {
                            MoveToEx (HdblDC, (int)((float)xPos-xStep), yPosLeft, NULL);
                            yPos = (r.bottom / 4/*4*/);
                            
                            yPosLeft = yPos + (int)((float)pSamples[idx] * sampReduce);
                            if (idx > 0)
                                LineTo (HdblDC, (int)xPos, yPosLeft);

                            yPos = (r.bottom  - (r.bottom / 4/*4*/));

                            MoveToEx (HdblDC, (int)((float)xPos-xStep), yPosRight, NULL);
                            yPosRight = yPos + (int)((float)pSamples[idx+1] * sampReduce);

                            if (idx > 0)
                                LineTo   (HdblDC, (int)xPos, yPosRight);

                            xPos += xStep;
					    }

                        BitBlt (hdc, r.left, r.top, r.right - r.left, r.bottom - r.top, HdblDC, r.left, r.top, SRCCOPY);
                        
                        CLEAN(sdata.buffer);
						
						DeleteObject(hpen);
                        ReleaseDC (hwnd, hdc);
			    break;
			}
		break;
        case WM_COMMAND:
           switch(LOWORD(wParam)){
              case IDM_START:
                   num = waveInGetNumDevs(); 
                  for( i=0;i<num;i++){
                      waveInGetDevCapsA(i, &wcaps, sizeof(wcaps));
                      
                      wsprintf(buf,"caps ID %d %s ", i, wcaps.szPname); 
                      
                      /*ERRORA str = 
                                { 
                                    buf,
                                    0
                                }; 
                                
                      cbdata->func(NULL, &str, WUSAGE);
                      */
                  }
                  wsprintf(buf,"Caps: %d - %d", num, waveInGetNumDevs());
                  //SetWindowText(hWnd, buf);
                   
                  if (SetInputDevice (MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT) == FALSE)
                    {
						if (SetInputDevice (MIXERLINE_COMPONENTTYPE_SRC_ANALOG) == FALSE)
						{
							if (SetInputDevice (MIXERLINE_COMPONENTTYPE_SRC_LAST) == FALSE)
							{
								//MessageBox(hWnd,"Error unable to set recording WAVEOUT device",
                                //        0, MB_OK | MB_ICONWARNING );
                                //while(1){ Sleep(100); }
								/*ERRORA str = 
                                { 
                                    "unable to set recording WAVEOUT device",
                                    32
                                }; 
								cbdata->func(NULL, &str, WERROR);
                                */
                                //break;
							}
						}
                    }
                    
                   ZeroMemory(&whdr[0],sizeof(WAVEHDR) * 2);
                    
                    wformat.wFormatTag = WAVE_FORMAT_PCM;
                    wformat.nChannels = 2;
                    wformat.nSamplesPerSec = 44100;
                    wformat.wBitsPerSample = 16;
                    wformat.nBlockAlign = wformat.nChannels * (wformat.wBitsPerSample / 8);
                    wformat.nAvgBytesPerSec = wformat.nSamplesPerSec * wformat.nBlockAlign;
                    
                    wformat.cbSize = 0;
                    
                    if((err = waveInOpen(&hWaveIn, WAVE_MAPPER , &wformat, 
                      (DWORD)hwnd, 0 , CALLBACK_WINDOW))){
                         
                         waveInGetErrorTextA(err, buf, sizeof(buf));
                        /* 
                         ERRORA str = 
                         { 
                           buf,
                            31
                        }; 
                         cbdata->func(NULL, &str, WERROR);
                         //MessageBox(hWnd, buf, 0, MB_OK);
                        */
                        goto end;
                        break;
                    }
                    //
                    whdr[1].dwBufferLength = whdr[0].dwBufferLength =  wformat.nAvgBytesPerSec << 1 ; // .25
			        if (!(whdr[0].lpData = (char*)VirtualAlloc(0, (whdr[0].dwBufferLength << 1), MEM_COMMIT, PAGE_READWRITE)))
			        {
				        //MessageBox(hWnd,
                        //   "ERROR: Can't allocate memory for WAVE buffer!\n" ,0, MB_OK);
                        //waveInClose(waveInHandle);
                        // while(1){ Sleep(100); }
                        /*ERRORA str = 
                        { 
                           "Can't allocate memory for WAVE buffer",
                           30
                        }; 
                        
                        cbdata->func(NULL, &str, WERROR);
                        */
                        goto end;
                        break;
			        }

				    whdr[1].lpData = whdr[0].lpData + whdr[0].dwBufferLength;

				    if ((err = waveInPrepareHeader(hWaveIn, &whdr[0], sizeof(WAVEHDR))))
				    {
                        //waveInClose(waveInHandle);
                        //MessageBox(hWnd,
                        //   "Error preparing WAVEHDR 1! -- %08X\n",0, MB_OK);
                        /*ERRORA str = 
                                { 
                                    "preparing WAVEHDR 1",
                                    29
                                }; 
                        cbdata->func(NULL, &str, WERROR);
                        */
                        VirtualFree (whdr[0].lpData, (whdr[0].dwBufferLength << 1), MEM_RELEASE); 
                        // while(1){ Sleep(100); }
                        goto end;
                        break;
				    }

					if ((err = waveInPrepareHeader(hWaveIn, &whdr[1], sizeof(WAVEHDR))))
					{
                        //waveInClose(waveInHandle);
					   // MessageBox(hWnd,
                       //      "Error preparing WAVEHDR 2! -- %08X\n",0, MB_OK);
                      /* ERRORA str = 
                                { 
                                    "preparing WAVEHDR 2",
                                    28
                                }; 
                       cbdata->func(NULL, &str , WERROR);
                        */
                        VirtualFree (whdr[0].lpData, (whdr[0].dwBufferLength << 1), MEM_RELEASE); 
                         //while(1){ Sleep(100); }
                        goto end;
                        break;
					}
					
					if ((err = waveInAddBuffer(hWaveIn, &whdr[0], sizeof(WAVEHDR))))
					{
                        //waveInClose(waveInHandle);
						//MessageBox(hWnd,
                         //  "Error queueing WAVEHDR 1! -- %08X\n", 0, MB_OK);
                       /* ERRORA str = 
                                { 
                                    "queueing WAVEHDR 1",
                                    27
                                }; 
                        cbdata->func(NULL, &str , WERROR);
                        */
                        VirtualFree (whdr[0].lpData, (whdr[0].dwBufferLength << 1), MEM_RELEASE); 
                        // while(1){ Sleep(100); }
                        goto end;
                        break;
					}

					if ((err = waveInAddBuffer(hWaveIn, &whdr[1], sizeof(WAVEHDR))))
					{
                        //waveInClose(waveInHandle);
					    // MessageBox(hWnd,
                        //    "Error queueing WAVEHDR 2! -- %08X\n", 0, MB_OK);
                        /* ERRORA str = 
                                { 
                                    "queueing WAVEHDR 2",
                                    26
                                };
                        cbdata->func(NULL, &str, WERROR);
                        */
                        VirtualFree (whdr[0].lpData, (whdr[0].dwBufferLength << 1), MEM_RELEASE); 
                        // while(1){ Sleep(100); }
                        goto end;
                        break;
					}

					if ((err = waveInStart(hWaveIn)))
					{
						
                        //   "Error starting record! -- %08X\n", 0, MB_OK);
                        /* ERRORA str = 
                                { 
                                    "starting record",
                                    25
                                };
                        cbdata->func(NULL, &str, WERROR);						
                        */
                        VirtualFree (whdr[0].lpData, (whdr[0].dwBufferLength << 1), MEM_RELEASE); 
                        // while(1){ Sleep(100); }
                        goto end;
                        break;
					}
					inRecord = TRUE;
					
                   wnd = GetDlgItem(hwnd, IDM_START);
                   EnableWindow(wnd, FALSE);
                  
                   wnd = GetDlgItem(hwnd, IDM_STOP);
                   EnableWindow(wnd, TRUE);
                   
                   wnd = GetDlgItem(hwnd, IDM_PLAY);
                   EnableWindow(wnd, FALSE);
                   // cbdata->func(NULL, 0, WINIT); 
					
                    break;
                    end:
                       waveInClose(hWaveIn);	
                       break;
                    ;
                  
                  
              break;
              case IDM_STOP:
                  if (inRecord == FALSE)
                        break;
				  waveInReset(hWaveIn);                 
                  waveInUnprepareHeader (hWaveIn, &whdr[0], sizeof(WAVEHDR));
                  waveInUnprepareHeader (hWaveIn, &whdr[1], sizeof(WAVEHDR));

                  waveInClose(hWaveIn);
                  VirtualFree(whdr[0].lpData, (whdr[0].dwBufferLength << 1), MEM_RELEASE); 
                  inRecord = FALSE;
                  
                  wnd = GetDlgItem(hwnd, IDM_START);
                  EnableWindow(wnd, TRUE);
                  wnd = GetDlgItem(hwnd, IDM_PLAY);
                  EnableWindow(wnd, TRUE);
                  wnd = GetDlgItem(hwnd, IDM_STOP);
                  EnableWindow(wnd, FALSE);
                  
                  SetInputDevice (MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE);  
              break;
              case IDM_OPEN:
              	    
              	    GetCurrentDirectoryW(sizeof(lpszFile ), lpszFile );
					cmemset(szFile,0, wcslen(szFile));
				    ZeroMemory(&ofn, sizeof(OPENFILENAME));
                    ofn.lStructSize = sizeof(OPENFILENAME);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFile = szFile;
                    ofn.nMaxFile = sizeof(szFile);
                    ofn.lpstrFilter = L"DAT Files\0*.dat\0All\0*.*\0";
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = lpszFile ;
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                    //MessageBox(hwnd, "asdasd", "cc", MB_OK);  
                    if (GetOpenFileNameW(&ofn)==TRUE){
                        
                        //SetWindowTextW(hwnd,szFile);
                        SetDlgItemTextW(hwnd, IDM_FILEDEV, szFile );
                        EnableWindow(GetDlgItem(hwnd, IDM_APPEND ), TRUE );
                        EnableWindow(GetDlgItem(hwnd, IDM_CDPLAY ), TRUE );
                        EnableWindow(GetDlgItem(hwnd, IDM_DELETE ), TRUE );
                        EnableWindow(GetDlgItem(hwnd, IDM_VIEW ), TRUE);
						wcstombs(szfile, szFile, wcslen(szFile));
						
						char *ptr = szfile;
						int  i= 0;
						while(*ptr){
							if(*ptr=='\0'){
							  szfile[i] = '\0';
							  *ptr='\0'; break;
						    }
							ptr++;
							i++;
						}
						
                        dat_getfile( szfile, &sdata, &msg );
                        
                        //printf("### OK [%d] [%d] \n", MGC_SEEK1, msg.length );
                        //printf("(%s)\n", sign_digest(msg.filedev, 64) );
						//printf("(%s)\n", sign_digest(msg.signdev, 16) );
                        
                        SetDlgItemTextA(hwnd, IDM_SIGNDEV, sign_digest(msg.signdev, 16));
                        SetDlgItemTextA(hwnd, IDM_TEXT, msg.message);
                        
                        CLEAN(sdata.buffer);
                        
					}
                    //MessageBox(hwnd, "asdasd", "cc", MB_OK); 	
              break;
              case IDM_CDPLAY:
              	   wformat.wFormatTag = WAVE_FORMAT_PCM;
                   wformat.nChannels = 2;
                   wformat.nSamplesPerSec = 44100;
                   wformat.wBitsPerSample = 16;
                   wformat.nBlockAlign = wformat.nChannels *(wformat.wBitsPerSample / 8);
                   wformat.nAvgBytesPerSec = wformat.nSamplesPerSec * wformat.nBlockAlign;
                   wformat.cbSize = 0;
  
                   if((err = waveOutOpen(&hWaveOut, WAVE_MAPPER , 
                         &wformat, 0, 0, CALLBACK_WINDOW))){
         
                         waveOutGetErrorTextA(err, buf, sizeof(buf));
                   }  
                   
                        GetDlgItemTextA(hwnd, IDM_FILEDEV, szfile, sizeof(szfile) );
                        cmemset(&msg,0, sizeof(struct _dat_node));
                        cmemset(&sdata, 0, sizeof(icore_read));
                        sdata.length = 0;
              	        dat_getfile(szfile, &sdata, &msg );
              	        
              	        if(sdata.buffer ==NULL && sdata.length <= 0)  break;
                        
                         
                        short int *pSamples = (short int *) pbuf;
                        int sampleCnt = num / 100;
                            
                         WaveHdr.lpData = (char*)VirtualAlloc(0, (num << 1), MEM_COMMIT, PAGE_READWRITE);
                       //WaveHdr.lpData = (LPSTR) GlobalAlloc(GPTR | GMEM_MOVEABLE , lDatasize);
        	           //WaveHdr.lpData = (LPSTR) malloc( lDatasize);
        	 
                       WaveHdr.lpData = sdata.buffer;
                       //printf("APPC: DONE \n"); 
	                   WaveHdr.dwBufferLength = sdata.length ;  
	                   WaveHdr.dwFlags = 0L;   
	                   WaveHdr.dwLoops = 0L;   
	                   WaveHdr.dwBytesRecorded = sdata.length ; 
	          
	                   if((err = waveOutPrepareHeader(hWaveOut, &WaveHdr , sizeof(WAVEHDR)))){
				          waveOutGetErrorText(err, buf, sizeof(buf));
			              //printf(">FATAL Prepare (%d) %s\n",err, buf);
		                }else{
			              if((err = waveOutWrite(hWaveOut, &WaveHdr, sizeof(WAVEHDR)))){
				             waveOutGetErrorText(err, buf, sizeof(buf));
			                // printf(">FATAL Write (%d) %s\n",err, buf);
			            }else{
			               i=0;
                           while ((WaveHdr.dwFlags & WHDR_DONE) != WHDR_DONE){
                           	     ndata[0]  = 0.333* WaveHdr.lpData[i] + (1.0-0.333)*ndata[0];
								 printf("%3d ", abs(ndata[0]));
	                          i++;
							  //Sleep(1);
                           }   
                              
                           if((err = waveOutUnprepareHeader(hWaveOut, &WaveHdr, sizeof(WAVEHDR)))){
					          waveOutGetErrorText(err, buf, sizeof(buf));
			                  // printf(">FATAL Unprepare (%d) %s\n",err, buf);
			               }
		                }
                      }
               
                      VirtualFree (WaveHdr.lpData , (WaveHdr.dwBufferLength << 1), MEM_RELEASE);    
                       
					  CLEAN(sdata.buffer);     
                 
              break;
              case IDM_APPEND:
              	
              	GetDlgItemTextA(hwnd, IDM_FILEDEV, szfile, sizeof(szfile) );
              	GetDlgItemTextA(hwnd, IDM_TEXT, szData, sizeof(szData) );
              	
              	dat_getfile(szfile, &sdata, &msg );
              	strcpy(msg.message, szData);
				dat_sethdr(szfile, &msg);
              	
              	CLEAN(sdata.buffer);
              	
              break;
              case IDM_DELETE:
                 GetDlgItemTextA(hwnd, IDM_FILEDEV, szfile, sizeof(szfile) );
                 EnableWindow(GetDlgItem(hwnd, IDM_VIEW ), FALSE);
                 EnableWindow(GetDlgItem(hwnd, IDM_CDPLAY ), FALSE);
                 EnableWindow(GetDlgItem(hwnd, IDM_APPEND), FALSE);
                 //if(access(szfile, F_OK) != -1){
                    // printf("FILE(%s)\n",szfile );	
				 //}
                 dat_remove(szfile);
				 //if(DeleteFile(szfile)==FALSE){
				 	//unlink(szfile);
				 //	if(remove(szfile)!=0)
				 //	   MessageBox(hwnd, "Error: Delete File", "Delete", MB_OK | MB_DEFBUTTON1 | MB_ICONERROR  );
				// }
                 
              break;
              case IDM_VIEW:
              	 SendMessage(hwnd, WM_USER, (WPARAM) LOWORD(0),0 );
              break;
              case IDM_PLAY:
                   wformat.wFormatTag = WAVE_FORMAT_PCM;
                   wformat.nChannels = 2;
                   wformat.nSamplesPerSec = 44100;
                   wformat.wBitsPerSample = 16;
                   wformat.nBlockAlign = wformat.nChannels *(wformat.wBitsPerSample / 8);
                   wformat.nAvgBytesPerSec = wformat.nSamplesPerSec * wformat.nBlockAlign;
                   wformat.cbSize = 0;
  
                   if((err = waveOutOpen(&hWaveOut, WAVE_MAPPER , 
                         &wformat, 0, 0, CALLBACK_WINDOW))){
         
                         waveOutGetErrorTextA(err, buf, sizeof(buf));
                   }  
                  
                  if((num = stp_open(&mio, "Data.wav"))<= 0){
                       MessageBox(hwnd, "ERROR: load file stream .", 0, MB_OK | MB_ICONERROR
                          | MB_DEFBUTTON1);
                        }else{
                        pbuf = (char *) malloc( num);
                        if((len = stp_read(mio, pbuf, num))<= 0){
                          MessageBox(hwnd, "ERROR: load read stream . ", 0, MB_OK | MB_ICONERROR
                           | MB_DEFBUTTON1 );
                        }else{
                         
                        short int *pSamples = (short int *) pbuf;
                        int sampleCnt = num / 100;
                            
                         WaveHdr.lpData = (char*)VirtualAlloc(0, (num << 1), MEM_COMMIT, PAGE_READWRITE);
                       //WaveHdr.lpData = (LPSTR) GlobalAlloc(GPTR | GMEM_MOVEABLE , lDatasize);
        	           //WaveHdr.lpData = (LPSTR) malloc( lDatasize);
        	 
                       WaveHdr.lpData = pbuf;
                       //printf("APPC: DONE \n"); 
	                   WaveHdr.dwBufferLength = num;  
	                   WaveHdr.dwFlags = 0L;   
	                   WaveHdr.dwLoops = 0L;   
	                   WaveHdr.dwBytesRecorded = num; 
	          
	                   if((err = waveOutPrepareHeader(hWaveOut, &WaveHdr , sizeof(WAVEHDR)))){
				          waveOutGetErrorText(err, buf, sizeof(buf));
			              //printf(">FATAL Prepare (%d) %s\n",err, buf);
		                }else{
			              if((err = waveOutWrite(hWaveOut, &WaveHdr, sizeof(WAVEHDR)))){
				             waveOutGetErrorText(err, buf, sizeof(buf));
			                // printf(">FATAL Write (%d) %s\n",err, buf);
			            }else{
			               i=0;
                           while ((WaveHdr.dwFlags & WHDR_DONE) != WHDR_DONE){
                           	     ndata[0]  = 0.333* WaveHdr.lpData[i] + (1.0-0.333)*ndata[0];
								 printf("%3d ", abs(ndata[0]));
	                          i++;
							  //Sleep(1);
                           }   
                              
                           if((err = waveOutUnprepareHeader(hWaveOut, &WaveHdr, sizeof(WAVEHDR)))){
					          waveOutGetErrorText(err, buf, sizeof(buf));
			                  // printf(">FATAL Unprepare (%d) %s\n",err, buf);
			               }
		                }
                      }
               
                      VirtualFree (WaveHdr.lpData , (WaveHdr.dwBufferLength << 1), MEM_RELEASE);    
                            
                         
                        free(pbuf);
                        stp_close(mio);
                        }
                       }
              break;
           }
        break;
        case MM_WIM_OPEN:
           st20_open(&mio, "Data.wav", mminfo);
           st20_header(mio, &ckOut, &ckOutRIFF);
        break;
        case MM_WIM_CLOSE:
           st20_close(mio, ckOut , ckOutRIFF);
        break;
        case MM_WIM_DATA:
           phdr        = (WAVEHDR*) lParam;
           if(phdr->dwBytesRecorded > 0){
              short int*   pSamples  = (short int*)phdr->lpData;
              int   sampleCnt = phdr->dwBytesRecorded/(sizeof(short int)) ;
              
              st20_write(mio, (unsigned char *) phdr->lpData , phdr->dwBytesRecorded);
              
              if(inRecord){
                waveInAddBuffer(hWaveIn, phdr, sizeof(WAVEHDR));
              } 
           }
        break;
        case WM_CLOSE:
  		    DeleteObject (SelectObject (HdblDC, HdblOldBitmap));
            DeleteDC (HdblDC);
            HdblDC        = NULL;
            HdblOldBitmap = NULL;
		break;
        case WM_DESTROY:
            PostQuitMessage (0);      
            break;
        default:                     
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return  DefWindowProc( hwnd, message, wParam, lParam  ) ;;
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
	MIXERCONTROLDETAILS_LISTTEXT*  pmxcdSelectText = (MIXERCONTROLDETAILS_LISTTEXT*)malloc( sizeof( MIXERCONTROLDETAILS_LISTTEXT));

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

		if(pmxcdSelectText)
		  free(pmxcdSelectText), pmxcdSelectText=NULL;
	}

    if (inDevIdx < 0)
    {
        mixerClose (hMixer);
        return FALSE;
    }

	// get all the values first
    MIXERCONTROLDETAILS_BOOLEAN* pmxcdSelectValue = (MIXERCONTROLDETAILS_BOOLEAN*)malloc(sizeof(MIXERCONTROLDETAILS_BOOLEAN));

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
                if(pmxcdSelectValue) free(pmxcdSelectValue), pmxcdSelectValue=NULL;
                mixerClose (hMixer);
                return FALSE;
			}
		}

		if(pmxcdSelectValue)
		   free(pmxcdSelectValue), pmxcdSelectValue=NULL;
	}

    mixerClose (hMixer);
    return TRUE;
}
BOOL STAPI st20_open(HMMIO *mio,LPSTR file,MMIOINFO mminfo)
{
   //cout << file << endl;
   ZeroMemory(&mminfo,sizeof(mminfo));
  // mio = mmioOpenA(file,&mminfo, MMIO_CREATE | MMIO_WRITE);
   if(!(*mio = mmioOpenA(file,NULL,MMIO_ALLOCBUF|MMIO_WRITE|MMIO_CREATE))){ 
	   //printf("STREAM: don't (open) create file.\n");
	   return false;
   }
   return true;
}
BOOL STAPI st20_close(HMMIO mio, MMCKINFO ckOut,MMCKINFO ckOutRIFF)
{
   if(mmioFlush(mio,MMIO_EMPTYBUF)) 
   {
	  //printf("STREAM: don't (done) md 'mmioFlush'.\n");
      return true;
   }
        /*mmioinfoOut.dwFlags|=MMIO_DIRTY;
        if(mmioSetInfo(hmmioOut,&mmioinfoOut,0)!=MMSYSERR_NOERROR){
        //throw 1;
        return;
    }*/
    if(mmioAscend(mio,&ckOut,0)!=MMSYSERR_NOERROR)
    {
            //throw 1;
	  //printf("STREAM: don't (done) md [ckOut] 'mmioAscend'.\n");
      return true;
    }
    if(mmioAscend(mio,&ckOutRIFF,0)!=MMSYSERR_NOERROR)
    {
            //throw 1;
	  //printf("STREAM: don't (done) md [ckOutRIFF] 'mmioAscend'.\n");
      return true;
    }  
   mmioClose(mio,MMIO_FHOPEN);
   return true;
}
BOOL STAPI st20_write(HMMIO mio,unsigned char * buffer,LONG cb)
{
	if(mmioWrite( mio,(const char *)buffer,cb) < 0){
		//printf("STREAM: don't (body) md 'mmioWrite'.\n");
		return false;
	}
   return true;
}
BOOL STAPI st20_header(HMMIO mio,MMCKINFO *ckOutA ,MMCKINFO *ckOutRIFFA )
{
	WAVEFORMATEX WAVFx;
	MMCKINFO ckOut;
	MMCKINFO ckOutRIFF;
     /*
     WAVFx.wFormatTag        = WAVE_FORMAT_PCM;
     WAVFx.wBitsPerSample    = 16;
     WAVFx.nChannels         = 2;
     WAVFx.nSamplesPerSec    = 44100;
     
     WAVFx.nAvgBytesPerSec   = WAVFx.nSamplesPerSec*WAVFx.nChannels*WAVFx.wBitsPerSample/8;
     WAVFx.nBlockAlign       = WAVFx.nChannels*WAVFx.wBitsPerSample/8;
     //WAVFx.nBlockAlign = WAVFx.nChannels * (WAVFx.wBitsPerSample / 8);
     //WAVFx.nAvgBytesPerSec = WAVFx.nSamplesPerSec * WAVFx.nBlockAlign;
      
     WAVFx.cbSize          = 0; 
     */
     ckOutRIFF.fccType = mmioFOURCC('W','A','V','E');
     //ckOutRIFF.cksize = sizeof(WAVFx);
     ckOutRIFF.cksize = 0;
     
     if(mmioCreateChunk(mio,&ckOutRIFF,MMIO_CREATERIFF) != MMSYSERR_NOERROR){
        //printf("STREAM: don't (header) md 'mmioCreateChunk'.\n");
       return false;
     }
     /*  
     WAVFx.wFormatTag        = WAVE_FORMAT_PCM;
     WAVFx.wBitsPerSample    = 16;
     WAVFx.nChannels         = 2;
     WAVFx.nSamplesPerSec    = 44100;
     
     //WAVFx.nAvgBytesPerSec   = WAVFx.nSamplesPerSec* (WAVFx.nChannels*(WAVFx.wBitsPerSample/8));
     //WAVFx.nBlockAlign       = WAVFx.nChannels*(WAVFx.wBitsPerSample/8);
     //WAVFx.nBlockAlign = WAVFx.nChannels * (WAVFx.wBitsPerSample / 8);
     //WAVFx.nAvgBytesPerSec = WAVFx.nSamplesPerSec * WAVFx.nBlockAlign;
     WAVFx.nAvgBytesPerSec = 176400L; 
     WAVFx.nBlockAlign = 4; 
     
     WAVFx.cbSize          = 0;
     */
     WAVFx.wFormatTag        = WAVE_FORMAT_PCM;
     WAVFx.wBitsPerSample    = 16;
     WAVFx.nChannels         = 2;
     WAVFx.nSamplesPerSec    = 44100;
     WAVFx.nAvgBytesPerSec   = WAVFx.nSamplesPerSec*WAVFx.nChannels*WAVFx.wBitsPerSample/8;
     WAVFx.nBlockAlign       = WAVFx.nChannels*WAVFx.wBitsPerSample/8;
     WAVFx.cbSize          = 0;
     
     ckOut.ckid=mmioFOURCC('f', 'm', 't', ' ');
     ckOut.cksize=sizeof(WAVFx);
   
        // ...and store it
     if(mmioCreateChunk(mio,&ckOut,0)!=MMSYSERR_NOERROR) {
       //printf("STREAM: don't (header) md 'mmioCreateChunk'.\n");
       return false;
     }
     if(mmioWrite(mio,(HPSTR)&WAVFx,sizeof(WAVFx))!=sizeof(WAVFx)){
       //printf("STREAM: don't (header) md 'mmioWrite'.\n");
       return false;
     }
     if(mmioAscend(mio,&ckOut,0)!=MMSYSERR_NOERROR) {
       // printf("STREAM: don't (header) md 'mmioAscend'.\n");
        return false;
     }
     ckOut.ckid=mmioFOURCC('d', 'a', 't', 'a');
     ckOut.cksize=0;
     if(mmioCreateChunk(mio,&ckOut,0)!=MMSYSERR_NOERROR){
       // printf("STREAM: don't (header) md 'mmioCreateChunk'.\n");
        return false;
     }

	 *ckOutRIFFA = ckOutRIFF;
     *ckOutA = ckOut;

     return true;
}
unsigned int STAPI stp_open(HMMIO *mio, LPSTR file)
{
     MMIOINFO mmioinfo;
     HMMIO hmmio = 0;
     LONG lByteReadFormat;
     
     memset(&mmioinfo, 0 ,sizeof(MMIOINFO));
	 mmioinfo.pIOProc = NULL;
	 mmioinfo.fccIOProc = FOURCC_MEM;
	 mmioinfo.pchBuffer = NULL;
	 mmioinfo.cchBuffer = sizeof(int);
     mmioinfo.adwInfo[0] = 0; 
	 mmioinfo.adwInfo[1] = 0; 
 	 mmioinfo.adwInfo[2] = 0; 
	 mmioinfo.dwFlags = 0;	// from now on etc
	 mmioinfo.wErrorRet = 0;
	 mmioinfo.htask = 0;
	 mmioinfo.pchNext = 0;
	 mmioinfo.pchEndRead = 0;
	 mmioinfo.pchEndWrite = 0;
 	 mmioinfo.lBufOffset = 0;
	 mmioinfo.lDiskOffset = 0;
	 mmioinfo.dwReserved1 = 0;
	 mmioinfo.dwReserved2 = 0;
	 mmioinfo.hmmio = 0;
  	 hmmio = mmioOpen(NULL, &mmioinfo, MMIO_READWRITE );
	       //ASSERT(hmmio);
	 if ( hmmio == 0 ){ 
                //printf("ERROR: mmio data \n");
                return 0L;
                //return FALSE;
	 }//  test with a real file on disc
     hmmio = mmioOpen((LPSTR)file, 
			 	NULL ,                   // MMCKINFO
			    MMIO_READ);        
	 if ( hmmio == 0 ){ 
                //printf("ERROR: mmio data \n");
                return 0L;
                //return FALSE;
	 }//  test with a real file on disc
		   
	        //2) FIND PARENT CHUNK
     MMRESULT  rc;
	 MMCKINFO  MMCkInfoParent;              
	 MMCkInfoParent.fccType = mmioFOURCC('W','A','V','E');      	
	 rc = mmioDescend( hmmio, &MMCkInfoParent, NULL, MMIO_FINDRIFF );    
	          //ASSERT(rc == MMSYSERR_NOERROR );
     if ( rc != MMSYSERR_NOERROR ){ 
                // printf("ERROR: descend wave 'InfoParent' \n");
                return 0L;
				 //return FALSE;
      }
	          //3) FIND CHILD CHUNK
      MMCKINFO  MMCkInfoChild;	
 	  MMCkInfoChild.ckid = mmioFOURCC('f','m','t',' ');  
	  rc = mmioDescend(hmmio, &MMCkInfoChild, &MMCkInfoParent, MMIO_FINDCHUNK); 
	          //ASSERT(rc == MMSYSERR_NOERROR );
	  if ( rc != MMSYSERR_NOERROR ){ 
                  // printf("ERROR: ck Info Child 'format' \n");
                  return 0L;
                   //return FALSE;
      }
	  PCMWAVEFORMAT  WaveRecord;     
	  lByteReadFormat = mmioRead( hmmio,  (LPSTR)&WaveRecord,  MMCkInfoChild.cksize ); 
	  if (lByteReadFormat == 0){ 
                  // printf("ERROR: mmio read \n");
				 return 0L;
                   //return FALSE;
       }
	   rc = mmioAscend( hmmio, &MMCkInfoChild, 0 ); 
	   if ( rc != MMSYSERR_NOERROR ){ 
                  // printf("ERROR: mmio ascend\n");
				 return 0L;
                   //return FALSE;
       }
	
	    MMCkInfoChild.ckid = mmioFOURCC ( 'd', 'a', 't', 'a' ) ;	
	    rc = mmioDescend ( hmmio, &MMCkInfoChild, &MMCkInfoParent, MMIO_FINDCHUNK ); 
		if ( rc != MMSYSERR_NOERROR ){ //return FALSE;
			     // printf("ERROR: mmio descend \n");
			     return 0L;
		}
		
		*mio = hmmio;
		return MMCkInfoChild.cksize; 
}
unsigned int STAPI stp_read(HMMIO mio, HPSTR buf, LONG cb)
{
     LONG lByteRead = mmioRead( mio, buf,  cb ); 
     if(lByteRead <= 0){
          //printf("ERROR: wave stream read. \n");
          return 0L;
     }
     return lByteRead;
}
unsigned int STAPI stp_close(HMMIO mmio)
{
   mmioClose(mmio,0);
   return 0L;
}
