#include<iostream>
#include<math.h>
#include<vector>
using namespace std;

/******************************************************************
Store information of inputs when tackling one-root finding problems
*******************************************************************/
class FindOneRootInput{
	private:
		float startPoint; //start point of search range
		float endPoint; //end point of search range
		float FofStartPoint; //f(startPoint)
		float FofEndPoint; //f(endPoint)
		float tolError; //tolerance error
		int maxIteNum; //maximum iteration number
		vector<float> polynomialCoeffs;//polynomial coefficients
	public:
		/*Called when input manually*/
		FindOneRootInput(){
			
			/*Input: Endpoints, Tolerance error, Maximum iteration number*/
			printf("Please input start point & end point of searching range:\n");
			float _startPoint=0.0,_endPoint=0.0; //Endpoints
			cin>>_startPoint>>_endPoint;
			printf("Please input tolerance error:\n");
			float _tolError=0.0; //Tolerance error
			cin>>_tolError;
			printf("Please input maximum iteration number:\n");
			int _maxIteNum; //Maximum iteration number
			cin>>_maxIteNum;
			
			startPoint = _startPoint;
			endPoint = _endPoint;
			tolError = _tolError;
			maxIteNum = _maxIteNum;
			
		}
		/*Called when initially specify the conditions*/
		FindOneRootInput(float _startPoint, float _endPoint, float _tolError, int _maxIteNum){
			
			startPoint = _startPoint;
			endPoint = _endPoint;
			tolError = _tolError;
			maxIteNum = _maxIteNum;
			
		}
		/*Store the coefficient of polynomial vector into polynomialCoeffs vector*/
		void setPolynomialCoeffs(vector<float> _coeffs){
			
			for(int i=0;i<_coeffs.size();i++){
				polynomialCoeffs.push_back(_coeffs[i]);
			}
			
		}
		/*Access start point*/
		float getStartPoint(){
			return startPoint;
		}
		/*Access end point*/
		float getEndPoint(){
			return endPoint;
		}
		/*Access tolError*/
		float getTolError(){
			return tolError;
		}
		/*Access maximum iteration number*/
		int getMaxIteNum(){
			return maxIteNum;
		}
		/*Access size of polynomialCoeffs vector size*/
		int getPolynomialCoeffSize(){
			return polynomialCoeffs.size();
		}
		/*Access a coefficient in polynomialCoeffs vector*/
		float getPolynomialCoeff(int _site){
			return polynomialCoeffs[_site];
		}
		/*Input x and then get the f(x) value*/
		float getPolynomialValue(float _x){
			
			float result=0.0;
			int size=polynomialCoeffs.size();
			for(int i=0;i<size;i++){
				result = result + polynomialCoeffs[i]*pow(_x,i);
			}
			return result;
			
		}
};