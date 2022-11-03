// Name: func_basic.cpp
// Discribe:
// Author: Master Legend
// Time: 2021/01/22

#include "func_basic.h"

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

vector<double> split_stod(string s){
	int j = 0;
	s += '\t';
	int size = s.size();
	vector<double> res;
	string word;
	
	for(int i = 0; i < size; i++){
		if(s[i] == '\t'){
			if(s[j] != '\t'){
				word = s.substr(j, i - j);
				res.push_back(stod(word));
			}
			j = i;
		} else{
			if(s[j] == '\t'){
				j = i;
			}
		}
	}
	return res;
}

vector<int> split_stoi(string s){
	int j = 0;
	s += ' ';
	int size = s.size();
	vector<int> res;
	string word;
	
	for(int i = 0; i < size; i++){
		if(s[i] == ' '){
			if(s[j] != ' '){
				word = s.substr(j, i - j);
				res.push_back(stoi(word));
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

