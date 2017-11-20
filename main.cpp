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
typedef vector<double> double_1d;
typedef vector<vector<double>> double_2d;
typedef vector<FileFont> FileFont_1d;
typedef vector<Path> Path_1d;

const int CityCount = 50 ;
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

struct city_info{
    double_2d tau_vector;//線段 ij 在時間 t 之費洛蒙濃度_
    double_2d delta_tau_vector;//線段 ij 在時間 t 之費洛蒙濃度_
    double_2d eta_vector;//代表期望値，通常為線段 ij 長度之倒數
};
class ant_info{
private:
    int allowed_city[CityCount];//the left cities which ant can go
private:
    int choose_next_city();
public:
    int route[CityCount];
public:
    ant_info();
    void move();
};
ant_info::ant_info() {
    for (int i = 0; i < CityCount; i++)
    {
        allowed_city[i] = 1;//1 = can go, 0 = can't
    }
}
void ant_info::move(){
    int n = choose_next_city();
    add_city(n);
}
int ant_info::choose_next_city(){

    return 1;
}


void reafFile(FileFont_1d &FileVector) {
    string line[CityCount];//store each line in txt
    string temp[CityCount][3];//save txt.
    ifstream file;//read only
    file.open("eil51.txt", ios::in);//read only mode
        for (int i = 0; i < CityCount; i++) {
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
double cal_Distance(FileFont_1d File_Vector,int node1, int node2) {
    int x1 = File_Vector[node1].X;
    int y1 = File_Vector[node1].Y;
    int x2 = File_Vector[node2].X;
    int y2 = File_Vector[node2].Y;
    return sqrt(pow(x1-x2,2)+pow(y1-y2,2));
}
double get_Lnn(FileFont_1d FileVector){
    //初始pheromone[50][50]，τ ij = τ 0 = (NLnn )-1，N為總線段數，
    //求 Lnn 想法 : 隨機取一點作為起點，利用(貪心解法) 所求出的一個可能路徑之路徑總距離
    //求 路徑長
    Path_1d PathArray;
    int index = get_Randon(0,CityCount-1);
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
//    cout << Lnn <<"\n";
    return Lnn;
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
    FileFont_1d File_Vector;//所有點資料
    reafFile(File_Vector);
//    print_FileVecter(File_Vector);
    srand((unsigned) time(NULL) + getpid());

    //1.起始状態及參數設定
    double pheromone_fallrate = 0.8;
    int ant_count = 50 ;
    double alpha = 0.5;//須測試
    double beta = 0.5;//須測試
    int T_max  = 10;

    int N = ( CityCount * (CityCount - 1)) / 2;//線段總數
    double tau = 1 / (N * get_Lnn(File_Vector));

    city_info city;
    //初始tau array ,eta array ,transition probability array
    for(int i = 0 ; i < CityCount ; i ++){
        for(int j = 0 ;j < CityCount ; j++ ){
            if(i==j){
                city.tau_vector[i][j] = 0;
                city.eta_vector[i][j] = 0;
            }else{
                city.tau_vector[i][j] = tau;
                city.delta_tau_vector[i][j] = 0;
                city.eta_vector[i][j] = cal_Distance(File_Vector,i,j);
            }
        }
    }
    //初始螞蟻
    ant_info ants[ant_count];
    for (int i = 0; i < ant_count; i++){//初始每一隻螞蟻的起始位置，且不重複
        do{
            ants[i].route[0] = rand() % CityCount;//螞蟻的起始範圍為城市個數

            for (int j = 0; j < i; j++){
                if (ants[i].route[0]==ants[j].route[0]) {
                    ants[i].route[0] = 0;
                    break;
                }
            }
        }while(ants[i].route[0]==0);
    }
    for(int i = 0 ;i < ant_count ; i ++ ){
        for(int j = 0 ; j < CityCount - 1 ; j++){//每隻螞蟻移動 CityCount - 1 初始的第一次
            ants[i].move();
        }
    }

    return 0;
}