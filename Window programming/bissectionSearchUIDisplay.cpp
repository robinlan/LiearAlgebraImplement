#include <windows.h>
#include <stdio.h>
#include <string>
using namespace std;

#define IDC_INPUT_BUTTON	101			// Input button identifier
#define IDC_X_EDIT	102			// X edit box identifier
#define IDC_STARTVAL_EDIT	103			// startVal edit box identifier
#define IDC_ENDVAL_EDIT	104			// endVal edit box identifier
#define IDC_TOLERROR_EDIT	105			// tolerance error edit box identifier
#define IDC_MAXITER_EDIT	106			// maximum iteration edit box identifier

#define IDC_POWER_BUTTON	107			// Power button identifier
#define IDC_POWER_POWER_EDIT	108			// Power power button identifier
#define IDC_POWER_COEFFS_EDIT	109			// Power coefficients button identifier

#define IDC_CALC_BUTTON	110			// Power coefficients button identifier

HWND hEdit_x; //Input x
HWND hEdit_startVal; //Input start value
HWND hEdit_endVal; //Input end value
HWND hEdit_tolError; //Input tolerance error
HWND hEdit_maxIter; //Input maximum iteration

HWND hEdit_powerCoeffs; //Input coeffeicients for power
HWND hEdit_powerPower; //Input power for power

