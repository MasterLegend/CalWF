// Name: func_basic.h
// Discribe:
// Author: Huang Zhichao
// Time: 2021/01/22

#ifndef FUNC_BASIC_H_
#define FUNC_BASIC_H_
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <Eigen/Dense>

using namespace std;

vector<string> split_s(string s);
inline double f_exp(double a, double t, double x) { return a * exp(- x / t); }
inline double f_err(double a, double t, double xi, double yi) { return yi - a * exp(- xi / t); }
inline double dfda(double a, double t, double x) { return -exp(- x / t); }
inline double dfdt(double a, double t, double x) { return -a * x * exp(- x / t) / (t * t); }
Eigen::Vector2d para_initial(vector<double> xvals, vector<double> yvals);
Eigen::MatrixXd cal_jac(double a, double t, vector<double> xvals);
double cal_cod(vector<double> xvals, vector<double> yvals, double a, double t);

#endif
