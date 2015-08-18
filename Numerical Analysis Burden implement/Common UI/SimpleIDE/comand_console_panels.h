#include <windows.h>
#include <string>
#include <vector>

#define BUFFER(x,y) *(pBuffer + y * cxBuffer + x)

using namespace std;

static HWND hWndMainFrame2  = NULL;

/*  Construct the MDI object  */
HWND CreateNewCommandConsoleMDIChildren(HWND hMDIClient, char* ChildClassName, HWND hWndMainFrameHwnd) {

	MDICREATESTRUCT mcs;
	HWND NewWnd;
	
	hWndMainFrame2 = hWndMainFrameHwnd;

	mcs.szTitle = "Untitled";
	mcs.szClass = ChildClassName;
	mcs.hOwner  = GetModuleHandle(NULL);
	mcs.x = mcs.cx = CW_USEDEFAULT;
	mcs.y = mcs.cy = CW_USEDEFAULT;
	mcs.style = MDIS_ALLCHILDSTYLES;

	NewWnd = (HWND)SendMessage(hMDIClient, WM_MDICREATE, 0, (LONG)&mcs);

	if( !NewWnd ) {
		MessageBox(NULL,
				   "Error creating child window",
				   "Creation Error",
				   MB_OK);
	}
	return NewWnd;
}

