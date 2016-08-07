#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

class node{
public:
	char value;
	node* left;
	node* right;
	node* parent;
	node(char v,node* l=NULL,node* r=NULL,node* p=NULL){
		value = v;
		left  = l;
		right = r;
		parent= p;
	}
};

class huffman_tree{
	char * prob_order;
	int prob_order_size;
	
public:
	node* root;
	huffman_tree(char* s){
		ifstream fin;
		fin.open(s);
		char c;
		int index=0;
		fin>>prob_order_size;
		prob_order = (char*) malloc(sizeof(char)*(prob_order_size+1));
		while(fin>>c && index<prob_order_size){
			if(c!='_' && c!='#')
				prob_order[index++]=c;
			else if(c=='#')
				prob_order[index++]='\n';
			else if(c=='_')
				prob_order[index++]=' ';
		}
		fin.close();
		root = create_huffman_tree(0,prob_order_size-1);
	}
	node* create_huffman_tree(int i,int j){
		if(i==j){
			node* T = new node(prob_order[i]);
			return T;
		}
		node* A = new node(prob_order[i]);
		node* B = create_huffman_tree(i+1,j);
		node* r = new node('*',A,B,NULL);
		A->parent = r;
		B->parent = r;
		return r;
	}
	void print_huffmann_tree(node* r){
		if(r->left == NULL){cout<<r->value<<endl;return;}
		cout<<(r->left)->value<<endl;
		print_huffmann_tree(r->right);
	}

	void compress(char* s){
		FILE* fp = fopen(s,"r");
		char c;
		char str[50];
		char* string;
		int byte[8];
		int packed = 0;
		while((c=fgetc(fp))!=EOF){
			strcpy(str,"");
			string = code(root,c,str);
			for(int i=0;i<strlen(string);i++){
				if(packed==8){
					print_byte(byte);
					packed=0;
				}
				byte[packed++]=string[i];
			}
		}
		strcpy(str,"");
		c = '$';
		string = code(root,c,str);
		for(int i=0;i<strlen(string);i++){
			if(packed==8){
				print_byte(byte);
				packed=0;
			}
			byte[packed++]=string[i];
		}
		if(packed!=8 && packed!=0){
			while(packed!=8){
				byte[packed++]='1';
			}
			packed=0;
			print_byte(byte);
		}	
		fclose(fp);
	}

	char* code(node* r,char c,char* s){
		if((r->left)==NULL){
			if((r->value) == c){ strcat(s,"1");return s;}
			else {
				cout<<"Some error occurred!! "<<endl;
				return s;
			}
		}
		if((r->left)->value == c) {strcat(s,"0"); return s;}
		else{
			strcat(s,"1");
			return code(r->right,c,s);
		}
	}

	void print_byte(int* byte){
		unsigned char c=0;
		for(int i=0;i<8;i++){
			if(byte[i]=='1'){
				c+=pow(2,(7-i));
			}
		}	
		cout<<c;
	}

	void decompress(char* file){
		FILE* fp = fopen(file,"r");
		unsigned char c;
		int byte[8];
		node* curr = root;
		while((c=fgetc(fp))!=EOF){
			for(int i=7;i>=0;i--){
				byte[i]=c%2;
				c/=2;
			}
			for(int i=0;i<8;i++){
				//if(curr->value!='*') {curr = root;if(curr->value=='$') break; else cout<<curr->value;}
				if(byte[i] == 1) curr = curr->right;
				else if(byte[i] == 0) curr = curr->left;
				if(curr->value!='*') {
					if(curr->value=='$'){
						fclose(fp);
						return;
					}
					else cout<<curr->value;
					curr = root;
				}
			}
		}
		fclose(fp);
	}
};


void usage(char* s){
	printf("For compression   : %s -c filename\n",s);
	printf("For decompression : %s -d filename\n",s);
	printf("Special option of altered prob : %s -c/-d filename prob_order\n",s);
}

int main(int argc,char* argv[]){
	if(argc!=3 && argc!=4){
		usage(argv[0]);
		return -1;
	}
	char z[100]="";
	if(argc==4){
		strcpy(z,argv[3]);
	}
	else{
		strcpy(z,"default_symbol");
	}
	
	huffman_tree H(z);
	if(strcmp(argv[1],"-c")==0)
		H.compress(argv[2]);
	else if(strcmp(argv[1],"-d")==0)
		H.decompress(argv[2]);
	else{
		cout<<"Wrong option!! \n Please refer to ReadMe for further details\n";
	}
	return 0;
}