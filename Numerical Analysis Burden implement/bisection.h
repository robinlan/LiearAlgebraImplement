#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include"root_input.h"
using namespace std;

/********************************************************************
Example usage:

	FindOneRootInput input(1,2,0.0001,20);
	vector<float> tmp;
	tmp.push_back(15);  //Store the polynomial function coefficients,
	tmp.push_back(-14); //from x^0 to x^size,
	tmp.push_back(3);   //function: 3*x^2 - 14*x + 15
	input.setPolynomialCoeffs(tmp);
	float root = bisectionSearchForOneRoot(input);
	printf("The root is: %f\n",root);
	
or
	
	FindOneRootInput input; //Here you should key in values manually
	vector<float> tmp;
	tmp.push_back(15);
	tmp.push_back(-14);
	tmp.push_back(3);
	input.setPolynomialCoeffs(tmp);
	float root = bisectionSearchForOneRoot(input);
	printf("The root is: %f\n",root);

********************************************************************/
float bisectionSearchForOneRoot(FindOneRootInput _input){
	
	/*Input: Endpoints, Tolerance error, Maximum iteration number*/
	float startPoint=_input.getStartPoint(),endPoint=_input.getEndPoint(); //Endpoints
	float tolError=_input.getTolError(); //Tolerance error
	int maxIteNum=_input.getMaxIteNum(); //Maximum iteration number
	
	/*Declare: counter, Start point value, the Root position*/
	int i=1; //Counter
	float FofA=_input.getPolynomialValue(startPoint); //Start point value of f(x)
	float root=0.0; //The root position
	float FofRoot=0.0;
	
	/*Looping: Iteratively update the p position, stop when error is less than tolerance*/
	while(i<=maxIteNum){
		
		/*Compute root position and f(root)*/
		root = (startPoint+endPoint)/2.0;
		FofRoot = _input.getPolynomialValue(root);
		
		/*Check if current error is below to the tolerance error*/
		if(FofRoot == 0.0 || (endPoint-startPoint)/2.0 < tolError){
			printf("Total iteration: %d\n",i);
			return root;
		}
		
		/*Update the counter*/
		i=i+1;
		
		/*Update the start point and end point*/
		if(FofA * FofRoot > 0.0){
			startPoint = root;
			FofA = FofRoot;
		} else{
			endPoint = root;
		}
		
	}
	
	/*Error message*/
	printf("Method failed after maximum iteration.\n");
	
	return root;
}
