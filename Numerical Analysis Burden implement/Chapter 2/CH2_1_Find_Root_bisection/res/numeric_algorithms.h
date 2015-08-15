#include <vector>
#include "math_function.h"
//#include "win_widget.h"
using namespace std;

//Get the result(y) with input variable(x) into function
float getFuncsResult(float _x, vector<MathFunction*> _mathFuncVector){
	
	float result = 0.0;
	for(int i=0;i<_mathFuncVector.size();i++){
		result = result + _mathFuncVector[i]->calculateResult(_x);
	}
	return result;
	
}

//Use binary search to search for solution in a specific range
float bisectionSearchForOneRoot(float _startPoint, float _endPoint, float _tolError, int _maxIteNum, vector<MathFunction*> _mathFuncVector){
	
	/*Declare: counter, Start point value, the Root position*/
	int i=1; //Counter
	float FofA=getFuncsResult(_startPoint,_mathFuncVector); //Start point value of f(x)
	float root=0.0; //The root position
	float FofRoot=0.0;
	
	/*Looping: Iteratively update the p position, stop when error is less than tolerance*/
	while(i<=_maxIteNum){
		
		/*Compute root position and f(root)*/
		root = (_startPoint+_endPoint)/2.0;
		FofRoot = getFuncsResult(root,_mathFuncVector);
		
		/*Check if current error is below to the tolerance error*/
		if(FofRoot == 0.0 || (_endPoint-_startPoint)/2.0 < _tolError){
			return root;
		}
		
		/*Update the counter*/
		i=i+1;
		
		/*Update the start point and end point*/
		if(FofA * FofRoot > 0.0){
			_startPoint = root;
			FofA = FofRoot;
		} else{
			_endPoint = root;
		}
		
	}
	
	/*Error message*/
	showMessage("Maximum iteration achieve, finding task failed!");
	
	return root;
}