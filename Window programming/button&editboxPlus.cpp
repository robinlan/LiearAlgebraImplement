#include <windows.h>
#include <stdio.h>
#include <sstream>
#include <string.h>
using namespace std;

#define IDC_MAIN_BUTTON	101			// OK Button identifier
#define IDC_MAIN_EDIT	102			// Edit box identifier
#define IDC_NUMONE_EDIT	103			// Number 1 edit box identifier
#define IDC_NUMTWO_EDIT	104			// Number 2 edit box identifier
#define IDC_PLUS_BUTTON	105			// PLUS Button identifier
HWND hEdit;
HWND hEdit1;
HWND hEdit2;

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	switch(Message) {
		
		case WM_CREATE: {
			
			/*******************Create an edit box********************
			
			HWND CreateWindowEx(
				DWOR DdwExStyle,        //New window dynamic setting
				LPCTSTR lpClassName,    //New window type
				LPCTSTR lpWindowName,   //Window title
				DWORD dwStyle,          //Window static style
				int x,                  //x-axis position
				int y,                  //y-axis position
				int nWidth,             //Window width
				int nHeight,            //Window height
				HWND hWndParent,        //Father window
				HMENU hMenu,            //Menu
				HINSTANCE hInstance,    //Application instance
				LPVOID lpParam          //Some messages send to CreateWindowEx()
			);
			
			*********************************************************/
			
			hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,
				"EDIT",
				"",
				WS_CHILD|WS_VISIBLE|
				ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
				50,
				100,
				200,
				100,
				hwnd,
				(HMENU)IDC_MAIN_EDIT,
				GetModuleHandle(NULL),
				NULL);
				
			/*Font and brush settings*/
			HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);
			
			/**************Send message to specific window**************
			
			LRESULT SendMessage（HWND hWnd, //Window that receive message 
								 UINT Msg, //Message to send
								 WPARAM wParam, 
								 LPARAM IParam）
								 
			************************************************************/
			SendMessage(hEdit,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
				
			SendMessage(hEdit,
				WM_SETTEXT,
				NULL,
				(LPARAM)"Insert text here...");
				
			/*Editbox to store the first number*/
			hEdit1=CreateWindowEx(WS_EX_CLIENTEDGE,
				"EDIT",
				"",
				WS_CHILD|WS_VISIBLE|
				ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
				300,
				100,
				100,
				20,
				hwnd,
				(HMENU)IDC_NUMONE_EDIT,
				GetModuleHandle(NULL),
				NULL);
				
			HGDIOBJ hfDefault2=GetStockObject(DEFAULT_GUI_FONT);
			
			SendMessage(hEdit1,
				WM_SETFONT,
				(WPARAM)hfDefault2,
				MAKELPARAM(FALSE,0));
				
			SendMessage(hEdit1,
				WM_SETTEXT,
				NULL,
				(LPARAM)"First number");
				
			/*Editbox to store the second number*/
			hEdit2=CreateWindowEx(WS_EX_CLIENTEDGE,
				"EDIT",
				"",
				WS_CHILD|WS_VISIBLE|
				ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
				300,
				130,
				100,
				20,
				hwnd,
				(HMENU)IDC_NUMTWO_EDIT,
				GetModuleHandle(NULL),
				NULL);
				
			HGDIOBJ hfDefault3=GetStockObject(DEFAULT_GUI_FONT);
			
			SendMessage(hEdit2,
				WM_SETFONT,
				(WPARAM)hfDefault3,
				MAKELPARAM(FALSE,0));
				
			SendMessage(hEdit2,
				WM_SETTEXT,
				NULL,
				(LPARAM)"Second number");

			// Create a push button
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
				
			SendMessage(hWndButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
				
			// Create the plus button
			HWND hWndPlusButton=CreateWindowEx(NULL,
				"BUTTON",
				"PLUS",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				300,
				220,
				100,
				24,
				hwnd,
				(HMENU)IDC_PLUS_BUTTON,
				GetModuleHandle(NULL),
				NULL);
				
			SendMessage(hWndPlusButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
				
			break;
		}
		
		case WM_COMMAND: {
			
			/*Identify which window is pressed*/
			switch(LOWORD(wParam)) {
				
				/*OK button is pressed*/
				case IDC_MAIN_BUTTON: {
					
					char buffer[256];
					
					/*Get the editbox message with buffer pointer*/
					SendMessage(hEdit,
						WM_GETTEXT,
						sizeof(buffer)/sizeof(buffer[0]),
						reinterpret_cast<LPARAM>(buffer));
						
					/*Trigger a message box and display the buffer message*/
					MessageBox(NULL,
						buffer,
						"Information",
						MB_ICONINFORMATION);
						
					break;
				}
				
				/*PLUS button is pressed*/
				case IDC_PLUS_BUTTON: {
					
					char* buffer;
					char firstNum[256];
					char secondNum[256];
					
					/*Get the editbox message with buffer pointer*/
					SendMessage(hEdit1,
						WM_GETTEXT,
						sizeof(firstNum)/sizeof(firstNum[0]),
						reinterpret_cast<LPARAM>(firstNum));
						
					/*Get the editbox message with buffer pointer*/
					SendMessage(hEdit2,
						WM_GETTEXT,
						sizeof(secondNum)/sizeof(secondNum[0]),
						reinterpret_cast<LPARAM>(secondNum));
					
					/*Plus the two values*/
					int num1 = firstNum[0] - '0';
					int num2 = secondNum[0] - '0';
					stringstream strs;
					string tmp;
					
					num1 = num1 + num2;
					strs << num1;
					tmp = strs.str();
					buffer = (char*) tmp.c_str();
					
					/*Trigger a message box and display the buffer message*/
					MessageBox(NULL,
						buffer,
						"Information",
						MB_ICONINFORMATION);
						
					break;
				}
				
			}
		
			break;
		}
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
