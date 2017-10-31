#include <iostream>
#include <time.h>
#include <process.h>
#include <cmath>
#include <vector>
using namespace std;
typedef vector<vector<double>> d2d;
typedef vector<double> d1d;

struct returnType{
    double value = 0;
    int index = 0 ;
};
double get_Randon(double Min, double Max){
    double rand_num = (Max - Min) * rand() / RAND_MAX + (Min);
    return rand_num;
}
void Print_particle_array(d2d &particle_array) {
    printf("\nparticle_array");
    int particle_count = particle_array.size();
    int particle_dimension = particle_array[0].size();
    for(int i = 0 ; i < particle_count ; i++){
        printf("\n%d\t",i);
        for(int j = 0 ;j < particle_dimension ;j++){
            printf("%f\t",particle_array[i][j]);
        }
    }
}

void Print_fitness_array(d1d &fitness_array) {
    printf("\nfitness_array");
    int fitness_count = fitness_array.size();
    for(int index = 0 ; index < fitness_count; index ++){
        printf("\n%d\t%f",index,fitness_array[index]);
    }
}

void Print_selfbest_array(d1d &selfbest_array){
    printf("\nselfbest_array");
    int selfbest_count = selfbest_array.size();
    for(int i = 0 ;i < selfbest_count ; i++){
        printf("\n%d\t%f",i,selfbest_array[i]);
    }
}


double cal_Fitness(int index,d2d particle_array){
    int dim = particle_array[0].size();//取得二維大小 10*d
    int temp = 10 * dim;
    double temp1 = 0;
    for(int i = 0 ;i < dim ;i++){
        double xi_square = particle_array[index][i] * particle_array[index][i];
        double yi = 10 * cos(2*M_PI*particle_array[index][i]);
        temp1 = temp1 + (xi_square-yi);
    }
    double Fitness = temp + temp1;
    return Fitness;
}

d1d get_fitness_array(d2d particle_array, d1d fitness_array) {
    int particle_count = particle_array.size();
    for(int index = 0 ; index < particle_count; index ++){
        fitness_array[index] = cal_Fitness(index,particle_array);
    }
    return fitness_array;
}

void compare_fitAndself(d1d fitness_array, d1d& selfbest_array,d2d particle_array,d2d& best_particle_array) {
    for(int i = 0 ; i < fitness_array.size() ;i++){
        if(fitness_array[i] < selfbest_array[i]){
            selfbest_array[i] = fitness_array[i];
            for(int j = 0 ; j < particle_array[0].size() ; j ++) { //若發現第i較好的fiftness時，更新best_particle_array[i][n]，複製particle_array[i][n]的
                best_particle_array[i][j] = particle_array[i][j];
            }
        }
    }
}
returnType find_bestIn(d1d selfbest_array){
    double Min = 10000;
    int Min_Index = 0;
    for(int i = 0 ; i < selfbest_array.size() ; i++){
        if(selfbest_array[i] < Min){
            Min = selfbest_array[i];
            Min_Index = i;
        }
    }
    returnType temp;
    temp.value = Min;
    temp.index = Min_Index;
    return temp;
}

d2d inital(d2d particle_array) {
    int particle_count = particle_array.size();
    int particle_dimension = particle_array[0].size();
    for(int i = 0 ; i < particle_count ; i++){
        for(int j = 0 ;j < particle_dimension ;j++){
            particle_array[i][j] = get_Randon(-5.12,5.12);
        }
    }
    return particle_array;
}

int main() {
    srand((unsigned) time(NULL) + getpid());

    int particle_count = 20 ;
    int particle_dimension = 2;
    double globalbest[particle_dimension + 1];
    d2d particle_array(particle_count, d1d(particle_dimension, 0.0));

    d2d best_particle_array(particle_count, d1d(particle_dimension, 0.0));

    d1d fitness_array(particle_count);//計算每一個particle的fitness，存放的array

    d1d selfbest_array(particle_count,10000.0);//更新每一個particle的最好

    particle_array = inital(particle_array);

    Print_particle_array(particle_array);

    fitness_array = get_fitness_array(particle_array, fitness_array);

    Print_fitness_array(fitness_array);

    //比較selfbest_array與fitness_array大小並更新
    //若發現第i較好的fiftness時，更新best_particle_array[i][n]，複製particle_array[i][n]的,
    //更新selfbest_array,best_particle_array
    compare_fitAndself(fitness_array,selfbest_array,particle_array,best_particle_array);

    Print_particle_array(best_particle_array);
    Print_selfbest_array(selfbest_array);

    //從selfbest_array中找出最好fitness和index用於找best_particle_array的值放temp_best
    returnType temp_best = find_bestIn(selfbest_array);
    printf("\ntemp_best\t%d\t%f",temp_best.index,temp_best.value);

    //初始globalbest[1+dim]
    globalbest[0] = 10000;
    for(int i = 1 ; i< particle_dimension + 1 ; i++){
        globalbest[i] = 0;
    }
    // temp_best 與 globalbest比較
    printf("\nglobalbest");
    if(temp_best.value < globalbest[0]){
        globalbest[0] = temp_best.value;
        for(int j = 1 ;j < particle_dimension + 1 ;j++){
            globalbest[j] = best_particle_array[temp_best.index][j-1];
        }
    }
    for(int i = 0 ; i< particle_dimension + 1 ; i++){
       printf("\t%f",globalbest[i]);
    }

    //更新速度，在更新位置
    int w = 1;
    double c1 = 0.4,c2 = 0.4;
    double v[particle_count][particle_dimension];

    for(int index = 0 ; index < particle_count ;index++) {
        for (int dimension = 0; dimension < particle_dimension; dimension++) {
            v[index][dimension] = 0;
        }
    }

    for(int index = 0 ; index < particle_count ;index++) {
        for (int dimension = 0; dimension < particle_dimension; dimension++) {
            v[index][dimension] = w * v[index][dimension]
                   + c1 * get_Randon(0, 1) * (selfbest_array[dimension] - particle_array[index][dimension])
                   + c2 * get_Randon(0, 1) * (globalbest[dimension] - particle_array[index][dimension]) ;
        }
    }
    printf("\n v");
    for(int index = 0 ; index < particle_count ;index++) {
        printf("\n%d",index);
        for (int dimension = 0; dimension < particle_dimension; dimension++) {
            printf("\t%f\t",v[index][dimension]);
        }
    }

    for(int index = 0 ; index < particle_count ;index++) {
        for (int dimension = 0; dimension < particle_dimension; dimension++) {
            particle_array[index][dimension] = particle_array[index][dimension] + v[index][dimension];
            if(particle_array[index][dimension] > 5.12){
                particle_array[index][dimension] = 5.12;
            }
        }
    }
    printf("\n updata");
    Print_particle_array(particle_array);
    return 0;
}



