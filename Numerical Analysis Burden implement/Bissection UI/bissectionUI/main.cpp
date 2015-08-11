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
#define IDC_X_BUTTON					118		// x button identifier
#define IDC_X_EDITBOX					119		// x edit box
#define IDC_FINDROOT_BUTTON				120		// Find root button identifier
#define IDC_FINDROOTSTARTVAL_EDITBOX	121		// Find root start value edit box
#define IDC_FINDROOTENDVAL_EDITBOX		122		// Find root END value edit box
#define IDC_FINDROOTTOLERROR_EDITBOX	123		// Find root tolerance value edit box
#define IDC_FINDROOTMAXITER_EDITBOX		124		// Find root maximum iteration edit box

#define MAXNUM_PER_FUNC			50		// Store the maximum number of function per type

using namespace std;

/***********************
** Global Declaration **
************************/
HWND hEditX; 										 //Store x value

HWND hEdit1; 										 //Store power coefficient
HWND hEdit2; 										 //Store power power
HWND hEdit3; 										 //Store serveaspower coefficient
HWND hEdit4; 										 //Store serveaspower bottomnum
HWND hEdit5;										 //Store square root coefficient
HWND hEdit6;										 //Store sin coefficient
HWND hEdit7;										 //Store cos coefficient
HWND hEdit8;										 //Store exp coefficient

HWND hEdit9; 										 //Store find root start value
HWND hEdit10; 										 //Store find root end value
HWND hEdit11; 										 //Store find root tolerance error
HWND hEdit12; 										 //Store find root maximum iteration

float x;											 //Store value of x
float startVal;										 //Store value of start value
float endVal;										 //Store value of end value
float tolError;										 //Store value of tolerance error
int maxIter;										 //Store value of maximum iteration

vector<MathFunction*> mathFuncVector; 				 //Store the address of function objects

