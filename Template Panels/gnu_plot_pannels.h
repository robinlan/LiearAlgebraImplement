/********************************************************************************
root_finding_panels.h, by Robin Lan - 2015/08/12
				
Example usage:
	Global setting:
		HWND typerHwnd;
		static TCHAR szTyperAppName[] = TEXT("Typer") ;
		int nGlobCmdShow;
	
	In WinMain:
		nGlobCmdShow = iCmdShow;
		WinWindows wincTyperObject(szTyperAppName,hInstance,iCmdShow);
		WNDCLASSEX wincTyper = wincTyperObject.getWinClass(TyperWindowProcedure);
		if( !wincTyperObject.getWinRegisterClass())
			return 0;
		typerHwnd = wincTyperObject.getWinHWND(444,275,_T("Typer Program"));
		
	In trigger place:
		ShowWindow (typerHwnd, nGlobCmdShow);
				
********************************************************************************/

#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include "res/win_widget.h"

#define IDC_TYPER_BUTTON	101
#define BUFFER(x,y) *(pBuffer + y * cxBuffer + x)

LRESULT CALLBACK TyperWindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     static DWORD   dwCharSet = DEFAULT_CHARSET ;
     static int     cxChar, cyChar, cxClient, cyClient, cxBuffer, cyBuffer,
                    xCaret, yCaret ;
     static TCHAR * pBuffer = NULL ;
     HDC            hdc ;
     int            x, y, i ;
     PAINTSTRUCT    ps ;
     TEXTMETRIC     tm ;
     
     switch (message)
     {
     case WM_INPUTLANGCHANGE:
          dwCharSet = wParam ;
                                        // fall through
     case WM_CREATE:
          hdc = GetDC (hwnd) ;
          SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
                                   dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;
          
          GetTextMetrics (hdc, &tm) ;
          cxChar = tm.tmAveCharWidth ;
          cyChar = tm.tmHeight ;
          
          DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
          ReleaseDC (hwnd, hdc) ;
                                        // fall through                
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
               // create and show the caret
          
          CreateCaret (hwnd, NULL, cxChar, cyChar) ;
          SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
          ShowCaret (hwnd) ;
          return 0 ;
          
     case WM_KILLFOCUS:
               // hide and destroy the caret

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
               
          case VK_PRIOR:
               yCaret = 0 ;
               break ;
               
          case VK_NEXT:
               yCaret = cyBuffer - 1 ;
               break ;
               
          case VK_LEFT:
               xCaret = max (xCaret - 1, 0) ;
               break ;
               
          case VK_RIGHT:
               xCaret = min (xCaret + 1, cxBuffer - 1) ;
               break ;
               
          case VK_UP:
               yCaret = max (yCaret - 1, 0) ;
               break ;
               
          case VK_DOWN:
               yCaret = min (yCaret + 1, cyBuffer - 1) ;
               break ;
               
          case VK_DELETE:
               for (x = xCaret ; x < cxBuffer - 1 ; x++)
                    BUFFER (x, yCaret) = BUFFER (x + 1, yCaret) ;
               
               BUFFER (cxBuffer - 1, yCaret) = ' ' ;
               
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
                    
               case '\r':                    // carriage return
                    xCaret = 0 ;
                    
                    if (++yCaret == cyBuffer)
                         yCaret = 0 ;
                    break ;
                    
               case '\x1B':                  // escape
                    for (y = 0 ; y < cyBuffer ; y++)
                         for (x = 0 ; x < cxBuffer ; x++)
                              BUFFER (x, y) = ' ' ;
                         
                    xCaret = 0 ;
                    yCaret = 0 ;
                         
                    InvalidateRect (hwnd, NULL, FALSE) ;
                    break ;
                         
               default:                      // character codes
                    BUFFER (xCaret, yCaret) = (TCHAR) wParam ;
                    
                    HideCaret (hwnd) ;
                    hdc = GetDC (hwnd) ;
          
                    SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
                                   dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;

                    TextOut (hdc, xCaret * cxChar, yCaret * cyChar,
                             & BUFFER (xCaret, yCaret), 1) ;

                    DeleteObject (
                         SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
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
          
     case WM_DESTROY:
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}