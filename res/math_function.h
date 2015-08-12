#include <iostream>
#include <vector>
#include <math.h>
#include "type_convert.h"
using namespace std;

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

/**********************************************

Example usage:
	

**********************************************/
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
	float x=M_PI/3;
	vector<MathFunction*> funcVector2;
	MultiplyTwoMathFunction multFunc(2);
	CosMathFunction func5(4);
	multFunc.functionStore(&func5);
	PowerMathFunction func6(1);
	func6.setPower(1);
	multFunc.functionStore(&func6);
	funcVector2.push_back(&multFunc);
	float tmpResult = addTheFunction(x,funcVector2);
	printf("result = %f\n",tmpResult);
	
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
	float x=M_PI/6;
	vector<MathFunction*> funcVector2;
	CalculateAnotherFunctionFirstMathFunction anotherFunc(3);
	PowerMathFunction func6(2);
	func6.setPower(1);
	anotherFunc.functionStore(&func6);
	CosMathFunction func5(1);
	anotherFunc.functionStore(&func5);
	funcVector2.push_back(&anotherFunc);
	float tmpResult = addTheFunction(x,funcVector2);
	printf("result = %f\n",tmpResult);
	
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