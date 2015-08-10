#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <string>
#include "win_widget.h"

#define IDC_MAIN_BUTTON			101		// Button identifier
#define IDC_MAIN_EDITBOX		102		// Edit box identifier
#define IDC_POWER_BUTTON		103		// Power function setting button
#define IDC_POWERCOEFF_EDITBOX	104		// Power coefficient edit box
#define IDC_POWERPOWER_EDITBOX	105		// Power power edit box

using namespace std;

/***********************
** Global Declaration **
************************/
HWND hEdit1;
HWND hEdit2;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow) {
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    
	HDC hdc;
	PAINTSTRUCT ps;
	
	switch (message)                  /* handle the messages */
    {
        case WM_CREATE: {
			
			WinButton button1("POWER",50,100,100,24,hwnd,(HMENU)IDC_POWER_BUTTON);
			HWND hbutton = button1.getButton();
			
			WinEditbox editbox1("",170,100,100,24,hwnd,(HMENU)IDC_POWERCOEFF_EDITBOX);
			editbox1.setEditboxText((LPARAM)"coeff");
			hEdit1 = editbox1.getEditbox();
			
			WinEditbox editbox2("",290,100,100,24,hwnd,(HMENU)IDC_POWERPOWER_EDITBOX);
			editbox2.setEditboxText((LPARAM)"power");
			hEdit2 = editbox2.getEditbox();
			
			break;
		}
		
		case WM_COMMAND: {
			
			switch(LOWORD(wParam)) {
				
				case IDC_POWER_BUTTON: {
				
					char buffer[256];
					
					SendMessage(hEdit1,
						WM_GETTEXT,
						sizeof(buffer)/sizeof(buffer[0]),
						reinterpret_cast<LPARAM>(buffer));
					string coeffStr(buffer);
						
					SendMessage(hEdit2,
						WM_GETTEXT,
						sizeof(buffer)/sizeof(buffer[0]),
						reinterpret_cast<LPARAM>(buffer));
					string powerStr(buffer);

					InvalidateRect(hwnd,NULL,TRUE);
					
				}
				break;
				
			}
			
			break;
		}
		
		case WM_PAINT: {
			
			hdc = BeginPaint(hwnd,&ps);
			TextOut (hdc, 150, 20,"Hello world!",12) ;
			EndPaint (hwnd, &ps) ;
			
			break;
			
		}
		
		case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
