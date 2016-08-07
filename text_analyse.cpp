#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

vector <pair <char, int> > symbols;

bool cmp(pair<char,int> a,pair<char,int> b){
	return b.second < a.second;
}

void analyse(char* s){
	FILE* fp = fopen(s,"r");
	char c;
	int set=0;
	vector <pair <char,int> >::iterator itr;
	while((c=fgetc(fp))!=EOF){
		set = 0;
		for(itr=symbols.begin();itr!=symbols.end();itr++){
			if((itr->first)==c){
				(itr->second)++;
				set=1;
				break;
			}
		}
		if(set==1) continue;
		symbols.push_back(make_pair(c,1));
		set = 0;
	}
	symbols.push_back(make_pair('$',1));
}

void print_prob_order(char* s){
	ofstream fout;
	fout.open(s);
	fout<<symbols.size()<<endl;
	sort(symbols.begin(),symbols.end(),cmp);
	vector <pair <char,int> >::iterator itr;
	for(itr=symbols.begin();itr!=symbols.end();itr++){
		if(itr->first == ' ')
			fout<<'_'<<endl;
		else if(itr->first == '\n')
			fout<<'#'<<endl;
		else
			fout<< itr->first<<endl;
	}
	fout.close();
}

void usage(char* s){
	printf("%s ip_filename op_file\n",s);
}

int main(int argc,char* argv[]){
	if(argc!=3){usage(argv[0]);return -1;}
	analyse(argv[1]);
	print_prob_order(argv[2]);
	return 0;
}