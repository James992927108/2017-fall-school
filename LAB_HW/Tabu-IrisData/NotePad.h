#include <iostream>
#include <fstream> //讀檔
#include <string>
#include <typeinfo> //查type
#include <sstream> //分割字串及string int間轉換
#include <stdlib.h> //rand
#include <time.h> //time
#include <math.h> //math
#include <algorithm> //min,max
#include <iomanip> //印小數點後幾位
#include <memory.h> //memset,memcpy
using namespace std;
const  int INT_MAX = 65536;
double Distance(double x0, double x1, double x2, double x3, double y0, double y1, double y2, double y3) {
	return pow((x0 - y0), 2) + pow((x1 - y1), 2) + pow((x2 - y2), 2) + pow((x3 - y3), 2);
}
double CalSSE(double matrix[150][5], double centroid[3][5], double x[150]) {//算距離
	double SSE = 0;

	centroid[0][4] = 0.0;//初始化
	centroid[1][4] = 0.0;
	centroid[2][4] = 0.0;

	for(int i = 0; i < 150; i++){
		matrix[i][4] = x[i];
	}
	////////////////////////////////////////////////////////NewCentroid
	double zero = 0.0, one = 0.0, two = 0.0;//求各歸屬個數
	for (int i = 0; i < 150; i++){
    	switch ((int)x[i]){
    		case 0:
    			zero++;
				break;
			case 1:
				one++;
				break;
			case 2:
				two++;
				break;
		  }
    }
	//cout<<"\nzero = "<<zero<<"\none = "<<one<<"\ntwo = "<<two<<endl;
	for (int j = 0; j < 4; j++){//算質心
		for (int i = 0; i < 150; i++){
			switch ((int)x[i]){
				case 0:
					centroid[0][j] = centroid[0][j] + matrix[i][j];
					break;
				case 1:
					centroid[1][j] = centroid[1][j] + matrix[i][j];
					break;
				case 2:
					centroid[2][j] = centroid[2][j] + matrix[i][j];
					break;
			}
        }
        centroid[0][j] = centroid[0][j] / zero;
        centroid[1][j] = centroid[1][j] / one;
        centroid[2][j] = centroid[2][j] / two;
    }
	////////////////////////////////////////////////////////
	for (int i = 0; i < 150; i++){
		switch ((int)matrix[i][4]){
			case 0:
				centroid[0][4] += Distance(matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3], centroid[0][0], centroid[0][1], centroid[0][2], centroid[0][3]);
				break;
			case 1:
				centroid[1][4] += Distance(matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3], centroid[1][0], centroid[1][1], centroid[1][2], centroid[1][3]);
				break;
			case 2:
				centroid[2][4] += Distance(matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3], centroid[2][0], centroid[2][1], centroid[2][2], centroid[2][3]);
				break;
		}
	}
	SSE = centroid[0][4] + centroid[1][4] + centroid[2][4];
	//cout<<fixed<<setprecision(8)<<"SSE = "<<SSE<<endl;
	return SSE;
}
double Neighborhood(double matrix[150][5], double x[150], double y[150]){
	int a = rand()%150+0;
	restart:
	int b = rand()%150+0;
	for(int i = 0; i < 150; i++){
		y[i] = x[i];
	}
	if(a==b){//比對過去產生的數字有相同就重新取
		goto restart;
	}
	else{
		if(y[a]==y[b]){
			goto restart;
		}
		/*int temp;
		temp = y[a];
		y[a] = y[b];
		y[b] = temp;*/
		switch ((int)y[a]){
    	case 0:
    		if((rand()%2+0)==0)
				y[a] = 1;
			else
				y[a] = 2;
			break;
		case 1:
			if((rand()%2+0)==0)
				y[a] = 0;
			else
				y[a] = 2;
			break;
		case 2:
			if((rand()%2+0)==0)
				y[a] = 0;
			else
				y[a] = 1;
			break;
		}
	}
}
bool CheckTabu(double TabuList[150][100], double x[150]) {
	int j;
	int flag = 0;
	for (j = 0; j < 100; j++){
		flag = 0;
		for (int i = 0; i < 150; i++){
			if(x[i] != TabuList[i][j]){
				flag = 1;
				break;
			}
		}
		if(flag == 0)
			break;
	}
	return !(j == 100);
}
double IntoTabu(double TabuList[150][100], double x[150]) {
	for (int i = 0; i < 99; i++){
		for (int j = 0; j < 150; j++){
			TabuList[j][i] = TabuList[j][i+1];
		}
		for (int k = 0; k < 150; k++){
			TabuList[k][99] = x[k];
		}
	}
}
double Print(double centroid[3][5], double TempSSE, double CurrSSE, double BestSSE, double Sol[150], double TempSol[150], double CurrSol[150], double BestSol[150], double TabuList[150][100]) {
	cout<<"\ncentroid = "<<endl;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 5; j++){
			cout<<centroid[i][j]<<' ';
		}
		cout<<endl;
	}
	cout<<"\nTempSSE = "<<TempSSE<<"\nCurrSSE = "<<CurrSSE<<"\nBestSSE = "<<BestSSE;
	/*cout<<"\nSol = ";
	for(int i = 0; i < 150; i++){
	  cout<<Sol[i];
	}
	cout<<"\nTempSol = ";
	for(int i = 0; i < 150; i++){
	  cout<<TempSol[i];
	}
	cout<<"\nCurrSol = ";
	for(int i = 0; i < 150; i++){
	  cout<<CurrSol[i];
	}
	cout<<"\nBestSol = ";
	for(int i = 0; i < 150; i++){
	  cout<<BestSol[i];
	}
	cout<<"\nTabuList = \n";
	for(int i = 0; i < 150; i++){
		for(int j = 0; j < 100; j++){
			cout<<TabuList[i][j]<<' ';
		}
		cout<<endl;
	}*/
}
int main(int argc, char** argv) {
	string text[150][4];
	double matrix[150][5];//[0~149][5]存歸屬於哪個質心
	double centroid[3][5];//[0~2][5]存歸屬於哪個質心的距離
	double BestCentroid[3][5];
	double TempSSE = 0;
	double CurrSSE = 0;
	double BestSSE = 0;
	double Sol[150];
	double TempSol[150];
	double CurrSol[150];
	double BestSol[150];
	double TabuList[150][100];//suggests using a tabu list size in the range [1/3 n, 3n], where n is related to the size of the problem
	memset(TabuList, 0, sizeof(TabuList));
	////////////////////////////////////////////////////////讀檔
	ifstream file;
	file.open("IrisData.txt", ios::in);
	if(!file){
		cout<<"read file failed\n";
		exit(0);
	}
	for(int i = 0; i < 150; i++){
		getline(file, text[i][0]);//一次讀一行
		stringstream ss(text[i][0]);
		for(int j = 0; j < 4; j++){
			//ss>>text[i][j];
			ss>>matrix[i][j];//string to int
		}
		ss.str("");//清空stringstream字串
		ss.clear();//清空stringstream
	}
	//cout<<fixed<<setprecision(1)<<"matrix[1][0] = "<<matrix[1][0]<<endl<<"matrix type = "<<typeid(matrix).name()<<endl;

	////////////////////////////////////////////////////////取亂數3點 (來自樣本)
	srand((unsigned)time(NULL));
	RandCen:
	for(int i = 0; i < 3; i++){
		for (int j = 0; j < 4; j++){
			int x = rand()%150+0;//0~150-1 比對過去產生的數字有相同就要重新取
			centroid[i][j] = matrix[x][j];
		}
	}
	////////////////////////////////////////////////////////算隨機3點與樣本距離
	//cout<<"Distance = "<<Distance(matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3], centroid[0][0], centroid[0][1], centroid[0][2], centroid[0][3])<<endl;
	for (int i = 0; i < 150; i++){
		double min = INT_MAX;//比較最小值用
		for (int j = 0; j < 3; j++){
        	double reg = Distance(matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3], centroid[j][0], centroid[j][1], centroid[j][2], centroid[j][3]);
        	if (reg < min){
        		min = reg;
        		matrix[i][4] = j;
			}
		}
	}
	double zero = 0, one = 0, two = 0;//判斷有沒有分配到0個點的
	for (int i = 0; i < 150; i++){
    	switch ((int)matrix[i][4]){
    		case 0:
    			zero++;
				  break;
			  case 1:
				  one++;
				  break;
			  case 2:
				  two++;
				  break;
		  }
    }
    if(zero==0||one==0||two==0)
    	goto RandCen;
	BestSSE = INT_MAX;
	CurrSSE = INT_MAX;
	TempSSE = INT_MAX;
	for(int i = 0; i < 150; i++){//初始化
		Sol[i] = matrix[i][4];
		//BestSol[i] = Sol[i];
		//cout<<BestSol[i];
	}//cout<<endl;
	memcpy(BestSol, Sol, sizeof(double)*150);
	BestSSE = CalSSE(matrix, centroid, Sol);
	Print(centroid, TempSSE, CurrSSE, BestSSE, Sol, TempSol, CurrSol, BestSol, TabuList);
	for (int i = 0; i < 1000; i++){//遞迴1

		CurrSSE = INT_MAX;
		for (int i = 0; i < 20; i++){//遞迴2
			Neighborhood(matrix, Sol, TempSol);
			if(!CheckTabu(TabuList, TempSol)){
				TempSSE = CalSSE(matrix, centroid, TempSol);
				if(TempSSE < CurrSSE){
					memcpy(CurrSol, TempSol, sizeof(double)*150);
					CurrSSE = TempSSE;
				}
			}
		}
		if(CurrSSE < BestSSE){
			memcpy(BestSol, CurrSol, sizeof(double)*150);
			BestSSE = CurrSSE;
		}
		memcpy(Sol, CurrSol, sizeof(double)*150);
		IntoTabu(TabuList, CurrSol);
	}
	Print(centroid, TempSSE, CurrSSE, BestSSE, Sol, TempSol, CurrSol, BestSol, TabuList);
	cout<<"\n\nBestSSE = ";
	cout<<BestSSE<<endl;
	for(int i = 0; i < 150; i++){
		cout<<BestSol[i];
	}
	return 0;
}
