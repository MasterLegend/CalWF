// CalWF

#include <fstream>
#include <iostream> 
#include <vector>
#include "iz.h"

using namespace std;

int main(){
	Iz curve;
	curve.read();
	curve.calwf();
	curve.writewf("WF.txt"); 
	return 0;
}
