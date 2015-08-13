#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <stdlib.h>
#include <string>
#include "typer_panels.h"

#define IDC_TYPER_BUTTON	101
#define BUFFER(x,y) *(pBuffer + y * cxBuffer + x)

using namespace std;

LRESULT CALLBACK TyperWindowProcedure (HWND, UINT, WPARAM, LPARAM) ;
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM) ;

HWND typerHwnd;
int nGlobCmdShow;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szMainAppName[] = TEXT ("Main") ;
	 static TCHAR szTyperAppName[] = TEXT ("Typer") ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;

     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WindowProcedure ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szMainAppName ;

	 nGlobCmdShow = iCmdShow;

	 WinWindows wincTyperObject(szTyperAppName,hInstance,iCmdShow);
	 WNDCLASSEX wincTyper = wincTyperObject.getWinClass(TyperWindowProcedure);

     if (!RegisterClass (&wndclass)) {
          return 0 ;
     }

	 if( !wincTyperObject.getWinRegisterClass())
		return 0;

     hwnd = CreateWindow (szMainAppName, TEXT ("Typing Program"),
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL) ;

	 typerHwnd = wincTyperObject.getWinHWND(444,275,_T("Typer Program"));

     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;

     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message) {

     case WM_CREATE: {

		WinButton button1("GO",50,220,100,24,hwnd,(HMENU)IDC_TYPER_BUTTON);
		HWND hbutton = button1.getButton();

		break;
	 }

	case WM_COMMAND: {

		switch(LOWORD(wParam)){

			case IDC_TYPER_BUTTON: {

				/* Make the window visible on the screen */
				ShowWindow (typerHwnd, nGlobCmdShow);

				break;
			}

		}

		break;
	}

     case WM_DESTROY:
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}
