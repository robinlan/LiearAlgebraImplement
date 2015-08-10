#include<iostream>
#include<vector>
#include<math.h>
using namespace std;

class MathFunction{
	private:
		float coefficient; //Coefficient of this item
	public:
		MathFunction(){}
		/*Construct object with coefficient*/
		MathFunction(float _coeff){
			coefficient = _coeff;
		}
		/*Use x to calculate f(x)*/
		virtual float calculateResult(float _x) = 0;
		/*Access the coefficient*/
		float getCoefficient(){
			return coefficient;
		}
		/*Allow setting the coefficient*/
		void setCoefficient(float _coeff){
			coefficient = _coeff;
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
	private:
		vector<MathFunction*> funcVector; //Two functions will multiply each other
	public:
		MultiplyTwoMathFunction(){}
		/*Construct object with coefficient*/
		MultiplyTwoMathFunction(float _coeff){
			setCoefficient(_coeff);
		}
		/*Store one function*/
		void functionStore(MathFunction* _func){
			funcVector.push_back(_func);
		}
		/*Calculate result with function vector*/
		float calculateResult(float _x){
			
			float tmpResult = 1.0;
			for(int i=0;i<funcVector.size();i++){
				tmpResult = tmpResult * funcVector[i]->calculateResult(_x);
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
	private:
		vector<MathFunction*> funcVector; //Calculate the first, and send the result to the second
	public:
		CalculateAnotherFunctionFirstMathFunction(){}
		/*Construct object with coefficient*/
		CalculateAnotherFunctionFirstMathFunction(float _coeff){
			setCoefficient(_coeff);
		}
		/*Store one function*/
		void functionStore(MathFunction* _func){
			funcVector.push_back(_func);
		}
		/*Calculate result with function vector*/
		float calculateResult(float _x){
			
			float result = funcVector[0]->calculateResult(_x);
			for(int i=0;i<funcVector.size();i++){
				result = funcVector[i]->calculateResult(result);
			}
			result = getCoefficient() * result;
			return result;
			
		}
};