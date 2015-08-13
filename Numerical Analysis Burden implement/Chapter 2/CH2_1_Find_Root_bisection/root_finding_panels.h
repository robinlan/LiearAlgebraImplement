/********************************************************************************
root_finding_panels.h, by Robin Lan - 2015/08/12
				
Example usage:
	Global setting:
		HWND rootfindingHwnd;
		TCHAR szClassNameRF[ ] = _T("RootFindingPanel");
		int nGlobCmdShow;
	
	In WinMain:
		WinWindows wincRFObject(szClassNameRF,hThisInstance,nCmdShow);
		WNDCLASSEX wincRF = wincRFObject.getWinClass(RootFindingWindowProcedure);
		nGlobCmdShow = nCmdShow;
		if( !wincRFObject.getWinRegisterClass())
			return 0;
		rootfindingHwnd = wincRFObject.getWinHWND(694,575,_T("Root finding Panel"));
		
	In trigger place:
		ShowWindow (rootfindingHwnd, nGlobCmdShow);
				
********************************************************************************/

#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <string>
#include "res/numeric_algorithms.h"

#define IDC_SETTING_BUTTON				101		// SETTING button identifier
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
#define IDC_MULTIPLY_BUTTON				125		// Multiply button identifier
#define IDC_MULTIPLY_EDITBOX			126		// Multiply edit box identifier
#define IDC_NESTEDFUNC_BUTTON			127		// Nested function button identifier
#define IDC_NESTEDFUNC_EDITBOX			128		// Nested function edit box identifier
#define IDC_BACK_BUTTON					129		// Back button identifier

#define MAXNUM_PER_FUNC			50		// Store the maximum number of function per type

