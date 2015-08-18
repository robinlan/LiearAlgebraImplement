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

class AddIntegratedMathFunction: public MathFunction{
	private:
		vector<MathFunction*> funcVector;
	public:
		AddIntegratedMathFunction(){}
		AddIntegratedMathFunction(float _coeff){
			
		}
		void storeMathfuncToVector(MathFunction* _func){
			
		}
		float calculateResult(float _x){
			
		}
};