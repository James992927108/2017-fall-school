#include<iostream>
#include<fstream>
#include <sstream>
#include <cmath>
#include <thread>
#include <cstring>
#include <unistd.h>

using namespace std;

const int dimension = 2;
const int totalNodes = 50; // 城市数量
int nodes[totalNodes][dimension+4];
int table[totalNodes][totalNodes];
void reafFile();
void print_Length();
void print_Nodes();

int cal_PathLength(int row);
int cal_InitalPathLength();
double cal_Distance(int node1, int node2);
void IntitalPathNearsetNeighbor();
void RandonTwoNodeSwap();
int get_RandonNum();

int counttableusestate = 0;
int main(){
    reafFile();
    time_t start,finish;
    start = clock();
    double T = 1000;// 初始温度
    double T_END = 0;
    double Q = 1;  // 退火系数
    int L = 100;//單次溫度迭代
    double R = 0.9999999;//接受率
    srand((unsigned)time(NULL));
    IntitalPathNearsetNeighbor();
    int f1,f2,df;
    int count = 0;
    while(T > T_END) {
        for (int i = 0; i < L; i++) {
            for (int j = 0; j < totalNodes; j++) {
                nodes[j][3] = nodes[j][0];
            }
            RandonTwoNodeSwap();

            f1 = cal_PathLength(3);//nodes[][3] old
            f2 = cal_PathLength(0);//nodes[][0] new
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
        if(counttableusestate == totalNodes*totalNodes-1){
            break;
        }
        T -= Q;
        count++;
        printf("%d new path :\t",count);
        print_Length();
    }
    finish = clock(); // 退火过程结束
    for(int i = 0 ;i < totalNodes ;i++){
        printf("%d--->",nodes[i][3]);
    }

    printf("\ncount:%d\n",count);
    double duration = ((double)(finish-start))/CLOCKS_PER_SEC; // 计算时间
    printf("time :%lfs.\n",duration);
    printf("counttableusestate:%d\n",counttableusestate);
    return 0;
}
void IntitalPathNearsetNeighbor(){
    //選任一點為起始節點
    int index  = get_RandonNum();
    //放入新路徑
    nodes[0][0] = index;
    //找到距離最近加入路徑最近的點，將其加入路徑
    double distance = 1000;
    for(int i = 0 ;i < 50 ; i++) {
        double j_mindistance = 1000;
        for (int j = 0; j < totalNodes; j++) {
            if (j == index || j == i || j == nodes[i][0]) //j == index : 取一次取點取到原點
                distance = 1000;                         //j == i : 取道同一點
            else                                          //j == nodes[i][0]判斷是否為以取過的職
                distance = cal_Distance(nodes[i][0], j);
            if (distance < j_mindistance && nodes[j][5] == 0) {
                j_mindistance = distance;
                nodes[i+1][0] = j;//紀錄是哪個點距離nodes[i][0]最近
                distance += 0.5;
                nodes[i+1][4] = (int)distance;//記下初始路徑依序兩點距離，方便計算總初始距離
            }
        }
        nodes[nodes[i+1][0]][5] = 1;//作為標籤避免重複找
    }
    for(int i = 0; i < totalNodes ;i++){//計算最後一個點與遠點的距離
        if(nodes[i][5]==0){
            nodes[0][4] = (int) cal_Distance(nodes[i][0], nodes[0][0]);
            nodes[i][5] = 1 ;
        }
    }
    //print_Nodes();
    printf("inital path : \t");
    print_Length();
}

int cal_InitalPathLength() {
    int path = 0;
    for(int i= 0 ;i < totalNodes; i++)
        path += nodes[i][4];
    return path;
}

void RandonTwoNodeSwap(){
    int index1 = get_RandonNum();
    int index2 = get_RandonNum();
    //printf("%d\n",table[index1][index2]);
    if(table[index1][index2] == 0){
        int temp = nodes[index1][0];
        nodes[index1][0] = nodes[index2][0];
        nodes[index2][0] = temp;
        table[index1][index2] = 1;//使用過記錄到table
        counttableusestate++;
    }
}

int get_RandonNum() {
    double R = ((double)rand()) / (RAND_MAX + 1.0);
    int RandonNum  = (int)(totalNodes * R);
    //printf("%d\n",RandonNum);
    return RandonNum;
}

double cal_Distance(int node1, int node2) {
    int x1 = nodes[node1][1];
    int y1 = nodes[node1][2];
    int x2 = nodes[node2][1];
    int y2 = nodes[node2][2];
    int x = (x1-x2)*(x1-x2);
    int y = (y1-y2)*(y1-y2);
    return sqrt(x+y);
}

int cal_PathLength(int row) {
    double path = 0;
    for(int i= 0 ;i < totalNodes-1; i++){
        int node1 = nodes[i][row];
        int node2 = nodes[i+1][row];
        double dis = cal_Distance(node1, node2) + 0.5;
        path += (int)dis;
    }
    path += cal_Distance(nodes[totalNodes - 1][row], nodes[0][row]);
    return path;
}

void print_Nodes(){
    for(int i=0;i<50;i++)
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",i,nodes[i][0],nodes[i][1],nodes[i][2],nodes[i][3],nodes[i][4],nodes[i][5]);
}
void print_Length() {
    int NewPathLength = cal_PathLength(3);
    int InitalPathLength = cal_InitalPathLength();
    printf("%d\t%d\n",InitalPathLength,NewPathLength);
}
void reafFile() {
    fstream file;
    file.open("eil51.txt", ios_base::in);
    char line[totalNodes];
    const char *d = " ";
    char *p;
    int i= 0;
    while(file.getline(line, sizeof(line),'\n')){
        p = strtok(line,d);
        int j=0;
        while(p) {
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
    //print_Nodes();
    cout << "Read file done" << endl;
}
