// iz.h
// t in unit of Angstrom
// current is logaarithm of I with e as the base

#ifndef IZ_H_
#define IZ_H_

#include <fstream>
#include <iostream>
#include <vector>
#include <Eigen/Dense>

using namespace std;
using Eigen::Vector2d;

class Iz{
private:
	int intv_min;
	double step;
	double step_opt;
	double cod_opt;
	Vector2d para;
	Vector2d para_opt;
	vector<double> threshold;
	vector<double> xvals;
	vector<double> yvals;
	void clean();

public:
	Iz();
	Iz(int min);
	~Iz() { };
	void read(ifstream &fin);
	void cal_intv();
	void fit_GN();
	void opt_step();
	void write(string file_name);
};

#endif
