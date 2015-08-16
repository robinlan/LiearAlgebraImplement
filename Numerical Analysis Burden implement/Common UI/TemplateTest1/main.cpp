#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "res/win_widget.h"
#include "gnuplot_typer_panels.h"
#include "typer_panels.h"
#include "root_finding_panels.h"
#include "mini_notepad_panels.h"

#define IDC_GNUPLOTTER_BUTTON  101
#define IDC_TYPER_BUTTON       102
#define IDC_ROOTFINDING_BUTTON 103
#define IDC_NOTEPAD_BUTTON     104

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("TemplateTest1");

/*  Global Settings  */
static TCHAR szNotepadAppName[] = TEXT ("MiniNotepad") ;
static TCHAR szTyperAppName[] = TEXT ("Typer") ;
static TCHAR szRootFindingAppName[] = TEXT ("RootFindingApp") ;
static TCHAR szGNUPlotTyperAppName[] = TEXT ("GNUPlotTyper") ;
static HWND notepadHwnd;
static HWND typerConsoleHwnd;
static HWND rootFindingHwnd;
static HWND GNUPlotTyperHwnd;
HINSTANCE hInstance;
LPSTR szCmdLine;
int nGlobCmdShow;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    hInstance = hThisInstance;
    szCmdLine = lpszArgument;
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

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Templates function testing 1"),       /* Title Text */
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

    typerConsoleHwnd = createTyperWindow(hInstance,szTyperAppName);
    rootFindingHwnd = createRootFindingWindow(hInstance,szRootFindingAppName);
    GNUPlotTyperHwnd = createGNUPlotTyperWindow(hInstance,szGNUPlotTyperAppName);

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
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE: {
            WinButton button1("GNU Plotter Console",50,50,120,24,hwnd,(HMENU)IDC_GNUPLOTTER_BUTTON);
            HWND hbutton1 = button1.getButton();
            WinButton button2("Typer Console",50,90,120,24,hwnd,(HMENU)IDC_TYPER_BUTTON);
            HWND hbutton2 = button2.getButton();
            WinButton button3("Root Finding App",50,130,120,24,hwnd,(HMENU)IDC_ROOTFINDING_BUTTON);
            HWND hbutton3 = button3.getButton();
            WinButton button4("Mini Notepad",50,170,120,24,hwnd,(HMENU)IDC_NOTEPAD_BUTTON);
            HWND hbutton4 = button4.getButton();
            break;
        }
        case WM_COMMAND: {

            switch(LOWORD(wParam)){
                case IDC_GNUPLOTTER_BUTTON: {
                    ShowWindow(GNUPlotTyperHwnd,nGlobCmdShow);
                    break;
                }
                case IDC_TYPER_BUTTON: {
                    ShowWindow(typerConsoleHwnd,nGlobCmdShow);
                    break;
                }
                case IDC_ROOTFINDING_BUTTON: {
                    ShowWindow(rootFindingHwnd,nGlobCmdShow);
                    break;
                }
                case IDC_NOTEPAD_BUTTON: {
                    NotepadWinMain(notepadHwnd,hInstance,szClassName,szNotepadAppName,nGlobCmdShow);
                    break;
                }
            }

            break;
        }
        case WM_DESTROY:
            SendMessage(typerConsoleHwnd, WM_DESTROY, 0, 0);
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
