#include<iostream>
#include<fstream>
#include <sstream>
#include <cmath>
#include <thread>
#include <cstring>
#include <unistd.h>

using namespace std;
//有n條染色體chromosome
//每一條染色體有三個中心點
//每個中心點 有 四維 資料（struct NodeStruct)

struct NodeStruct{
    int index = 0;
    double x = 0;
    double y = 0;
    double z = 0;
    double w = 0;
};

struct CenterNodeStruct{
    NodeStruct Node;
    double FitnessValue = 0;

};

const int GeneSize = 3;
struct Chromosome{
    CenterNodeStruct Center[GeneSize];
    double TotalFitnessValue = 0;
    double Reciprocal_TotalFitnessValue = 0;
    double SelectProbability = 0 ;
};

const int ChromosomeSize = 5;
Chromosome ChromosomeList[ChromosomeSize];

double ListTotalFitnessValue = 0;//所有染色體的TotalFitnessValue的總和
double Reciprocal_ListTotalFitnessValue = 0;


const int NodesNum = 150;
NodeStruct Nodes[NodesNum];

void reafFile();


void RandonCreatChromosome();
int get_RandonNum(int Num);

double cal_FitnessValueByDistance(NodeStruct OtherNode, NodeStruct CenterNode);

void print_Chromosome();
void print_Nodes();


void print_EachChromosomeFitnessValue();

void cal_FitnessValue();

void Select_RWS();

void print_SelectRWS();

void Copy_RWS();

int main(){
    reafFile();
    //print_Nodes();
    time_t start,finish;
    start = clock();
    srand((unsigned)time(NULL));
    //------------------

    //隨機產生染色體
    RandonCreatChromosome();
    //print_Chromosome();

    //計算適應函數
    cal_FitnessValue();
    print_EachChromosomeFitnessValue();

    //選擇、複製
    Select_RWS();
    print_SelectRWS();
    Copy_RWS();
    print_SelectRWS();
    print_EachChromosomeFitnessValue();

    //交配


    //------------------
    finish = clock(); //结束
    double duration = ((double)(finish-start))/CLOCKS_PER_SEC; // 计算时间
    printf("\ntime :%lfs.\n",duration);
    return 0;
}

void cal_FitnessValue() {
    //ChromosomeList[j].Center[k].FitnessValue
    for(int i = 0 ; i < NodesNum ; i ++){
        for(int j = 0 ; j < ChromosomeSize ; j ++){
            for(int k = 0 ; k < GeneSize ; k++){
                ChromosomeList[j].Center[k].FitnessValue += cal_FitnessValueByDistance
                        (Nodes[i], ChromosomeList[j].Center[k].Node);
            }
        }
    }
}

void Select_RWS() {//ChromosomeList[j].TotalFitnessValue
    for(int j = 0 ; j < ChromosomeSize ; j ++){
        for(int k = 0 ; k < GeneSize ; k++){
            ChromosomeList[j].TotalFitnessValue += ChromosomeList[j].Center[k].FitnessValue ;
            //取倒數，數字越小，取倒數越大
            ChromosomeList[j].Reciprocal_TotalFitnessValue = 1 / ChromosomeList[j].TotalFitnessValue;
        }
        ListTotalFitnessValue += ChromosomeList[j].TotalFitnessValue;//目前沒用到
        //倒數相加總，用於算比例
        Reciprocal_ListTotalFitnessValue += ChromosomeList[j].Reciprocal_TotalFitnessValue;
    }
    //計算比例
    for(int j = 0 ; j < ChromosomeSize ; j ++){
        ChromosomeList[j].SelectProbability =
                ChromosomeList[j].Reciprocal_TotalFitnessValue / Reciprocal_ListTotalFitnessValue;
    }
}