using namespace std;

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK RootFindingWindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	/***********************
	** Global Declaration **
	************************/
	static HWND hEditX; 										 //Store x value

	static HWND hEdit1; 										 //Store power coefficient
	static HWND hEdit2; 										 //Store power power
	static HWND hEdit3; 										 //Store serveaspower coefficient
	static HWND hEdit4; 										 //Store serveaspower bottom number
	static HWND hEdit5;											 //Store square root coefficient
	static HWND hEdit6;											 //Store sin coefficient
	static HWND hEdit7;											 //Store cos coefficient
	static HWND hEdit8;											 //Store exp coefficient

	static HWND hEdit13; 										 //Store multiply coefficient
	static HWND hEdit14; 										 //Store nested coefficient

	static HWND hEdit9; 										 //Store find root start value
	static HWND hEdit10; 										 //Store find root end value
	static HWND hEdit11; 										 //Store find root tolerance error
	static HWND hEdit12; 										 //Store find root maximum iteration

	static vector<MathFunction*> mathFuncVector; 				 //Store the address of function objects
	static string funcStr;										 //To display the current function

	static string resultRootStr;								 //To display the root finding result

	static int powerFuncNum;									 						//Store power function number
	static PowerMathFunction powerFuncArray[MAXNUM_PER_FUNC];   						//Store power function objects
	static int serveAsPowerFuncNum;									 					//Store serveAsPower function number
	static ServeAsPowerMathFunction serveAsPowerFuncArray[MAXNUM_PER_FUNC];			    //Store serveAsPower function objects
	static int squareRootFuncNum;									 					//Store sqrt function number
	static SquareRootMathFunction squareRootFuncArray[MAXNUM_PER_FUNC];   				//Store sqrt function objects
	static int sinFuncNum;										 						//Store sin function number
	static SinMathFunction sinFuncArray[MAXNUM_PER_FUNC];   							//Store sin function objects
	static int cosFuncNum;										 						//Store cos function number
	static CosMathFunction cosFuncArray[MAXNUM_PER_FUNC];   							//Store cos function objects
	static int expFuncNum;										 						//Store exp function number
	static ExpMathFunction expFuncArray[MAXNUM_PER_FUNC];   							//Store exp function objects
	static bool isMultiply;																//Store if is in multiplying process
	static int multiplyFuncNum;									 						//Store multiply function number
	static MultiplyTwoMathFunction multiplyFuncArray[MAXNUM_PER_FUNC];					//Store multiply function objects
	static bool isNested;																//Store if is in nested process
	static int nestedFuncNum;										 					//Store nested function number
	static CalculateAnotherFunctionFirstMathFunction nestedFuncArray[MAXNUM_PER_FUNC];  //Store nested function objects
	
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message) {                 /* handle the messages */
        case WM_CREATE: {

			funcStr = "Function: ";

			powerFuncNum = 0;
			serveAsPowerFuncNum = 0;
			squareRootFuncNum = 0;
			sinFuncNum = 0;
			cosFuncNum = 0;
			expFuncNum = 0;
			isMultiply = 0;
			multiplyFuncNum = 0;
			isNested = 0;
			nestedFuncNum = 0;

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



			WinButton button10("MULTIPLY",50,290,100,24,hwnd,(HMENU)IDC_MULTIPLY_BUTTON);
			HWND hbutton10 = button10.getButton();

			WinEditbox editbox13("",170,290,100,24,hwnd,(HMENU)IDC_MULTIPLY_EDITBOX);
			editbox13.setEditboxText((LPARAM)"coeff");
			hEdit13 = editbox13.getEditbox();



			WinButton button11("NESTED",290,290,100,24,hwnd,(HMENU)IDC_NESTEDFUNC_BUTTON);
			HWND hbutton11 = button11.getButton();

			WinEditbox editbox14("",410,290,100,24,hwnd,(HMENU)IDC_NESTEDFUNC_EDITBOX);
			editbox14.setEditboxText((LPARAM)"coeff");
			hEdit14 = editbox14.getEditbox();



			WinButton buttonX("f(x)",50,370,100,24,hwnd,(HMENU)IDC_X_BUTTON);
			HWND hbuttonX = buttonX.getButton();

			WinEditbox editboxX("",170,370,100,24,hwnd,(HMENU)IDC_X_EDITBOX);
			editboxX.setEditboxText((LPARAM)"X");
			hEditX = editboxX.getEditbox();



			WinButton button9("findRoot",50,450,100,24,hwnd,(HMENU)IDC_FINDROOT_BUTTON);
			HWND hbutton9 = button9.getButton();

			WinEditbox editbox9("",170,450,100,24,hwnd,(HMENU)IDC_FINDROOTSTARTVAL_EDITBOX);
			editbox9.setEditboxText((LPARAM)"startVal");
			hEdit9 = editbox9.getEditbox();

			WinEditbox editbox10("",290,450,100,24,hwnd,(HMENU)IDC_FINDROOTENDVAL_EDITBOX);
			editbox10.setEditboxText((LPARAM)"endVal");
			hEdit10 = editbox10.getEditbox();

			WinEditbox editbox11("",410,450,100,24,hwnd,(HMENU)IDC_FINDROOTTOLERROR_EDITBOX);
			editbox11.setEditboxText((LPARAM)"tolError");
			hEdit11 = editbox11.getEditbox();

			WinEditbox editbox12("",530,450,100,24,hwnd,(HMENU)IDC_FINDROOTMAXITER_EDITBOX);
			editbox12.setEditboxText((LPARAM)"maxIter");
			hEdit12 = editbox12.getEditbox();



			WinButton button2("SETTING",50,490,100,24,hwnd,(HMENU)IDC_SETTING_BUTTON);
			HWND hbutton2 = button2.getButton();

			WinButton button3("CLEAR",170,490,100,24,hwnd,(HMENU)IDC_CLEAR_BUTTON);
			HWND hbutton3 = button3.getButton();
			
			WinButton buttonBack("BACK",290,490,100,24,hwnd,(HMENU)IDC_BACK_BUTTON);
			HWND hbuttonBack = buttonBack.getButton();



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

				case IDC_MULTIPLY_BUTTON: {

					/*Multiply the functions*/
					WinEditbox editbox1(hEdit13);
					string coeffStr = editbox1.getEditboxContent();

					if(isMultiply == 0 && coeffStr != "0"){

						isMultiply = 1;
						MultiplyTwoMathFunction func(str2float(coeffStr));
						MathFunction* multFunc = mathFuncVector.back();
						mathFuncVector.pop_back();
						func.functionStore(multFunc);
						multiplyFuncArray[multiplyFuncNum] = func;
						mathFuncVector.push_back(&(multiplyFuncArray[multiplyFuncNum]));
						editbox1.setEditboxText((LPARAM)"1");
						hEdit13 = editbox1.getEditbox();

						funcStr = funcStr.substr(0,funcStr.length()-3);
						funcStr = funcStr + "* ";

					} else if(isMultiply == 1 && coeffStr == "1"){

						MathFunction* multFunc = mathFuncVector.back();
						mathFuncVector.pop_back();
						MathFunction* multMainFunc = mathFuncVector.back();
						mathFuncVector.pop_back();
						multMainFunc->functionStore(multFunc);
						mathFuncVector.push_back(multMainFunc);
						editbox1.setEditboxText((LPARAM)"1");
						hEdit13 = editbox1.getEditbox();

						funcStr = funcStr + "* ";

					} else if(isMultiply == 1 && coeffStr == "0"){

						isMultiply = 0;
						multiplyFuncNum = multiplyFuncNum + 1;
						funcStr = funcStr.substr(0,funcStr.length()-3);
						MathFunction* multMainFunc = mathFuncVector.back();
						multMainFunc->setType("* "+float2str(multMainFunc->getCoefficient()));

					}

					InvalidateRect(hwnd,NULL,TRUE);
					break;
				}

				case IDC_NESTEDFUNC_BUTTON: {
					/*Nested the functions*/
					WinEditbox editbox1(hEdit14);
					string coeffStr = editbox1.getEditboxContent();

					if(isNested == 0 && coeffStr != "0"){

						isNested = 1;
						CalculateAnotherFunctionFirstMathFunction func(str2float(coeffStr));
						MathFunction* nestedFunc = mathFuncVector.back();
						mathFuncVector.pop_back();
						func.functionStore(nestedFunc);
						nestedFuncArray[nestedFuncNum] = func;
						mathFuncVector.push_back(&(nestedFuncArray[nestedFuncNum]));
						editbox1.setEditboxText((LPARAM)"1");
						hEdit14 = editbox1.getEditbox();
						
						funcStr = funcStr.substr(0,funcStr.length()-5);

					} else if(isNested == 1 && coeffStr == "1"){

						MathFunction* nestedFunc = mathFuncVector.back();
						mathFuncVector.pop_back();
						MathFunction* nestedMainFunc = mathFuncVector.back();
						mathFuncVector.pop_back();
						nestedMainFunc->functionStore(nestedFunc);
						mathFuncVector.push_back(nestedMainFunc);
						editbox1.setEditboxText((LPARAM)"1");
						hEdit14 = editbox1.getEditbox();
						
						funcStr = funcStr.substr(0,funcStr.length()-3);

					} else if(isNested == 1 && coeffStr == "0"){

						isNested = 0;
						nestedFuncNum = nestedFuncNum + 1;
						
						funcStr = funcStr.substr(0,funcStr.length()-3);
						MathFunction* nestedMainFunc = mathFuncVector.back();
						nestedMainFunc->setType(" )* "+float2str(nestedMainFunc->getCoefficient()));

					}

					InvalidateRect(hwnd,NULL,TRUE);
					
					break;
				}

				case IDC_X_BUTTON: {

					WinEditbox editbox1(hEditX);
					string xStr = editbox1.getEditboxContent();

					float x = str2float(xStr);

					string tmpMessage;

					float result = getFuncsResult(x,mathFuncVector);
					tmpMessage = "f("+float2str(x)+") is: "+float2str(result);

					showMessage(tmpMessage);

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

					resultRootStr = float2str(bisectionSearchForOneRoot(str2float(startValStr),str2float(endValStr),str2float(tolErrorStr),str2int(maxIterStr),mathFuncVector));

					InvalidateRect(hwnd,NULL,TRUE);

					break;

				}

				case IDC_SETTING_BUTTON: {

					/*Do setting of algorithm or other here*/

					break;

				}

				case IDC_CLEAR_BUTTON: {

					funcStr = "";
					resultRootStr = "";

					powerFuncNum = 0;
					serveAsPowerFuncNum = 0;
					squareRootFuncNum = 0;
					sinFuncNum = 0;
					cosFuncNum = 0;
					expFuncNum = 0;
					mathFuncVector.clear();

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
					WinEditbox editbox13(hEdit13);
					editbox13.setEditboxText((LPARAM)"coeff");
					WinEditbox editbox14(hEdit14);
					editbox14.setEditboxText((LPARAM)"coeff");
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
				
				case IDC_BACK_BUTTON: {
					
					PostMessage(hwnd, WM_CLOSE, 0, 0);
					
					break;
				}

			}

			break;
		}

		case WM_PAINT: {

			hdc = BeginPaint(hwnd,&ps);

			if(!mathFuncVector.empty() && resultRootStr == ""){
				funcStr = funcStr + (mathFuncVector.back())->getType() + " ";

				if(isMultiply == 0 && isNested == 0){
                    funcStr = funcStr + "+ ";
				} else if(isMultiply == 0 && isNested == 1){
					funcStr = funcStr + "   ";
				}

				TextOut(hdc,50,20,funcStr.c_str(),funcStr.length());
				TextOut(hdc,50,420,"Please input value to find root here.",37) ;
			} else if(mathFuncVector.empty() && resultRootStr == "") {
				TextOut(hdc,50,20,"Please input some function here.",32) ;
				TextOut(hdc,50,420,"Please input value to find root here.",37) ;
			} else if(resultRootStr != ""){
				string tmp = "Root: "+resultRootStr;
				TextOut(hdc,50,20,funcStr.c_str(),funcStr.length());
				TextOut(hdc,50,420,tmp.c_str(),tmp.length());
			}

			TextOut(hdc,50,340,"Please input value of x here.",29) ;

			EndPaint (hwnd,&ps) ;

			break;

		}

		case WM_CLOSE:
			ShowWindow(hwnd, SW_HIDE);
			return 0 ;
        default:                       /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
