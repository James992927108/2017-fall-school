#include<iostream>
#include<fstream>
#include <sstream>
#include <cmath>
#include <thread>

using namespace std;


const int dimension = 2;
const int totalNodes = 50; // 城市数量
int nodes[totalNodes][dimension+2];

const int T0 = 1000;// 初始温度
const int T_END = 0;
const double q = 0.98;  // 退火系数
const int L = 100;  // 每个温度时的迭代次数，即链长


void ReadFile();
void create_new();
double path_len(int row);  // 计算路径长度
double distance(int node1,int node2);
int main(){
    srand((unsigned)time(NULL)); //初始化随机数种子
    time_t start,finish;
    start = clock(); // 程序运行开始计时
    double T;
    int count = 0; // 记录降温次数
    T = T0; //初始温度
    ReadFile();
    cout << "start simualate" << endl;
    double f1,f2,df; //f1为初始解目标函数值，f2为新解目标函数值，df为二者差值
    double r = 0.8; // 0-1之间的随机数，用来决定是否接受新解
    while(T > T_END){
        for(int i = 0;i < L ;i++) {
            for(int j = 0 ;j < totalNodes ;j++) {
                nodes[j][3] = nodes[j][0];
            }
            create_new();
            f1 = path_len(3);//nodes[][3]
            f2 = path_len(0);//nodes[][0]
            df = f2 - f1;
            // 以下是Metropolis准则
            if(df >= 0)
            {
                if(exp(-df/T) <= r) // 保留原来的解
                {
                    for(int j = 0 ;j < totalNodes ;j++) {
                        nodes[j][3] = nodes[j][0];
                    }
                }
            }
        }
        T *= q; // 降温
        count++;
    }
    finish = clock(); // 退火过程结束

    double duration = ((double)(finish-start))/CLOCKS_PER_SEC; // 计算时间
    printf("采用模拟退火算法，初始温度T0=%.2d,降温系数q=%.2f,每个温度迭代%d次,共降温%d次，得到的TSP最优路径为:\n",T0,q,L,count);
    for(int i=0;i<totalNodes;i++)  // 输出最优路径
    {
        printf("%d_%d\n",i+1,nodes[i][0]);
    }
    printf("%d\n",nodes[totalNodes-1]);
    double len = path_len(0); // 最优路径长度
    printf("最优路径长度为:%lf\n",len);
    printf("程序运行耗时:%lf秒.\n",duration);
    return 0;
}

double distance(int node1,int node2)
{
    double x1 = nodes[node1][1];
    double y1 = nodes[node1][2];
    double x2 = nodes[node2][1];
    double y2 = nodes[node2][2];
    double dis = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    //printf("dis__%d\n",dis);
    return dis;
}
// 计算路径长度
double path_len(int row)
{
    int path = 0; // 初始化路径长度
    for(int i= 0 ;i < totalNodes; i++){
        int index1 = nodes[i][row];
        int index2 = nodes[i+1][row];
        //printf("%d\t%d\n",index1,index2);
        int dis = distance(index1,index2);
        path += dis;
    }
    //printf("path__%d\n",path);
    return path; // 返回总的路径长度
}

// 产生一个新解
// 此处采用随机交叉两个位置的方式产生新的解
void create_new()
{
    double r1 = ((double)rand())/(RAND_MAX+1.0);
    double r2 = ((double)rand())/(RAND_MAX+1.0);
    int pos1 = (int)(totalNodes*r1); //第一个交叉点的位置
    int pos2 = (int)(totalNodes*r2);
    int temp = nodes[pos1][0];
    nodes[pos1][0] = nodes[pos2][0];
    nodes[pos2][0] = temp;   // 交换两个点
}


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
    /*for(int i=0;i<50;i++)
        printf("%d\t%d\t%d\t%d\n",nodes[i][0],nodes[i][1],nodes[i][2],nodes[i][3]);*/
    cout << "Read file done" << endl;
}