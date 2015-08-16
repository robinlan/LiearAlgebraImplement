/***********************************************************************************

Example usage:
	Include:
		res/win_widget.h
	
	HINSTANCE hFatherInstance = (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE) ;
	HWND settingHwnd;
	TCHAR settingSzClassName[ ] = _T("Some_text_here");
	WinWindows wincSettingObject(settingSzClassName,hFatherInstance,SW_SHOWDEFAULT);
	WNDCLASSEX wincSetting = wincSettingObject.getWinClass(EditableWindowProcedure);
	if( !wincSettingObject.getWinRegisterClass())
		return 0;
	settingHwnd = wincSettingObject.getWinHWND(250,100,_T("Setting"));
	ShowWindow (settingHwnd, SW_SHOWDEFAULT);
	
	In previous trigger place:
		char cName[256];
		GetClassName(hwnd, cName, 256);
		SendMessage (addressHWND, WM_NOTIFY, 0, (LPARAM)cName);
		
	Global setting:
		static HWND prevHwnd;

************************************************************************************/
#include <windows.h>
#include <string>

#define IDC_SETTING_BUTTON_3  101
#define IDC_SETTING_EDITBOX_3 102

LRESULT CALLBACK EditableWindowProcedure3(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	static HWND prevHwnd;
	static HWND hSettingEdit3;
	static string resultString3;

	switch(Message) {

		case WM_CREATE: {
			// Create an edit box
			WinButton button1("OK",10,10,80,24,hwnd,(HMENU)IDC_SETTING_BUTTON_3);
			HWND hbutton = button1.getButton();
			WinEditbox editbox1("",110,10,80,24,hwnd,(HMENU)IDC_SETTING_EDITBOX_3);
			hSettingEdit3 = editbox1.getEditbox();
			break;
		}

		case WM_COMMAND: {

			switch(LOWORD(wParam)) {

				case IDC_SETTING_BUTTON_3:
				{
					char buffer[256];
					SendMessage(hSettingEdit3,
						WM_GETTEXT,
						sizeof(buffer)/sizeof(buffer[0]),
						reinterpret_cast<LPARAM>(buffer));
					string tmp(buffer);
					resultString3 = tmp;
					SendMessage(prevHwnd, WM_NOTIFY, 0, (LPARAM)tmp.c_str());
				}
				break;

			}

			break;
		}
		
		case WM_NOTIFY: {
			string tmp((char*)lParam);
            TCHAR* targetWinName = _T((char*)lParam);
            prevHwnd = FindWindow(targetWinName,0);
			break;
		}

		/* Upon destruction, tell the main thread to stop */
		case WM_CLOSE: {
			ShowWindow(hwnd, SW_HIDE);
			break;
		}

		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}
