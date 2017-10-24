#include<iostream>
#include<fstream>
#include <sstream>
#include <cmath>
#include <thread>
#include <cstring>
using namespace std;
int get_RandonNum(int Num);
int get_RandonLocality(int Min, int Max);

void get_Sample();

int main() {
    cout << "start" << endl;
    srand((unsigned)time(NULL));

    get_Sample();

    return 0;
}

void get_Sample() {
    int Ref_str = 350;
    int Numofmem_ref = 75000;
    int Ref_str_RandonArray[Numofmem_ref];
    int Ref_str_LocalityArray[Numofmem_ref];
    char* path = "..\\";

    string Sample_Randon = "Sample_Randon.txt";
    ofstream fout_Randon( path + Sample_Randon);
    if ( fout_Randon ) {
        for(int i = 0 ; i < Numofmem_ref;i++) {
            int RandNum = get_RandonNum(Ref_str);
            Ref_str_RandonArray[i] = RandNum;
            fout_Randon << RandNum;
            fout_Randon << "\t";
        }
        cout << "success" <<endl;
        fout_Randon.close();
    } else{
        cout << "fail" <<endl;
    }

    string Sample_Locality = "Sample_Locality.txt";
    ofstream fout_Locality( path + Sample_Locality);
    if ( fout_Locality ) {
        int Min =  Ref_str / 6 ;
        int Max =  Ref_str / 4 ;
        int count = 0 ;
        while (count < 70000){
            int LocalitySize = get_RandonLocality(Min, Max);//用於曲每次locality大小 350 /6  < LocalitySize < 350 / 4
            int LocalityStart = get_RandonLocality(0, Ref_str - Max); // 0 ~ (350 - Max)
            int LocalityEnd = LocalityStart + LocalitySize ; // ( (0 ~ (350 - Max)) + LocalitySize )< 350
            for(int i = 0 ; i < LocalitySize ; i ++ ){
                int RandNum = get_RandonLocality(LocalityStart,LocalityEnd);
                Ref_str_LocalityArray[i] = RandNum;
                fout_Locality << RandNum;
                fout_Locality << "\t";
            }
            count += LocalitySize;
        }
        cout << "success" <<endl;
        fout_Locality.close();
    } else{
        cout << "fail" <<endl;
    }
}

int get_RandonLocality(int Min, int Max)
{
    double R = double(rand()) / double(RAND_MAX) ;
    int RandonNum  = (int)(R * (Max - Min));
    return  RandonNum + Min;
}

int get_RandonNum(int Num) {
    double R = double(rand()) / (RAND_MAX + 1.0);
    int RandonNum  = (int)(Num * R);
    return RandonNum;
}