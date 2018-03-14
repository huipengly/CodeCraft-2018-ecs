#include "predict.h"
#include "matrix.h"
#include <stdio.h>
#include <iostream>
#include "cstring"
#include <vector>

using namespace std;

//输入信息
PhysicalMachine physical_machine;               //物理机信息
bool flavor_type_to_predict[16] = {false};     //需要预测的虚拟机类型，共15中，需要预测的为true。0代表flavor0， 1代表flavor1
char *resources_to_optimize = new char[4]();   //0为优化cpu，1为优化memory
int predict_day = 0;                            //预测时间范围
struct tm predict_start_time = {0};             //预测开始时间
struct tm predict_end_time = {0};                      //预测结束时间
int info_status = 0;

//flavor训练信息
//FlavorDemand history_demand[MAX_DATA_NUM] = {0};
vector<vector<FlavorDemand>> history_demand(MAX_INFO_NUM);

//预测信息
vector<vector<FlavorDemand>> predict_demand(MAX_INFO_NUM);
extern char *strptime(const char *buf, const char *fmt, struct tm *tm);
//你要完成的功能总入口
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{
    readInfo(info);

    readData(data, data_num);

//    double a[] = {1, 2, 3, 4, 5, 6};
//    double b[] = {4, 3, 2, 1};
//    double *res = new double[3 * 2];
//
//    matrix_mul(a, 3, 2, b, 2, 2, res);

//    double a[] = {1, 2, 3, 4, 5, 6,7,8,9};
//    double *b = new double[3*3];
//    matrix_trans(a, 3, 3, b);

    double mydata[58] = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,2,0,2,0,0,0,0,0,0,2,0,0,0,0,3,0,10,0};    //flavor2数据
    int mydata_num = 50;
    
    //最小二乘拟合
    int fun_num = 19;
//    double *H = new double[(mydata_num-fun_num)*fun_num];
    double H[(31)*19] = {0};
    for(int i = 0; i < mydata_num - fun_num; i++ )
    {
        for(int j = 0; j < fun_num; j++)
        {
            H[i*fun_num + j] = mydata[i + fun_num - j - 1];
        }
    }

    //inv(H'*H)*H'*(temp);
//    double *H_trans = new double[fun_num*(mydata_num-fun_num)];
    double H_trans[19*(31)] = {0};
    memset(H_trans,0,sizeof(H_trans));
    matrix_trans(H, mydata_num-fun_num, fun_num, H_trans);

//    double *HH = new double[fun_num * fun_num];
    double HH[19 * 19] = {0};
    memset(HH,0,sizeof(HH));
    matrix_mul(H_trans, fun_num, mydata_num-fun_num, H, mydata_num-fun_num, fun_num, HH);

//    double *HH_inv = new double[fun_num * fun_num];
    double HH_inv[19 * 19] = {0};
    memset(HH_inv,0,sizeof(HH_inv));
    bool te = Gauss(HH, HH_inv, fun_num);//FIXME:19阶在这里有bug

//    double *res1 = new double[fun_num * (mydata_num - fun_num)];
    double res1[19 * (31)] = {0};
    memset(res1,0,sizeof(res1));
    matrix_mul(HH_inv, fun_num, fun_num, H_trans, fun_num, mydata_num-fun_num, res1);

//    double *res = new double[fun_num];
    double res[19] = {0};
    memset(res,0,sizeof(res));
    matrix_mul(res1, fun_num, mydata_num-fun_num, mydata+fun_num, mydata_num-fun_num, 1, res);

	// 需要输出的内容
	char * result_file = (char *)"5\nflavor1 1\nflavor2 1\nflavor3 1\nflavor4 2\nflavor5 2\n\n1 flavor1 1 flavor2 1 flavor3 1 flavor4 2 flavor5 2";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(result_file, filename);
}

void readInfo(char * info[MAX_INFO_NUM])
{
    int temp_flavor = -1;
    int flavor_number = 0;
    sscanf(info[0], "%d %d", &physical_machine.cpu, &physical_machine.memory);                      //物理机信息
    sscanf(info[2], "%d", &flavor_number);                                                          //需要预测的个数
    for(int i = 3; i < 3 + flavor_number; i++)                                                      //需要预测的种类
    {
        sscanf(info[i], "flavor%d", &temp_flavor);
        flavor_type_to_predict[temp_flavor] = true;
    }
    strcpy(resources_to_optimize, info[3 + flavor_number + 1]);
    strptime(info[3 + flavor_number + 3], "%Y-%m-%d %H:%M:%S", &predict_start_time);                //开始时间
    strptime(info[3 + flavor_number + 4], "%Y-%m-%d %H:%M:%S", &predict_end_time);                  //结束时间
    predict_day = static_cast<int>(
            (static_cast<long>(mktime(&predict_end_time)) - static_cast<long>(mktime(&predict_start_time))) / (60 * 60 * 24));       //计算预测天数
}

void readData(char * data[MAX_INFO_NUM], int &data_num)
{
    char temp_time[50];
    FlavorDemand temp_history_demand = {0};
    for(int i = 0; i < data_num; i++)
    {
        sscanf(data[i],"%*[a-z0-9-]\tflavor%d\t%s", &temp_history_demand.type, temp_time);
        strptime(temp_time, "%Y-%m-%d %H:%M:%S", &temp_history_demand.create_time);
        history_demand[temp_history_demand.type].push_back(temp_history_demand);
    }
}