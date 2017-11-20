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


const int CityCount = 50 ;
struct NodeInfo{
    int Index = 0;
    int X = 0;
    int Y = 0;
    NodeInfo(int index, int x, int y) : Index(index),X(x),Y(y){}
};
struct FileFont : NodeInfo{
    int Vaild_bit = 0;
    FileFont(int index,int x,int y,int vaild_bit):NodeInfo(index , x , y ),Vaild_bit(vaild_bit) {}
};
struct Path : NodeInfo{
    double Lnn = 0;
    Path(int index,int x,int y,double lnn):NodeInfo(index , x , y ),Lnn(lnn) {}
};
typedef vector<double> double_1d;
typedef vector<vector<double>> double_2d;

struct city_info{
    double_2d Tau_vector;//線段 ij 在時間 t 之費洛蒙濃度_
    double_2d Eta_vector;//代表期望値，通常為線段 ij 長度之倒數
//    city_info(double_2d tau_vector,double_2d eta_vector):Tau_vector(tau_vector),Eta_vector(eta_vector){}
};

typedef vector<FileFont> FileFont_1d;
typedef vector<Path> Path_1d;

typedef vector<int> int_1d;
typedef vector<double> double_1d;
struct ant_info{
    int_1d route;
    int_1d allowed_city;
    double_1d probability;
    double fitness = 10000;
    int index = 0;
};


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
int main() {
    FileFont_1d File_Vector;//所有點資料
    reafFile(File_Vector);
//    print_FileVecter(File_Vector);
    srand((unsigned) time(NULL) + getpid());


    //1.起始状態及參數設定
    double pheromone_fallrate = 0.9;
    int ant_count = 50;
    int Q = 100;
    double alpha = 1;//須測試
    double beta = 2;//須測試


    int N = (CityCount * (CityCount - 1)) / 2;//線段總數
    double tau = 1 / (N * get_Lnn(File_Vector));

    city_info city;
    city.Tau_vector = double_2d(CityCount, double_1d(CityCount, 0));
    city.Eta_vector = double_2d(CityCount, double_1d(CityCount, 0));

    //初始tau array ,eta array ,transition probability array
    for (int i = 0; i < CityCount; i++) {
        for (int j = 0; j < CityCount; j++) {
            if (i == j) {
                city.Tau_vector[i][j] = 0;
                city.Eta_vector[i][j] = 0;
            } else {
                city.Tau_vector[i][j] = tau;
                city.Eta_vector[i][j] = 1 / cal_Distance(File_Vector, i, j);
            }
        }
    }
    //初始螞蟻
    ant_info ants[ant_count];
    ant_info bestAnt;

    for (int i = 0; i < ant_count; i++) {
        bestAnt.route = int_1d(CityCount,0);
        ants[i].route = int_1d(CityCount, 0);
        ants[i].allowed_city = int_1d(CityCount, 0);
        ants[i].probability = double_1d(CityCount, 0.0);
    }
    for(int q = 0 ; q < 1000 ; q++){
        for (int i = 0; i < ant_count; i++) {
            //每一隻皆要與剩下的點做計算
            for (int j = 0; j < CityCount; j++) {
                double denominator = 0.0;
                double prob_total = 0;
                if (j == 0) //第一次隨機選點
                {
                    int select = rand() % CityCount;
                    ants[i].route[j] = select;
                    ants[i].allowed_city[select] = 1;
                    ants[i].probability[select] = 0;
                } else//根據機率
                {
                    //先算分母
                    for (int k = 0; k < CityCount; k++) {
                        if (ants[i].allowed_city[k] == 0) {
                            denominator = denominator + pow((1.0 / city.Eta_vector[ants[i].route[j - 1]][k]), beta) *
                                                        pow((city.Tau_vector[ants[i].route[j - 1]][k]), alpha);
                        }
                    }
                    //再算分子
                    for (int k = 0; k < CityCount; k++) {
                        if (ants[i].allowed_city[k] == 0) {
                            ants[i].probability[k] = pow((1.0 / city.Eta_vector[ants[i].route[j - 1]][k]), beta) *
                                                     pow((city.Tau_vector[ants[i].route[j - 1]][k]), alpha) / denominator;
                            prob_total += ants[i].probability[k];
                        } else
                            ants[i].probability[k] = 0;
                    }

                    //Roulette wheel selection
                    double deal = get_Randon(0, prob_total);
                    //cout << prob_total << endl;
                    double wheel = 0;
                    for (int k = 0; k < CityCount; k++) {
                        if (ants[i].allowed_city[k] == 0) {
                            wheel += ants[i].probability[k];
                        }
                        if (wheel >= deal) {
                            ants[i].route[j] = k;
                            ants[i].allowed_city[k] = 1;
                            ants[i].probability[k] = 0;
                            break;

                        }
                    }
                }
            }
            //計算每一隻螞蟻的 總路徑長
            double total_distance = 0;
            for (int j = 0; j < CityCount - 1; j++) {
                int node1 = ants[i].route[j];
                int node2 = ants[i].route[j + 1];
                //cout << node1 << node2<<endl;
                total_distance += cal_Distance(File_Vector, node1, node2);
            }
            total_distance += cal_Distance(File_Vector, ants[i].route[0], ants[i].route[CityCount-1]);
            ants[i].fitness = total_distance;


        }
        //更新費洛蒙
        //找出50隻最好路徑的那隻
        double Min = 10000;
        int Min_index = 0;
        for (int i = 0; i < ant_count; i++) {
            if (ants[i].fitness < Min) {
                Min = ants[i].fitness;
                Min_index = i;
            }
        }
        //與bestAnt比較
        if(Min < bestAnt.fitness){
            for(int i = 0 ; i < CityCount ; i++){
                bestAnt.route[i] = ants[Min_index].route[i];
            }
            bestAnt.fitness = Min;
            bestAnt.index = Min_index;
        }


        for (int j = 0; j < CityCount; j++) {
            for (int k = 0; k < CityCount; k++) {
                city.Tau_vector[j][k] = pheromone_fallrate * city.Tau_vector[j][k];
            }
        }

        for (int j = 0; j < CityCount - 1; j++) {
            int node1 = bestAnt.route[j];
            int node2 = bestAnt.route[j + 1];
            city.Tau_vector[node1][node2] += (Q / bestAnt.fitness);
        }
        city.Tau_vector[bestAnt.route[0]][bestAnt.route[CityCount - 1]] += ( Q / bestAnt.fitness);
        //cout << "\n" << "fitness" << "\n";
        cout << bestAnt.fitness << "\t"<<endl;

        //clear ant[i]
        for (int i = 0; i < ant_count; i++) {
            for (int j = 0; j < CityCount; j++) {
                ants[i].route[j] = 0;
                ants[i].allowed_city[j] = 0;
                ants[i].probability[j] = 0;
            }
            ants[i].fitness = 0;
        }
    }

    return 0;
}