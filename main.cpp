#include<iostream>
#include<fstream>
#include <sstream>

using namespace std;


const int dimension = 2;
const int totalNodes = 50;
int nodes[totalNodes][dimension+2];
int start_node_num;

void ReadFile();

void SelectStartNode();

void anneal(){
    ReadFile();
    SelectStartNode();
}

void SelectStartNode() {
    srand((unsigned)time(0));
    start_node_num = (rand()%50)+1;
    nodes[start_node_num][3] = 0;
    cout << "Start node = "<<nodes[start_node_num][0]<<":[" <<nodes[start_node_num][1]<<"]["<<nodes[start_node_num][2]<<"]" << "\n";
}

int main(){
    anneal();
    system("pause");
    return 0;
}

void ReadFile() {
    fstream file;
    file.open("eil51.txt", ios_base::in);
    char line[0];
    const char *d = " ";
    char *p;
    int i= 1;
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
           /* cout << "nodes[" << i << "]["<< j <<"]= ";
            cout << nodes[i][j] << endl;*/
            p = strtok(NULL,d);
            j++;
        }
        /*cout << "+++" << endl;*/
        i++;
    }
    cout << "Read file done" << endl;
}