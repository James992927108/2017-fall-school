#include <iostream>
#include <cmath>
#include <process.h>
#include <time.h>

const int Dimension = 2;
struct ParticleStruct{
    double Particle[Dimension];
    double Fitness = 0;
    double BestParticle[Dimension];
    double BestFitness = 0;
    double Speed[Dimension];
};
struct GloabStruct{
    double GloabParticle[Dimension];
    double GloabFitness = 0;
};

double get_Randon(double Min, double Max){
    double rand_num = (Max - Min) * rand() / RAND_MAX + (Min);
    return rand_num;
}
double cal_Fitness(ParticleStruct ParticleArray_index){
    int temp = 10 * Dimension;
    double temp1 = 0;
    for (int i = 0; i < Dimension; i++) {
        double xi_square = ParticleArray_index.Particle[i] * ParticleArray_index.Particle[i];
        double yi = 10 * cos(2 * M_PI * ParticleArray_index.Particle[i]);
        temp1 = temp1 + (xi_square - yi);
    }
    return  temp + temp1;
}
void print_ParticleArray(int ParticleSize,ParticleStruct ParticleArray[]){
    for (int i = 0; i < ParticleSize; i++) {
        printf("\n%d\t", i);
        for (int j = 0; j < Dimension; j++) {
            printf("%f\t", ParticleArray[i].Particle[j]);
        }
        printf("%f\t", ParticleArray[i].Fitness);
        for (int j = 0; j < Dimension; j++) {
            printf("%f\t", ParticleArray[i].BestParticle[j]);
        }
        printf("%f\t", ParticleArray[i].BestFitness);
        for (int j = 0; j < Dimension; j++) {
            printf("%f\t", ParticleArray[i].Speed[j]);
        }
    }
}

int main() {
    srand((unsigned) time(NULL) + getpid());
    int ParticleSize = 20 ;    //宣告一個一個大小維20的array or vector 使用ParticleStruct 型態
    ParticleStruct ParticleArray[ParticleSize];
    GloabStruct Gloab;
    //----------------------------------------------------------------------------
    //inital ParticleArray
    for(int i = 0 ;i <ParticleSize ; i++) {
        for (int j = 0; j < Dimension; j++) {
           ParticleArray[i].Particle[j] = 0;
        }
        ParticleArray[i].Fitness = 0;
        for (int j = 0; j < Dimension; j++) {
             ParticleArray[i].BestParticle[j] = 0;
        }
        ParticleArray[i].BestFitness = 1000;
        for (int j = 0; j < Dimension; j++) {
            ParticleArray[i].Speed[j] = 0;
        }
    }
    // intital Gloab
    for(int i = 0 ;i < Dimension ; i++){
        Gloab.GloabParticle[i] = 0 ;
    }
    Gloab.GloabFitness = 1000;

    //----------------------------------------------------------------------------
    //取得ParticleArray中每一個particle的每一維度資料
    for(int i = 0 ; i < ParticleSize ;i ++){
        for(int j = 0;j < Dimension ; j++) {
            ParticleArray[i].Particle[j] = get_Randon(-5.12 , 5.12);
        }
    }
    //----------------------------------------------------------------------------
    for(int RunTime = 0 ; RunTime < 100 ;RunTime++) {
        //計算ParticleArray中每一個Fitness
        //fitness = 10d +sum (particle[d]*particle[d] - 10 cos(2 pi * particle[d])
        for (int index = 0; index < ParticleSize; index++) {
            ParticleArray[index].Fitness = cal_Fitness(ParticleArray[index]);
        }
        //----------------------------------------------------------------------------

        //比較當前的ParticleArray的Fitness與BestFitness,若較佳則更新BestParticle[dim] = Particle[dim]，
        //和BestFitness = Fitness
        for (int i = 0; i < ParticleSize; i++) {
            if (ParticleArray[i].Fitness < ParticleArray[i].BestFitness) {
                ParticleArray[i].BestFitness = ParticleArray[i].Fitness;
                for (int j = 0; j < Dimension; j++) {
                    ParticleArray[i].BestParticle[j] = ParticleArray[i].Particle[j];
                }
            }
        }
        //----------------------------------------------------------------------------
        //ParticleArray[i].BestFitness 與Gloab比較，
        // 若較好紀錄Gloab.GloabFitness = ParticleArray[i].BestFitness
        // 和紀錄當前得ParticleArray[i]中Particle[j]於Gloab中GloabParticle[j]

        for (int i = 0; i < ParticleSize; i++) {
            if (ParticleArray[i].BestFitness < Gloab.GloabFitness) {
                Gloab.GloabFitness = ParticleArray[i].BestFitness;
                for (int j = 0; j < Dimension; j++) {
                    Gloab.GloabParticle[j] = ParticleArray[i].Particle[j];
                }
            }
        }

        int w = 1;
        double c1 = 0.4, c2 = 0.4;
        //----------------------------------------------------------------------------
        //更新ParticleArray[i].Particle[j]對應的ParticleArray[i].Speed[j]
        for (int i = 0; i < ParticleSize; i++) {
            for (int j = 0; j < Dimension; j++) {
                ParticleArray[i].Speed[j] = w * ParticleArray[i].Speed[j]
                                            + c1 * get_Randon(0, 1) *
                                              (ParticleArray[i].BestParticle[j] - ParticleArray[i].Particle[j])
                                            + c2 * get_Randon(0, 1) *
                                              (Gloab.GloabParticle[j] - ParticleArray[i].Particle[j]);

            }
        }
        //跟更新ParticleArray[i].Particle[j]
        for (int i = 0; i < ParticleSize; i++) {
            for (int j = 0; j < Dimension; j++) {
                ParticleArray[i].Particle[j] = ParticleArray[i].Particle[j] + ParticleArray[i].Speed[j];
                if (ParticleArray[i].Particle[j] > 5.12)
                    ParticleArray[i].Particle[j] = 5.12;
                if (ParticleArray[i].Particle[j] < -5.12)
                    ParticleArray[i].Particle[j] = -5.12;
            }
        }
        //----------------------------------------------------------------------------

        //印出ParticleArray
        print_ParticleArray(ParticleSize,ParticleArray);
    }
    printf("\nGloabParticle\n");
    for(int i = 0 ;i < Dimension ; i++){
        printf("%f\t",Gloab.GloabParticle[i]);
    }
    printf("\nGloabFitness\n");
    printf("%f\t",Gloab.GloabFitness);

    //----------------------------------------------------------------------------
    return 0;
}