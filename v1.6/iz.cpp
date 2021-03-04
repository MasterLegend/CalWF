// iz.cpp

#include <algorithm>
#include <iomanip>
#include <cmath>
#include <numeric>
#include "func_basic.h"
#include "iz.h"

using Eigen::MatrixXd;
using Eigen::Vector2d;
using Eigen::VectorXd;

Iz::Iz(){
	clean();
}

Iz::Iz(int min){
	clean();
	intv_min = min;
}

void Iz::clean(){
	step = 0.1;
	step_opt = 0.1;
	cod_opt = 0.0;
	para << 1.0, 1.0;
	para_opt << 1.0, 1.0;
	threshold.clear();
	xvals.clear();
	yvals.clear();
}

void Iz::read(ifstream &fin){
	clean();
	string str_line;
	
	vector<double> current_data;
	while(getline(fin, str_line)){
		if(str_line.empty()) continue;
		threshold.push_back(stod(split_s(str_line)[1]));
	}
}

void Iz::cal_intv(){
	xvals.clear();
	yvals.clear();
	cout<<setiosflags(ios::fixed)<<setprecision(2);
	
	double max_val = *max_element(threshold.begin(),threshold.end());
	int intv_size = max_val / step + 1;
	double xmean[intv_size] = {0.0}; 
	int intv[intv_size] = {0};
	cout<<endl<<"step: "<<step<<" intervel num: "<<intv_size<<endl;
	for(auto &val : threshold){
		int pos = val / step;
		xmean[pos] += val;
		intv[pos]++;
	}
	for(int i = 0; i < intv_size; i++){
		if(intv[i]){
//			xvals.push_back((i + 0.5) * step);
			yvals.push_back(intv[i]);
			xvals.push_back(xmean[i] / yvals.back());
		}
	}
	cout<<"data to fit:"<<endl;
	cout<<setiosflags(ios::fixed)<<setprecision(3);
	for(int i = 0; i < xvals.size(); i++){
		cout<<xvals[i]<<" "<<int(yvals[i])<<endl;
	}	
	cout<<resetiosflags(ios::fixed);
}

void Iz::fit_GN(){
	cout<<setiosflags(ios::fixed)<<setprecision(4);
	
	para = para_initial(xvals, yvals);
	Vector2d para_d(0.0, 0.0);
	VectorXd r(xvals.size());
	double cod = cal_cod(xvals, yvals, para(0), para(1));
	cout<<"LOOP:"<<endl;
	cout<<"n\tA\tt\tcod"<<endl;
	cout<<"0\t"<<para(0)<<"\t"<<para(1)<<"\t"<<cod<<endl;
	for(int i = 0; i < 50; i++){
		for(int j = 0; j < xvals.size(); j++){
			r(j) = f_err(para(0), para(1), xvals[j], yvals[j]);
		}
		MatrixXd jac = cal_jac(para(0), para(1), xvals);
		para_d = - (jac.transpose() * jac).inverse() * (jac.transpose() * r);
		para = para + para_d;
		cod = cal_cod(xvals, yvals, para(0), para(1));
		cout<<i + 1<<"\t"<<para(0)<<"\t"<<para(1)<<"\t"<<cod<<endl;
		if(cod < 0){
			cout<<"Bad cod value..."<<endl;
			break;
		}
		if(abs(para_d(0)) < 0.0001 && abs(para_d(1)) < 0.0001){
			cout<<"Accuracy achieved, leave the loop..."<<endl;
			break;
		}
		if(i == 49){
			cout<<"Unable to converge..."<<endl;
			break;
		}	
	}
	if(cod > cod_opt){
		para_opt(0) = para(0);
		para_opt(1) = para(1);
		step_opt = step;
		cod_opt = cod;
	}
	
	cout<<resetiosflags(ios::fixed);
}

void Iz::opt_step(){
	cout<<setiosflags(ios::fixed)<<setprecision(2);
	double max_val = *max_element(threshold.begin(),threshold.end());
	cout<<"max value: "<<max_val<<endl;
	double step_min = 0.01, step_max = max_val / (intv_min - 1);
	cout<<"step_min: "<<step_min<<" step_max: "<<step_max<<endl;
	cout<<resetiosflags(ios::fixed);
	cout.setstate(ios_base::failbit);
	step = step_min;
	while(step < step_max){
		cal_intv();
		if(xvals.size() < intv_min){
			step += 0.01;
			cout<<"too small intv num..."<<endl;
			continue;
		}
		fit_GN();
		step += 0.01;
	}
	step = step_opt;
	cout.clear();
	cout<<"opt result:";
	cal_intv();
	cout<<"A = "<<para_opt(0)<<", t = "<<para_opt(1)<<", COD = "<<cod_opt<<endl;
}

void Iz::write(string file_name){
	ofstream fout(file_name, ios_base::out);
	
	fout<<setiosflags(ios::fixed)<<setprecision(2);
	fout<<"step: "<<step_opt<<", intv num: "<<xvals.size()<<endl;
	fout<<setiosflags(ios::fixed)<<setprecision(3);
	fout<<"intv result:"<<endl;
	for(int i = 0; i < xvals.size(); i++){
		fout<<xvals[i]<<" "<<int(yvals[i])<<endl;
	}
	fout<<"opt result:"<<endl;
	fout<<"A = "<<para_opt(0)<<endl;
	fout<<"t = "<<para_opt(1)<<endl;
	fout<<"COD = "<<cod_opt<<endl;
	
	fout<<resetiosflags(ios::fixed);
	fout.clear();
	fout.close();
}

