#include<iostream>
#include<fstream>
#include <sstream>
#include <cmath>
#include <thread>
#include <cstring>

using namespace std;

const int dimension = 2;
const int totalNodes = 50; // 城市数量
int nodes[totalNodes][dimension+4];

void printnode();

void ReadFile1();

int PathLength(int row);
int PathLength_row4();

double Caldistance(int node1,int node2);

void Intital_Path();
void Nearest_Neighbor();

int main(){
    time_t start,finish;
    int count = 0;
    ReadFile1();

    start = clock();
    double T = 100;// 初始温度
    double T_END = 0.01;
    double Q = 0.99;  // 退火系数
    int L = 1000;//單次溫度迭代
    double R = 0.9;//接受率

    Intital_Path();
    int f1,f2,df;
    while(T > T_END) {
        for (int i = 0; i < L; i++) {
            for (int j = 0; j < totalNodes; j++) {
                nodes[j][3] = nodes[j][0];
            }
            Nearest_Neighbor();
            f1 = PathLength(3);//nodes[][3] old
            f2 = PathLength(0);//nodes[][0] new
            df = f2 - f1;
            if (df < 0) {
                for (int j = 0; j < totalNodes; j++) {
                    nodes[j][3] = nodes[j][0];
                }
            } else {
                if (exp(-df / T) >= R) {
                    for (int j = 0; j < totalNodes; j++) {
                        nodes[j][3] = nodes[j][0];
                    }
                } else {
                    for (int j = 0; j < totalNodes; j++) {
                        nodes[j][0] = nodes[j][3];
                    }
                }
            }
        }
        T *= Q;
        count++;
    }
    finish = clock(); // 退火过程结束
    double duration = ((double)(finish-start))/CLOCKS_PER_SEC; // 计算时间
    int len = PathLength(3);
    printf("count:%ld\n",count);
    printf("path length:%ld\n",len);
    printf("time :%lfs.\n",duration);
    return 0;
}
void Intital_Path(){
    srand((unsigned)time(NULL));
    //選任一點為起始節點 i= 0
    double r1 = ((double)rand()) / (RAND_MAX + 1.0);
    int index  = (int)(totalNodes * r1);

    int temp = nodes[0][0];
    nodes[0][0] = nodes[index][0];
    nodes[index][0] = temp;

    // i 從 1 開始 ，產生 0 ~ 49
    for (int i=1; i<totalNodes; i++) {
        nodes[i][0] = rand() % totalNodes;
        for (int j = 0; j < i; j++) {
            if (nodes[i][0] == nodes[j][0]) {
                i--;
                break;
            }
        }
    }
    //printnode();
    int Intital_Path = PathLength(0);
    printf("Intital_Path : %d\n",Intital_Path);
}

void Nearest_Neighbor() {
    //選任一點為起始節點
    double r1 = ((double)rand()) / (RAND_MAX + 1.0);
    int index  = (int)(totalNodes * r1);
    //放入新路徑
    nodes[0][3] = index;
    //找到距離最新加入路徑最近的點，將其加入路徑
    double distance = 1000;
    for(int i = 0 ;i < 50 ; i++) {
        double j_mindistance = 1000;
        for (int j = 0; j < totalNodes; j++) {
            if (j == index || j == i ||nodes[i][3] == j)
                distance = 1000;
            else
                distance = Caldistance(nodes[i][3], j);
            if (distance < j_mindistance && nodes[j][5] == 0) {
                j_mindistance = distance;
                nodes[i+1][3] = j;
                distance += 0.5;
                nodes[i+1][4] = (int)distance;
            }
        }
        nodes[nodes[i+1][3]][5] = 1;
    }
    for(int i = 0; i < totalNodes ;i++){
        if(nodes[i][5]==0){
            nodes[0][4] = (int)Caldistance(nodes[totalNodes-1][3], nodes[0][3]);
            nodes[i][5] = 1 ;
        }
    }
    //printnode();
    int Nearest_Neighbor = PathLength(3);
    int Nearest_Neighbor1 = PathLength_row4();
    printf("Nearest_Neighbor : %d\t%d\n",Nearest_Neighbor,Nearest_Neighbor1);
}

double Caldistance(int node1,int node2)
{
    int x1 = nodes[node1][1];
    int y1 = nodes[node1][2];
    int x2 = nodes[node2][1];
    int y2 = nodes[node2][2];
    int x = (x1-x2)*(x1-x2);
    int y = (y1-y2)*(y1-y2);
    double dis = sqrt(x+y);
    //printf("--%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t\n",node1,node2,x1,y1,x2,y2,x,y,dis);
    return dis;
}

int PathLength_row4()
{
    int path = 0;
    for(int i= 0 ;i < totalNodes; i++)
        path += nodes[i][4];
    return path;
}
int PathLength(int row)
{
    double path = 0;
    for(int i= 0 ;i < totalNodes-1; i++){
        int node1 = nodes[i][row];
        int node2 = nodes[i+1][row];
        double dis = Caldistance(node1,node2) + 0.5;
        path += (int)dis;
    }
    path += Caldistance(nodes[totalNodes-1][row],nodes[0][row]);
    return path;
}

void printnode(){
    for(int i=0;i<50;i++)
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",i,nodes[i][0],nodes[i][1],nodes[i][2],nodes[i][3],nodes[i][4],nodes[i][5]);
}

void ReadFile1() {
    fstream file;
    file.open("eil51.txt", ios_base::in);
    char line[totalNodes];
    const char *d = " ";
    char *p;
    int i= 0;
    while(file.getline(line, sizeof(line),'\n')){
        p = strtok(line,d);
        int j=0;
        while(p)
        {
            stringstream strValue;
            strValue << p;
            unsigned int temp;
            strValue >> temp;
            nodes[i][j] = temp;
            p = strtok(NULL,d);
            j++;
        }
        i++;
    }
    //printnode();
    cout << "Read file done" << endl;
}