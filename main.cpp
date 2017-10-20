#include<iostream>
#include<fstream>
#include <sstream>
#include <cmath>
#include <thread>
#include <cstring>
#include <unistd.h>

using namespace std;

struct Node{
    int index = 0;
    double x = 0;
    double y = 0;
    double z = 0;
    double w = 0;
};
struct Chromosome{
    Node center1;
    Node center2;
    Node center3;
};

Chromosome Chromosomelist[10];

//有n條染色體chromosome
//每一條染色體有三個中心點
//每個中心點 有 四維 資料（struct Node)
const int Dimension = 4;//點維度
const int NodesNum = 150; // 城市数量
Node Nodes[NodesNum];

void reafFile();
void print_Nodes();

int main(){
    reafFile();
    time_t start,finish;
    start = clock();
    //------------------
    finish = clock(); //结束
    double duration = ((double)(finish-start))/CLOCKS_PER_SEC; // 计算时间
    printf("time :%lfs.\n",duration);
    return 0;
}

void print_Nodes(){
    for(int i=0;i<NodesNum;i++)
        printf("%d\t%.2f\t%.2f\t%.2f\t%.2f\n",Nodes[i].index,Nodes[i].x,Nodes[i].y,Nodes[i].z,Nodes[i].w);
    printf("//---------------------------------------------------------------------------------------\n");
}

void reafFile() {
    fstream file;
    file.open("IrisData.txt", ios_base::in);
    char line[NodesNum];
    const char *d = ",";
    char *p;
    int i= 0;
    while(file.getline(line, sizeof(line),'\n')){
        Nodes[i].index = i;
        p = strtok(line,d);
        int j=0;
        while(p) {
            stringstream strValue;
            strValue << p;
            double temp;
            strValue >> temp;
            switch(j){
                case 0:
                    Nodes[i].x = temp;
                    break;
                case 1:
                    Nodes[i].y = temp;
                    break;
                case 2:
                    Nodes[i].z = temp;
                    break;
                case 3:
                    Nodes[i].w = temp;
                    break;
            }
            p = strtok(NULL,d);
            j++;
        }
        i++;
    }
    print_Nodes();
    cout << "Read file done" << endl;
}
