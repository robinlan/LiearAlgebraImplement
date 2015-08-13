#include <stdlib.h>
#include <string>
#include "gnu_plotter.h"
using namespace std;

int main(){
    GNUplot plotter;
    plotter("plot sin(x)");
    system("pause");
    return 0;
}
