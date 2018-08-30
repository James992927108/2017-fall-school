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
    int index = 0;
    int count =0;
};
const int Dimension = 4;//點維度
const int NodesNum = 150; // 城市数量
double Nodes[NodesNum][Dimension+1];
const int ClassesNum = 3;//群個數
double Classes[ClassesNum][Dimension+1];
int ClassesNodeIndex[ClassesNum][NodesNum];
const int TabuListSize = 7;
int TabuListState = 0;//判斷有沒有滿
NodeStruct TabuList[TabuListSize][ClassesNum];

NodeStruct TotalEachElement[ClassesNum];
NodeStruct ClassesEachElementTotal[ClassesNum];
NodeStruct StandardDeviationClasses[ClassesNum];

void reafFile();

void RandomAssignClusterCenter();
int get_RandonNum( int nodenum);
void ClassifyNodesByDistance();
double cal_Distance(int node1, int node2);

void print_Nodes();
void print_Classes();

void Cal_ClassesElementAvg();


void cal_StandardDeviation();

void UpdateTabuList();

void SecondAssignClusterCenter();

void print_TabuList();

void InitalStructValue();

int main(){
    reafFile();
    srand((unsigned)time(NULL));
    RandomAssignClusterCenter();
    ClassifyNodesByDistance();
    cal_StandardDeviation();    //計算每一組的x,y,z,w的標準差
    UpdateTabuList();
    
    for(int i = 0;i < 3 ;i++) {
        SecondAssignClusterCenter();//重新找中心點
        InitalStructValue();
        ClassifyNodesByDistance();//再分類
        cal_StandardDeviation();    //計算每一組的x,y,z,w的標準差

        if(TabuListState <7)//若tabulist未滿，哲直接放入
            UpdateTabuList();
        else {//比較標準差，若叫好更新tabulist

            TabuListState = 0;//重置TabuList FIFO(有問題)
        }
    }

    print_TabuList();
    return 0;
}

void InitalStructValue() {
    for(int i = 0 ; i < ClassesNum ; i++){
            ClassesEachElementTotal[i].x  = 0;
            ClassesEachElementTotal[i].y  = 0;
            ClassesEachElementTotal[i].z  = 0;
            ClassesEachElementTotal[i].w  = 0;
            ClassesEachElementTotal[i].index  = 0;
            ClassesEachElementTotal[i].count  = 0;
            TotalEachElement[i].x = 0;
            TotalEachElement[i].y = 0;
            TotalEachElement[i].z = 0;
            TotalEachElement[i].w = 0;
            TotalEachElement[i].index = 0;
            TotalEachElement[i].count = 0;
        }
}

void SecondAssignClusterCenter() {
    for(int i = 0; i < ClassesNum; i++){
        printf("%d\n",ClassesEachElementTotal[i].count);
        int NodeIndex = get_RandonNum(ClassesEachElementTotal[i].count);
        printf("--%d\n",NodeIndex);
        printf("**%d\n", ClassesNodeIndex[i][NodeIndex]);
        Classes[i][4] = ClassesNodeIndex[i][NodeIndex];
        for (int j = 0; j < Dimension; j++)
            Classes[i][j] = Nodes[NodeIndex][j];
    }
}
void UpdateTabuList() {
    for(int i = 0 ; i < ClassesNum ; i ++){
        TabuList[TabuListState][i].x = StandardDeviationClasses[i].x;
        TabuList[TabuListState][i].y = StandardDeviationClasses[i].y;
        TabuList[TabuListState][i].z = StandardDeviationClasses[i].z;
        TabuList[TabuListState][i].w = StandardDeviationClasses[i].w;
    }
    TabuListState++;
}


