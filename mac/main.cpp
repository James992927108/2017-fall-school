#include<iostream>
#include<fstream>
#include <sstream>
#include <cmath>
#include <thread>

using namespace std;


const int dimension = 2;
const int totalNodes = 50; // 城市数量
int nodes[totalNodes][dimension+3];

void printnode();
void ReadFile();

int PathLength(int row);
int Caldistance(int node1,int node2);

void Intital_Path();
void Nearest_Neighbor();

void create_new();
void TwoOptSwap(int i, int k);

int main(){
    time_t start,finish;
    int count = 0;

    ReadFile();

    start = clock();

    ReadFile();
    double T = 1000;// 初始温度
    double T_END = 0.001;
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
    int len = PathLength(0);
    printf("count:%ld\n",count);
    printf("path length:%ld\n",len);
    printf("time :%lfs.\n",duration);
    /*srand((unsigned)time(NULL));
    time_t start,finish;
    int count = 0;
    ReadFile();
    start = clock();
    int f1,f2,df;
    while(T > T_END){
        for(int j = 0 ;j < totalNodes ;j++) {
            nodes[j][3] = nodes[j][0];
        }
        f1 = PathLength(3);//nodes[][3]
        create_new();
        f2 = PathLength(0);//nodes[][0]
        df = f2 - f1;
        *//*if(df < 0) {
            for(int j = 0 ;j < totalNodes ;j++) {
                nodes[j][3] = nodes[j][0];
            }
        }else{
            if(exp(-df/T) >= r){
                for(int j = 0 ;j < totalNodes ;j++) {
                    nodes[j][3] = nodes[j][0];
                }
            }else{
                for(int j = 0 ;j < totalNodes ;j++) {
                    nodes[j][0] = nodes[j][3];
                }
            }
        }*//*
        if(df >= 0)
        {
            if(exp(-df/T) <= Ｒ) // 保留原来的解
            {
                for(int j = 0 ;j < totalNodes ;j++) {
                    nodes[j][0] = nodes[j][3];
                }
            }
        }
        //printf("%d\n",count);
        T *= Ｑ;
        count++;
    }
    finish = clock(); // 退火过程结束

    double duration = ((double)(finish-start))/CLOCKS_PER_SEC; // 计算时间
    *//*for(int i=0;i<totalNodes;i++)
    {
        printf("%d_%d\n",i+1,nodes[i][0]);
    }*//*
    int len = PathLength(0);
    printf("count:%ld\n",count);
    printf("path length:%ld\n",len);
    printf("time :%lfs.\n",duration);*/
    return 0;
}
void Intital_Path(){
    for (int i=1; i<50; i++) {
        nodes[i][0] = rand() % 50 + 1;
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
    srand((unsigned)time(NULL));
    nodes[0][0] = rand()%50+1;

    //找到距離最新加入路徑最近的點，將其加入路徑

    int distance = 0;
    int i_mindistance = 1000;
    for(int i = 0;i < totalNodes; i++){
        int j_mindistance = 1000;
        for(int j = 0;j < totalNodes;j++) {
            int node1 = nodes[i][0];
            int node2 = nodes[j][0];
            if( i == 0 && j ==0)
                distance = 1000;
            if(j!=i)
                distance = Caldistance(node1, node2);
            if(distance < j_mindistance){
                nodes[i][3] = j;
                nodes[i][4] = distance;
                j_mindistance = distance;
                printf("%d_%d_j_mindistance_%d\n",i,j,j_mindistance);
                i_mindistance = j_mindistance;
            }
        }
        printf("%d_mindistance_%d\n",i,i_mindistance);
    }
    //printnode();
    int Nearest_Neighbor = PathLength(4);
    printf("Nearest_Neighbor : %d\n",Nearest_Neighbor);
}

int Caldistance(int node1,int node2)
{
    double x1 = nodes[node1][1];
    double y1 = nodes[node1][2];
    double x2 = nodes[node2][1];
    double y2 = nodes[node2][2];
    int dis = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    return dis;
}

int PathLength(int row)
{
    int path = 0;
    for(int i= 0 ;i < totalNodes; i++){
        path += nodes[i][row];
    }
    path += Caldistance(nodes[0][row],nodes[totalNodes][row]);
    return path;
}
/*int PathLength(int row)
{
    int path = 0;
    for(int i= 0 ;i < totalNodes; i++){
        int node1 = nodes[i][row];
        int node2 = nodes[i+1][row];
        int dis = Caldistance(node1,node2);
        path += dis;
    }
    path += Caldistance(nodes[0][row],nodes[totalNodes][row]);
    return path;
}*/

void ReadFile() {
    fstream file;
    file.open("eil51.txt", ios_base::in);
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
            p = strtok(NULL,d);
            j++;
        }
        i++;
    }
    printnode();
    cout << "Read file done" << endl;
}

void printnode(){
    for(int i=0;i<50;i++)
        printf("%d\t%d\t%d\t%d\n",nodes[i][0],nodes[i][1],nodes[i][2],nodes[i][3]);
}

void TwoOptSwap(int i, int k ) {
    // 1. take route[0] to route[i-1] and add them in order to new_route
    for ( int c = 0; c <= i - 1; c++ ) {
        nodes[c][3] = nodes[c][0];
    }
    // 2. take route[i] to route[k] and add them in reverse order to new_route
    int dec = 0;
    for ( int c = i; c <= k; c++ ) {
        nodes[c][3] = nodes[k-dec][0];
        dec++;
    }
    // 3. take route[k+1] to end and add them in order to new_route
    for ( int c = k + 1; c < totalNodes; c++ ) {
        nodes[c][3] = nodes[c][0];
    }
    for(int i= 0 ;i < totalNodes ; i++)
    {
        nodes[i][0] = nodes[i][3];
    }
}

void create_new() {
    /*int pos[10];
    for(int i = 0; i < 10;i++)
    {
        double r = ((double)rand())/(RAND_MAX+1.0);
        pos[i] = (int)(totalNodes*r);
    }
    for(int i = 0; i < 9 ;i++) {
        int temp = nodes[pos[i]][0];
        nodes[pos[i]][0] = nodes[pos[i + 1]][0];
        nodes[pos[i + 1]][0] = temp;
    }*/
    double r1 = ((double)rand())/(RAND_MAX+1.0);
    int i = (int)(totalNodes * r1);
    int k = 0;
    while(k < i)
    {
        double r2 = ((double)rand())/(RAND_MAX+1.0);
        k = (int)(totalNodes * r2);
    }
    printf("r1 = %d r2 = %d\n",i,k);
    for(int i= 0 ;i < totalNodes ; i++) {
        printf("%i--%d\n",i,nodes[i][0]);
    }
    TwoOptSwap( i, k );
    printf("after\n");
    for(int i= 0 ;i < totalNodes ; i++) {
        printf("%i--%d\n",i,nodes[i][0]);
    }
}
