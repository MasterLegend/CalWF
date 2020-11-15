#include<iostream>
#include<cmath>
#include<iomanip>
#include<fstream>
#include<algorithm>
#include<valarray>
#include<vector>
#include<windows.h>
#include<stdio.h>
#include<direct.h>
#include<string.h>
using namespace std;

struct Izpoint{
	float z;
	float current;
};

#define LEN 1024

bool DirectoryList(LPCSTR Path){
	WIN32_FIND_DATA FindData;
	HANDLE hError;
	int FileCount = 0;
	char FilePathName[LEN];
	char FullPathName[LEN];
	strcpy(FilePathName, Path);
	strcat(FilePathName, "\\*.*");
	hError = FindFirstFile(FilePathName, &FindData);
	if(hError == INVALID_HANDLE_VALUE){
		printf("Search failed...\n");
		return 0;
	}
	while(::FindNextFile(hError, &FindData)){
		if(strcmp(FindData.cFileName,".") == 0||strcmp(FindData.cFileName,"..") == 0 ){
			continue;
		}
		wsprintf(FullPathName, "%s\\%s", Path,FindData.cFileName);
		FileCount++;
		printf("\n%d %s ", FileCount, FullPathName);
 
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			printf("<Dir>");
			DirectoryList(FullPathName);
		}
	}
	return 0;
}

double Fit_slope(float z_fit[5000],float current_ln_fit[5000],int data_point){
	double a=0.0,b=0.0,c=0.0,d=0.0;
	double slope=0.0,intercept=0.0,temp=0.0;
	
	valarray<float> data_x(z_fit,data_point);
	valarray<float> data_y(current_ln_fit,data_point);
	
	a=(data_x*data_x).sum();  
    b=data_x.sum();  
    c=(data_x*data_y).sum();  
    d=data_y.sum();
    if(temp=(a*data_point-b*b)){
    	slope=(c*data_point-b*d)/temp;
    	intercept=(a*d-c*b)/temp;
	} else{
		slope=1;
		intercept=0;
	}
	
	return slope;
}

double Fit_slope(double z_fit[10000], double current_ln_fit[10000], int data_point){
	double a=0.0,b=0.0,c=0.0,d=0.0;
	double slope=0.0,intercept=0.0,temp=0.0;
	
	valarray<double> data_x(z_fit,data_point);
	valarray<double> data_y(current_ln_fit,data_point);
	
	a=(data_x*data_x).sum();  
    b=data_x.sum();  
    c=(data_x*data_y).sum();  
    d=data_y.sum();
    if(temp=(a*data_point-b*b)){
    	slope=(c*data_point-b*d)/temp;
    	intercept=(a*d-c*b)/temp;
	} else{
		slope=1;
		intercept=0;
	}
	
	return slope;
}

