#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include "gnuplot_typer_panels.h"
#include "root_finding_panels.h"

#define IDC_FINDROOT_BUTTON 101
#define IDC_CALLPLOT_BUTTON 102
#define IDC_EXIT_BUTTON     103

using namespace std;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Store the hwnds  */
HWND GNUPlotTyperHwnd;
HWND rootfindingHwnd;

int nGlobCmdShow;

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
TCHAR szGNUPlotTyperAppName[] = TEXT("GNUPlotTyper") ;
TCHAR szClassNameRF[ ] = _T("RootFindingPanel");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
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

    WinWindows wincRFObject(szClassNameRF,hThisInstance,nCmdShow);
    WNDCLASSEX wincRF = wincRFObject.getWinClass(RootFindingWindowProcedure);

    WinWindows wincGNUPlotTyperObject(szGNUPlotTyperAppName,hThisInstance,nCmdShow);
    WNDCLASSEX wincGNUPlotTyper = wincGNUPlotTyperObject.getWinClass(GNUPlotTyperWindowProcedure);

    nGlobCmdShow = nCmdShow;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    if( !wincGNUPlotTyperObject.getWinRegisterClass())
        return 0;

    if( !wincRFObject.getWinRegisterClass())
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

    rootfindingHwnd = wincRFObject.getWinHWND(694,575,_T("Root finding Panel"));
    GNUPlotTyperHwnd = wincGNUPlotTyperObject.getWinHWND(444,275,_T("GNUPlot Typer Program"));

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

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:{
            WinButton findRootButton("Find Root",50,100,100,24,hwnd,(HMENU)IDC_FINDROOT_BUTTON);
            HWND button1 = findRootButton.getButton();
            WinButton callGNUPlotButton("Call GNUPlot",50,150,100,24,hwnd,(HMENU)IDC_CALLPLOT_BUTTON);
            HWND button2 = callGNUPlotButton.getButton();
            WinButton exitButton("Exit",50,200,100,24,hwnd,(HMENU)IDC_EXIT_BUTTON);
            HWND button3 = exitButton.getButton();
            break;
        }
        case WM_COMMAND:{
            switch(LOWORD(wParam)){

                case IDC_FINDROOT_BUTTON: {
                    ShowWindow (rootfindingHwnd, nGlobCmdShow);
                    break;
                }
                case IDC_CALLPLOT_BUTTON: {
                    ShowWindow (GNUPlotTyperHwnd, nGlobCmdShow);
                    break;
                }
                case IDC_EXIT_BUTTON: {
                    PostQuitMessage (0);
                    break;
                }

            }
            break;
        }
        case WM_PAINT:{
            hdc = BeginPaint(hwnd,&ps);
            string tmp1("Numerical Analysis 5th edit,                          ");
            string tmp2("    chapter 2-1: bisection search program,     ");
            string tmp3("                              by Robin Lan, 2015/08/13");
            TextOut(hdc,50,20,tmp1.c_str(),tmp1.length());
            TextOut(hdc,50,35,tmp2.c_str(),tmp2.length());
            TextOut(hdc,50,50,tmp3.c_str(),tmp3.length());
            EndPaint (hwnd,&ps) ;
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
