#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include "win_widget.h"
using namespace std;

#define IDC_INEERGNU_BUTTON  101 //button that call the GNUPlot
#define IDC_INNERBACK_BUTTON 102 //button that call the GNUPlot

HWND mainHwnd;               /* This is the handle for our window */
HWND anotherHwnd;
int nGlobCmdShow;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProcedure2 (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("InnerGNUPlotApp");
TCHAR szClassName2[ ] = _T("InnerGNUPlotPanel");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
	WNDCLASSEX winc2;        /* Data structure for the windowclass */

	nGlobCmdShow = nCmdShow;

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
	
	
	
	/* The Window structure */
    winc2.hInstance = hThisInstance;
    winc2.lpszClassName = szClassName2;
    winc2.lpfnWndProc = WindowProcedure2;      /* This function is called by windows */
    winc2.style = CS_DBLCLKS;                 /* Catch double-clicks */
    winc2.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    winc2.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    winc2.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    winc2.hCursor = LoadCursor (NULL, IDC_ARROW);
    winc2.lpszMenuName = NULL;                 /* No menu */
    winc2.cbClsExtra = 0;                      /* No extra bytes after the window class */
    winc2.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    winc2.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
	
	

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;
		
	/* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&winc2))
        return 0;

    /* The class is registered, let's create the program*/
    mainHwnd = CreateWindowEx (
           0,                   									/* Extended possibilites for variation */
           szClassName,         									/* Classname */
           _T("Integrate Root Finding with Inner GNUPlot"),         /* Title Text */
           WS_OVERLAPPEDWINDOW, 									/* default window */
           CW_USEDEFAULT,       									/* Windows decides the position */
           CW_USEDEFAULT,    									    /* where the window ends up on the screen */
           544,             									    /* The programs width */
           375,             									    /* and height in pixels */
           HWND_DESKTOP,    									    /* The window is a child-window to desktop */
           NULL,            									    /* No menu */
           hThisInstance,										    /* Program Instance handler */
           NULL                 									/* No Window Creation data */
           );
		   
	
	/* The class is registered, let's create the program*/
    anotherHwnd = CreateWindowEx (
           0,                   									/* Extended possibilites for variation */
           szClassName2,         									/* Classname */
           _T("Inner GNUPlotChild panel"), 	        				/* Title Text */
           WS_OVERLAPPEDWINDOW, 									/* default window */
           CW_USEDEFAULT,       									/* Windows decides the position */
           CW_USEDEFAULT,    									    /* where the window ends up on the screen */
           344,             									    /* The programs width */
           175,             									    /* and height in pixels */
           HWND_DESKTOP,    									    /* The window is a child-window to desktop */
           NULL,            									    /* No menu */
           hThisInstance,										    /* Program Instance handler */
           NULL                 									/* No Window Creation data */
		   );
           
	
	
	

    /* Make the window visible on the screen */
    ShowWindow (mainHwnd, nCmdShow);

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

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE: {
			
			WinButton innerGNUButton("GNUPlot",50,220,100,24,hwnd,(HMENU)IDC_INEERGNU_BUTTON);
			HWND innerGNUPlotButton = innerGNUButton.getButton();
			
			break;
		}
		
		case WM_COMMAND: {
			
			switch(LOWORD(wParam)){
				
				case IDC_INEERGNU_BUTTON: {
					
					/* Make the window visible on the screen */
					ShowWindow (mainHwnd, SW_HIDE);
					ShowWindow (anotherHwnd, nGlobCmdShow);
					
					break;
				}
				
			}
			
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


LRESULT CALLBACK WindowProcedure2 (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE: {
			
			WinButton innerGNUButton("BACK",50,100,100,24,hwnd,(HMENU)IDC_INNERBACK_BUTTON);
			HWND innerGNUPlotButton = innerGNUButton.getButton();
			
			break;
		}
		
		case WM_COMMAND: {
			
			switch(LOWORD(wParam)){
				
				case IDC_INNERBACK_BUTTON: {
					
					/* Make the window visible on the screen */
					ShowWindow (anotherHwnd, SW_HIDE);
					ShowWindow (mainHwnd, nGlobCmdShow);
					
					break;
				}
				
			}
			
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
