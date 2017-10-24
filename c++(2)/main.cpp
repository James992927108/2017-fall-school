#include<iostream>
#include<fstream>
#include <sstream>
#include <cmath>
#include <thread>
#include <cstring>
using namespace std;
int get_RandonNum(int Num);
int get_RandonLocality(int Min, int Max);


void RandonSample(const char *path);

void LocalitySample(const char *path);

void getSample(const char *path);

void FIFO_Randon(const char *path, int FrameSize);

void FIFO_Locality(const char *path, int FrameSize);

const int Ref_str = 350;
const int Numofmem_ref = 75000;

int Ref_str_RandonArray[Numofmem_ref];

int Ref_str_LocalityArray[Numofmem_ref];

int main() {
    srand((unsigned)time(NULL));

    char* path = "..\\";
    getSample(path);

    int FrameSize = 5;
    FIFO_Randon(path, FrameSize);
    //-------------------------------
    FIFO_Locality(path, FrameSize);
    //-------------------------------

    return 0;
}

void FIFO_Locality(const char *path, int FrameSize) {
    int Frame[FrameSize];
    for(int i = 0 ; i < FrameSize ; i ++){//初始化
        Frame[i] = 0;
    }
    string FIFO_Locality = "FIFO_Locality.txt";
    ofstream FIFO( path + FIFO_Locality);
    int flag = 0;
    int pagefault = 0;
    for(int i = 0 ;i < Numofmem_ref ; i++){
        FIFO << i;
        FIFO << "\t";
        int input = Ref_str_LocalityArray[i];
        bool check ; //check是否存在frame裡面
        int count = 0;
        for(int i = 0 ;i < FrameSize ;i++){
            if( Frame[i]!= input ){
                count++;
            }else{
                check = false;
            }
            if(count == FrameSize)
                check = true;
        }
        if(check == 1){//input 不在frame中
            int temp = Frame[flag];
            Frame[flag] = input;
            FIFO << Frame[flag];
            FIFO << "\t";
            for(int k = 0 ;k <FrameSize ;k++) {
                FIFO << Frame[k];
                FIFO << "\t";
            }
            FIFO << "swap out : ";
            FIFO << temp;
            FIFO << "\n";
            pagefault++ ;
            flag++;
        }
        else{//input 在frame中
            FIFO << Frame[flag];
            FIFO << "\t";
            for(int i = 0 ;i <FrameSize ;i++) {
                FIFO << Frame[i];
                FIFO << "\t";
            }
            FIFO << "\n";
        }
        if(flag == 5){
            flag = 0 ;
        }
    }
    printf("\nFIFO Locailty \tpagefault %d",pagefault);
    FIFO << "pagefault : ";
    FIFO << pagefault;
    FIFO.close();
}

void FIFO_Randon(const char *path, int FrameSize) {
    int Frame[FrameSize];
    for(int i = 0 ; i < FrameSize ; i ++){//初始化
        Frame[i] = 0;
    }
    string FIFO_Randon = "FIFO_Randon.txt";
    ofstream FIFO( path + FIFO_Randon);

    int flag = 0;
    int pagefault = 0;
    for(int i = 0 ;i < Numofmem_ref ; i++){
        FIFO << i;
        FIFO << "\t";
        int input = Ref_str_RandonArray[i];
        //check是否存在frame裡面
        bool check ;
        int count = 0;
        for(int i = 0 ;i < FrameSize ;i++){
            if( Frame[i]!= input ){
                count++;
            } else{
                check = false;
            }
            if(count == FrameSize)
                check = true;
        }
        if(check == 1){//input 不在frame中
            int temp = Frame[flag];
            Frame[flag] = input;
            FIFO << Frame[flag];
            FIFO << "\t";
            for(int k = 0 ;k <FrameSize ;k++) {
                FIFO << Frame[k];
                FIFO << "\t";
            }
            FIFO << "swap out : ";
            FIFO << temp;
            FIFO << "\n";
            pagefault++ ;
            flag++;
        }
        else{//input 在frame中
            FIFO << Frame[flag];
            FIFO << "\t";
            for(int i = 0 ;i <FrameSize ;i++) {
                FIFO << Frame[i];
                FIFO << "\t";
            }
            FIFO << "\n";
        }
        if(flag == 5){
            flag = 0 ;
        }
    }
    printf("FIFO Randon \tpagefault %d",pagefault);
    FIFO << "pagefault : ";
    FIFO << pagefault;
    FIFO.close();
}

void getSample(const char *path) {
    RandonSample(path);
    LocalitySample(path);
}

void LocalitySample(const char *path) {
    string Sample_Locality = "Sample_Locality.txt";
    ofstream fout_Locality( path + Sample_Locality);
    if ( fout_Locality ) {
        int Min =  Ref_str / 6 ;
        int Max =  Ref_str / 4 ;
        int count = 0 ;
        while (count < Numofmem_ref){
            int LocalitySize = get_RandonLocality(Min, Max);//用於曲每次locality大小 350 /6  < LocalitySize < 350 / 4
            int LocalityStart = get_RandonLocality(0, Ref_str - Max); // 0 ~ (350 - Max)
            int LocalityEnd = LocalityStart + LocalitySize ; // ( (0 ~ (350 - Max)) + LocalitySize )< 350
            for(int i = count ; i < count + LocalitySize ; i ++ ){
                int RandNum = get_RandonLocality(LocalityStart,LocalityEnd);
                Ref_str_LocalityArray[i] = RandNum;
                fout_Locality << RandNum;
                fout_Locality << "\t";
            }
            count += LocalitySize;
        }
        printf("success Sample_Locality.txt\n");
        fout_Locality.close();
    } else{
        printf("fail Sample_Locality\n");
    }
}

void RandonSample(const char *path) {
    string Sample_Randon = "Sample_Randon.txt";
    ofstream fout_Randon( path + Sample_Randon);
    if ( fout_Randon ) {
        for(int i = 0 ; i < Numofmem_ref;i++) {
            int RandNum = get_RandonNum(Ref_str);
            Ref_str_RandonArray[i] = RandNum;
            fout_Randon << RandNum;
            fout_Randon << "\t";
        }
        printf("success Sample_Randon.txt\n");
        fout_Randon.close();
    } else{
        printf("fail Sample_Randon.txt\n");
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