#include <windows.h>
#include <stdio.h>

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	
	PAINTSTRUCT	ps;          // used in WM_PAINT
	HDC hdc;         // handle to a device context
	char buffer[80];  // used to print strings
	
	switch(Message) {
		
		case WM_CREATE: {
			return 0;
			break;
		}
		
		case WM_MOVE: {
			// extract the position
         	int xpos = LOWORD(lParam);
         	int ypos = HIWORD(lParam);

         	// get a graphics context
         	hdc = GetDC(hwnd);

         	// set the foreground color to green
         	SetTextColor(hdc, RGB(0,255,0));
         
         	// set the background color to black
         	SetBkColor(hdc, RGB(0,0,0));
         
         	// set the transparency mode to OPAQUE
         	SetBkMode(hdc, OPAQUE);

         	// draw the size of the window
         	sprintf(buffer,"WM_MOVE Called -  New Positition = (%d,%d)", xpos, ypos);
         	TextOut(hdc, 0,0, buffer, strlen(buffer));
         	
         	// release the dc back
         	ReleaseDC(hwnd, hdc);
         	
			break;
		} 
		
		case WM_PAINT: {
			// simply validate the window 
   	        hdc = BeginPaint(hwnd,&ps);	 
        
            // end painting
            EndPaint(hwnd,&ps);

            // return success
			return(0);
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
