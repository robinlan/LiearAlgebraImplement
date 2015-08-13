/********************************************************************************
GNUplot_typer_panels.h, by Robin Lan - 2015/08/12

Example usage:
	Global setting:
		HWND GNUPlotTyperHwnd;
		static TCHAR szTyperAppName[] = TEXT("GNUPlotTyper") ;
		int nGlobCmdShow;

	In WinMain:
		nGlobCmdShow = iCmdShow;
		WinWindows wincGNUPlotTyperObject(szGNUPlotTyperAppName,hInstance,iCmdShow);
		WNDCLASSEX wincGNUPlotTyper = wincGNUPlotTyperObject.getWinClass(GNUPlotTyperWindowProcedure);
		if( !wincGNUPlotTyperObject.getWinRegisterClass())
			return 0;
		GNUPlotTyperHwnd = wincGNUPlotTyperObject.getWinHWND(444,275,_T("GNUPlot Typer Program"));

	In trigger place:
		ShowWindow (GNUPlotTyperHwnd, nGlobCmdShow);

********************************************************************************/

#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <windows.h>
#include "res/gnu_plotter.h"

#define IDC_TYPER_BUTTON	101
#define BUFFER(x,y) *(pBuffer + y * cxBuffer + x)

using namespace std;

LRESULT CALLBACK GNUPlotTyperWindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static DWORD   dwCharSet = DEFAULT_CHARSET ;
     static int     cxChar, cyChar, cxClient, cyClient, cxBuffer, cyBuffer,
                    xCaret, yCaret ;
     static TCHAR * pBuffer = NULL ;

	 static string order = "";   //Store the order every line
	 static vector<string> orderVector;
	 static int curOrderNum = 0;
	 
	 static GNUplot plotter;

     HDC            hdc ;
     int            x, y, i ;
     PAINTSTRUCT    ps ;
     TEXTMETRIC     tm ;

     switch (message) {

     case WM_INPUTLANGCHANGE:
          dwCharSet = wParam ;

     case WM_CREATE:
          hdc = GetDC (hwnd) ;
          SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
                                   dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;

          GetTextMetrics (hdc, &tm) ;
          cxChar = tm.tmAveCharWidth ;
          cyChar = tm.tmHeight ;

          DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
          ReleaseDC (hwnd, hdc) ;

     case WM_SIZE:
               // obtain window size in pixels

          if (message == WM_SIZE)
          {
               cxClient = LOWORD (lParam) ;
               cyClient = HIWORD (lParam) ;
          }
               // calculate window size in characters

          cxBuffer = max (1, cxClient / cxChar) ;
          cyBuffer = max (1, cyClient / cyChar) ;

               // allocate memory for buffer and clear it

          if (pBuffer != NULL)
               free (pBuffer) ;

          pBuffer = (TCHAR *) malloc (cxBuffer * cyBuffer * sizeof (TCHAR)) ;

          for (y = 0 ; y < cyBuffer ; y++)
               for (x = 0 ; x < cxBuffer ; x++)
				   BUFFER(x,y) = ' ' ;

               // set caret to upper left corner

          xCaret = 0 ;
          yCaret = 0 ;

          if (hwnd == GetFocus ())
               SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;

          InvalidateRect (hwnd, NULL, TRUE) ;
          return 0 ;

     case WM_SETFOCUS:
          CreateCaret (hwnd, NULL, cxChar, cyChar) ;
          SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
          ShowCaret (hwnd) ;
          return 0 ;

     case WM_KILLFOCUS:
          HideCaret (hwnd) ;
          DestroyCaret () ;
          return 0 ;

     case WM_KEYDOWN:
          switch (wParam)
          {
          case VK_HOME:
               xCaret = 0 ;
               break ;

          case VK_END:
               xCaret = cxBuffer - 1 ;
               break ;

          case VK_LEFT:
               xCaret = max (xCaret - 1, 0) ;
               break ;

          case VK_RIGHT:
               xCaret = min (xCaret + 1, cxBuffer - 1) ;
               break ;
			   
		  case VK_UP:{

                char* tmpCharArray = const_cast<char*>(orderVector.at(curOrderNum-1).c_str());
				
				for (x = 0 ; x < cxBuffer ; x++)
                    BUFFER (x, yCaret) =  ' ';
				
				for (x = 0 ; x < orderVector.at(curOrderNum-1).length() ; x++)
                    BUFFER (x, yCaret) =  *(tmpCharArray+x);
				
				order = orderVector.at(curOrderNum-1);
				xCaret = orderVector.at(curOrderNum-1).length();
				
				if(curOrderNum > 0)curOrderNum = curOrderNum - 1;

                HideCaret (hwnd) ;
                hdc = GetDC (hwnd) ;

                SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
                                   dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;

                TextOut (hdc, 0, yCaret * cyChar,
                        & BUFFER (0, yCaret),
                        cxBuffer) ;

                DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
                ReleaseDC (hwnd, hdc) ;
                ShowCaret (hwnd) ;

			   break;
		  }

          case VK_DELETE:
               for (x = xCaret ; x < cxBuffer - 1 ; x++)
                    BUFFER (x, yCaret) = BUFFER (x + 1, yCaret) ;
			   BUFFER (cxBuffer - 1, yCaret) = ' ' ;
				
			   order.erase(xCaret,1);

               HideCaret (hwnd) ;
               hdc = GetDC (hwnd) ;

               SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
                                   dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;

               TextOut (hdc, xCaret * cxChar, yCaret * cyChar,
                        & BUFFER (xCaret, yCaret),
                        cxBuffer - xCaret) ;

               DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
               ReleaseDC (hwnd, hdc) ;
               ShowCaret (hwnd) ;
               break ;
          }

          SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
          return 0 ;

     case WM_CHAR:
          for (i = 0 ; i < (int) LOWORD (lParam) ; i++)
          {
               switch (wParam)
               {
               case '\b':                    // backspace
                    if (xCaret > 0)
                    {
                         xCaret-- ;
                         SendMessage (hwnd, WM_KEYDOWN, VK_DELETE, 1) ;
                    }
                    break ;

               case '\t':                    // tab
                    do
                    {
                         SendMessage (hwnd, WM_CHAR, ' ', 1) ;
                    }
                    while (xCaret % 8 != 0) ;
                    break ;

               case '\n':                    // line feed
                    if (++yCaret == cyBuffer)
                         yCaret = 0 ;

                    break ;

               case '\r': {                  // carriage return
					//showMessage(order);
					if(order == "exit"){
						for (y = 0 ; y < cyBuffer ; y++)
							for (x = 0 ; x < cxBuffer ; x++)
								BUFFER (x, y) = ' ' ;

						xCaret = 0 ;
						yCaret = 0 ;
						
						PostMessage(hwnd, WM_CLOSE, 0, 0);
					} else if(order.substr(0,2) == "cd"){
						
						plotter.setAddress(order.substr(4,order.length()-4));
						
					} else {
						plotter(order);     	 // Call GNU plot
						
						xCaret = 0 ;

						if (++yCaret == cyBuffer)
							yCaret = 0 ;
					}
					
					orderVector.push_back(order);
					order = "";
					curOrderNum++;

                    break ;
				}

               case '\x1B':                  // escape
                    for (y = 0 ; y < cyBuffer ; y++)
                         for (x = 0 ; x < cxBuffer ; x++)
                              BUFFER (x, y) = ' ' ;

                    xCaret = 0 ;
                    yCaret = 0 ;
					
					order = "";

                    InvalidateRect (hwnd, NULL, FALSE) ;
                    break ;

               default:                      // character codes

					if(BUFFER (xCaret, yCaret) == ' '){
						
						BUFFER (xCaret, yCaret) = (TCHAR) wParam ;
						order += (char) wParam;
						
					} else {
						
						for(x = cxBuffer-1;x > xCaret;x--){
							BUFFER (x, yCaret) = BUFFER (x-1, yCaret);
						}
						
						BUFFER (xCaret, yCaret) = (TCHAR) wParam ;
						
						order.insert(xCaret,1,(char)wParam);
						
					}
					
					HideCaret (hwnd) ;
					hdc = GetDC (hwnd) ;

					SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
										dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;

					TextOut (hdc, xCaret * cxChar, yCaret * cyChar,
							& BUFFER (xCaret, yCaret),
							cxBuffer - xCaret) ;
							
					DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
					ReleaseDC (hwnd, hdc) ;
					ShowCaret (hwnd) ;

                    if (++xCaret == cxBuffer)
                    {
                         xCaret = 0 ;

                         if (++yCaret == cyBuffer)
                              yCaret = 0 ;
                    }
                    break ;
               }
          }

          SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
          return 0 ;

     case WM_PAINT:
          hdc = BeginPaint (hwnd, &ps) ;

          SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
                                   dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;

          for (y = 0 ; y < cyBuffer ; y++)
               TextOut (hdc, 0, y * cyChar, & BUFFER(0,y), cxBuffer) ;

          DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
          EndPaint (hwnd, &ps) ;
          return 0 ;

     case WM_CLOSE:
		  ShowWindow(hwnd, SW_HIDE);
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}