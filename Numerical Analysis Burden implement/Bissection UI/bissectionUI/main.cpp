#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <string>
#include <vector>
#include "math_function.h"
#include "type_convert.h"
#include "win_widget.h"

#define IDC_CALC_BUTTON					101		// Calculate button identifier
#define IDC_MAIN_EDITBOX				102		// Edit box identifier
#define IDC_POWER_BUTTON				103		// Power function setting button
#define IDC_POWERCOEFF_EDITBOX			104		// Power coefficient edit box
#define IDC_POWERPOWER_EDITBOX			105		// Power power edit box
#define IDC_CLEAR_BUTTON				106		// Clear button identifier
#define IDC_SERVEASPOWER_BUTTON			107		// ServeAsPower function setting button
#define IDC_SERVEASPOWERCOEFF_EDITBOX	108		// ServeAsPower coefficient edit box
#define IDC_SERVEASPOWERBOTNUM_EDITBOX	109		// ServeAsPower bottom number edit box
#define IDC_SQUAREROOT_BUTTON			110		// Square root function setting button
#define IDC_SQUAREROOT_EDITBOX			111		// Square root coefficient edit box
#define IDC_SIN_BUTTON					112		// Sin function setting button
#define IDC_SIN_EDITBOX					113		// Sin coefficient edit box
#define IDC_COS_BUTTON					114		// Cos function setting button
#define IDC_COS_EDITBOX					115		// Cos coefficient edit box
#define IDC_EXP_BUTTON					116		// Exp function setting button
#define IDC_EXP_EDITBOX					117		// Exp coefficient edit box

#define MAXNUM_PER_FUNC			50		// Store the maximum number of function per type

using namespace std;

/***********************
** Global Declaration **
************************/
HWND hEdit1; 										 //Store power coefficient
HWND hEdit2; 										 //Store power power
HWND hEdit3; 										 //Store serveaspower coefficient
HWND hEdit4; 										 //Store serveaspower bottomnum
HWND hEdit5;										 //Store square root coefficient
HWND hEdit6;										 //Store sin coefficient
HWND hEdit7;										 //Store cos coefficient
HWND hEdit8;										 //Store exp coefficient

vector<MathFunction*> mathFuncVector; 				 //Store the address of function objects

