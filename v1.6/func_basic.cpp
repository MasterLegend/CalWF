// Name: func_basic.cpp
// Discribe:
// Author: Master Legend
// Time: 2021/01/22

#include "func_basic.h"
#include <iostream>

using Eigen::MatrixXd;
using Eigen::Vector2d;

vector<string> split_s(string s){
	int j = 0;
	s += ' ';
	int size = s.size();
	vector<string> res;
	string word;
	
	for(int i = 0; i < size; i++){
		if(s[i] == ' '){
			if(s[j] != ' '){
				word = s.substr(j, i - j);
				res.push_back(word);
			}
			j = i;
		} else{
			if(s[j] == ' '){
				j = i;
			}
		}
	}
	return res;
}

Eigen::Vector2d para_initial(vector<double> xvals, vector<double> yvals){
	Vector2d para_i;
	vector<double>::iterator y0 = max_element(yvals.begin(), yvals.end());
	int pos0 = distance(yvals.begin(), y0);
	para_i(1) = (xvals[pos0] - xvals[xvals.size() - 1]) / log(yvals[yvals.size() - 1] / yvals[pos0]);
	para_i(0) = yvals[pos0] / exp(- xvals[pos0] / para_i(1));
	return para_i;
}

MatrixXd cal_jac(double a, double t, vector<double> xvals){
	MatrixXd jac(xvals.size(), 2);
	for(int i = 0; i < xvals.size(); i++){
		jac(i, 0) = dfda(a, t, xvals[i]);
		jac(i, 1) = dfdt(a, t, xvals[i]);
	}
	return jac;
}

double cal_cod(vector<double> xvals, vector<double> yvals, double a, double t){
	double ysum = accumulate(yvals.begin(), yvals.end(), 0.0);
	double ymean = ysum / yvals.size();
	double ssreg = 0.0, ssres = 0.0, sstot = 0.0;
	for(int i = 0; i < xvals.size(); i++){
		ssreg += (f_exp(a, t, xvals[i]) - ymean) * (f_exp(a, t, xvals[i]) - ymean);
		ssres += (yvals[i] - f_exp(a, t, xvals[i])) * (yvals[i] - f_exp(a, t, xvals[i]));
		sstot += (yvals[i] - ymean) * (yvals[i] - ymean);
	}
	return 1 - (ssres / sstot);
}

