#include<iostream>
#include<vector>
#include<math.h>
using namespace std;

class MathFunction{
	private:
		float x; //The variable x
		float result; //f(x)
		float coefficient; //Coefficient of this item
	public:
		MathFunction(){}
		/*Construct object with coefficient*/
		MathFunction(float _coeff){
			coefficient = _coeff;
		}
		/*Use x to calculate f(x)*/
		virtual void calculateResult() = 0;
		/*Access the x*/
		float getX(){
			return x;
		}
		/*Access the result*/
		float getResult(){
			return result;
		}
		/*Access the coefficient*/
		float getCoefficient(){
			return coefficient;
		}
		/*Allow setting the result*/
		void setX(float _x){
			x = _x;
		}
		/*Allow setting the result*/
		void setResult(float _result){
			result = coefficient*_result;
			//system("pause");
			//result = coefficient*_result;
		}
		/*Allow setting the coefficient*/
		void setCoefficient(float _coeff){
			coefficient = _coeff;
		}
		
};

/******************************************

Example usage:
	float x=0.64;
	vector<MathFunction*> funcVector;
	PowerMathFunction func1(2);
	func1.setPower(2);
	funcVector.push_back(&func1);
	float tmpResult = addTheFunction(x,funcVector);
	printf("result = %f\n",tmpResult);
	
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
		/*Calculate result with x and power*/
		void calculateResult(){
			setResult(pow(getX(),power));
		}
};

/******************************************

Example usage:
	float x=3;
	vector<MathFunction*> funcVector;
	ServeAsPowerMathFunction func6(2);
	func6.setBottomNum(2);
	funcVector.push_back(&func6);
	float tmpResult = addTheFunction(x,funcVector);
	printf("result = %f\n",tmpResult);
	
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
		void calculateResult(){
			setResult(pow(bottomNum,getX()));
		}
};

/**********************************************

Example usage:
	float x=0.64;
	vector<MathFunction*> funcVector;
	SquareRootMathFunction func3(2);
	funcVector.push_back(&func3);
	float tmpResult = addTheFunction(x,funcVector);
	printf("result = %f\n",tmpResult);

**********************************************/
class SquareRootMathFunction: public MathFunction{
	public:
		SquareRootMathFunction(){}
		/*Construct object with coefficient*/
		SquareRootMathFunction(float _coeff){
			setCoefficient(_coeff);
		}
		/*Calculate result with x and power*/
		void calculateResult(){
			setResult(sqrt(getX()));
		}
};

/**********************************************

Example usage:
	float x=0.64;
	vector<MathFunction*> funcVector;
	SinMathFunction func2(2);
	funcVector.push_back(&func2);
	float tmpResult = addTheFunction(x,funcVector);
	printf("result = %f\n",tmpResult);

**********************************************/
class SinMathFunction: public MathFunction{
	public:
		SinMathFunction(){}
		/*Construct object with coefficient*/
		SinMathFunction(float _coeff){
			setCoefficient(_coeff);
		}
		/*Calculate result with x and power*/
		void calculateResult(){
			setResult(sin(getX()));
		}
};

/**********************************************

Example usage:
	float x=0.64;
	vector<MathFunction*> funcVector;
	CosMathFunction func4(2);
	funcVector.push_back(&func4);
	float tmpResult = addTheFunction(x,funcVector);
	printf("result = %f\n",tmpResult);

**********************************************/
class CosMathFunction: public MathFunction{
	public:
		CosMathFunction(){}
		/*Construct object with coefficient*/
		CosMathFunction(float _coeff){
			setCoefficient(_coeff);
		}
		/*Calculate result with x and power*/
		void calculateResult(){
			setResult(cos(getX()));
		}
};

/**********************************************

Example usage:
	float x=0.64;
	vector<MathFunction*> funcVector;
	ExpMathFunction func4(2);
	funcVector.push_back(&func4);
	float tmpResult = addTheFunction(x,funcVector);
	printf("result = %f\n",tmpResult);

**********************************************/
class ExpMathFunction: public MathFunction{
	public:
		ExpMathFunction(){}
		/*Construct object with coefficient*/
		ExpMathFunction(float _coeff){
			setCoefficient(_coeff);
		}
		/*Calculate result with x and power*/
		void calculateResult(){
			setResult(exp(getX()));
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
		void calculateResult(){
			
			float tmpResult = 1.0;
			for(int i=0;i<funcVector.size();i++){
				
				funcVector[i]->setX(getX());
				funcVector[i]->calculateResult();
				tmpResult = tmpResult * funcVector[i]->getResult();
				
			}
			setResult(tmpResult);
			
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
		void calculateResult(){
			
			funcVector[0]->setX(getX());
			for(int i=0;i<funcVector.size();i++){
				
				if(i>0)
					funcVector[i]->setX(funcVector[i-1]->getResult());
				funcVector[i]->calculateResult();
				
			}
			float tmpResult = funcVector[funcVector.size()-1]->getResult();
			setResult(tmpResult);
			
		}
};