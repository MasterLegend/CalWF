// iz.h
// current in unit pA

#ifndef IT_H_
#define IT_H_

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class It{
private:
	vector<pair<double, int>> current;
	vector<pair<int, int>> state;

public:
	It();
	~It() {	};
	void classification();
	void clear();
	void read(ifstream &fin);
	void write(string file_name);
};

#endif
