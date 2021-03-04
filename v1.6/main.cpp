// CalWF

#include <io.h>
#include <iomanip>
#include <vector>
#include "iz.h"

using namespace std;

int main(){
	int intv_min = 0;
	while(!intv_min){
		cout<<"Input minimum intevals (in the range of 4 - 8):"<<endl;
		cin>>intv_min;
		if(intv_min < 4 || intv_min > 8){
			cout<<endl;
			cout<<" YY    YY  OOOOO  UU    UU       A     RRRRRR   EEEEEEE   PPPPPP  IIIIII  GGGGGGG   !!!!   "<<endl;
			cout<<"  YY  YY  OO   OO UU    UU      AAA    RR   RRR EE        PP   PP   II   GG         !!!!   "<<endl;
			cout<<"    YY    OO   OO UU    UU     A   A   RRRRRR   EEEEE     PPPPPP    II   GG   GGG   !!!!   "<<endl;
			cout<<"    YY    OO   OO UU    UU    AAAAAAA  RR   RR  EE        PP        II   GG    GG          "<<endl;
			cout<<"    YY     OOOOO   UUUUUU    AA     AA RR    RR EEEEEEE   PP      IIIIII  GGGGG G    !!    "<<endl;
			cout<<endl;
			cout<<"Please input in the range of 4 - 8..."<<endl;
			intv_min = 0;
		}
	}
	Iz data(intv_min);
	
	_finddata_t sFind;
	long lResult = 0;
	lResult = _findfirst("result*", &sFind);
	if(lResult == -1){
		cout<<"ERROR: Can not find the dat file..."<<endl;
		return -1;
	}
	do{
		ifstream fin(sFind.name);
		if(!fin.is_open()){
			cout<<"ERROR: Failed to open "<<sFind.name<<"..."<<endl;
			return -1;
		}
		cout<<endl<<"File name: "<<sFind.name<<endl;
		data.read(fin);
		fin.clear();
		fin.close();
		data.opt_step();
		string file_output = sFind.name;
		data.write("res_" + file_output);
	} while(_findnext(lResult, &sFind) != -1);
	
	system("pause"); 
	return 0;
}
