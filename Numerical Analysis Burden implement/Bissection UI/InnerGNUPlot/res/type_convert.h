#include <sstream>
#include <ostream>
#include <string>
using namespace std;

//Convert string to integer
int str2int(string _str){
	istringstream is(_str);
	int val;
	is>>val;
	return val;
}

//Covert string to float
float str2float(string _str){
	stringstream fs(_str);
	float val;
	fs>>val;
	return val;
}

//Convert integer to string
string int2str(int i) {
	string s;
  	stringstream ss(s);
  	ss << i;
  	return ss.str();
}

//Convert integer to string
string float2str(float f) {
	string s;
  	stringstream ss(s);
  	ss << f;
  	return ss.str();
}