#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "resource.h"

const char MainClassName[]  = "MainMDIWnd";
const char ChildClassName[] = "MDIChildWnd";

const int StartChildrenNo = 994;

HWND hWndMainFrame  = NULL;
HWND hWndChildFrame = NULL;

BOOL CreateNewDocument(HINSTANCE hInstance);
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg,
							 WPARAM wParam, LPARAM lParam);
BOOL CreateNewDocument(HINSTANCE hInstance);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX WndClsEx;
	MSG Msg;

	WndClsEx.cbSize		 = sizeof(WNDCLASSEX);
	WndClsEx.style		 = 0;
	WndClsEx.lpfnWndProc	 = MainWndProc;
	WndClsEx.cbClsExtra	 = 0;
	WndClsEx.cbWndExtra	 = 0;
	WndClsEx.hInstance	 = hInstance;
	WndClsEx.hIcon		 = LoadIcon(NULL, IDI_ASTERISK);
	WndClsEx.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	WndClsEx.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	WndClsEx.lpszMenuName  = MAKEINTRESOURCE(IDR_MAIN_MENU);
	WndClsEx.lpszClassName = MainClassName;
	WndClsEx.hIconSm		 = LoadIcon(NULL, IDI_ASTERISK);

	if(!RegisterClassEx(&WndClsEx))
	{
		MessageBox(NULL,
			       "Window Registration Failed!", "Error!",
			       MB_OK);
		return 0;
	}

	if( !CreateNewDocument(hInstance) )
		return 0;

	hWndMainFrame = CreateWindowEx(0L,
                          MainClassName,
						  "Multiple Document Application",
						  WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);

	if(hWndMainFrame == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hWndMainFrame, nCmdShow);
	UpdateWindow(hWndMainFrame);

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		if (!TranslateMDISysAccel(hWndChildFrame, &Msg))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
	return 0;
}

