#include <iostream>
#include <vector>
#include <math.h>
#include "type_convert.h"
using namespace std;

/***********************************

Example usage:
	float result = 0.0;
	for(int i=0;i<_mathFuncVector.size();i++){
		result = result + _mathFuncVector[i]->calculateResult(_x);
	}

***********************************/
class MathFunction{
	private:
		string type;       				  //Store the type name of the function
		float coefficient; 				  //Coefficient of this item
		vector<MathFunction*> funcVector; //Two functions will multiply each other
	public:
		MathFunction(){}
		/*Construct object with coefficient*/
		MathFunction(float _coeff){
			coefficient = _coeff;
		}
		/*Use x to calculate f(x)*/
		virtual float calculateResult(float _x) = 0;
		/*Access the type name*/
		string getType(){
			return type;
		}
		/*Access the coefficient*/
		float getCoefficient(){
			return coefficient;
		}
		/*Access the funcVector*/
		vector<MathFunction*> getFuncVector(){
			return funcVector;
		}
		/*Allow setting the type name*/
		void setType(string _type){
			type = _type;
		}
		/*Allow setting the coefficient*/
		void setCoefficient(float _coeff){
			coefficient = _coeff;
		}
		/*Store one function*/
		void functionStore(MathFunction* _func){
			funcVector.push_back(_func);
		}
		
};

/******************************************

Example usage:
	PowerMathFunction func(str2float(coeffStr));
	func.setPower(str2float(powerStr));
	mathFuncVector.push_back(&func);
	
******************************************/
class PowerMathFunction: public MathFunction{
	private:
		float power; //power of x
	public:
		PowerMathFunction(){}
		/*Construct object with coefficient*/
		PowerMathFunction(float _coeff){
			setCoefficient(_coeff);
		}
		/*Set the power*/
		void setPower(int _power){
			
			power = _power;
			setType(float2str(getCoefficient())+"*power(x,"+int2str(_power)+")");
			
		}
		/*Use x to calculate f(x)*/
		float calculateResult(float _x){
			
			float result = getCoefficient() * pow(_x,power);
			return result;
			
		}
};

/******************************************

Example usage:
	ServeAsPowerMathFunction func(str2float(coeffStr));
	func.setBottomNum(str2float(bottomNumStr));
	mathFuncVector.push_back(&func);
	
******************************************/
class ServeAsPowerMathFunction: public MathFunction{
	private:
		float bottomNum; //bottom number of x
	public:
		ServeAsPowerMathFunction(){}
		/*Construct object with coefficient*/
		ServeAsPowerMathFunction(float _coeff){
			setCoefficient(_coeff);
		}
		/*Set the bottom number*/
		void setBottomNum(float _bottomNum){
			
			bottomNum = _bottomNum;
			setType(float2str(getCoefficient())+"*serve_as_power("+float2str(_bottomNum)+",x)");
			
		}
		/*Calculate result with x and power*/
		float calculateResult(float _x){
			
			float result = getCoefficient() * pow(bottomNum,_x);
			return result;
			
		}
};

/****************************************************

Example usage:
	SquareRootMathFunction func(str2float(coeffStr));
	mathFuncVector.push_back(&func);

****************************************************/
class SquareRootMathFunction: public MathFunction{
	public:
		SquareRootMathFunction(){}
		/*Construct object with coefficient*/
		SquareRootMathFunction(float _coeff){
			
			setCoefficient(_coeff);
			setType(float2str(_coeff)+"*square_root(x)");
			
		}
		/*Calculate result with x and power*/
		float calculateResult(float _x){
			
			float result = getCoefficient() * sqrt(_x);
			return result;
			
		}
};

/**********************************************

Example usage:
	SinMathFunction func(str2float(coeffStr));
	mathFuncVector.push_back(&func);

**********************************************/
class SinMathFunction: public MathFunction{
	public:
		SinMathFunction(){}
		/*Construct object with coefficient*/
		SinMathFunction(float _coeff){
			
			setCoefficient(_coeff);
			setType(float2str(_coeff)+"*sin(x)");
			
		}
		/*Calculate result with x and power*/
		float calculateResult(float _x){
			
			float result = getCoefficient() * sin(_x);
			return result;
			
		}
};