int powerFuncNum;									 				//Store power function number
PowerMathFunction powerFuncArray[MAXNUM_PER_FUNC];   				//Store power function objects
int serveAsPowerFuncNum;									 		//Store serveAsPower function number
ServeAsPowerMathFunction serveAsPowerFuncArray[MAXNUM_PER_FUNC];    //Store serveAsPower function objects
int squareRootFuncNum;									 			//Store sqrt function number
SquareRootMathFunction squareRootFuncArray[MAXNUM_PER_FUNC];   		//Store sqrt function objects
int sinFuncNum;										 				//Store sin function number
SinMathFunction sinFuncArray[MAXNUM_PER_FUNC];   					//Store sin function objects
int cosFuncNum;										 				//Store cos function number
CosMathFunction cosFuncArray[MAXNUM_PER_FUNC];   					//Store cos function objects
int expFuncNum;										 				//Store exp function number
ExpMathFunction expFuncArray[MAXNUM_PER_FUNC];   					//Store exp function objects

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
           _T("Function Calculation Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           644,                 /* The programs width */
           525,                 /* and height in pixels */
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
			serveAsPowerFuncNum = 0;
			squareRootFuncNum = 0;
			sinFuncNum = 0;
			cosFuncNum = 0;
			expFuncNum = 0;
			
			x = 0.0;
			startVal = 0.0;
			endVal = 0.0;
			tolError = 0.0;
			maxIter = 0;
			
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
			
			
			
			WinButton button8("EXP",50,250,100,24,hwnd,(HMENU)IDC_EXP_BUTTON);
			HWND hbutton8 = button8.getButton();
			
			WinEditbox editbox8("",170,250,100,24,hwnd,(HMENU)IDC_EXP_EDITBOX);
			editbox8.setEditboxText((LPARAM)"coeff");
			hEdit8 = editbox8.getEditbox();
			
			
			
			WinButton buttonX("X",50,330,100,24,hwnd,(HMENU)IDC_X_BUTTON);
			HWND hbuttonX = buttonX.getButton();
			
			WinEditbox editboxX("",170,330,100,24,hwnd,(HMENU)IDC_X_EDITBOX);
			editboxX.setEditboxText((LPARAM)"X");
			hEditX = editboxX.getEditbox();
			
			
			
			WinButton button9("findRoot",50,410,100,24,hwnd,(HMENU)IDC_FINDROOT_BUTTON);
			HWND hbutton9 = button9.getButton();
			
			WinEditbox editbox9("",170,410,100,24,hwnd,(HMENU)IDC_FINDROOTSTARTVAL_EDITBOX);
			editbox9.setEditboxText((LPARAM)"startVal");
			hEdit9 = editbox9.getEditbox();
			
			WinEditbox editbox10("",290,410,100,24,hwnd,(HMENU)IDC_FINDROOTENDVAL_EDITBOX);
			editbox10.setEditboxText((LPARAM)"endVal");
			hEdit10 = editbox10.getEditbox();
			
			WinEditbox editbox11("",410,410,100,24,hwnd,(HMENU)IDC_FINDROOTTOLERROR_EDITBOX);
			editbox11.setEditboxText((LPARAM)"tolError");
			hEdit11 = editbox11.getEditbox();
			
			WinEditbox editbox12("",530,410,100,24,hwnd,(HMENU)IDC_FINDROOTMAXITER_EDITBOX);
			editbox12.setEditboxText((LPARAM)"maxIter");
			hEdit12 = editbox12.getEditbox();
			
			
			
			WinButton button2("CALC",50,450,100,24,hwnd,(HMENU)IDC_CALC_BUTTON);
			HWND hbutton2 = button2.getButton();
			
			WinButton button3("CLEAR",170,450,100,24,hwnd,(HMENU)IDC_CLEAR_BUTTON);
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
				
				case IDC_SERVEASPOWER_BUTTON: {
					
					WinEditbox editbox1(hEdit3);
					string coeffStr = editbox1.getEditboxContent();
					WinEditbox editbox2(hEdit4);
					string bottomNumStr = editbox2.getEditboxContent();
					
					ServeAsPowerMathFunction func(str2float(coeffStr));
					func.setBottomNum(str2float(bottomNumStr));
					serveAsPowerFuncArray[serveAsPowerFuncNum] = func;
					mathFuncVector.push_back(&(serveAsPowerFuncArray[serveAsPowerFuncNum]));
					serveAsPowerFuncNum = serveAsPowerFuncNum + 1;

					InvalidateRect(hwnd,NULL,TRUE);
				
					break;
				
				}
				
				case IDC_SQUAREROOT_BUTTON: {
					
					WinEditbox editbox1(hEdit5);
					string coeffStr = editbox1.getEditboxContent();
					
					SquareRootMathFunction func(str2float(coeffStr));
					squareRootFuncArray[squareRootFuncNum] = func;
					mathFuncVector.push_back(&(squareRootFuncArray[squareRootFuncNum]));
					squareRootFuncNum = squareRootFuncNum + 1;

					InvalidateRect(hwnd,NULL,TRUE);
				
					break;
				
				}
				
				case IDC_SIN_BUTTON: {
					
					WinEditbox editbox1(hEdit6);
					string coeffStr = editbox1.getEditboxContent();
					
					SinMathFunction func(str2float(coeffStr));
					sinFuncArray[sinFuncNum] = func;
					mathFuncVector.push_back(&(sinFuncArray[sinFuncNum]));
					sinFuncNum = sinFuncNum + 1;

					InvalidateRect(hwnd,NULL,TRUE);
				
					break;
				
				}
				
				case IDC_COS_BUTTON: {
					
					WinEditbox editbox1(hEdit7);
					string coeffStr = editbox1.getEditboxContent();
					
					CosMathFunction func(str2float(coeffStr));
					cosFuncArray[cosFuncNum] = func;
					mathFuncVector.push_back(&(cosFuncArray[cosFuncNum]));
					cosFuncNum = cosFuncNum + 1;

					InvalidateRect(hwnd,NULL,TRUE);
				
					break;
				
				}
				
				case IDC_EXP_BUTTON: {
					
					WinEditbox editbox1(hEdit8);
					string coeffStr = editbox1.getEditboxContent();
					
					ExpMathFunction func(str2float(coeffStr));
					expFuncArray[expFuncNum] = func;
					mathFuncVector.push_back(&(expFuncArray[expFuncNum]));
					expFuncNum = expFuncNum + 1;

					InvalidateRect(hwnd,NULL,TRUE);
				
					break;
				
				}
				
				case IDC_X_BUTTON: {
					
					WinEditbox editbox1(hEditX);
					string xStr = editbox1.getEditboxContent();
					
					x = str2float(xStr);

					InvalidateRect(hwnd,NULL,TRUE);
				
					break;
				
				}
				
				case IDC_FINDROOT_BUTTON: {
					
					WinEditbox editbox1(hEdit9);
					string startValStr = editbox1.getEditboxContent();
					WinEditbox editbox2(hEdit10);
					string endValStr = editbox2.getEditboxContent();
					WinEditbox editbox3(hEdit11);
					string tolErrorStr = editbox3.getEditboxContent();
					WinEditbox editbox4(hEdit12);
					string maxIterStr = editbox4.getEditboxContent();
					
					startVal = str2float(startValStr);
					endVal = str2float(endValStr);
					tolError = str2float(tolErrorStr);
					maxIter = str2int(maxIterStr);

					InvalidateRect(hwnd,NULL,TRUE);
				
					break;
				
				}
				
				case IDC_CALC_BUTTON: {
					
					string tmpMessage;
					
					float result = 0.0;
					for(int i=0;i<mathFuncVector.size();i++){
						result = result + mathFuncVector[i]->calculateResult(x);
					}
					tmpMessage = "f("+float2str(x)+") is: "+float2str(result);
					
					showMessage(tmpMessage);
					
					break;
					
				}
				
				case IDC_CLEAR_BUTTON: {
					
					powerFuncNum = 0;
					serveAsPowerFuncNum = 0;
					squareRootFuncNum = 0;
					sinFuncNum = 0;
					cosFuncNum = 0;
					expFuncNum = 0;
					mathFuncVector.clear();
					
					x=0.0;
					startVal = 0.0;
					endVal = 0.0;
					tolError = 0.0;
					maxIter = 0;
					
					WinEditbox editbox1(hEdit1);
					editbox1.setEditboxText((LPARAM)"coeff");	
					WinEditbox editbox2(hEdit2);
					editbox2.setEditboxText((LPARAM)"power");
					WinEditbox editbox3(hEdit3);
					editbox3.setEditboxText((LPARAM)"coeff");
					WinEditbox editbox4(hEdit4);
					editbox4.setEditboxText((LPARAM)"butnum");
					WinEditbox editbox5(hEdit5);
					editbox5.setEditboxText((LPARAM)"coeff");
					WinEditbox editbox6(hEdit6);
					editbox6.setEditboxText((LPARAM)"coeff");
					WinEditbox editbox7(hEdit7);
					editbox7.setEditboxText((LPARAM)"coeff");
					WinEditbox editbox8(hEdit8);
					editbox8.setEditboxText((LPARAM)"coeff");
					WinEditbox editboxX(hEditX);
					editboxX.setEditboxText((LPARAM)"X");	
					WinEditbox editbox9(hEdit9);
					editbox9.setEditboxText((LPARAM)"startVal");
					WinEditbox editbox10(hEdit10);
					editbox10.setEditboxText((LPARAM)"endVal");
					WinEditbox editbox11(hEdit11);
					editbox11.setEditboxText((LPARAM)"tolError");
					WinEditbox editbox12(hEdit12);
					editbox12.setEditboxText((LPARAM)"maxIter");
					
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
				TextOut(hdc,50,20,tmp.c_str(),tmp.length());
			} else{
				TextOut(hdc,50,20,"Please input some function here.",32) ;
			}
			
			if(x != 0.0){
				string tmp = "X: "+float2str((x));
				TextOut(hdc,50,300,tmp.c_str(),tmp.length());
			} else{
				TextOut(hdc,50,300,"Please input value of x here.",29) ;
			}
			
			if(tolError != 0.0){
				string tmp = "Input: "+float2str((startVal))+", "+float2str((endVal))+", "+float2str((tolError))+", "+int2str((maxIter));
				TextOut(hdc,50,380,tmp.c_str(),tmp.length());
			} else{
				TextOut(hdc,50,380,"Please input value to find root here.",37) ;
			}
			
			EndPaint (hwnd,&ps) ;
			
			break;
			
		}
		
		case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                       /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