/*  Child window procedure  */
LRESULT CALLBACK CommandConsoleWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	static HINSTANCE hFatherInstance = (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE) ;
	
	static HMENU hMenu;

    static DWORD   dwCharSet = DEFAULT_CHARSET ;
    static int     cxChar, cyChar, cxClient, cyClient, cxBuffer, cyBuffer,
                   xCaret, yCaret ;
    static TCHAR * pBuffer = NULL ;

	static string order = "";   //Store the order every line
	static vector<string> orderVector;
	static int curOrderNum = 0;

	static string address;
	static HWND addressHWND;

    HDC            hdc ;
    int            x, y, i ;
    PAINTSTRUCT    ps ;
    TEXTMETRIC     tm ;
	
	switch(msg) {
	
		case WM_CREATE: {
		
			hdc = GetDC (hwnd) ;
			SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
                                   dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;

			GetTextMetrics (hdc, &tm) ;
			cxChar = tm.tmAveCharWidth ;
			cyChar = tm.tmHeight ;

			DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
			ReleaseDC (hwnd, hdc) ;
		}
		break;
		
		case WM_SIZE: {
			if (msg == WM_SIZE) {
               cxClient = LOWORD (lParam) ;
               cyClient = HIWORD (lParam) ;
            }
            // calculate window size in characters

            cxBuffer = max (1, cxClient / cxChar) ;
            cyBuffer = max (1, cyClient / cyChar) ;

            // allocate memory for buffer and clear it

            if (pBuffer != NULL)
				free (pBuffer) ;

			pBuffer = (TCHAR *) malloc (cxBuffer * cyBuffer * sizeof (TCHAR)) ;

			for (y = 0 ; y < cyBuffer ; y++)
				for (x = 0 ; x < cxBuffer ; x++)
					BUFFER(x,y) = ' ' ;

				// set caret to upper left corner

			xCaret = 0 ;
			yCaret = 0 ;

			if (hwnd == GetFocus ())
				SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;

			InvalidateRect (hwnd, NULL, TRUE) ;
			break;
		}
	 
		case WM_SETFOCUS:
			CreateCaret (hwnd, NULL, cxChar, cyChar) ;
			SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
			ShowCaret (hwnd) ;
			return 0 ;

		case WM_KILLFOCUS:
			HideCaret (hwnd) ;
			DestroyCaret () ;
			return 0 ;
			
		case WM_KEYDOWN:
		
			switch (wParam) {
			
				case VK_HOME:
					xCaret = 0 ;
					break ;

				case VK_END:
					xCaret = cxBuffer - 1 ;
					break ;

				case VK_LEFT:
					xCaret = max (xCaret - 1, 0) ;
					break ;

				case VK_RIGHT:
					xCaret = min (xCaret + 1, cxBuffer - 1) ;
					break ;

				case VK_UP:{

					for (x = 0 ; x < cxBuffer ; x++)
						BUFFER (x, yCaret) =  ' ';
				
					if(curOrderNum > 0){
						char* tmpCharArray = const_cast<char*>(orderVector.at(curOrderNum-1).c_str());
					
						for (x = 0 ; x < orderVector.at(curOrderNum-1).length() ; x++)
						BUFFER (x, yCaret) =  *(tmpCharArray+x);

						order = orderVector.at(curOrderNum-1);
						xCaret = orderVector.at(curOrderNum-1).length();
					
						curOrderNum = curOrderNum - 1;
					} else {
						xCaret = 0;
					}

					HideCaret (hwnd) ;
					hdc = GetDC (hwnd) ;

					SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
									dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;

					TextOut (hdc, 0, yCaret * cyChar,
							& BUFFER (0, yCaret),
							cxBuffer) ;

					DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
					ReleaseDC (hwnd, hdc) ;
					ShowCaret (hwnd) ;

					break;
				}

				case VK_DELETE:
					for (x = xCaret ; x < cxBuffer - 1 ; x++)
						BUFFER (x, yCaret) = BUFFER (x + 1, yCaret) ;
					BUFFER (cxBuffer - 1, yCaret) = ' ' ;

					order.erase(xCaret,1);

					HideCaret (hwnd) ;
					hdc = GetDC (hwnd) ;

					SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
									dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;

					TextOut (hdc, xCaret * cxChar, yCaret * cyChar,
							& BUFFER (xCaret, yCaret),
							cxBuffer - xCaret) ;

					DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
					ReleaseDC (hwnd, hdc) ;
					ShowCaret (hwnd) ;
					break ;
			}

			SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
			return 0 ;

		case WM_CHAR:
			for (i = 0 ; i < (int) LOWORD (lParam) ; i++) {
			
				switch (wParam) {
				
					case '\b':                    // backspace
						if (xCaret > 0) {
						
							xCaret-- ;
							SendMessage (hwnd, WM_KEYDOWN, VK_DELETE, 1) ;
						}
						break ;

					case '\t':                    // tab
						do {
                         SendMessage (hwnd, WM_CHAR, ' ', 1) ;
						} while (xCaret % 8 != 0) ;
						break ;

					case '\n':                    // line feed
						if (++yCaret == cyBuffer)
							yCaret = 0 ;

						break ;

					case '\r': {                  // carriage return
						//showMessage(order);
						if(order == "exit"){
							for (y = 0 ; y < cyBuffer ; y++)
								for (x = 0 ; x < cxBuffer ; x++)
									BUFFER (x, y) = ' ' ;

							xCaret = 0 ;
							yCaret = 0 ;

							PostMessage(hwnd, WM_CLOSE, 0, 0);
						} else {
							xCaret = 0 ;

							if (++yCaret == cyBuffer)
								yCaret = 0 ;
						}

						orderVector.push_back(order);
						order = "";
						curOrderNum++;

						break ;
					}

					case '\x1B':                  // escape
						for (y = 0 ; y < cyBuffer ; y++)
							for (x = 0 ; x < cxBuffer ; x++)
								BUFFER (x, y) = ' ' ;

						xCaret = 0 ;
						yCaret = 0 ;

						order = "";

						InvalidateRect (hwnd, NULL, FALSE) ;
						break ;

					default:                      // character codes

						if(BUFFER (xCaret, yCaret) == ' '){

							BUFFER (xCaret, yCaret) = (TCHAR) wParam ;
							order += (char) wParam;

						} else {

							for(x = cxBuffer-1;x > xCaret;x--){
								BUFFER (x, yCaret) = BUFFER (x-1, yCaret);
							}

							BUFFER (xCaret, yCaret) = (TCHAR) wParam ;

							order.insert(xCaret,1,(char)wParam);

						}

						HideCaret (hwnd) ;
						hdc = GetDC (hwnd) ;

						SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
											dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;

						TextOut (hdc, xCaret * cxChar, yCaret * cyChar,
								& BUFFER (xCaret, yCaret),
								cxBuffer - xCaret) ;

						DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
						ReleaseDC (hwnd, hdc) ;
						ShowCaret (hwnd) ;

						if (++xCaret == cxBuffer) {
						
							xCaret = 0 ;

							if (++yCaret == cyBuffer)
								yCaret = 0 ;
						}
						break ;
				}
			}

			SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
			return 0 ;

		case WM_PAINT:
			hdc = BeginPaint (hwnd, &ps) ;

			SelectObject (hdc, CreateFont (0, 0, 0, 0, 0, 0, 0, 0,
									dwCharSet, 0, 0, 0, FIXED_PITCH, NULL)) ;

			for (y = 0 ; y < cyBuffer ; y++)
				TextOut (hdc, 0, y * cyChar, & BUFFER(0,y), cxBuffer) ;

			DeleteObject (SelectObject (hdc, GetStockObject (SYSTEM_FONT))) ;
			EndPaint (hwnd, &ps) ;
			return 0 ;

		case WM_MDIACTIVATE: {
		
			HMENU hMenu, hFileMenu;
			UINT EnableFlag;

			hMenu = GetMenu(hWndMainFrame2);
			if(hwnd == (HWND)lParam) {
				EnableFlag = MF_ENABLED;
			} else {
				EnableFlag = MF_GRAYED;
			}

			EnableMenuItem(hMenu, 1, MF_BYPOSITION | EnableFlag);
			EnableMenuItem(hMenu, 2, MF_BYPOSITION | EnableFlag);

			hFileMenu = GetSubMenu(hMenu, 0);

			EnableMenuItem(hFileMenu, IDM_FILE_CLOSE, MF_BYCOMMAND | EnableFlag);
			EnableMenuItem(hFileMenu, IDM_WINDOW_CLOSE_ALL, MF_BYCOMMAND | EnableFlag);

			DrawMenuBar(hWndMainFrame2);
		}
		break;
		
		case WM_CLOSE: {
			ShowWindow(hwnd, SW_HIDE);
			return 0;
		}

		default:
			return DefMDIChildProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

/*  Construct new child window  */
BOOL CreateNewCommandCosoleDocument(HINSTANCE hInstance,char* ChildClassName) {

	WNDCLASSEX WndClsEx;

	WndClsEx.cbSize		 = sizeof(WNDCLASSEX);
	WndClsEx.style		 = CS_HREDRAW | CS_VREDRAW;
	WndClsEx.lpfnWndProc = CommandConsoleWindowProcedure;
	WndClsEx.cbClsExtra	 = 0;
	WndClsEx.cbWndExtra	 = 0;
	WndClsEx.hInstance	 = hInstance;
	WndClsEx.hIcon		 = LoadIcon(NULL, IDI_WARNING);
	WndClsEx.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	WndClsEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	WndClsEx.lpszMenuName  = NULL;
	WndClsEx.lpszClassName = ChildClassName;
	WndClsEx.hIconSm		 = LoadIcon(NULL, IDI_WARNING);

	if(!RegisterClassEx(&WndClsEx)) {
		MessageBox(NULL,
			       "There was a problem when attempting to create a document",
			       "Application Error",
				   MB_OK);
		return FALSE;
	} else
		return TRUE;
}