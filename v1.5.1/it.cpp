// it.cpp

#include <cmath>
#include "it.h"

It::It(){
	current.clear();
	state.clear();
}

void It::classification(){
	int size = current.size();
	double k0 = 0.0, k1 = 0.0;
	double b0 = current[0].first, b1 = current[0].first;
	for(auto &data : current){
		if(data.first < b0) b0 = data.first;
		if(data.first > b1) b1 = data.first;
	}
	cout<<"min data: "<<b0<<", max data: "<<b1<<endl;
	double k0_bak = 10.0, k1_bak = 10.0, b0_bak = b0 + 10.0, b1_bak = b1 + 10.0;
	int loop = 1;
	
	while(loop < 50 && (k0_bak - k0 > 0.00001 || k1_bak - k1 > 0.00001 || b0_bak - b0 > 0.00001 || b1_bak - b1 > 0.00001)){
		k0_bak = k0;
		k1_bak = k1;
		b0_bak = b0;
		b1_bak = b1;
		double s0 = 0;
		double s1 = 0;
		for(int i = 0; i < size; i++){
			double dis0 = abs(k0 * i + b0 - current[i].first);
			double dis1 = abs(k1 * i + b1 - current[i].first);
			if(dis0 < dis1){
				current[i].second = 0;
				s0++;
			}
			else{
				current[i].second = 1;
				s1++;
			}
		}
		cout<<"LOOP "<<loop<<":\nstate0 = "<<s0<<",state1 = "<<s1<<endl;
		double para_a0 = 0.0, para_b0 = 0.0, para_c0 = 0.0, para_d0 = 0.0, denominator0 = 1.0;
		double para_a1 = 0.0, para_b1 = 0.0, para_c1 = 0.0, para_d1 = 0.0, denominator1 = 1.0;
		for(int i = 0; i < size; i++){
			if(!current[i].second){
				para_a0 += i * i;
				para_b0 += i;
				para_c0 += i * current[i].first;
				para_d0 += current[i].first;
			}else{
				para_a1 += i * i;
				para_b1 += i;
				para_c1 += i * current[i].first;
				para_d1 += current[i].first;
			}
		}
		denominator0 = para_a0 * s0 - para_b0 * para_b0;
		if(denominator0){
			k0 = (para_c0 * s0 - para_b0 * para_d0) / denominator0;
			b0 = (para_a0 * para_d0 - para_c0 * para_b0) / denominator0;
		} else{
			k0 = 1;
			b0 = 0;
		}
		denominator1 = para_a1 * s1 - para_b1 * para_b1;
		if(denominator1){
			k1 = (para_c1 * s1 - para_b1 * para_d1) / denominator1;
			b1 = (para_a1 * para_d1 - para_c1 * para_b1) / denominator1;
		} else{
			k1 = 1;
			b1 = 0;
		}
//		cout<<"k0 = "<<k0<<", b0 = "<<b0<<endl;
//		cout<<"k1 = "<<k1<<", b1 = "<<b1<<endl;
		loop++;
	}
	cout<<"loop end..."<<endl;
	
	int state_pair[2] = {0, 0};
	int p_slow = 0, p_fast = 0, state_bak = current[0].second;
	for(int i = 0; i < size + 1; i++){
		if(i == size){
			state_pair[state_bak] = p_fast - p_slow;
			state.push_back(make_pair(state_pair[0], state_pair[1]));
			break;
		}
		else if(current[i].second == state_bak){
			p_fast++;
			continue;
		} else{
			state_pair[state_bak] = p_fast - p_slow;
			p_slow = p_fast;
			state_bak = current[i].second;
			p_fast++;
		}
		if(state_pair[1]){
			state.push_back(make_pair(state_pair[0], state_pair[1]));
			state_pair[0] = 0;
			state_pair[1] = 0;
		}
	}
//	cout<<"state size: "<<state.size()<<endl;
}

void It::clear(){
	current.clear();
	state.clear();
}

void It::read(ifstream &fin){
	clear();
	int data_num = 0;
	double current_read;
	while(!fin.eof()){
		fin>>current_read;
		current.push_back(make_pair(current_read * 1e12, 0));
		data_num++;
	}
	if(data_num) current.pop_back();
	cout<<"Data num: "<<current.size()<<endl;
}

void It::write(string file_name){
	ofstream fout(file_name, ios_base::out);
	
	for(auto &s : state){
		fout<<s.first<<"\t"<<s.second<<endl;
	}
	
	fout.clear();
	fout.close();
}
