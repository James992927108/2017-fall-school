#include<iostream>
#include<fstream>
#include <sstream>
#include <cmath>
#include <thread>
#include <cstring>
using namespace std;
int get_RandonNum(int Num);
int get_RandonLocality(int Min, int Max);


const int Numofmem_ref = 75000;
const int Ref_str = 350;
struct SampleRandon{
    int Ref_str_RandonArray[Numofmem_ref];
};
struct SampleLocality{
    int Ref_str_LocalityArray[Numofmem_ref];
};


SampleRandon get_SampleRandon(SampleRandon sample);
SampleLocality get_SampleLocality(SampleLocality sample);

const int FrameSize = 5;
struct Frame{
    int Frame[FrameSize];
};

bool checkexist(int FrameSize,Frame frame,int input);

void FIFO_Randon(SampleRandon Sampleresult);
void FIFO_Locality(SampleRandon Sampleresult);
int main() {
    srand((unsigned)time(NULL));

    SampleRandon sampleRandon;
    SampleRandon sampleRandonResult = get_SampleRandon(sampleRandon);
    FIFO_Randon(sampleRandonResult);

    SampleLocality sampleLocality;
    SampleLocality sampleLocalityResult = get_SampleLocality(sampleLocality);

    return 0;
}

void FIFO_Randon(SampleRandon Sampleresult) {//FIFO
    Frame frame;
    char* path = "..\\";
    string FIFO_Randon = "FIFO_Randon.txt";
    ofstream FIFO( path + FIFO_Randon);

    int flag = 0;
    int pagefault = 0;
    for(int i = 0 ;i < Numofmem_ref ; i++){
        FIFO << i;
        FIFO << "\t";
        int input_Randon = Sampleresult.Ref_str_RandonArray[i];

        bool check = checkexist(FrameSize,frame,input_Randon);

        if(check == 1){//input 不在frame中
            int temp = frame.Frame[flag];
            frame.Frame[flag] = input_Randon;
            FIFO << frame.Frame[flag];
            FIFO << "\t";
            for(int k = 0 ;k <FrameSize ;k++) {
                FIFO << frame.Frame[k];
                FIFO << "\t";
            }
            FIFO << "swap out : ";
            FIFO << temp;
            FIFO << "\n";
            pagefault++ ;
            flag++;
        }
        else{//input 在frame中
            FIFO << frame.Frame[flag];
            FIFO << "\t";
            for(int i = 0 ;i <FrameSize ;i++) {
                FIFO << frame.Frame[i];
                FIFO << "\t";
            }
            FIFO << "\n";
        }
        if(flag == 5){
            flag = 0 ;
        }
    }
    printf("pagefault %d",pagefault);
    FIFO << "pagefault : ";
    FIFO << pagefault;
    FIFO.close();
}



bool checkexist(int FrameSize,Frame frame,int input){

    int count = 0;
    for(int i = 0 ;i < FrameSize ;i++){
        if(frame.Frame[i]!= input ){
            count++;
        } else{
            return false;
        }
        if(count == FrameSize)
            return true;
    }
}

SampleLocality get_SampleLocality(SampleLocality sample){
    char* path = "..\\";
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
                sample.Ref_str_LocalityArray[i] = RandNum;
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
    return sample;
}
SampleRandon get_SampleRandon(SampleRandon sample) {

    char* path = "..\\";

    string Sample_Randon = "Sample_Randon.txt";
    ofstream fout_Randon( path + Sample_Randon);
    if ( fout_Randon ) {
        for(int i = 0 ; i < Numofmem_ref;i++) {
            int RandNum = get_RandonNum(Ref_str);
            sample.Ref_str_RandonArray[i] = RandNum;
            fout_Randon << RandNum;
            fout_Randon << "\t";
        }
        printf("success Sample_Randon.txt\n");
        fout_Randon.close();
    } else{
        printf("fail Sample_Randon.txt\n");
    }
    return sample;
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