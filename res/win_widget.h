#include <windows.h>
#include <string>
using namespace std;

/***************************************************************************

Example usage:
	Set global values: 
		HWND aHwnd;
		int nGlobCmdShow;
		LRESULT CALLBACK aWindowProcedure (HWND, UINT, WPARAM, LPARAM);
		TCHAR aSzClassName[ ] = _T("Some_text_here");
	
	In WinMain function:
		WinWindows winc3Object(szClassName3,hThisInstance,nCmdShow);
		WNDCLASSEX winc3 = winc3Object.getWinClass(WindowProcedure3);
		nGlobCmdShow = nCmdShow;
		if( !winc3Object.getWinRegisterClass())
			return 0;
		thirdHwnd = winc3Object.getWinHWND(444,275,_T("Window title here"));
	
	In some WinProc function that trigger the window:
		ShowWindow (thirdHwnd, nGlobCmdShow);

***************************************************************************/
class WinWindows{
	private:
		WNDCLASSEX winc;		 //Windows class object
		TCHAR* szClassName;  	 //Class identity
		HWND hwnd; 				 //News handler
		HINSTANCE hThisInstance; //APP object
		int nCmdShow;			 //Windows showing type
	public:
		WinWindows(){}
		/*Construct object with szClassName, HINSTANCE and show type*/
		WinWindows(TCHAR* _szClassName, HINSTANCE _hThisInstance,int _nCmdShow){
			szClassName = _szClassName;
			hThisInstance = _hThisInstance;
			nCmdShow = _nCmdShow;
		}
		/*Access and construct the WNDCLASSEX object with WINPROC object*/
		WNDCLASSEX getWinClass(WNDPROC _winProc){
			
			/* The Window structure */
			winc.hInstance = hThisInstance;
			winc.lpszClassName = szClassName;
			winc.lpfnWndProc = _winProc;      		  /* This function is called by windows */
			winc.style = CS_DBLCLKS;                 /* Catch double-clicks */
			winc.cbSize = sizeof (WNDCLASSEX);

			/* Use default icon and mouse-pointer */
			winc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
			winc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
			winc.hCursor = LoadCursor (NULL, IDC_ARROW);
			winc.lpszMenuName = NULL;                 /* No menu */
			winc.cbClsExtra = 0;                      /* No extra bytes after the window class */
			winc.cbWndExtra = 0;                      /* structure or the window instance */
			
			/* Use Windows's default colour as the background of the window */
			winc.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
			
			return winc;
		}
		/*Access RegisterClassEx ATOM return value*/
		ATOM getWinRegisterClass(){
			
			ATOM isRegister = RegisterClassEx(&winc);
			return isRegister;
			
		}
		/*Access the HWND object*/
		HWND getWinHWND(int _length,int _width,LPCTSTR _lpWindowName){
			
			hwnd = CreateWindowEx (
				0,                   									/* Extended possibilites for variation */
				szClassName,         									/* Classname */
				_lpWindowName,        									/* Title Text */
				WS_OVERLAPPEDWINDOW, 									/* default window */
				CW_USEDEFAULT,       									/* Windows decides the position */
				CW_USEDEFAULT,    									    /* where the window ends up on the screen */
				_length,           									    /* The programs width */
				_width,            									    /* and height in pixels */
				HWND_DESKTOP,    									    /* The window is a child-window to desktop */
				NULL,            									    /* No menu */
				hThisInstance,										    /* Program Instance handler */
				NULL                 									/* No Window Creation data */
				);
			
			return hwnd;
		}
};

/*********************************************************************

Example usage in WM_CREATE:
	WinButton button1("OK",50,220,100,24,hwnd,(HMENU)IDC_MAIN_BUTTON);
	HWND hbutton = button1.getButton();
	
*********************************************************************/
class WinButton{
	private:
		HWND winButton;                 //The real button object
	public:
		WinButton(){}
		
		/*Create a button with button text, x, y, width, height, father window object*/
		WinButton(LPCTSTR _text,        //Text content on the button
				  int _x,
				  int _y,
				  int _width,
				  int _height,
				  HWND _parentWindow,   //Father window
				  HMENU _childMark) {   //Mark number of the button
			
			winButton = CreateWindowEx(NULL,
				"BUTTON",
				_text,
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				_x,
				_y,
				_width,
				_height,
				_parentWindow,
				_childMark,
				GetModuleHandle(NULL),
				NULL);
			
			/*Set default font*/
			HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(winButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
			
		}
		
		/*Set the button text font*/
		void setFont(int _fontType){
			
			HGDIOBJ hfDefault=GetStockObject(_fontType);
			SendMessage(winButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
			
		}
		
		/*Access the button object*/
		HWND getButton(){
			return winButton;
		}
};

/***********************************************************************

Example usage in WM_CREATE:
	WinEditbox editbox1("",50,100,200,100,hwnd,(HMENU)IDC_MAIN_EDITBOX);
	editbox1.setEditboxText((LPARAM)"Input sth here....");
	HWND hEdit = editbox1.getEditbox();
or to get the content:
	WinEditbox editbox1(hEdit1);
	string coeffStr = editbox1.getEditboxContent();
	
***********************************************************************/
class WinEditbox{
	private:
		HWND winEditbox; //The real edit box object
	public:
		WinEditbox(){}
		/*Construct object with living HWND object*/
		WinEditbox(HWND _winEditbox){
			winEditbox = _winEditbox;
		}
		/*Create an edit box with text, x, y, width, height, father window object*/
		WinEditbox(LPCTSTR _text,        //Text content on the button
				   int _x,
				   int _y,
				   int _width,
				   int _height,
				   HWND _parentWindow,   //Father window
				   HMENU _childMark) {   //Mark number of the edit box
			
			winEditbox = CreateWindowEx(WS_EX_CLIENTEDGE,
				"EDIT",
				_text,
				WS_CHILD|WS_VISIBLE|
				ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
				_x,
				_y,
				_width,
				_height,
				_parentWindow,
				_childMark,
				GetModuleHandle(NULL),
				NULL);
			
			/*Set default font*/
			HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(winEditbox,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
			
		}
		
		/*Set the button text font*/
		void setFont(int _fontType){
			
			HGDIOBJ hfDefault=GetStockObject(_fontType);
			SendMessage(winEditbox,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));
			
		}
		
		/*Set the text inside edit box*/
		void setEditboxText(LPARAM _text){
			SendMessage(winEditbox,
				WM_SETTEXT,
				NULL,
				_text);
		}
		
		/*Access edit box content*/
		string getEditboxContent(){
			
			char tmpBuffer[256];
			SendMessage(winEditbox,
				WM_GETTEXT,
				sizeof(tmpBuffer)/sizeof(tmpBuffer[0]),
				reinterpret_cast<LPARAM>(tmpBuffer));
				
			string tmpStr(tmpBuffer);
			return tmpStr;
			
		}
		
		/*Access the edit box object*/
		HWND getEditbox(){
			return winEditbox;
		}
};

/*Message box*/
void showMessage(string _message){
	MessageBox(NULL,
		_message.c_str(),
		"Information",
		MB_ICONINFORMATION);
}