void createInputInfo(HWND hwnd);
void createPowerInputInfo(HWND hwnd);

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);
	
	switch(Message) {
		
		case WM_CREATE: {
			
			/*Create the input setting editboxes*/
			createInputInfo(hwnd);
			
			// Create input push button
			HWND hWndInputButton=CreateWindowEx(NULL,
				"BUTTON",
				"INPUT",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				400,
				100,
				50,
				20,
				hwnd,
				(HMENU)IDC_INPUT_BUTTON,
				GetModuleHandle(NULL),
				NULL);
				
			SendMessage(hWndInputButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
				
			// Create power button
			HWND hWndPowerButton=CreateWindowEx(NULL,
				"BUTTON",
				"POWER",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				50,
				140,
				50,
				20,
				hwnd,
				(HMENU)IDC_POWER_BUTTON,
				GetModuleHandle(NULL),
				NULL);
				
			SendMessage(hWndPowerButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
				
			/*Create the power input setting editboxes*/
			createPowerInputInfo(hwnd);
			
			// Create calculate button
			HWND hWndCalcButton=CreateWindowEx(NULL,
				"BUTTON",
				"CALC",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				50,
				180,
				50,
				20,
				hwnd,
				(HMENU)IDC_CALC_BUTTON,
				GetModuleHandle(NULL),
				NULL);
				
			SendMessage(hWndCalcButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
			
			break;
		}
		
		case WM_COMMAND: {
			
			switch(LOWORD(wParam)) {
				
				case IDC_INPUT_BUTTON:
				{
					char buffer[256];
					SendMessage(hEdit_x,
						WM_GETTEXT,
						sizeof(buffer)/sizeof(buffer[0]),
						reinterpret_cast<LPARAM>(buffer));
						
					MessageBox(NULL,
						buffer,
						"Information",
						MB_ICONINFORMATION);
				}
				break;
				
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

void createInputInfo(HWND hwnd){
	
	HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);
	
	/*create the x input editbox*/
	hEdit_x=CreateWindowEx(WS_EX_CLIENTEDGE,
		"EDIT",
		"",
		WS_CHILD|WS_VISIBLE|
		ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
		50,
		100,
		50,
		20,
		hwnd,
		(HMENU)IDC_X_EDIT,
		GetModuleHandle(NULL),
		NULL);
	
	SendMessage(hEdit_x,
		WM_SETFONT,
		(WPARAM)hfDefault,
		MAKELPARAM(FALSE,0));
		
	SendMessage(hEdit_x,
		WM_SETTEXT,
		NULL,
		(LPARAM)"x");
		
	/*create the start value input editbox*/
	hEdit_startVal=CreateWindowEx(WS_EX_CLIENTEDGE,
		"EDIT",
		"",
		WS_CHILD|WS_VISIBLE|
		ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
		120,
		100,
		50,
		20,
		hwnd,
		(HMENU)IDC_STARTVAL_EDIT,
		GetModuleHandle(NULL),
		NULL);
	
	SendMessage(hEdit_startVal,
		WM_SETFONT,
		(WPARAM)hfDefault,
		MAKELPARAM(FALSE,0));
		
	SendMessage(hEdit_startVal,
		WM_SETTEXT,
		NULL,
		(LPARAM)"startVal");
		
	/*create the end value input editbox*/
	hEdit_endVal=CreateWindowEx(WS_EX_CLIENTEDGE,
		"EDIT",
		"",
		WS_CHILD|WS_VISIBLE|
		ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
		190,
		100,
		50,
		20,
		hwnd,
		(HMENU)IDC_ENDVAL_EDIT,
		GetModuleHandle(NULL),
		NULL);
	
	SendMessage(hEdit_endVal,
		WM_SETFONT,
		(WPARAM)hfDefault,
		MAKELPARAM(FALSE,0));
		
	SendMessage(hEdit_endVal,
		WM_SETTEXT,
		NULL,
		(LPARAM)"endVal");
		
	/*create the tolerance error input editbox*/
	hEdit_tolError=CreateWindowEx(WS_EX_CLIENTEDGE,
		"EDIT",
		"",
		WS_CHILD|WS_VISIBLE|
		ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
		260,
		100,
		50,
		20,
		hwnd,
		(HMENU)IDC_TOLERROR_EDIT,
		GetModuleHandle(NULL),
		NULL);
	
	SendMessage(hEdit_tolError,
		WM_SETFONT,
		(WPARAM)hfDefault,
		MAKELPARAM(FALSE,0));
		
	SendMessage(hEdit_tolError,
		WM_SETTEXT,
		NULL,
		(LPARAM)"tolError");
		
	/*create the maximum iteration input editbox*/
	hEdit_maxIter=CreateWindowEx(WS_EX_CLIENTEDGE,
		"EDIT",
		"",
		WS_CHILD|WS_VISIBLE|
		ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
		330,
		100,
		50,
		20,
		hwnd,
		(HMENU)IDC_MAXITER_EDIT,
		GetModuleHandle(NULL),
		NULL);
	
	SendMessage(hEdit_maxIter,
		WM_SETFONT,
		(WPARAM)hfDefault,
		MAKELPARAM(FALSE,0));
		
	SendMessage(hEdit_maxIter,
		WM_SETTEXT,
		NULL,
		(LPARAM)"maxIter");
		
}

void createPowerInputInfo(HWND hwnd){
	
	HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);
	
	/*create the power power input editbox*/
	hEdit_powerPower=CreateWindowEx(WS_EX_CLIENTEDGE,
		"EDIT",
		"",
		WS_CHILD|WS_VISIBLE|
		ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
		120,
		140,
		50,
		20,
		hwnd,
		(HMENU)IDC_POWER_POWER_EDIT,
		GetModuleHandle(NULL),
		NULL);
	
	SendMessage(hEdit_powerPower,
		WM_SETFONT,
		(WPARAM)hfDefault,
		MAKELPARAM(FALSE,0));
		
	SendMessage(hEdit_powerPower,
		WM_SETTEXT,
		NULL,
		(LPARAM)"power");
		
	/*create the power power input editbox*/
	hEdit_powerCoeffs=CreateWindowEx(WS_EX_CLIENTEDGE,
		"EDIT",
		"",
		WS_CHILD|WS_VISIBLE|
		ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
		190,
		140,
		50,
		20,
		hwnd,
		(HMENU)IDC_POWER_COEFFS_EDIT,
		GetModuleHandle(NULL),
		NULL);
	
	SendMessage(hEdit_powerCoeffs,
		WM_SETFONT,
		(WPARAM)hfDefault,
		MAKELPARAM(FALSE,0));
		
	SendMessage(hEdit_powerCoeffs,
		WM_SETTEXT,
		NULL,
		(LPARAM)"coeffs");
	
}