// iz.cpp

#include <cmath>
#include <fstream>
#include <io.h>
#include <iomanip>
#include <iostream>
#include <numeric>
#include "func_basic.h"
#include "iz.h"

Iz::Iz(){
	data_num = 0;
	z.clear();
	current.clear();
	wf.clear();
}

void Iz::calwf(){
	wf.clear();
	
	int point_num = z.size();
	double slope, intercept, denominator, wf_value;
	double para_a = 0.0, para_b = 0.0, para_c = 0.0, para_d = 0.0;
	for(auto &z_val : z){
		para_a += (z_val * z_val);
	}
	para_b = accumulate(z.begin(), z.end(), 0.0); 
	for(int i = 0; i < data_num; i++){
		slope = 0.0;
		intercept = 0.0;
		para_c = 0.0;
		for(int j = 0; j < point_num; j++){
			para_c += (z[j] * current[i][j]);
		}
		para_d = accumulate(current[i].begin(), current[i].end(), 0.0);
		denominator = para_a * point_num - para_b * para_b;
		if(denominator){
			slope = (para_c * point_num - para_b * para_d) / denominator;
			intercept = (para_a * para_d - para_c * para_b) / denominator;
		} else{
			slope = 1;
			intercept = 0;
		}
		wf.push_back(slope * slope * 0.952);
	}
}

void Iz::clear(){
	data_num = 0;
	z.clear();
	current.clear();
	wf.clear();
}

int Iz::read(){
	clear();
	string file_name;
	int file_num = 0;
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
		
		string str_line;
		while(getline(fin, str_line)){
			if(str_line == "[DATA]"){
				break;
			}
		}
		getline(fin, str_line);
		
		vector<double> current_data;
		while(getline(fin, str_line)){
			if(!data_num){
				z.push_back(split_stod(str_line)[0] * 1e10);
			}
			current_data.push_back(log(abs(split_stod(str_line)[1])));
		}
		current.push_back(current_data);
		
		data_num++;
		fin.clear();
		fin.close();
	} while(_findnext(lResult, &sFind) != -1);
	
	if(data_num != current.size()){
		return -1;
	}
	
	return 0;
}

void Iz::writewf(string file_name){
	ofstream fout(file_name, ios_base::out);
	fout<<setiosflags(ios::fixed)<<setprecision(3);
	
	int col_num = sqrt(data_num);
	cout<<col_num<<endl;
	for(int i = 0; i < data_num; i++){
		fout<<wf[i];
		if(i % col_num == col_num - 1){
			fout<<endl;
		} else{
			fout<<"\t";
		}
	}
	
	fout<<resetiosflags(ios::fixed);
	fout.clear();
	fout.close();
}

