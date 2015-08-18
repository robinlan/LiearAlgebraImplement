#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <string>
#include <vector>
#include "resource.h"
#include "comand_console_panels.h"

#define IDC_TYPER_BUTTON	101
#define IDC_SETTING_ADDRESS	102
#define BUFFER(x,y) *(pBuffer + y * cxBuffer + x)

using namespace std;

const char MainClassName[]  = "MainMDIWnd";
char ChildClassName[] = "MDIChildWnd";

const int StartChildrenNo = 994;

HWND hWndMainFrame  = NULL;
HWND hWndChildFrame = NULL;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg,
							 WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {

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

	if(!RegisterClassEx(&WndClsEx)) {

		MessageBox(NULL,
			       "Window Registration Failed!", "Error!",
			       MB_OK);
		return 0;
	}

	if( !CreateNewCommandCosoleDocument(hInstance,ChildClassName) )
		return 0;

	hWndMainFrame = CreateWindowEx(0L,
                          MainClassName,
						  "Multiple Document Application",
						  WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, //Clip the overlapping part of the child window
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  CW_USEDEFAULT,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);

	if(hWndMainFrame == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hWndMainFrame, nCmdShow);
	UpdateWindow(hWndMainFrame);

	while(GetMessage(&Msg, NULL, 0, 0) > 0) {

		if (!TranslateMDISysAccel(hWndChildFrame, &Msg))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
	return 0;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch(Msg) {

		case WM_CREATE: {

			CLIENTCREATESTRUCT ccs; //Contains information about menu and first child window

			ccs.hWindowMenu  = GetSubMenu(GetMenu(hWnd), 2);
			ccs.idFirstChild = StartChildrenNo;

			/*  Use CLIENTCREATESTRUCT to construct first children window  */
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
		CreateNewCommandConsoleMDIChildren(hWndChildFrame,ChildClassName,hWndMainFrame);
		SendMessage(hWnd,WM_COMMAND,(WPARAM)IDM_WINDOW_TILE,0);
		break;
		case WM_SIZE: {

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
			switch(LOWORD(wParam)) {

				case IDM_FILE_NEW:
					CreateNewCommandConsoleMDIChildren(hWndChildFrame,ChildClassName,hWndMainFrame);
					SendMessage(hWnd,WM_COMMAND,(WPARAM)IDM_WINDOW_TILE,0);
				break;
				case IDM_FILE_CLOSE: {

					HWND hChild = (HWND)SendMessage(hWndChildFrame, WM_MDIGETACTIVE,0,0);
					if(hChild) {
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

				case IDM_WINDOW_CLOSE_ALL: {

					HWND hWndCurrent;

					do {
						hWndCurrent = (HWND)SendMessage(hWndChildFrame, WM_MDIGETACTIVE,0,0);
						SendMessage(hWndCurrent, WM_CLOSE, 0, 0);
					} while(hWndCurrent);
				}
				break;

				default: {

					if(LOWORD(wParam) >= StartChildrenNo) {
						//Activate another current children window
						DefFrameProc(hWnd, hWndChildFrame, WM_COMMAND, wParam, lParam);
					} else {
						HWND hWndCurrent = (HWND)SendMessage(hWndChildFrame, WM_MDIGETACTIVE,0,0);
						if( hWndCurrent ) {
							//If child window exists, send the message to the current children window
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
