#include<iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

/**************************

Example usage:
	GNUplot plotter;
	plotter("plot sin(x)");

**************************/

class GNUplot {
public:
	GNUplot() throw(string);
	//Construct GNUplot with address
	GNUplot(string _address) throw(string);
	void setAddress(string _address);
	~GNUplot();
	void operator ()(const string & command);
protected:
	FILE *gnuplotpipe;
	string GNUAddress;
};

GNUplot::GNUplot() throw(string) {
	gnuplotpipe=popen("D:/gnuplot/bin/gnuplot ","w");
	if (!gnuplotpipe) {
		throw("Gnuplot not found !");
	}
}

GNUplot::GNUplot(string _address) throw(string) {
	string order = _address+"/gnuplot ";
	gnuplotpipe=popen(order.c_str(),"w");
	if (!gnuplotpipe) {
		throw("Gnuplot not found !");
	}
}

void GNUplot::setAddress(string _address){
    GNUAddress = _address;
}

GNUplot::~GNUplot() {
	fprintf(gnuplotpipe,"exit\n");
	pclose(gnuplotpipe);
}

void GNUplot::operator()(const string & command) {
	fprintf(gnuplotpipe,"%s\n",command.c_str());
	fflush(gnuplotpipe);
};
