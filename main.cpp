#include<iostream>
#include<fstream>
#include <sstream>
#include <thread>
#include <vector>
#include <iostream>
#include <cmath>
#include <process.h>
#include <time.h>
using namespace std;

int NodeCount = 50 ;
struct NodeInfo{
    int Index = 0;
    int X = 0;
    int Y = 0;
};
struct FileFont{
    int Index = 0;
    int X = 0;
    int Y = 0;
    int Vaild_bit = 0;
    FileFont(int index,int x,int y,int vaild_bit):Index(index),X(x),Y(y),Vaild_bit(vaild_bit) {}
};
struct Path{
    int Index = 0;
    int X = 0;
    int Y = 0;
    double Lnn = 0;
    Path(int index,int x,int y,double lnn):Index(index),X(x),Y(y),Lnn(lnn) {}
};

typedef vector<double> double_1d;
typedef vector<vector<double>> double_2d;
typedef vector<FileFont> FileFont_1d;
typedef vector<Path> Path_1d;

void reafFile(FileFont_1d &FileVector) {
    string line[NodeCount];//store each line in txt
    string temp[NodeCount][3];//save txt.
    ifstream file;//read only
    file.open("eil51.txt", ios::in);//read only mode
        for (int i = 0; i < NodeCount; i++) {
            getline(file, line[i]);
            stringstream ss_line(line[i]);//ex 1 37 52
            for (int j = 0; j < 3; j++)
                ss_line >> temp[i][j];
            //str to int (method 1)
            int Index = atoi(temp[i][0].c_str());
            int x = atoi(temp[i][1].c_str());
            int y = atoi(temp[i][2].c_str());
            FileVector.push_back(FileFont(Index-1,x,y,0));
        }
        cout << "success\n";

}
double get_Randon(double Min, double Max) {
    double rand_num = (Max - Min) * rand() / RAND_MAX + (Min);
    return rand_num;
}
double cal_Distance(FileFont_1d FileVector,int node1, int node2) {
    int x1 = FileVector[node1].X;
    int y1 = FileVector[node1].Y;
    int x2 = FileVector[node2].X;
    int y2 = FileVector[node2].Y;
    return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}
void print_FileVecter(FileFont_1d FileVector) {
    cout << "FileVecter\n";
    for(int i = 0 ; i < FileVector.size() ; i ++){
        cout << i <<"\t"<< FileVector[i].Index <<"\t"<< FileVector[i].X <<"\t"<<FileVector[i].Y <<"\t"<<FileVector[i].Vaild_bit<<"\n" ;
    }
}

void print_PathArray(Path_1d PathArray) {
    cout << "PathArray\n";
    for(int i = 0 ;i < PathArray.size() ;i++){
        cout <<i<<"\t"
             <<"select\t"<<PathArray[i].Index<<"\t"
             <<PathArray[i].X<<"\t"
             <<PathArray[i].Y<<"\t"
             <<"totaldistance\t"<< PathArray[i].Lnn <<"\n";
    }
}
int main(){
    FileFont_1d FileVector;//所有點資料
    reafFile(FileVector);
    cout << FileVector.size()<<"\n" ;

    print_FileVecter(FileVector);

    srand((unsigned) time(NULL) + getpid());
    //1.起始状態及參數設定
    double_1d row;
    row.assign(NodeCount,0.0);

    double_2d pheromone_2d;
    pheromone_2d.assign(NodeCount,row);

    double pheromone_fallrate = 0.8;
    int ant_count = 50 ;
    double alpha = 0.5;
    double beta = 0.5;
    int T_max  = 10;
    //初始pheromone[50][50]，τ ij = τ 0 = (NLnn )-1，N為總線段數，
    //求 Lnn 想法 : 隨機取一點作為起點，利用(貪心解法) 所求出的一個可能路徑之路徑總距離
    //求 路徑長
    Path_1d PathArray;

    int index = get_Randon(0,NodeCount-1);
    int x = FileVector[index].X;
    int y = FileVector[index].Y;
    double Lnn = 0 ;
    PathArray.push_back(Path(index,x,y,Lnn));
    FileVector[index].Vaild_bit = 1;    //將選到的index對應FileVector的Vaild bit 設為 1

    for(int i = 1 ; i < FileVector.size() ; i ++){
        double mindistan = 1000 ;
        int mindistance_index = 0 ;
        for(int j = 0 ; j < FileVector.size() ; j ++){
            if(index != j && FileVector[j].Vaild_bit==0){
                double tempdistan = cal_Distance(FileVector,index,j);
                if(mindistan > tempdistan){
                    mindistan = tempdistan;
                    mindistance_index = j;
                }
            }
        }
        index = mindistance_index;
        x = FileVector[index].X;
        y= FileVector[index].Y;
        Lnn += mindistan;
        PathArray.push_back(Path(index,x,y,Lnn));
        FileVector[index].Vaild_bit = 1;
    }
    print_PathArray(PathArray);
    print_FileVecter(FileVector);


    return 0;
}