/**********************************************

Example usage:
	CosMathFunction func(str2float(coeffStr));
	mathFuncVector.push_back(&func);

**********************************************/
class CosMathFunction: public MathFunction{
	public:
		CosMathFunction(){}
		/*Construct object with coefficient*/
		CosMathFunction(float _coeff){
			
			setCoefficient(_coeff);
			setType(float2str(_coeff)+"*cos(x)");
			
		}
		/*Calculate result with x and power*/
		float calculateResult(float _x){
			
			float result = getCoefficient() * cos(_x);
			return result;
			
		}
};

/**********************************************

Example usage:
	ExpMathFunction func(str2float(coeffStr));
	mathFuncVector.push_back(&func);

**********************************************/
class ExpMathFunction: public MathFunction{
	public:
		ExpMathFunction(){}
		/*Construct object with coefficient*/
		ExpMathFunction(float _coeff){
			
			setCoefficient(_coeff);
			setType(float2str(_coeff)+"*exp(x)");
			
		}
		/*Calculate result with x and power*/
		float calculateResult(float _x){
			
			float result = getCoefficient() * exp(_x);
			return result;
			
		}
};
/**********************************************

Example usage:
	Global setting:
		int isMultiply = 0;
		string coeffStr;
		
	In trigger place:
		if(isMultiply == 0 && coeffStr != "0"){

			isMultiply = 1;
			MultiplyTwoMathFunction func(str2float(coeffStr));
			MathFunction* multFunc = mathFuncVector.back();
			mathFuncVector.pop_back();
			func.functionStore(multFunc);
			mathFuncVector.push_back(&func);

		} else if(isMultiply == 1 && coeffStr == "1"){

			MathFunction* multFunc = mathFuncVector.back();
			mathFuncVector.pop_back();
			MathFunction* multMainFunc = mathFuncVector.back();
			mathFuncVector.pop_back();
			multMainFunc->functionStore(multFunc);
			mathFuncVector.push_back(multMainFunc);

		} else if(isMultiply == 1 && coeffStr == "0"){
			isMultiply = 0;
		}
	
**********************************************/
class MultiplyTwoMathFunction: public MathFunction{
	public:
		MultiplyTwoMathFunction(){}
		/*Construct object with coefficient*/
		MultiplyTwoMathFunction(float _coeff){
			setCoefficient(_coeff);			
			setType("");
		}
		/*Calculate result with function vector*/
		float calculateResult(float _x){
			
			float tmpResult = 1.0;
			vector<MathFunction*> _funcVector = getFuncVector();
			for(int i=0;i<_funcVector.size();i++){
				tmpResult = tmpResult * _funcVector[i]->calculateResult(_x);
			}
			tmpResult = getCoefficient() * tmpResult;
			return tmpResult;
			
		}
};

/**********************************************

Example usage:
	Global setting:
		int isNested;
		string coeffStr;
	
	In trigger place:
		if(isNested == 0 && coeffStr != "0"){

			isNested = 1;
			CalculateAnotherFunctionFirstMathFunction func(str2float(coeffStr));
			MathFunction* nestedFunc = mathFuncVector.back();
			mathFuncVector.pop_back();
			func.functionStore(nestedFunc);
			mathFuncVector.push_back(&func);
			
		} else if(isNested == 1 && coeffStr == "1"){

			MathFunction* nestedFunc = mathFuncVector.back();
			mathFuncVector.pop_back();
			MathFunction* nestedMainFunc = mathFuncVector.back();
			mathFuncVector.pop_back();
			nestedMainFunc->functionStore(nestedFunc);
			mathFuncVector.push_back(nestedMainFunc);

		} else if(isNested == 1 && coeffStr == "0"){
			isNested = 0;
		}
	
**********************************************/

class CalculateAnotherFunctionFirstMathFunction: public MathFunction{
	public:
		CalculateAnotherFunctionFirstMathFunction(){}
		/*Construct object with coefficient*/
		CalculateAnotherFunctionFirstMathFunction(float _coeff){
			
			setCoefficient(_coeff);
			setType("");
			
		}
		/*Calculate result with function vector*/
		float calculateResult(float _x){
			
			vector<MathFunction*> _funcVector = getFuncVector();
			float result = _funcVector[_funcVector.size()-1]->calculateResult(_x);
			for(int i=_funcVector.size()-2;i>=0;i--){
				result = _funcVector[i]->calculateResult(result);
			}
			result = getCoefficient() * result;
			return result;
			
		}
};