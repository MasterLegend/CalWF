// CalWF

#include <fstream>
#include <io.h>
#include <iomanip>
#include <iostream> 
#include <vector>
#include "iz.h"
#include "it.h"

using namespace std;

int main(){
	cout<<"*** Welcome to CalWF_1.5.1 ***"<<endl<<endl;
	cout<<"please input to choose mode..."<<endl;
	cout<<"INPUT 1: SOLUTE WORK FUNCTION"<<endl;
	cout<<"INPUT 2: CLASSIFICATION"<<endl;
	cout<<"INPUT 0: Exit"<<endl;
	
	int mode = 0;
	cin>>mode;
	if(mode == 1){
		Iz curve;
		curve.read();
		curve.calwf();
		curve.writewf("WF.txt"); 
	} else if(mode == 2){
		It data;
		string file_name;
		_finddata_t sFind;
		long lResult = 0;
		lResult = _findfirst("*.dat", &sFind);
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
			data.classification();
			string file_output = sFind.name;
			data.write(file_output + ".txt");
		} while(_findnext(lResult, &sFind) != -1);
	} else if(mode == 0){
		return 0;
	}
	
	system("pause");
	return 0;
}
