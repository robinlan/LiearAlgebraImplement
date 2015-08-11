#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

#define IDC_MAIN_BUTTON 101
#define IDC_CHILD_BUTTON 102

long PASCAL WndProc (HWND, UINT, WPARAM, LPARAM);
long PASCAL WndProc2 (HWND, UINT, WPARAM, LPARAM);

HWND _hwnd;
HWND hwnd2;
int _nCmdShow;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpszCmdParam, int nCmdShow)
{
   static char szClassName[] = "Hello World";
   static char szClassName2[] = "Hello World Child";
   MSG         msg;
   WNDCLASS    wndclass;
   WNDCLASS    wndclass2;
   _nCmdShow = nCmdShow;

   memset(&wndclass, '\0', sizeof(wndclass));
   memset(&wndclass2, '\0', sizeof(wndclass2));

   if (!hPrevInstance) {
      // define the 'Hello World' window class
      wndclass.style          = CS_HREDRAW|CS_VREDRAW;
      wndclass.lpfnWndProc    = WndProc;
      wndclass.cbClsExtra     = 0;
      wndclass.cbWndExtra     = 0;
      wndclass.hInstance      = hInstance;
      wndclass.hIcon          = LoadIcon (NULL, IDI_APPLICATION);
      wndclass.hCursor        = LoadCursor (NULL, IDC_ARROW);
      wndclass.hbrBackground  = (HBRUSH)GetStockObject (WHITE_BRUSH);
      wndclass.lpszMenuName   = 0;
      wndclass.lpszClassName  = szClassName;

      // register the 'Hello World' window class
      RegisterClass (&wndclass);



      // define the 'Hello World' window class
      wndclass2.style          = CS_HREDRAW|CS_VREDRAW;
      wndclass2.lpfnWndProc    = WndProc2;
      wndclass2.cbClsExtra     = 0;
      wndclass2.cbWndExtra     = 0;
      wndclass2.hInstance      = hInstance;
      wndclass2.hIcon          = LoadIcon (NULL, IDI_APPLICATION);
      wndclass2.hCursor        = LoadCursor (NULL, IDC_ARROW);
      wndclass2.hbrBackground  = (HBRUSH)GetStockObject (WHITE_BRUSH);
      wndclass2.lpszMenuName   = 0;
      wndclass2.lpszClassName  = szClassName2;

      // register the 'Hello World' window class
      RegisterClass (&wndclass2);

   }

   // create a new window that is a 'Hello World' window class
   _hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                              szClassName,
                              "My Hello World Window",
                              WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              NULL,
                              NULL,
                              hInstance,
                              NULL);

    // create a new window that is a 'Hello World' window class
    hwnd2 = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
                              szClassName2,
                              "My Hello World Window Child",
                              WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              NULL,
                              NULL,
                              hInstance,
                              NULL);

   ShowWindow (_hwnd, nCmdShow);

   while (GetMessage (&msg, NULL, 0, 0)) {
      TranslateMessage (&msg);
      DispatchMessage (&msg);
   }

   return msg.wParam;
}

long APIENTRY WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
       case WM_CREATE:{
            HWND hWndButton=CreateWindowEx(NULL,
				"BUTTON",
				"OK",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				50,
				220,
				100,
				24,
				hwnd,
				(HMENU)IDC_MAIN_BUTTON,
				GetModuleHandle(NULL),
				NULL);
            break;
       }
       case WM_COMMAND:{
            switch(LOWORD(wParam)) {

				case IDC_MAIN_BUTTON:
				{
					ShowWindow (_hwnd,SW_HIDE);
					ShowWindow (hwnd2,_nCmdShow);
				}
				break;

			}
			break;
       }
       case WM_DESTROY:
          PostQuitMessage (0);
          return 0;
    }

    return DefWindowProc (hwnd, message, wParam, lParam);
}

long APIENTRY WndProc2 (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
       case WM_CREATE:{
            HWND hWndButton=CreateWindowEx(NULL,
				"BUTTON",
				"BACK",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				50,
				220,
				100,
				24,
				hwnd,
				(HMENU)IDC_CHILD_BUTTON,
				GetModuleHandle(NULL),
				NULL);
            break;
       }
       case WM_COMMAND:{
            switch(LOWORD(wParam)) {

				case IDC_CHILD_BUTTON:
				{
					ShowWindow (hwnd2,SW_HIDE);
					ShowWindow (_hwnd,_nCmdShow);
				}
				break;

			}
			break;
       }
       case WM_DESTROY:
          PostQuitMessage (0);
          return 0;
    }

    return DefWindowProc (hwnd, message, wParam, lParam);
}
