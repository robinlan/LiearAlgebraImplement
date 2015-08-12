#include<iostream>
#include<stdlib.h>
using namespace std;

/*************************

Example usage:
	GNUplot plotter;
	plotter("plot sin(x)");

*************************/

class GNUplot {
public:
	GNUplot() throw(string);
	~GNUplot();
	void operator ()(const string & command);
protected:
	FILE *gnuplotpipe;
};

GNUplot::GNUplot() throw(string) {
	gnuplotpipe=popen("gnuplot ","w");
	if (!gnuplotpipe) {
		throw("Gnuplot not found !");
	}
}

GNUplot::~GNUplot() {
	fprintf(gnuplotpipe,"exit\n");
	pclose(gnuplotpipe);
}

void GNUplot::operator()(const string & command) {
	fprintf(gnuplotpipe,"%s\n",command.c_str());
	fflush(gnuplotpipe);
};