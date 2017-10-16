#include<iostream>
#include<fstream>
#include <sstream>
#include <cmath>
#include <cstring>
#include <ctime>

using namespace std;


struct NodeStruct {
    double x = 0;
    double y = 0;
    double z = 0;
    double w = 0;
    int classes = 0;
    int count =0;
    int index =0;
};
const int TabuListSize = 7;
int TabuListState = 0;//判斷有沒有滿
double TabuList[TabuListSize];

const int Dimension = 4;//點維度
const int NodesNum = 150; // 城市数量
NodeStruct Nodes[NodesNum];

const int ClassesNum = 3;//群個數
NodeStruct ClassesNodes[TabuListSize][ClassesNum];
int ClassesNodeIndex[ClassesNum][NodesNum];


NodeStruct TotalEachElement[ClassesNum];
NodeStruct ClassesEachElementTotal[ClassesNum];
NodeStruct StandardDeviationClasses[ClassesNum];

void reafFile();
//---------------------------------------------------------------------------------------
void RandomAssignClusterCenter( int classes);
int get_RandonNum( int nodenum);
//---------------------------------------------------------------------------------------
void ClassifyNodesByDistance(int classes);
double cal_Distance(int i,int classes ,int j);
//---------------------------------------------------------------------------------------
double SEE(int classes);
double cal_SSE(double x1, double y1, double z1, double w1, double x2, double y2, double z2, double w2);
//---------------------------------------------------------------------------------------
void UpdateTabuList(double sse,int clesses);
//---------------------------------------------------------------------------------------
void cal_ClassElementCount();
void SecondAssignClusterCenter();
void InitalStructValue();
//---------------------------------------------------------------------------------------

void print_Nodes();
void print_Classes();
void print_TabuList();
//---------------------------------------------------------------------------------------


int main(){
    reafFile();

    time_t start,finish;
    start = clock();//開始計時

    srand((unsigned)time(NULL));
    RandomAssignClusterCenter(0);
    ClassifyNodesByDistance(0);
    double sse = SEE(0);
    UpdateTabuList(sse,0);

    print_Classes();
    print_TabuList();

    for(int i = 1;i < 100000 ;i++) {
        RandomAssignClusterCenter(i%7);
        ClassifyNodesByDistance(i%7);//再分類
        double sse = SEE(i%7);
        UpdateTabuList(sse,i%7);
    }
    finish = clock(); //结束

    print_Classes();
    print_TabuList();

    double duration = ((double)(finish-start))/CLOCKS_PER_SEC; //計算時間
    printf("time :%lfs.\n",duration);
    return 0;
}
//---------------------------------------------------------------------------------------
void RandomAssignClusterCenter(int classes){
    for(int i = 0;i < ClassesNum; i++){
        int centerNode = get_RandonNum(NodesNum);
        ClassesNodes[classes][i].classes = centerNode;
        ClassesNodes[classes][i].x = Nodes[centerNode].x;
        ClassesNodes[classes][i].y = Nodes[centerNode].y;
        ClassesNodes[classes][i].z = Nodes[centerNode].z;
        ClassesNodes[classes][i].w = Nodes[centerNode].w;
    }
}

int get_RandonNum(int Num) {
    double R = ((double)rand()) / (RAND_MAX + 1.0);
    int RandonNum  = (int)(Num * R);
    return RandonNum;
}
//--------------------------------------------------------------------------------------
void ClassifyNodesByDistance(int classes) {
    for (int i = 0; i < NodesNum; i++) {
        double min = 100000;
        for (int j = 0; j < ClassesNum; j++) {
            double mindDistance = cal_Distance(i,classes,j);
            if (mindDistance < min) {
                min = mindDistance;
                Nodes[i].classes = j;
            }
        }
    }
}


double cal_Distance(int i,int classes, int j) {
    double column_x =(Nodes[i].x-ClassesNodes[classes][j].x)*(Nodes[i].x-ClassesNodes[classes][j].x);
    double column_y =(Nodes[i].y-ClassesNodes[classes][j].y)*(Nodes[i].y-ClassesNodes[classes][j].y);
    double column_z =(Nodes[i].z-ClassesNodes[classes][j].z)*(Nodes[i].z-ClassesNodes[classes][j].z);
    double column_w =(Nodes[i].w-ClassesNodes[classes][j].w)*(Nodes[i].w-ClassesNodes[classes][j].w);
    double sum = column_x + column_y + column_z + column_w;
    return sqrt(sum);
}
//---------------------------------------------------------------------------------------
double SEE(int classes) {
    double sse = 0;
    for (int i = 0; i < ClassesNum; i++) {
        for (int j = 0; j < NodesNum; j++) {
            if (Nodes[j].classes== i) {
                sse = sse + cal_SSE(Nodes[j].x, Nodes[j].y, Nodes[j].z, Nodes[j].w,
                                    ClassesNodes[classes][i].x, ClassesNodes[classes][i].y,
                                    ClassesNodes[classes][i].z, ClassesNodes[classes][i].w);
            }
        }
    }
    return sse;
}

