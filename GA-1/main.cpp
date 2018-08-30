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
    int classes = 0;
};

const int NodesNum = 150;
NodeStruct Nodes[NodesNum];

struct CenterNodeStruct{
    NodeStruct Node;
    double count = 0;

};

const int GeneSize = 3;//群中心個數
struct Chromosome{
    CenterNodeStruct Center[GeneSize];
    double SSE = 0;//紀錄每組的sse
    double Reciprocal_SSE = 0;
    double SelectProbability = 0 ;
};

const int ChromosomeSize = 3;
Chromosome ChromosomeList[ChromosomeSize];

NodeStruct ClusterClass[ChromosomeSize][NodesNum];//紀錄每組染色體的分群狀態

double Reciprocal_TotalSSE = 0;

void reafFile();

void RandonCreatChromosome();
int get_RandonNum(int Num);


void print_Nodes();
void print_Chromosome();

//------------------
void cal_FitnessValueBySSE();

void Cluster();
double cal_Distance(NodeStruct OtherNode, NodeStruct CenterNode);
void print_Cluster();

double SSE( double best);
double cal_SSE(NodeStruct OtherNode, NodeStruct CenterNode);
void print_SSE();
//------------------

void print_SelectRWS();

void Select_RWS();


void Copy_RWS();

void Swap(int index);

void Crossover(double CrossoverRate);


void Mutation(double MutationRate);


int main(){
    reafFile();
    //print_Nodes();
    time_t start,finish;
    start = clock();
    srand((unsigned)time(NULL));
    //------------------

    double CrossoverRate = 0.999;
    double MutationRate = 0.1;
    double best = 10000;
    //隨機產生染色體
    RandonCreatChromosome();
    print_Chromosome();
    Cluster();
    printf("first \n");
    print_Cluster();

    int iterations = 1;
    for(int i = 0 ;i < iterations ; i++) {

        //計算適應函數
        printf("\n%d\n",i);
        best = SSE(best);
//    print_Cluster();
//    print_SSE();

//
        //選擇、複製
        Select_RWS();
//        print_SelectRWS();
         Copy_RWS();
//        print_SelectRWS();
//        print_Chromosome();

        //交配
        Crossover(CrossoverRate);
        printf("after crossover \n");
        print_Cluster();

        //突變
        Mutation(MutationRate);
        //重算群中心
        //要更新群中心~sse才可以算
        double temp_x[ChromosomeSize][GeneSize],temp_y[ChromosomeSize][GeneSize]
        ,temp_z[ChromosomeSize][GeneSize],temp_w[ChromosomeSize][GeneSize]
        ,temp_count[ChromosomeSize][GeneSize];
        for(int i = 0 ;i < ChromosomeSize ;i++)
        {
            for (int j = 0; j < GeneSize; j++)
            {
                temp_x[i][j] = 0;
                temp_y[i][j] = 0;
                temp_z[i][j] = 0;
                temp_w[i][j] = 0;
                temp_count[i][j] = 0;
            }
        }
        for(int i = 0 ;i < ChromosomeSize ;i++)
        {
            for (int j = 0; j < GeneSize; j++)
            {
                for (int k = 0; k < NodesNum; k++)
                {
                    if (ClusterClass[i][k].classes == j)
                    {
                        temp_x[i][j] += ChromosomeList[i].Center[j].Node.x;
                        temp_y[i][j] += ChromosomeList[i].Center[j].Node.y;
                        temp_z[i][j] += ChromosomeList[i].Center[j].Node.z;
                        temp_w[i][j] += ChromosomeList[i].Center[j].Node.w;
                        temp_count[i][j] ++;
                    }
                }
            }
        }

//        for(int i = 0 ;i < ChromosomeSize ;i++)
//        {
//            for (int j = 0; j < GeneSize; j++)
//            {
//                printf("%d\t%d\t%f\t%f\t%f\t%f\t%f\n",i,j,
//                temp_x[i][j] ,
//                temp_y[i][j] ,
//                temp_z[i][j] ,
//                temp_w[i][j] ,
//                temp_count[i][j]);
//            }
//        }
//        printf("\n//---------------------------------------------------------------------------------------\n");

        for(int i = 0 ;i < ChromosomeSize ;i++)
        {
            for (int j = 0; j < GeneSize; j++)
            {
                ChromosomeList[i].Center[j].Node.x = temp_x[i][j]/temp_count[i][j];
                ChromosomeList[i].Center[j].Node.y = temp_y[i][j]/temp_count[i][j];
                ChromosomeList[i].Center[j].Node.z = temp_z[i][j]/temp_count[i][j];
                ChromosomeList[i].Center[j].Node.w = temp_w[i][j]/temp_count[i][j];
            }
        }
        print_Chromosome();

        print_SelectRWS();
    }
//
//    print_Chromosome();
    printf("%f\n",best);
    //------------------
    finish = clock(); //结束
    double duration = ((double)(finish-start))/CLOCKS_PER_SEC; // 计算时间
    printf("\ntime :%lfs.\n",duration);
    return 0;
}
//------------------

