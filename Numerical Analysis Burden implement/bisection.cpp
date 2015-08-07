#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include"bisection.h"
using namespace std;

int main(){
	
	FindOneRootInput input(1,2,0.0001,20);
	vector<float> tmp;
	tmp.push_back(15);
	tmp.push_back(-14);
	tmp.push_back(3);
	input.setPolynomialCoeffs(tmp);
	float root = bisectionSearchForOneRoot(input);
	printf("The root is: %f\n",root);
	
	system("pause");
	return 0;
}