void cal_StandardDeviation() {
    for(int i = 0 ; i < NodesNum ; i ++){
        ClassesEachElementTotal[(int)Nodes[i][4]].x += Nodes[i][0];
        ClassesEachElementTotal[(int)Nodes[i][4]].y += Nodes[i][1];
        ClassesEachElementTotal[(int)Nodes[i][4]].z += Nodes[i][2];
        ClassesEachElementTotal[(int)Nodes[i][4]].w += Nodes[i][3];
        ClassesEachElementTotal[(int)Nodes[i][4]].count ++;
    }
    for(int i = 0 ;i <ClassesNum;i++) {
        printf("class%d--%d\n", i,ClassesEachElementTotal[i].count);
        printf("x%d---%f\n", i, ClassesEachElementTotal[i].x / ClassesEachElementTotal[i].count);
        printf("y%d---%f\n", i, ClassesEachElementTotal[i].y / ClassesEachElementTotal[i].count);
        printf("z%d---%f\n", i, ClassesEachElementTotal[i].z / ClassesEachElementTotal[i].count);
        printf("w%d---%f\n", i, ClassesEachElementTotal[i].w / ClassesEachElementTotal[i].count);
    }

    int count_0 = 0,count_1 = 0,count_2 = 0;
    for(int i = 0 ;i < ClassesNum ;i++) {
        for (int j = 0; j < NodesNum; j++) {
            if ((int) Nodes[j][4] == i) {//class_0
                double x_temp = Nodes[j][0] - (ClassesEachElementTotal[i].x / ClassesEachElementTotal[i].count);
                TotalEachElement[i].x += x_temp * x_temp;
                double y_temp = Nodes[j][1] - (ClassesEachElementTotal[i].y / ClassesEachElementTotal[i].count);
                TotalEachElement[i].y += y_temp * y_temp;
                double z_temp = Nodes[j][2] - (ClassesEachElementTotal[i].z / ClassesEachElementTotal[i].count);
                TotalEachElement[i].z += z_temp * z_temp;
                double w_temp = Nodes[j][3] - (ClassesEachElementTotal[i].w / ClassesEachElementTotal[i].count);
                TotalEachElement[i].w += w_temp * w_temp;
                TotalEachElement[i].index = j;
                //須重構，只為之道需index值，多設一個變數
                if((int) Nodes[j][4]  == 0){
                    ClassesNodeIndex[i][count_0] = j;//將點分類並放入新的array
                    count_0++;
                }
                if((int) Nodes[j][4]  == 1){
                    ClassesNodeIndex[i][count_1] = j;//將點分類並放入新的array
                    count_1++;
                }
                if((int) Nodes[j][4]  == 2){
                    ClassesNodeIndex[i][count_2] = j;//將點分類並放入新的array
                    count_2++;
                }
                printf("%d\t%d\t%f\t%f\t%f\t%f\n", i, TotalEachElement[i].index, TotalEachElement[i].x, TotalEachElement[i].y,
                       TotalEachElement[i].z, TotalEachElement[i].w);
            }
        }
    }
    printf("**************\n");
    for(int i = 0;i <ClassesNum ; i++){
        printf("%d\t%f\t%f\t%f\t%f\n",i,TotalEachElement[i].x, TotalEachElement[i].y,
               TotalEachElement[i].z, TotalEachElement[i].w);
    }
    printf("**************\n");
    for(int i = 0 ; i < ClassesNum ;i++){
        StandardDeviationClasses[i].x = sqrt(TotalEachElement[i].x / (ClassesEachElementTotal[i].count - 1));
        StandardDeviationClasses[i].y = sqrt(TotalEachElement[i].y / (ClassesEachElementTotal[i].count - 1));
        StandardDeviationClasses[i].z = sqrt(TotalEachElement[i].z / (ClassesEachElementTotal[i].count - 1));
        StandardDeviationClasses[i].w = sqrt(TotalEachElement[i].w / (ClassesEachElementTotal[i].count - 1));
    }
    for(int i = 0;i <ClassesNum ; i++){
        printf("%d\t%f\t%f\t%f\t%f\n",i,StandardDeviationClasses[i].x, StandardDeviationClasses[i].y,
               StandardDeviationClasses[i].z, StandardDeviationClasses[i].w);
    }
}

//---------------------------------------------------------------------------------------
void ClassifyNodesByDistance() {
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
    //print_Nodes();
}

double cal_Distance(int Nodes_index, int Classes_index) {
    double sum = 0;
    for(int i = 0 ;i <Dimension;i++){
        double Nodes_column = Nodes[Nodes_index][i];
        double Classes_column = Classes[Classes_index][i];
        double column =(Nodes_column-Classes_column)*(Nodes_column-Classes_column);
        sum += column;
    }
    //printf("%f\n",sqrt(sum));
    return sqrt(sum);
}
//---------------------------------------------------------------------------------------
void RandomAssignClusterCenter(){
    for(int i = 0;i < ClassesNum; i++){
        int centerNode = get_RandonNum(NodesNum);
        Classes[i][4] = centerNode;
        for (int j = 0; j < Dimension; j++)
            Classes[i][j] = Nodes[centerNode][j];
    }
    //print_Classes();
}

int get_RandonNum(int Num) {
    double R = ((double)rand()) / (RAND_MAX + 1.0);
    int RandonNum  = (int)(Num * R);
    return RandonNum;
}

//---------------------------------------------------------------------------------------
void print_Nodes(){
    for(int i=0;i<NodesNum;i++)
        printf("%d\t%f\t%f\t%f\t%f\t%f\n",i,Nodes[i][0],Nodes[i][1],Nodes[i][2],Nodes[i][3],Nodes[i][4]);
}
void print_Classes() {
    for(int i=0; i < ClassesNum; i++)
        printf("%d\t%f\t%f\t%f\t%f\t%0.f\n",i,Classes[i][0],Classes[i][1],Classes[i][2],Classes[i][3],Classes[i][4]);
}

void print_TabuList() {
    printf("TabuList\n");
    for(int i = 0 ; i < TabuListSize ; i++){
        for(int j= 0 ;j < ClassesNum ;j++) {
            printf("%d\t%d\t%f\t%f\t%f\t%f\n", i,j, TabuList[i][j].x, TabuList[i][j].y,
                   TabuList[i][j].z, TabuList[i][j].w);
        }
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
            Nodes[i][j] = temp;
            p = strtok(NULL,d);
            j++;
        }
        i++;
    }
    //print_Nodes();
    cout << "Read file done" << endl;
}
//---------------------------------------------------------------------------------------
