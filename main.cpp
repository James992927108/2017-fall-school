#include<iostream>
#include<fstream>
#include <sstream>
#include <cstring>

using namespace std;
int main(){

    fstream file;
    file.open("eil51.txt",ios::in);

    const int dimension = 2;
    const int totalNodes = 50;

    int nodes[totalNodes][dimension+1];
    char line[0];

    const char *d = " ";
    char *p;

    int i= 0;
    while(file.getline(line,totalNodes,'\n')){
        p = strtok(line,d);
        int j=0;
        while(p)
        {
            stringstream strValue;
            strValue << p;
            unsigned int temp;
            strValue >> temp;

            nodes[i][j] = temp;
            cout << "nodes[" << i << "]["<< j <<"]= ";
            cout << nodes[i][j] << endl;
            p = strtok(NULL,d);
            j++;
        }
        cout << "+++" << endl;
        i++;
    }

    system("pause");
    return 0;
}