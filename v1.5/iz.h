// iz.h
// z in unit of Angstrom
// current is logaarithm of I with e as the base

#ifndef IZ_H_
#define IZ_H_

#include <vector>

using namespace std;

class Iz{
private:
	int data_num;
	vector<double> z;
	vector<vector<double>> current;
	vector<double> wf; 

public:
	Iz();
	~Iz() { };
	void calwf();
	void clear();
	int read();
	void writewf(string file_name);
};

#endif
