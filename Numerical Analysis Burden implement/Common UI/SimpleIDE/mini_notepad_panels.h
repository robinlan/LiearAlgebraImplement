#include <windows.h>
#include <commdlg.h>
#include "res/notepad/resource.h"
#include "res/notepad/PopFile.h"
#include "res/notepad/PopFont.h"
#include "res/notepad/PopPrnt.h"
#include "res/notepad/PopFind.h"

#define EDITID   1
#define UNTITLED TEXT ("(untitled)")

static TCHAR* szAppName = _T("MDIChildNotepadWnd");
static HWND hDlgModeless ;
static HWND hWndMainFrame3  = NULL;

BOOL CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM);

/*  Construct the MDI object  */
HWND CreateNewNotepadMDIChildren(HWND hMDIClient, char* ChildClassName, HWND hWndMainFrameHwnd) {

	MDICREATESTRUCT mcs;
	HWND NewWnd;
	
	hWndMainFrame3 = hWndMainFrameHwnd;

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

void DoCaption (HWND hwnd, TCHAR * szTitleName) {
	
    TCHAR szCaption[64 + MAX_PATH] ;

    wsprintf (szCaption, TEXT ("%s - %s"), szAppName,
              szTitleName[0] ? szTitleName : UNTITLED) ;

    SetWindowText (hwnd, szCaption) ;
}

void OkMessage (HWND hwnd, TCHAR * szMessage, TCHAR * szTitleName) {
	
    TCHAR szBuffer[64 + MAX_PATH] ;

    wsprintf (szBuffer, szMessage, szTitleName[0] ? szTitleName : UNTITLED) ;

    MessageBox (hwnd, szBuffer, szAppName, MB_OK | MB_ICONEXCLAMATION) ;
}

short AskAboutSave (HWND hwnd, TCHAR * szTitleName) {
	
    TCHAR szBuffer[64 + MAX_PATH] ;
    int   iReturn ;

    wsprintf (szBuffer, TEXT ("Save current changes in %s?"),
              szTitleName[0] ? szTitleName : UNTITLED) ;

    iReturn = MessageBox (hwnd, szBuffer, szAppName,
                          MB_YESNOCANCEL | MB_ICONQUESTION) ;

    if (iReturn == IDYES)
        if (!SendMessage (hwnd, WM_COMMAND, IDM_FILE_SAVE, 0))
            iReturn = IDCANCEL ;

    return iReturn ;
}

LRESULT CALLBACK NotepadWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	static BOOL      bNeedSave = FALSE ;
    static HINSTANCE hInst ;
    static HWND      hwndEdit ;
    static int       iOffset ;
    static TCHAR     szFileName[MAX_PATH], szTitleName[MAX_PATH] ;
    static UINT      messageFindReplace ;
    int              iSelBeg, iSelEnd, iEnable ;
    LPFINDREPLACE    pfr ;
	
	switch(msg) {
		
		case WM_CREATE:{
			hInst = ((LPCREATESTRUCT) lParam) -> hInstance ;

               // Create the edit control child window

			hwndEdit = CreateWindow (TEXT ("edit"), NULL,
								WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
								WS_BORDER | ES_LEFT | ES_MULTILINE |
								ES_NOHIDESEL | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
								0, 0, 0, 0,
								hwnd, (HMENU) EDITID, hInst, NULL) ;

			SendMessage (hwndEdit, EM_LIMITTEXT, 32000, 0L) ;

               // Initialize common dialog box stuff

			PopFileInitialize (hwnd) ;
			PopFontInitialize (hwndEdit) ;

			messageFindReplace = RegisterWindowMessage (FINDMSGSTRING) ;

			DoCaption (hwnd, szTitleName) ;
		}
		break;
		
		case WM_SETFOCUS:
			SetFocus (hwndEdit) ;
			return 0 ;

		case WM_SIZE:
			MoveWindow (hwndEdit, 0, 0, LOWORD (lParam), HIWORD (lParam), TRUE) ;
			return 0 ;

		case WM_INITMENUPOPUP:
			switch (lParam) {
				
				case 1:             // Edit menu

                    // Enable Undo if edit control can do it

					EnableMenuItem ((HMENU) wParam, IDM_EDIT_UNDO,
						SendMessage (hwndEdit, EM_CANUNDO, 0, 0L) ?
												MF_ENABLED : MF_GRAYED) ;

                    // Enable Paste if text is in the clipboard

					EnableMenuItem ((HMENU) wParam, IDM_EDIT_PASTE,
						IsClipboardFormatAvailable (CF_TEXT) ?
												MF_ENABLED : MF_GRAYED) ;

                    // Enable Cut, Copy, and Del if text is selected

					SendMessage (hwndEdit, EM_GETSEL, (WPARAM) &iSelBeg,
													(LPARAM) &iSelEnd) ;

					iEnable = iSelBeg != iSelEnd ? MF_ENABLED : MF_GRAYED ;

					EnableMenuItem ((HMENU) wParam, IDM_EDIT_CUT,   iEnable) ;
					EnableMenuItem ((HMENU) wParam, IDM_EDIT_COPY,  iEnable) ;
					EnableMenuItem ((HMENU) wParam, IDM_EDIT_CLEAR, iEnable) ;
					break ;

				case 2:             // Search menu

				// Enable Find, Next, and Replace if modeless
				//   dialogs are not already active

					iEnable = hDlgModeless == NULL ?
									MF_ENABLED : MF_GRAYED ;

					EnableMenuItem ((HMENU) wParam, IDM_SEARCH_FIND,    iEnable) ;
					EnableMenuItem ((HMENU) wParam, IDM_SEARCH_NEXT,    iEnable) ;
					EnableMenuItem ((HMENU) wParam, IDM_SEARCH_REPLACE, iEnable) ;
					break ;
			}
			return 0 ;

		case WM_COMMAND:
               // Messages from edit control

			if (lParam && LOWORD (wParam) == EDITID) {
				
				switch (HIWORD (wParam)) {
					
					case EN_UPDATE :
						bNeedSave = TRUE ;
						return 0 ;

					case EN_ERRSPACE :
					case EN_MAXTEXT :
						MessageBox (hwnd, TEXT ("Edit control out of space."),
									szAppName, MB_OK | MB_ICONSTOP) ;
						return 0 ;
				}
				break ;
			}

			switch (LOWORD (wParam)) {
               // Messages from File menu

				case IDM_FILE_NEW:
					if (bNeedSave && IDCANCEL == AskAboutSave (hwnd, szTitleName))
						return 0 ;

					SetWindowText (hwndEdit, TEXT ("\0")) ;
					szFileName[0]  = '\0' ;
					szTitleName[0] = '\0' ;
					DoCaption (hwnd, szTitleName) ;
					bNeedSave = FALSE ;
					return 0 ;

				case IDM_FILE_OPEN:
					if (bNeedSave && IDCANCEL == AskAboutSave (hwnd, szTitleName))
						return 0 ;
	
					if (PopFileOpenDlg (hwnd, szFileName, szTitleName)) {
						
						if (!PopFileRead (hwndEdit, szFileName)) {
							OkMessage (hwnd, TEXT ("Could not read file %s!"),
										szTitleName) ;
							szFileName[0]  = '\0' ;
							szTitleName[0] = '\0' ;
						}
					}

					DoCaption (hwnd, szTitleName) ;
					bNeedSave = FALSE ;
					return 0 ;

				case IDM_FILE_SAVE:
					if (szFileName[0]) {
						
						if (PopFileWrite (hwndEdit, szFileName))  {
                         bNeedSave = FALSE ;
                         return 1 ;
						} else {
                         OkMessage (hwnd, TEXT ("Could not write file %s"),
                                    szTitleName) ;
                         return 0 ;
						}
					}
                                   // fall through
				case IDM_FILE_SAVE_AS:
					if (PopFileSaveDlg (hwnd, szFileName, szTitleName)) {
						DoCaption (hwnd, szTitleName) ;

						if (PopFileWrite (hwndEdit, szFileName)) {
                         bNeedSave = FALSE ;
                         return 1 ;
						} else {
                         OkMessage (hwnd, TEXT ("Could not write file %s"),
                                    szTitleName) ;
                         return 0 ;
						}
					}
					return 0 ;

				case IDM_FILE_PRINT:
					if (!PopPrntPrintFile (hInst, hwnd, hwndEdit, szTitleName))
						OkMessage (hwnd, TEXT ("Could not print file %s"),
										szTitleName) ;
					return 0 ;

				case IDM_APP_EXIT:
					SendMessage (hwnd, WM_CLOSE, 0, 0) ;
					return 0 ;

                    // Messages from Edit menu

				case IDM_EDIT_UNDO:
					SendMessage (hwndEdit, WM_UNDO, 0, 0) ;
					return 0 ;

				case IDM_EDIT_CUT:
					SendMessage (hwndEdit, WM_CUT, 0, 0) ;
					return 0 ;

				case IDM_EDIT_COPY:
					SendMessage (hwndEdit, WM_COPY, 0, 0) ;
					return 0 ;

				case IDM_EDIT_PASTE:
					SendMessage (hwndEdit, WM_PASTE, 0, 0) ;
					return 0 ;

				case IDM_EDIT_CLEAR:
					SendMessage (hwndEdit, WM_CLEAR, 0, 0) ;
					return 0 ;

				case IDM_EDIT_SELECT_ALL:
					SendMessage (hwndEdit, EM_SETSEL, 0, -1) ;
					return 0 ;

                    // Messages from Search menu

				case IDM_SEARCH_FIND:
					SendMessage (hwndEdit, EM_GETSEL, 0, (LPARAM) &iOffset) ;
					hDlgModeless = PopFindFindDlg (hwnd) ;
					return 0 ;

				case IDM_SEARCH_NEXT:
					SendMessage (hwndEdit, EM_GETSEL, 0, (LPARAM) &iOffset) ;

					if (PopFindValidFind ())
						PopFindNextText (hwndEdit, &iOffset) ;
					else
						hDlgModeless = PopFindFindDlg (hwnd) ;

					return 0 ;

				case IDM_SEARCH_REPLACE:
					SendMessage (hwndEdit, EM_GETSEL, 0, (LPARAM) &iOffset) ;
					hDlgModeless = PopFindReplaceDlg (hwnd) ;
					return 0 ;

				case IDM_FORMAT_FONT:
					if (PopFontChooseFont (hwnd))
						PopFontSetFont (hwndEdit) ;

					return 0 ;

                    // Messages from Help menu

				case IDM_HELP:
					OkMessage (hwnd, TEXT ("Help not yet implemented!"),
										TEXT ("\0")) ;
					return 0 ;

				case IDM_APP_ABOUT:
					DialogBox (hInst, TEXT ("AboutBox"), hwnd, AboutDlgProc) ;
					return 0 ;
			}
			break ;

		case WM_CLOSE:
			if (!bNeedSave || IDCANCEL != AskAboutSave (hwnd, szTitleName))
				ShowWindow(hwnd,SW_HIDE);

			return 0 ;

		case WM_QUERYENDSESSION :
			if (!bNeedSave || IDCANCEL != AskAboutSave (hwnd, szTitleName))
				return 1 ;

			return 0 ;

		case WM_MDIACTIVATE: {
			
			HMENU hMenu, hFileMenu;
			UINT EnableFlag;

			hMenu = GetMenu(hWndMainFrame3);
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

			DrawMenuBar(hWndMainFrame3);
		}
		break;

		default:
			if (msg == messageFindReplace) {
				
				pfr = (LPFINDREPLACE) lParam ;

				if (pfr->Flags & FR_DIALOGTERM)
					hDlgModeless = NULL ;

				if (pfr->Flags & FR_FINDNEXT)
                    if (!PopFindFindText (hwndEdit, &iOffset, pfr))
							OkMessage (hwnd, TEXT ("Text not found!"),
											TEXT ("\0")) ;

				if (pfr->Flags & FR_REPLACE || pfr->Flags & FR_REPLACEALL)
                    if (!PopFindReplaceText (hwndEdit, &iOffset, pfr))
							OkMessage (hwnd, TEXT ("Text not found!"),
											TEXT ("\0")) ;

				if (pfr->Flags & FR_REPLACEALL)
                    while (PopFindReplaceText (hwndEdit, &iOffset, pfr)) ;

				return 0 ;
			}
			return DefMDIChildProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
     switch (message)
     {
     case WM_INITDIALOG:
          return TRUE ;

     case WM_COMMAND:
          switch (LOWORD (wParam))
          {
          case IDOK:
               EndDialog (hDlg, 0) ;
               return TRUE ;
          }
          break ;
     }
     return FALSE ;
}

/*  Construct new child window  */
BOOL CreateNewNotepadDocument(HINSTANCE hInstance,char* ChildClassName) {

	WNDCLASSEX WndClsEx;

	WndClsEx.cbSize		 = sizeof(WNDCLASSEX);
	WndClsEx.style		 = CS_HREDRAW | CS_VREDRAW;
	WndClsEx.lpfnWndProc = NotepadWindowProcedure;
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