#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include"bisection.h"
using namespace std;

int main(){
	
	FindOneRootInput input(3.2,4,0.01,20);
	vector<float> tmp;
	tmp.push_back(-6);
	tmp.push_back(14);
	tmp.push_back(-7);
	tmp.push_back(1);
	input.setPolynomialCoeffs(tmp);
	float root = bisectionSearchForOneRoot(input);
	printf("The root is: %f\n",root);
	
	system("pause");
	return 0;
}
