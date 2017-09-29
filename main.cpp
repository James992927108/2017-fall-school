#include<iostream>
#include<fstream>
#define SIZE 100
using namespace std;

char line[SIZE];

int main(){
    fstream fin;
    fin.open("eil51.txt",ios::in);
    while(fin.getline(line,sizeof(line),'\n')){
        cout<<line<<endl;
    }
    system("pause");
    return 0;
}