double cal_SSE(double x1, double y1, double z1, double w1, double x2, double y2, double z2, double w2) {
    return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2) + (w1 - w2)*(w1 - w2);
}
//---------------------------------------------------------------------------------------

void UpdateTabuList(double sse ,int classes) {
    if(TabuListState <7)//若tabulist未滿，哲直接放入
        TabuList[TabuListState] = sse;
    else {//若叫好更新tabulist,找最大替換
        int max = 0 ,max_index = 0;
        for(int i = 0 ; i < TabuListSize ;i++){
            if(TabuList[i] > max) {
                max = TabuList[i];
                max_index = i;//找出最大值之index
            }
        }
        if(sse < max) { //當sse 小於 max 才更新
            TabuList[max_index] = sse;//更新TabuList 的sse
            for (int i = 0; i < ClassesNum; i++) {//更新對應的classes 的value
                ClassesNodes[max_index][i].classes = ClassesNodes[classes][i].classes;
                ClassesNodes[max_index][i].x = ClassesNodes[classes][i].x;
                ClassesNodes[max_index][i].y = ClassesNodes[classes][i].y;
                ClassesNodes[max_index][i].z = ClassesNodes[classes][i].z;
                ClassesNodes[max_index][i].w = ClassesNodes[classes][i].w;
            }
        }
    }
    TabuListState++;
}

//---------------------------------------------------------------------------------------

//void cal_ClassElementCount(){
//    for(int i = 0 ;i < ClassesNum ; i++){
//        ClassesNodes[i].count = 0;
//    }
//    for(int i = 0 ;i < NodesNum ;i ++){
//        switch (Nodes[i].classes){
//            case 0:
//                ClassesNodes[0].count++;
//                break;
//            case 1:
//                ClassesNodes[1].count++;
//                break;
//            case 2:
//                ClassesNodes[2].count++;
//                break;
//        }
//    }
//}
//
//void SecondAssignClusterCenter() {
//    cal_ClassElementCount();
//    for(int i = 0; i < ClassesNum; i++){
//        printf("//%d\n",ClassesNodes[i].count);
//        int NodeIndex = get_RandonNum(ClassesNodes[i].count);
//        printf("--%d\n",NodeIndex);
//        for(int j = 0 ;j < NodesNum ; j++){
//            if(Nodes[j].count == NodeIndex && Nodes[j].classes == i){
//                printf("%d**%d\n",i,j);
//                ClassesNodes[i].classes = j;
//            }
//        }
//        ClassesNodes[i].x = Nodes[ClassesNodes[i].classes].x;
//        ClassesNodes[i].y = Nodes[ClassesNodes[i].classes].y;
//        ClassesNodes[i].z = Nodes[ClassesNodes[i].classes].z;
//        ClassesNodes[i].w = Nodes[ClassesNodes[i].classes].w;
//    }
//}
//---------------------------------------------------------------------------------------
void print_Nodes(){
    for(int i=0;i<NodesNum;i++)
        printf("%d\t%f\t%f\t%f\t%f\t%d\t%d\n",i,Nodes[i].x,Nodes[i].y,Nodes[i].z,Nodes[i].w,Nodes[i].classes,Nodes[i].count);
    printf("//---------------------------------------------------------------------------------------\n");
}
void print_Classes() {
    for(int i = 0 ;i < TabuListSize ;i++) {
        for (int j = 0; j < ClassesNum; j++)
            printf("%d\t%d\t%f\t%f\t%f\t%f\t%d\n",i, j, ClassesNodes[i][j].x, ClassesNodes[i][j].y, ClassesNodes[i][j].z,
                   ClassesNodes[i][j].w, ClassesNodes[i][j].classes);
    }
    printf("//---------------------------------------------------------------------------------------\n");
}

void print_TabuList() {
    printf("TabuList\n");
    for(int i = 0 ; i < TabuListSize ; i++){
        printf("%d\t%f\n", i, TabuList[i]);
    }
    printf("//---------------------------------------------------------------------------------------\n");
}
//---------------------------------------------------------------------------------------

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
    //print_Nodes();
    cout << "Read file done" << endl;
}
//---------------------------------------------------------------------------------------