int SortFile(LPCSTR Path){
	WIN32_FIND_DATA FindData;
	HANDLE hError;
	int FileCount=0;
	char FilePathName[LEN];
	char FullPathName[LEN];
	ifstream fin;
	ofstream fout;
	ofstream foutln;
	ofstream foutavg;
	ofstream fout_slope;
	string temp;
	int data_point_num=0; 
	float z[21][5000]={0.0};
	float current[21][5000]={0.0};
	float current_ln[21][5000]={0.0};
	float CurrAvg[5000]={0.0};
	float LnCurrAvg[5000]={0.0};
	float slope[21]={0.0},wf[21]={0.0};
	float wf_avg=0.0,wf_accum=0.0,wf_stdev=0.0;
	float z_fit[5000]={0.0},current_ln_fit[5000]={0.0};
	int fit_point_num=0; 
	
	strcpy(FilePathName, Path);
	strcat(FilePathName, "\\*.*");
	hError=FindFirstFile(FilePathName, &FindData);
	if(hError == INVALID_HANDLE_VALUE){
		cout<<"Can not find the file...\n"<<endl;
		return 0;
	}
	while(FindNextFile(hError, &FindData)){
		if(strcmp(FindData.cFileName,".") == 0||strcmp(FindData.cFileName,"..") == 0){
			continue;
		}
		wsprintf(FullPathName, "%s\\%s", Path,FindData.cFileName);
		
		if(FileCount>20){
			cout<<"Warning! The number of files exceeds the maximum value that the program can store(20 files)!\n";
			system("pause");
			exit(0);
		}
		fin.open(FullPathName);
		if(!fin.is_open()){
			cout<<"could not open the input file..."<<endl;
			exit(-1);
		}
		for(int i=0;i<575;i++){
			getline(fin,temp);
		}
		fin>>data_point_num;
		if(data_point_num>5000){
			cout<<"Warning! Data points exceed the maximum value that the program can store(5000 points)!"<<endl;
		}
		fin>>temp;
		fin>>temp;
		fin>>temp;
		for(int i=0;i<data_point_num;i++){
			fin>>temp;
			fin>>temp;
			fin>>z[FileCount][i];
			z[FileCount][i]=z[FileCount][i]*0.008;
			fin>>temp;
			fin>>current[FileCount][i];
			current_ln[FileCount][i]=log(fabs(current[FileCount][i]));
			fin>>temp;
		}
		
		FileCount++;
		fin.close();
	}
	
	FileCount--;
	
	fout.open("WF_Original.txt");
	for(int i=0;i<data_point_num;i++){	
		fout<<setiosflags(ios::fixed)<<setprecision(2);
		fout<<z[0][i]<<" ";
		fout<<setiosflags(ios::fixed)<<setprecision(3);
		for(int j=0;j<FileCount;j++){
			fout<<current[j][i]<<" ";
		}
		fout<<endl;
		fout<<resetiosflags(ios::fixed);
	}
	fout.clear();
	fout.close();
	
	fout_slope.open("WF_result.txt");
	fout_slope<<setiosflags(ios::fixed)<<setprecision(3);
	for(int i=0;i<FileCount;i++){
		long first_over=0,last_over=data_point_num;
		
		for(;current[i][first_over]<522000&&(first_over<data_point_num);first_over++){
			;
		}
		first_over--;
		for(;current[i][last_over]<522000&&(last_over>0);last_over--){
			;
		}
		last_over++;
		if(first_over<last_over){
			for(;first_over<=last_over;first_over++){
				z[i][first_over]=0.0;
			}
		}
		fit_point_num=0;
		for(int j=0;j<data_point_num;j++){
			if(fabs(z[i][j])>0.01){
				z_fit[fit_point_num]=z[i][j];
				current_ln_fit[fit_point_num]=current_ln[i][j];
				fit_point_num++;
			}
		}
		
		slope[i]=Fit_slope(z_fit,current_ln_fit,fit_point_num);
		wf[i]=slope[i]*slope[i]*0.952;
		fout_slope<<setw(2)<<i+1<<" ";
		fout_slope<<wf[i]<<endl;	
	}
	for(int i=0;i<FileCount;i++){
		wf_avg=wf_avg+wf[i];
	}
	wf_avg=wf_avg/FileCount;
	for(int i=0;i<FileCount;i++){
		wf_accum=wf_accum+(wf[i]-wf_avg)*(wf[i]-wf_avg);
	}
	wf_stdev=sqrt(wf_accum/FileCount);
	fout_slope<<endl<<"avg wf= "<<wf_avg<<endl<<"stdev= "<<wf_stdev<<endl;
	fout_slope<<resetiosflags(ios::fixed);
	fout_slope.clear();
	fout_slope.close();
	
	return 0;
}

