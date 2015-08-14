#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "win_widget.h"

#define IDM_FILE_NEW  101
#define IDM_FILE_OPEN 102
#define IDM_EDIT_COPY 103
#define IDM_EDIT_CUT  104
#define IDM_ABOUT_MYSELF  105
#define IDM_ABOUT_MYFAMILY  106
#define IDM_VIEW_BLACK  107
#define IDM_VIEW_WHITE  108

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

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

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UINT uTemp = MF_BYPOSITION | MF_POPUP;

    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:{
        HMENU hMenu;
        /*HMENU hMenuPop;
        hMenu = CreateMenu();
        hMenuPop = CreateMenu();
        AppendMenu(hMenuPop,MF_STRING,IDM_FILE_NEW,TEXT("New"));
        AppendMenu(hMenuPop,MF_STRING,IDM_FILE_OPEN,TEXT("Open"));
        AppendMenu(hMenu,MF_POPUP,(unsigned int)hMenuPop,TEXT("File"));

        hMenuPop = CreateMenu();

        AppendMenu(hMenuPop,MF_STRING,IDM_EDIT_COPY,TEXT("Copy"));
        AppendMenu(hMenuPop, MF_SEPARATOR, 0, NULL) ;
        AppendMenu(hMenuPop,MF_STRING,IDM_EDIT_CUT,TEXT("Cut"));
        AppendMenu(hMenu,MF_POPUP,(unsigned int)hMenuPop,TEXT("Edit"));
        InsertMenu(hMenu, 0, uTemp, IDM_EDIT_CUT+7, TEXT("NewAdd"));*/

        WinMenu thirdPopupMenu;
        thirdPopupMenu.insertStrOptions(IDM_ABOUT_MYSELF,TEXT("Myself"),1);
        thirdPopupMenu.insertStrOptions(IDM_ABOUT_MYFAMILY,TEXT("My family"),1);
        thirdPopupMenu.linkPopupMenuToMainMenu(TEXT("About"),1);

        thirdPopupMenu.createPopupMenu();
        thirdPopupMenu.insertStrOptions(IDM_ABOUT_MYSELF,TEXT("Black"),2);
        thirdPopupMenu.insertStrOptions(IDM_ABOUT_MYFAMILY,TEXT("White"),2);
        thirdPopupMenu.linkPopupMenuToMainMenu(TEXT("View"),2);

        hMenu = thirdPopupMenu.getMainMenu();

        SetMenu(hwnd, hMenu);
        break;
    }
    case WM_COMMAND: {

        switch(LOWORD(wParam)){

            case IDM_FILE_NEW: {
                MessageBox(NULL,
						"File->New is called.",
						"Information",
						MB_ICONINFORMATION);
                break;
            }
            case IDM_FILE_OPEN: {
                MessageBox(NULL,
						"File->Open is called.",
						"Information",
						MB_ICONINFORMATION);
                break;
            }
            case IDM_EDIT_COPY: {
                MessageBox(NULL,
						"Edit->Copy is called.",
						"Information",
						MB_ICONINFORMATION);
                break;
            }
            case IDM_EDIT_CUT: {
                MessageBox(NULL,
						"Edit->Cut is called.",
						"Information",
						MB_ICONINFORMATION);
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
