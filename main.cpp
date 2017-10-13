#include<iostream>
#include<fstream>
#include <sstream>
#include <cmath>
#include <cstring>
#include <ctime>

using namespace std;

const int Dimension = 4;//點維度
const int NodesNum = 150; // 城市数量
double Nodes[NodesNum][Dimension];
const int ClassesNum = 3;//群個數
double Classes[ClassesNum][Dimension+1];

void reafFile();

void print_Length();
void print_Nodes();

double cal_PathLength(int row);
double cal_InitalPathLength();
double cal_Distance(int node1, int node2);
void RandomAssignClusterCenter();
void RandonTwoNodeSwap();
int get_RandonNum();

void print_Classes();

void ClassifyNodes();

int main(){
    reafFile();

    time_t start,finish;
    start = clock();

    double T = 110;// 初始温度
    double T_END = 0.1;
    double Q = 0.9996;  // 退火系数

    srand((unsigned)time(NULL));
    RandomAssignClusterCenter();

    ClassifyNodes();

//    int f1,f2,df;
//    int count = 0;
//    double NewPathLength = 0;
//    while (T > T_END) {
//        for (int j = 0; j < NodesNum; j++) {
//            Nodes[j][3] = Nodes[j][0];
//        }
//        RandonTwoNodeSwap();
//
//        f1 = cal_PathLength(3);//Nodes[][3] old
//        f2 = cal_PathLength(0);//Nodes[][0] new
//        df = f2 - f1;
//        if (df < 0) {
//            T = T * Q;
//            for (int j = 0; j < NodesNum; j++) {
//                Nodes[j][3] = Nodes[j][0];
//            }
//        } else {
//            if (exp((double)-df / T) >= ((double)rand()/(RAND_MAX))) {
//                T = T * Q;
//                for (int j = 0; j < NodesNum; j++) {
//                    Nodes[j][3] = Nodes[j][0];
//                }
//            } else {
//                for (int j = 0; j < NodesNum; j++) {
//                    Nodes[j][0] = Nodes[j][3];
//                }
//            }
//        }
//        NewPathLength = cal_PathLength(3);
//        printf("C : %d\tP :%f\t T:%f\n",count,NewPathLength,T);
//        count++;
//    }
//    finish = clock(); // 退火过程结束
//    for(int i = 0 ;i < NodesNum ;i++){
//        printf("%d--->",Nodes[i][3]);
//    }
//    printf("\ncount:%d\t Path : %f\n",count,NewPathLength);
//    double duration = ((double)(finish-start))/CLOCKS_PER_SEC; // 计算时间
//    printf("time :%lfs.\n",duration);
    return 0;
}

void ClassifyNodes() {
    for (int i = 0; i < NodesNum; i++) {
        double min = 100000;
        for (int j = 0; j < ClassesNum; j++) {
            double mindDistance = cal_Distance(i,j);
            if (mindDistance < min) {
                min = mindDistance;
                Nodes[i][Dimension] = j;
            }
        }
    }
    print_Nodes();
}

void RandomAssignClusterCenter(){
    for(int i = 0;i < ClassesNum; i++){
        int centerNode = get_RandonNum();
        Classes[i][4] = centerNode;
        for (int j = 0; j < Dimension; j++)
            Classes[i][j] = Nodes[centerNode][j];
    }
    print_Classes();
}

int get_RandonNum() {
    double R = ((double)rand()) / (RAND_MAX + 1.0);
    int RandonNum  = (int)(NodesNum * R);
    return RandonNum;
}

double cal_Distance(int Nodes_index, int Classes_index) {
    int sum = 0;
    for(int i = 0 ;i <Dimension;i++){
        int Nodes_column = Nodes[Nodes_index][i];
        int Classes_column = Classes[Classes_index][i];
        int column =(Nodes_column-Classes_column)*(Nodes_column-Classes_column);
        sum += column;
    }
    //printf("%f\n",sqrt(sum));
    return sqrt(sum);
}

double cal_PathLength(int row) {
    double path = 0;
    for(int i= 0 ;i < NodesNum-1; i++){
        int node1 = Nodes[i][row];
        int node2 = Nodes[i+1][row];
        double dis = cal_Distance(node1, node2);
        path += dis;
    }
    path += cal_Distance(Nodes[NodesNum - 1][row], Nodes[0][row]);
    return path;
}

void print_Nodes(){
    for(int i=0;i<NodesNum;i++)
        printf("%d\t%f\t%f\t%f\t%f\t%f\n",i,Nodes[i][0],Nodes[i][1],Nodes[i][2],Nodes[i][3],Nodes[i][4]);
}
void print_Classes() {
    for(int i=0; i < ClassesNum; i++)
        printf("%d\t%f\t%f\t%f\t%f\tcenterNode : %0.f\n",i,Classes[i][0],Classes[i][1],Classes[i][2],Classes[i][3],Classes[i][4]);
}

void print_Length() {
    int NewPathLength = cal_PathLength(3);
    int InitalPathLength = cal_InitalPathLength();
    printf("%d\t%d\n",InitalPathLength,NewPathLength);
}
double cal_InitalPathLength() {
    int path = 0;
    for(int i= 0 ;i < NodesNum; i++)
        path += Nodes[i][4];
    return path;
}

void RandonTwoNodeSwap(){
    int index1 = get_RandonNum();
    int index2 = get_RandonNum();
    int temp = Nodes[index1][0];
    Nodes[index1][0] = Nodes[index2][0];
    Nodes[index2][0] = temp;
}


void reafFile() {
    fstream file;
    file.open("IrisData.txt", ios_base::in);
    char line[NodesNum];
    const char *d = ",";
    char *p;
    int i= 0;
    while(file.getline(line, sizeof(line),'\n')){
        p = strtok(line,d);
        int j=0;
        while(p) {
            stringstream strValue;
            strValue << p;
            double temp;
            strValue >> temp;
            Nodes[i][j] = temp;
            p = strtok(NULL,d);
            j++;
        }
        i++;
    }
    print_Nodes();
    cout << "Read file done" << endl;
}