HWND CreateNewMDIChild(HWND hMDIClient)
{
	MDICREATESTRUCT mcs;
	HWND NewWnd;

	mcs.szTitle = "Untitled";
	mcs.szClass = ChildClassName;
	mcs.hOwner  = GetModuleHandle(NULL);
	mcs.x = mcs.cx = CW_USEDEFAULT;
	mcs.y = mcs.cy = CW_USEDEFAULT;
	mcs.style = MDIS_ALLCHILDSTYLES;

	NewWnd = (HWND)SendMessage(hMDIClient, WM_MDICREATE, 0, (LONG)&mcs);

	if( !NewWnd )
	{
		MessageBox(NULL,
				   "Error creaing child window",
				   "Creation Error",
				   MB_OK);
	}
	return NewWnd;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_CREATE:
		{
			CLIENTCREATESTRUCT ccs;

			ccs.hWindowMenu  = GetSubMenu(GetMenu(hWnd), 2);
			ccs.idFirstChild = StartChildrenNo;

			hWndChildFrame = CreateWindowEx(WS_EX_CLIENTEDGE,
				                           "MDICLIENT",
										   NULL,
										   WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL
										            | WS_HSCROLL | WS_VISIBLE,
				                           CW_USEDEFAULT,
										   CW_USEDEFAULT,
										   680,
										   460,
										   hWnd,
										   (HMENU)IDM_FILE_NEW,
										   GetModuleHandle(NULL),
										   (LPVOID)&ccs);

			if(hWndChildFrame == NULL)
				MessageBox(hWnd, "Could not create MDI client.", "Error", MB_OK | MB_ICONERROR);
		}
		CreateNewMDIChild(hWndChildFrame);
		break;
		case WM_SIZE:
		{
			HWND hWndMDI;
			RECT rctClient;

			GetClientRect(hWnd, &rctClient);

			hWndMDI = GetDlgItem(hWnd, IDM_FILE_NEW);
			SetWindowPos(hWndMDI, NULL, 0, 0, rctClient.right, rctClient.bottom, SWP_NOZORDER);
		}
		break;
		case WM_CLOSE:
			DestroyWindow(hWnd);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDM_FILE_NEW:
					CreateNewMDIChild(hWndChildFrame);
				break;
				case IDM_FILE_CLOSE:
				{
					HWND hChild = (HWND)SendMessage(hWndChildFrame, WM_MDIGETACTIVE,0,0);
					if(hChild)
					{
						SendMessage(hChild, WM_CLOSE, 0, 0);
					}
				}
				break;
				case IDM_FILE_EXIT:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
				break;
				case IDM_WINDOW_TILE:
					SendMessage(hWndChildFrame, WM_MDITILE, 0, 0);
					break;

				case IDM_WINDOW_CASCADE:
					SendMessage(hWndChildFrame, WM_MDICASCADE, 0, 0);
					break;

				case IDM_WINDOW_ICONS:
					SendMessage(hWndChildFrame, WM_MDIICONARRANGE, 0, 0);
					break;

				case IDM_WINDOW_CLOSE_ALL:
				{
					HWND hWndCurrent;

					do {
						hWndCurrent = (HWND)SendMessage(hWndChildFrame, WM_MDIGETACTIVE,0,0);
						SendMessage(hWndCurrent, WM_CLOSE, 0, 0);
					}while(hWndCurrent);
				}
				break;

				default:
				{
					if(LOWORD(wParam) >= StartChildrenNo)
					{
						DefFrameProc(hWnd, hWndChildFrame, WM_COMMAND, wParam, lParam);
					}
					else
					{
						HWND hWndCurrent = (HWND)SendMessage(hWndChildFrame, WM_MDIGETACTIVE,0,0);
						if( hWndCurrent )
						{
							SendMessage(hWndCurrent, WM_COMMAND, wParam, lParam);
						}
					}
				}
			}
		break;
		default:
			return DefFrameProc(hWnd, hWndChildFrame, Msg, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
		break;

		case WM_MDIACTIVATE:
		{
			HMENU hMenu, hFileMenu;
			UINT EnableFlag;

			hMenu = GetMenu(hWndMainFrame);
			if(hwnd == (HWND)lParam)
			{
				EnableFlag = MF_ENABLED;
			}
			else
			{
				EnableFlag = MF_GRAYED;
			}

			EnableMenuItem(hMenu, 1, MF_BYPOSITION | EnableFlag);
			EnableMenuItem(hMenu, 2, MF_BYPOSITION | EnableFlag);

			hFileMenu = GetSubMenu(hMenu, 0);

			EnableMenuItem(hFileMenu, IDM_FILE_CLOSE, MF_BYCOMMAND | EnableFlag);
			EnableMenuItem(hFileMenu, IDM_WINDOW_CLOSE_ALL, MF_BYCOMMAND | EnableFlag);

			DrawMenuBar(hWndMainFrame);
		}
		break;

		default:
			return DefMDIChildProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

BOOL CreateNewDocument(HINSTANCE hInstance)
{
	WNDCLASSEX WndClsEx;

	WndClsEx.cbSize		 = sizeof(WNDCLASSEX);
	WndClsEx.style		 = CS_HREDRAW | CS_VREDRAW;
	WndClsEx.lpfnWndProc = ChildWndProc;
	WndClsEx.cbClsExtra	 = 0;
	WndClsEx.cbWndExtra	 = 0;
	WndClsEx.hInstance	 = hInstance;
	WndClsEx.hIcon		 = LoadIcon(NULL, IDI_WARNING);
	WndClsEx.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	WndClsEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	WndClsEx.lpszMenuName  = NULL;
	WndClsEx.lpszClassName = ChildClassName;
	WndClsEx.hIconSm		 = LoadIcon(NULL, IDI_WARNING);

	if(!RegisterClassEx(&WndClsEx))
	{
		MessageBox(NULL,
			       "There was a problem when attempting to create a document",
			       "Application Error",
				   MB_OK);
		return FALSE;
	}
	else
		return TRUE;
}