void Cluster() {
    for (int i = 0; i < ChromosomeSize; i++)
    {
        for (int j = 0; j < NodesNum; j++)
        {
            double min = 100000;
            for (int k = 0; k < GeneSize; k++) {
                double mindDistance = cal_Distance(Nodes[j],ChromosomeList[i].Center[k].Node);
                if (mindDistance < min)
                {
                    min = mindDistance;
                    ClusterClass[i][j].classes = k;//紀錄每組染色體的分群狀態
                }
            }
        }
    }
}
double SSE( double best) {
    //非常重要~~~~~~~~~~~~~~~~
    for(int i=0; i < ChromosomeSize ; i++){
        for(int j = 0;j < GeneSize ; j++){
            ChromosomeList[i].SSE = 0;
            ChromosomeList[i].Reciprocal_SSE = 0;
            ChromosomeList[i].SelectProbability = 0;
        }
    }//初始變數

    for(int i = 0 ;i < ChromosomeSize ;i++)
    {
        for (int j = 0; j < GeneSize; j++)
        {
            for (int k = 0; k < NodesNum; k++)
            {
                if (ClusterClass[i][k].classes == j)
                {
                    ChromosomeList[i].SSE = ChromosomeList[i].SSE + cal_SSE(Nodes[k], ChromosomeList[i].Center[j].Node);
                }
            }
        }
    }

    for(int i = 0 ;i < ChromosomeSize ;i++){
        if(ChromosomeList[i].SSE< best){
            best = ChromosomeList[i].SSE;
        }
    }
    return best;
}



double cal_SSE(NodeStruct OtherNode, NodeStruct CenterNode) {
    double column_x =(OtherNode.x-CenterNode.x) * (OtherNode.x-CenterNode.x);
    double column_y =(OtherNode.y-CenterNode.y) * (OtherNode.y-CenterNode.y);
    double column_z =(OtherNode.z-CenterNode.z) * (OtherNode.z-CenterNode.z);
    double column_w =(OtherNode.w-CenterNode.w) * (OtherNode.w-CenterNode.w);
    double sum = column_x + column_y + column_z + column_w;
    return sum;
}

double cal_Distance(NodeStruct OtherNode, NodeStruct CenterNode) {
    double column_x =(OtherNode.x-CenterNode.x) * (OtherNode.x-CenterNode.x);
    double column_y =(OtherNode.y-CenterNode.y) * (OtherNode.y-CenterNode.y);
    double column_z =(OtherNode.z-CenterNode.z) * (OtherNode.z-CenterNode.z);
    double column_w =(OtherNode.w-CenterNode.w) * (OtherNode.w-CenterNode.w);
    double sum = column_x + column_y + column_z + column_w;
    return sqrt(sum);
}
//------------------
void Mutation(double MutationRate) {
    for (int i = 0; i < ChromosomeSize; i++)
    {
        double temp_RandonNum = get_RandonNum(100);
        double RandonMutationRate  = temp_RandonNum/100;
        printf("\tMR : %f\n",RandonMutationRate);
        if(MutationRate > RandonMutationRate ){//MutationRate 全域變數 設定很 ，即突變率較低
            int RandonIndex = get_RandonNum(NodesNum);
            int RandonClass = get_RandonNum(GeneSize);
            ClusterClass[i][RandonIndex].classes = RandonClass;//突變一個點
            printf("\tMutation success\n");
        }
    }
}
//------------------
//select與max做群分類交換,先取得交換範圍例如 50~99 ，交換後更新群中心
void Crossover(double CrossoverRate) {
    int CrossoverTime = ChromosomeSize / 2;
    for (int i = 0; i <= CrossoverTime; i= i+2) {
        //交換第i與第i+1條染色體的第 Min~ Max 個點，每要交換都換再重新產生新的交換率，不一定每個染色體皆有交換
        double temp_RandonNum = get_RandonNum(100);
        double RandonCrossoverRate  = temp_RandonNum/100;
        printf("\tCR : %f\n",RandonCrossoverRate);
        if(CrossoverRate > RandonCrossoverRate ){//CrossoverRate 全域變數 設定很大 ，即交換率很高
            printf("+++%d\n",i);
            Swap(i);
            printf("\tCrossover success\n");
        }
    }
}