void Copy_RWS() {//根據比例去選擇
    //隨機產生一個0~1數用於選擇哪一個群
    double temp_RandonNum = get_RandonNum(100);
    double ProbabilityNum  = temp_RandonNum/100;
    printf("---------%f",ProbabilityNum);
    double temp = ChromosomeList[0].SelectProbability ;
    int SelectNum = 0 ;
    for(int i = 1 ;i <= ChromosomeSize ; i++){
        printf("\n%f\t",temp);
        if(temp > ProbabilityNum){
            printf("/*%d",i-1);
            SelectNum = i-1;//取得第幾個Chromosome要被複製
            break;
        }
        temp +=ChromosomeList[i].SelectProbability;
    }
    double Max = 0;
    int Max_index = 0;
    //找出最大的Chromosome
    for (int i = 0; i < ChromosomeSize ; i++) {
        if(ChromosomeList[i].TotalFitnessValue > Max){
            Max = ChromosomeList[i].TotalFitnessValue;
            Max_index = i;
        }
    }
    //替換最大的Chromosome
    ChromosomeList[Max_index].TotalFitnessValue = ChromosomeList[SelectNum].TotalFitnessValue;
    for (int i = 0; i < GeneSize; i++) {
        ChromosomeList[Max_index].Center[i].FitnessValue= ChromosomeList[SelectNum].Center[i].FitnessValue;
    }
    ChromosomeList[Max_index].Reciprocal_TotalFitnessValue= ChromosomeList[SelectNum].Reciprocal_TotalFitnessValue;
    ChromosomeList[Max_index].SelectProbability= ChromosomeList[SelectNum].SelectProbability;
}


double cal_FitnessValueByDistance(NodeStruct OtherNode, NodeStruct CenterNode) {
    double column_x =(OtherNode.x-CenterNode.x) * (OtherNode.x-CenterNode.x);
    double column_y =(OtherNode.y-CenterNode.y) * (OtherNode.y-CenterNode.y);
    double column_z =(OtherNode.z-CenterNode.z) * (OtherNode.z-CenterNode.z);
    double column_w =(OtherNode.w-CenterNode.w) * (OtherNode.w-CenterNode.w);
    double sum = column_x + column_y + column_z + column_w;
    return sqrt(sum);
}

void RandonCreatChromosome() {
    for(int i = 0 ; i < ChromosomeSize ; i++){
        for(int j = 0 ; j < GeneSize ; j++){
            ChromosomeList[i].Center[j].Node.index = get_RandonNum(NodesNum);
            ChromosomeList[i].Center[j].Node.x = Nodes[ChromosomeList[i].Center[j].Node.index].x;
            ChromosomeList[i].Center[j].Node.y = Nodes[ChromosomeList[i].Center[j].Node.index].y;
            ChromosomeList[i].Center[j].Node.z = Nodes[ChromosomeList[i].Center[j].Node.index].z;
            ChromosomeList[i].Center[j].Node.w = Nodes[ChromosomeList[i].Center[j].Node.index].w;
        }
    }
}

int get_RandonNum(int Num) {
    double R = ((double)rand()) / (RAND_MAX + 1.0);
    int RandonNum  = (int)(Num * R);
    return RandonNum;
}
//---------------------------------------------------------------------------------------
void print_Nodes(){
    for(int i=0;i<NodesNum;i++)
        printf("%d\t%.2f\t%.2f\t%.2f\t%.2f\n",Nodes[i].index,Nodes[i].x,Nodes[i].y,Nodes[i].z,Nodes[i].w);
    printf("\n//---------------------------------------------------------------------------------------\n");
}
void print_Chromosome() {
    for(int i=0; i < ChromosomeSize ; i++)
        for(int j = 0;j < GeneSize ; j++){
            printf("%d\t%d\t%f\t%f\t%f\t%f\n",i,
                   ChromosomeList[i].Center[j].Node.index,
                   ChromosomeList[i].Center[j].Node.x,
                   ChromosomeList[i].Center[j].Node.y,
                   ChromosomeList[i].Center[j].Node.z,
                   ChromosomeList[i].Center[j].Node.w);
        }
    printf("\n//---------------------------------------------------------------------------------------\n");
}
void print_EachChromosomeFitnessValue() {
    for(int j = 0 ; j < ChromosomeSize ; j ++){
        printf("\n%d\t",j);
        for(int k = 0 ; k < GeneSize ; k++){
            printf("%f\t",ChromosomeList[j].Center[k].FitnessValue);
        }
    }
    printf("\n//---------------------------------------------------------------------------------------\n");
}

void print_SelectRWS() {
    for(int j = 0 ; j < ChromosomeSize ; j ++){
        printf("\n%d\t%f\t%f\t%f",j,
               ChromosomeList[j].TotalFitnessValue,
               ChromosomeList[j].Reciprocal_TotalFitnessValue,
               ChromosomeList[j].SelectProbability);
    }
    printf("\n\t%f\t%f",ListTotalFitnessValue,Reciprocal_ListTotalFitnessValue);
    printf("\n//---------------------------------------------------------------------------------------\n");
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
    cout << "Read file done" << endl;
}
