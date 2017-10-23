#include<iostream>
#include<fstream>
#include <sstream>
#include <cmath>
#include <thread>
#include <cstring>
using namespace std;
int get_RandonNum(int Num);

bool CreatFile(const string &filename);

int main() {
    cout << "start" << endl;
    srand((unsigned)time(NULL));

    int Ref_str = 350;
    int Numofmem_ref = 75000;
    int Ref_strRandonArray[Numofmem_ref];


    string filename = "randon.txt";

    bool creatfilestate = CreatFile(filename);

    if(creatfilestate){
        fstream file;
        file.open(filename,ios :: out);
        //randon

        for(int i = 0 ; i < Numofmem_ref;i++) {
            int RandNum = get_RandonNum(Ref_str);
            Ref_strRandonArray[i] = RandNum;
            file << RandNum;
            file << "\t";
        }
        file.close();
    }

    return 0;
}

bool CreatFile(const string &filename) {
    char* path = "E:\\GitHub\\HW_OS_PageReplacement\\"; // 你要创建文件的路径
    ofstream fout( path + filename);
    if ( fout ) { // 如果创建成功
        //fout << "写入内容" << endl; // 使用与cout同样的方式进行写入
        cout << "success" <<endl;
        fout.close();  // 执行完操作后关闭文件句柄
        return true;
    } else{
        cout << "fail" <<endl;
        return false;
    }
}

int get_RandonNum(int Num) {
    double R = ((double)rand()) / (RAND_MAX + 1.0);
    int RandonNum  = (int)(Num * R);
    return RandonNum;
}