void Swap(int index) {
    int Min = get_RandonNum(NodesNum);
    int Max= get_RandonNum(NodesNum);
    if(Min > Max){
        int temp = Min;
        Min = Max;
        Max = temp;
    }
    for(int i = Min ; i < Max ;i++){
        ClusterClass[index][i].classes =  ClusterClass[index+1][i].classes ;
    }
    printf("***%d\t%d\n",Min,Max);
}

//------------------


void Select_RWS() {//ChromosomeList[j].TotalFitnessValue
    for(int j = 0 ; j < ChromosomeSize ; j ++){
        for(int k = 0 ; k < GeneSize ; k++){
            ChromosomeList[j].Reciprocal_SSE = 1 / ChromosomeList[j].SSE; //取倒數，數字越小，取倒數越大
        }
        //TotalSSE += ChromosomeList[j].SSE;//目前沒用到
        //倒數相加總，用於算比例
        Reciprocal_TotalSSE += ChromosomeList[j].Reciprocal_SSE;
    }
    //計算比例，每一個染色體占全部多少百分比
    for(int j = 0 ; j < ChromosomeSize ; j ++){
        ChromosomeList[j].SelectProbability = ChromosomeList[j].Reciprocal_SSE / Reciprocal_TotalSSE;
    }
}

void Copy_RWS() {//根據比例去選擇
    //隨機產生一個0~1數用於選擇哪一個群,每一個群在上一步驟已算出自己的百分比
    //若隨機產生的數為0.7，上一步驟假設有4個染色體別比例為0.2,0.3,0.4,0.1，所以選擇第三個染色體0.2+0.3+0.4 > 0.7
    int Max_index = 0;
    int SelectNum = 0 ;
    double temp_RandonNum = get_RandonNum(100);
    double ProbabilityNum  = temp_RandonNum/100;
    //printf("---------%f",ProbabilityNum);
    double temp = ChromosomeList[0].SelectProbability ;//第一次比較temp = 第一個群的的機率
    for(int i = 1 ;i <= ChromosomeSize ; i++){
        //printf("\n%f\t",temp);
        if(temp > ProbabilityNum){
            //printf("/*%d",i-1);
            SelectNum = i-1;//取得第幾個染色體要被複製
            break;
        }
        temp +=ChromosomeList[i].SelectProbability;
    }
    double Max = 0;
    //找出最大的染色體
    for (int i = 0; i < ChromosomeSize ; i++) {
        if(ChromosomeList[i].SSE > Max){
            Max = ChromosomeList[i].SSE;
            Max_index = i;
        }
    }
    //
    ChromosomeList[Max_index].SSE = ChromosomeList[SelectNum].SSE;
    for (int i = 0; i < GeneSize; i++) {
        ChromosomeList[Max_index].Center[i].count= ChromosomeList[SelectNum].Center[i].count;
        ChromosomeList[Max_index].Center[i].Node= ChromosomeList[SelectNum].Center[i].Node;

    }
    ChromosomeList[Max_index].Reciprocal_SSE= ChromosomeList[SelectNum].Reciprocal_SSE;
    ChromosomeList[Max_index].SelectProbability= ChromosomeList[SelectNum].SelectProbability;

    //新加入便於crocessover 運算
    for (int j = 0; j < NodesNum; j++)
    {
        ClusterClass[Max_index][j].classes =  ClusterClass[SelectNum][j].classes ;
    }
    printf("--%d\t%d\n",Max_index,SelectNum);

}

//------------------
void RandonCreatChromosome() {
    for(int i = 0 ; i < ChromosomeSize ; i++){
        for(int j = 0 ; j < GeneSize ; j++){
            int RandonIndex = get_RandonNum(100);
            ChromosomeList[i].Center[j].Node = Nodes[RandonIndex];
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
void print_Cluster() {
    for(int i = 0 ; i < GeneSize ; i++){
        printf("--%d\n",i);
        for(int j= 0 ; j < NodesNum ;j++){
            printf("[%d]%d\t", j,ClusterClass[i][j].classes );
        }
    }
    printf("\n//---------------------------------------------------------------------------------------\n");
}
void print_SSE() {
    for(int i = 0 ; i < ChromosomeSize ; i++){
        printf("\n--%d\n%f",i,ChromosomeList[i].SSE);
    }
    printf("\n//---------------------------------------------------------------------------------------\n");
}


void print_SelectRWS() {
    for(int j = 0 ; j < ChromosomeSize ; j ++){
        printf("\n%d\t%f\t%f\t%f",j,
               ChromosomeList[j].SSE,
               ChromosomeList[j].Reciprocal_SSE,
               ChromosomeList[j].SelectProbability);
    }
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
