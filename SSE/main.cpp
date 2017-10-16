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
const int Dimension = 4;//點維度
const int NodesNum = 150; // 城市数量
NodeStruct Nodes[NodesNum];
const int ClassesNum = 3;//群個數
NodeStruct ClassesNodes[ClassesNum];
int ClassesNodeIndex[ClassesNum][NodesNum];
const int TabuListSize = 7;
int TabuListState = 0;//判斷有沒有滿
double TabuList[TabuListSize];

NodeStruct TotalEachElement[ClassesNum];
NodeStruct ClassesEachElementTotal[ClassesNum];
NodeStruct StandardDeviationClasses[ClassesNum];

void reafFile();
//---------------------------------------------------------------------------------------
void RandomAssignClusterCenter();
int get_RandonNum( int nodenum);
//---------------------------------------------------------------------------------------
void ClassifyNodesByDistance();
double cal_Distance(int i, int j);
//---------------------------------------------------------------------------------------
double SEE();
double cal_SSE(double x1, double y1, double z1, double w1, double x2, double y2, double z2, double w2);
//---------------------------------------------------------------------------------------
void UpdateTabuList(double sse);
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
    srand((unsigned)time(NULL));
    RandomAssignClusterCenter();
    print_Classes();
    ClassifyNodesByDistance();

    double sse = SEE();
    UpdateTabuList(sse);

    for(int i = 0;i < 6 ;i++) {
        SecondAssignClusterCenter();//重新找中心點
        print_Classes();
        ClassifyNodesByDistance();//再分類
        double sse = SEE();
        UpdateTabuList(sse);
    }
    print_TabuList();
    return 0;
}
//---------------------------------------------------------------------------------------
void RandomAssignClusterCenter(){
    for(int i = 0;i < ClassesNum; i++){
        int centerNode = get_RandonNum(NodesNum);
        ClassesNodes[i].classes = centerNode;
        ClassesNodes[i].x = Nodes[centerNode].x;
        ClassesNodes[i].y = Nodes[centerNode].y;
        ClassesNodes[i].z = Nodes[centerNode].z;
        ClassesNodes[i].w = Nodes[centerNode].w;
    }
}

int get_RandonNum(int Num) {
    double R = ((double)rand()) / (RAND_MAX + 1.0);
    int RandonNum  = (int)(Num * R);
    return RandonNum;
}
//---------------------------------------------------------------------------------------
int indexclass0 = 0,indexclass1 = 0 ,indexclass2 = 0 ;
void ClassifyNodesByDistance() {
    for (int i = 0; i < NodesNum; i++) {
        double min = 100000;
        for (int j = 0; j < ClassesNum; j++) {
            double mindDistance = cal_Distance(i,j);
            if (mindDistance < min) {
                min = mindDistance;
                Nodes[i].classes = j;
            }
        }
        switch (Nodes[i].classes){//紀錄地i個node的class序號
            case 0:
                indexclass0++;
                Nodes[i].count = indexclass0;
                break;
            case 1:
                indexclass1++;
                Nodes[i].count = indexclass1;
                break;
            case 2:
                indexclass2++;
                Nodes[i].count = indexclass2;
                break;
        }
    }
}


double cal_Distance(int i, int j) {
    double column_x =(Nodes[i].x-ClassesNodes[j].x)*(Nodes[i].x-ClassesNodes[j].x);
    double column_y =(Nodes[i].y-ClassesNodes[j].y)*(Nodes[i].y-ClassesNodes[j].y);
    double column_z =(Nodes[i].z-ClassesNodes[j].z)*(Nodes[i].z-ClassesNodes[j].z);
    double column_w =(Nodes[i].w-ClassesNodes[j].w)*(Nodes[i].w-ClassesNodes[j].w);
    double sum = column_x + column_y + column_z + column_w;
    //printf("%f\n",sqrt(sum));
    return sqrt(sum);
}
//---------------------------------------------------------------------------------------
double SEE() {
    double sse = 0;
    for (int i = 0; i < ClassesNum; i++) {
        for (int j = 0; j < NodesNum; j++) {
            if ((int)Nodes[j].classes== i) {
                sse = sse + cal_SSE(Nodes[j].x, Nodes[j].y, Nodes[j].z, Nodes[j].w,
                                    ClassesNodes[i].x, ClassesNodes[i].y, ClassesNodes[i].z, ClassesNodes[i].w);
            }
        }
    }
    return sse;
}

double cal_SSE(double x1, double y1, double z1, double w1, double x2, double y2, double z2, double w2) {
    return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2) + (w1 - w2)*(w1 - w2);
}
//---------------------------------------------------------------------------------------

void UpdateTabuList(double sse) {
    if(TabuListState <7)//若tabulist未滿，哲直接放入
        TabuList[TabuListState] = sse;
    else {//比較標準差，若叫好更新tabulist

        TabuListState = 0;//重置TabuList FIFO(有問題)
    }
    TabuListState++;
}

//---------------------------------------------------------------------------------------

void cal_ClassElementCount(){
    for(int i = 0 ;i < ClassesNum ; i++){
        ClassesNodes[i].count = 0;
    }
    for(int i = 0 ;i < NodesNum ;i ++){
        switch (Nodes[i].classes){
            case 0:
                ClassesNodes[0].count++;
                break;
            case 1:
                ClassesNodes[1].count++;
                break;
            case 2:
                ClassesNodes[2].count++;
                break;
        }
    }
}

void SecondAssignClusterCenter() {
    cal_ClassElementCount();
    for(int i = 0; i < ClassesNum; i++){
        printf("//%d\n",ClassesNodes[i].count);
        int NodeIndex = get_RandonNum(ClassesNodes[i].count);
        printf("--%d\n",NodeIndex);
        for(int j = 0 ;j < NodesNum ; j++){
            if(Nodes[j].count == NodeIndex && Nodes[j].classes == i){
                printf("%d**%d\n",i,j);
                ClassesNodes[i].classes = j;
            }
        }
        ClassesNodes[i].x = Nodes[ClassesNodes[i].classes].x;
        ClassesNodes[i].y = Nodes[ClassesNodes[i].classes].y;
        ClassesNodes[i].z = Nodes[ClassesNodes[i].classes].z;
        ClassesNodes[i].w = Nodes[ClassesNodes[i].classes].w;
    }
}
//---------------------------------------------------------------------------------------
void print_Nodes(){
    for(int i=0;i<NodesNum;i++)
        printf("%d\t%f\t%f\t%f\t%f\t%d\t%d\n",i,Nodes[i].x,Nodes[i].y,Nodes[i].z,Nodes[i].w,Nodes[i].classes,Nodes[i].count);
}
void print_Classes() {
    for(int i=0; i < ClassesNum; i++)
        printf("%d\t%f\t%f\t%f\t%f\t%d\n",i,ClassesNodes[i].x,ClassesNodes[i].y,ClassesNodes[i].z,ClassesNodes[i].w,ClassesNodes[i].classes);
}

void print_TabuList() {
    printf("TabuList\n");
    for(int i = 0 ; i < TabuListSize ; i++){
        printf("%d\t%f\n", i, TabuList[i]);
    }
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
