#include <windows.h>
#include <string>
using namespace std;

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