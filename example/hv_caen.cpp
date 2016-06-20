// this file is distributed under 
// MIT license
#include <iostream>
#include <string>
#include <fstream>
#include <libhv_global.h>
#include <HV/CAEN.h>
using namespace std;
using namespace Hardware;
int main(int argc,char**argv){
	string constr;
	{
		string filepath(argv[0]);
		filepath+=".txt";
		ifstream cfg;
		cfg.open(filepath);
		if(cfg){
			string token;
			while(cfg>>token)
				constr+=" "+token;
			cfg.close();
			cout<<constr<<endl;
		}
	}
	CAEN hv(constr);
	cout<<"========"<<endl;
	cout<<hv.ChannelCount()<<" channels"<<endl;
	for(size_t i=0;i<hv.ChannelCount();i++)
		cout<<hv[i]->getChannelName()<<endl;
	cout<<"========"<<endl;
}