int powerFuncNum;									 //Store power function number
PowerMathFunction powerFuncArray[MAXNUM_PER_FUNC];   //Store power function objects

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow) {
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    
	HDC hdc;
	PAINTSTRUCT ps;
	
	switch (message) {                 /* handle the messages */
        case WM_CREATE: {
			
			powerFuncNum = 0;
			
			WinButton button1("POWER",50,50,100,24,hwnd,(HMENU)IDC_POWER_BUTTON);
			HWND hbutton = button1.getButton();
			
			WinEditbox editbox1("",170,50,100,24,hwnd,(HMENU)IDC_POWERCOEFF_EDITBOX);
			editbox1.setEditboxText((LPARAM)"coeff");
			hEdit1 = editbox1.getEditbox();
			
			WinEditbox editbox2("",290,50,100,24,hwnd,(HMENU)IDC_POWERPOWER_EDITBOX);
			editbox2.setEditboxText((LPARAM)"power");
			hEdit2 = editbox2.getEditbox();
			
			
			
			WinButton button4("SERVEASPOWER",50,90,100,24,hwnd,(HMENU)IDC_SERVEASPOWER_BUTTON);
			HWND hbutton4 = button1.getButton();
			
			WinEditbox editbox3("",170,90,100,24,hwnd,(HMENU)IDC_SERVEASPOWERCOEFF_EDITBOX);
			editbox3.setEditboxText((LPARAM)"coeff");
			hEdit3 = editbox3.getEditbox();
			
			WinEditbox editbox4("",290,90,100,24,hwnd,(HMENU)IDC_SERVEASPOWERBOTNUM_EDITBOX);
			editbox4.setEditboxText((LPARAM)"botnum");
			hEdit4 = editbox4.getEditbox();
			
			
			
			WinButton button5("SQUAREROOT",50,130,100,24,hwnd,(HMENU)IDC_SQUAREROOT_BUTTON);
			HWND hbutton5 = button5.getButton();
			
			WinEditbox editbox5("",170,130,100,24,hwnd,(HMENU)IDC_SQUAREROOT_EDITBOX);
			editbox5.setEditboxText((LPARAM)"coeff");
			hEdit5 = editbox5.getEditbox();
			
			
			WinButton button6("SIN",50,170,100,24,hwnd,(HMENU)IDC_SIN_BUTTON);
			HWND hbutton6 = button6.getButton();
			
			WinEditbox editbox6("",170,170,100,24,hwnd,(HMENU)IDC_SIN_EDITBOX);
			editbox6.setEditboxText((LPARAM)"coeff");
			hEdit6 = editbox6.getEditbox();
			
			
			WinButton button7("COS",50,210,100,24,hwnd,(HMENU)IDC_COS_BUTTON);
			HWND hbutton7 = button7.getButton();
			
			WinEditbox editbox7("",170,210,100,24,hwnd,(HMENU)IDC_COS_EDITBOX);
			editbox7.setEditboxText((LPARAM)"coeff");
			hEdit7 = editbox7.getEditbox();
			
			
			WinButton button8("SIN",50,250,100,24,hwnd,(HMENU)IDC_EXP_BUTTON);
			HWND hbutton8 = button8.getButton();
			
			WinEditbox editbox8("",170,250,100,24,hwnd,(HMENU)IDC_EXP_EDITBOX);
			editbox8.setEditboxText((LPARAM)"coeff");
			hEdit8 = editbox8.getEditbox();
			
			
			WinButton button2("CALC",50,290,100,24,hwnd,(HMENU)IDC_CALC_BUTTON);
			HWND hbutton2 = button2.getButton();
			
			WinButton button3("CLEAR",170,290,100,24,hwnd,(HMENU)IDC_CLEAR_BUTTON);
			HWND hbutton3 = button3.getButton();
			
			break;
		}
		
		case WM_COMMAND: {
			
			switch(LOWORD(wParam)) {
				
				case IDC_POWER_BUTTON: {
					
					WinEditbox editbox1(hEdit1);
					string coeffStr = editbox1.getEditboxContent();
					WinEditbox editbox2(hEdit2);
					string powerStr = editbox2.getEditboxContent();
					
					PowerMathFunction func(str2float(coeffStr));
					func.setPower(str2float(powerStr));
					powerFuncArray[powerFuncNum] = func;
					mathFuncVector.push_back(&(powerFuncArray[powerFuncNum]));
					powerFuncNum = powerFuncNum + 1;

					InvalidateRect(hwnd,NULL,TRUE);
				
					break;
				
				}
				
				case IDC_CALC_BUTTON: {
					
					string tmpMessage;
					
					float result = 0.0;
					for(int i=0;i<mathFuncVector.size();i++){
						result = result + mathFuncVector[i]->calculateResult(2.0);
					}
					tmpMessage = "f(2) is: "+float2str(result);
					
					showMessage(tmpMessage);
					
					break;
					
				}
				
				case IDC_CLEAR_BUTTON: {
					
					powerFuncNum = 0;
					mathFuncVector.clear();
					
					WinEditbox editbox1(hEdit1);
					editbox1.setEditboxText((LPARAM)"");	
					
					WinEditbox editbox2(hEdit2);
					editbox2.setEditboxText((LPARAM)"");
					
					InvalidateRect(hwnd,NULL,TRUE);
					
					break;
				}
				
			}
			
			break;
		}
		
		case WM_PAINT: {
			
			hdc = BeginPaint(hwnd,&ps);
			
			if(!mathFuncVector.empty()){
				string tmp = "Coefficient: "+float2str((mathFuncVector.back())->getCoefficient());
				TextOut(hdc,50,10,tmp.c_str(),tmp.length());
			} else{
				TextOut(hdc,50,10,"Please input some function here.",32) ;
			}
			
			EndPaint (hwnd,&ps) ;
			
			break;
			
		}
		
		case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