int WF_mapping(LPCSTR Path){
	WIN32_FIND_DATA FindData;
	HANDLE hError;
	int FileCount=0,point_perline=0;
	char FilePathName[LEN];
	char FullPathName[LEN];
	ifstream fin;
	ofstream fout;
	string temp;
	int data_point_num=0;
	double z[10000]={0.0};
	double current[10000]={0.0};
	double current_ln[10000]={0.0};
	double z_fit[10000]={0.0},current_ln_fit[10000]={0.0};
	int fit_point_num=0;
	double slope=0.0,wf=0.0;
	
	strcpy(FilePathName, Path);
	strcat(FilePathName, "\\*.*");
	hError=FindFirstFile(FilePathName, &FindData);
	if(hError == INVALID_HANDLE_VALUE){
		cout<<"Can not find the file...\n"<<endl;
		return 0;
	}
	while(FindNextFile(hError, &FindData)){
		if(strcmp(FindData.cFileName,".") == 0 || strcmp(FindData.cFileName,"..") == 0){
			continue;
		}
		wsprintf(FullPathName, "%s\\%s", Path,FindData.cFileName);
		FileCount++;
	}
	FileCount--;
	
	if((int) sqrt(FileCount)-sqrt(FileCount)==0){
		point_perline=sqrt(FileCount);
	}else{
		point_perline=1;
	}
	
	hError=FindFirstFile(FilePathName, &FindData);
	while(FindNextFile(hError, &FindData)){
		if(strcmp(FindData.cFileName,".") == 0||strcmp(FindData.cFileName,"..") == 0 ){
			continue;
		}
		wsprintf(FullPathName, "%s\\%s", Path,FindData.cFileName);
		
		fin.open(FullPathName);
		if(!fin.is_open()){
			cout<<"could not open the input file..."<<endl;
			exit(-1);
		}
		for(int i=0;i<575;i++){
			getline(fin,temp);
		}
		fin>>data_point_num;
		if(data_point_num>10000){
			cout<<"Warning! Data points exceed the maximum value that the program can store(10000 points)!"<<endl;
		}
		fin>>temp;
		fin>>temp;
		fin>>temp;
		for(int i=0; i<data_point_num; i++){
			fin>>temp;
			fin>>temp;
			fin>>z[i];
			z[i]=z[i]*0.008;
			fin>>current[i];
			current_ln[i]=log(fabs(current[i]));
			fin>>temp;
			fin>>temp;
		}
		fin.close();
		
		fout.open("WF_mapping.txt",ios::app);
		fout<<setiosflags(ios::fixed)<<setprecision(3);
		
		long first_over=0,last_over=data_point_num;
		while(current[first_over]<522000&&(first_over<data_point_num)){
			first_over++;
		}
		first_over--;
		while(current[last_over]<522000&&(last_over>0)){
			last_over--;
		}
		last_over++;
		if(first_over<last_over){
			for(;first_over<=last_over;first_over++){
				z[first_over]=0.0;
			}
		}
		fit_point_num=0;
		for(int j=0;j<data_point_num;j++){
			if(fabs(z[j])>0.01){
				z_fit[fit_point_num]=z[j];
				current_ln_fit[fit_point_num]=current_ln[j];
				fit_point_num++;
			}
		}
		
		slope=Fit_slope(z_fit,current_ln_fit,fit_point_num);
		wf=slope*slope*0.952;
		fout<<wf;
		FileCount--;
		if(FileCount%point_perline==0) fout<<endl;
		else fout<<"\t";
		
		fout.clear();
		fout.close();
	}
	
	return 0;
}

void choose_mode(){
	char buffer[MAX_PATH];
	int check=1;
	int mode=-1;
	
	cout<<"Please choose the mode...\n";
	cout<<"Input '1': Calculate the Work Function of Single Point...\n";
	cout<<"Input '2': Calculate the Work Function Mapping...\n";
	cout<<"input '0': Exit...\n";
	cin>>mode;
	
	if(mode==1){
		check=SortFile(getcwd(buffer,MAX_PATH));
		if(check==0){
			cout<<"The work function is resolved and the programs ends normally...\n";
		}
		return;
	}else if(mode==2){
		check=WF_mapping(getcwd(buffer,MAX_PATH));
		if(check==0){
			cout<<"The work function is resolved and the programs ends normally...\n";
		}
	}else if(mode==0){
		exit(0);
	}else{
		cout<<"Error input, please input again...\n";
		choose_mode();
	}
}

int main(){
	cout<<"Welcome to CalWF!!!\n";
	cout<<"\n";
	choose_mode();
	
	system("pause");
	return 0;
}
