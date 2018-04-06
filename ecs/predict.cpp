#include "predict.h"
#include "matrix.h"
#include "GA.h"
#include <stdio.h>
#include <iostream>
#include "cstring"
#include <vector>
#include <algorithm>
#include "BPnet.h"
#include "myMinMaxScaler.h"
#include "ARMA_Plus.h"

using namespace std;

#define MAX_FUNCTION_NUM 20

FlaverInfo flavor_info[16] =
        {{0}, {1,1,1}, {2,1,2}, {3,1,4}, {4,2,2}, {5,2,4}, {6,2,8}, {7,4,4}, {8,4,8}, {9,4,16}, {10,8,8}, {11,8,16}, {12,8,32}, {13,16,16}, {14,16,32}, {15,16,64}};

//输入信息
PhysicalMachine physical_machine;      //物理机信息
bool flavor_type_to_predict[16] = {false};     //需要预测的虚拟机类型，共15中，需要预测的为true。0代表flavor0， 1代表flavor1
char *resources_to_optimize = new char[4]();   //0为优化cpu，1为优化memory
int predict_day = 0; //预测时间范围
struct tm predict_start_time = {0};    //预测开始时间
struct tm predict_end_time = {0};    //预测结束时间
int info_status = 0;
int lookback = 3;

//flavor训练信息
vector<vector<FlavorDemand>> history_demand(MAX_INFO_NUM);
double train[MAX_INFO_NUM][MAX_DATA_NUM] = {0};
struct tm train_start_time = {0};      //训练开始时间
struct tm train_end_time = {0};        //训练结束时间
int train_day = 0;   //训练持续时间

//预测信息
vector<vector<FlavorDemand>> predict_demand(MAX_INFO_NUM);
double predict[MAX_INFO_NUM][MAX_DATA_NUM] = {0};
vector<int> vec_predict_demand(16, 0);
vector<double> vec_predict_demand_double(16, 0);
double allFlavorNum = 0;

extern int day_ma;
extern vector<vector<double>> trainMA;

//你要完成的功能总入口
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{
    srand(time(0));

    readInfo(info);

    readData(data, data_num);

    dataMoveAvg();

    predictDay();

    //ga分配
    vector<vector<int>> vec_outputs;
    genetic_algorithm(vec_predict_demand, vec_outputs, 50);

    int flavor_num(0);

    for(size_t i = 0; i < vec_outputs.size(); ++i)
    {
        for(size_t j = 0; j < vec_outputs[i].size(); ++j)
        {
            flavor_num += vec_outputs[i][j];
        }
    }

    //输出
    char result_file[100000] = "";
    char str_tmp[100] = "";
    sprintf(str_tmp,"%d\n",flavor_num);
    strcat(result_file,str_tmp);
    for(int i = 0; i < 16; i ++)
    {
        if(flavor_type_to_predict[i])
        {
            sprintf(str_tmp, "flavor%d %d\n", i, vec_predict_demand[i]);
            strcat(result_file, str_tmp);
        }
    }
    strcat(result_file,"\n");
    sprintf(str_tmp, "%d\n", static_cast<int>(vec_outputs.size()));
    strcat(result_file,str_tmp);
    for(size_t i = 0; i < vec_outputs.size(); i ++)
    {
        sprintf(str_tmp,"%d",i + 1);
        strcat(result_file,str_tmp);
//        for(auto it = vec_outputs[i].begin(); it != vec_outputs[i].end(); ++it)
        for(size_t j = 0; j < vec_outputs[i].size(); ++j)
        {
            if(flavor_type_to_predict[j])
            {
                if(vec_outputs[i][j] != 0)
                {
                    sprintf(str_tmp, " flavor%d %d", j, vec_outputs[i][j]);
                    strcat(result_file, str_tmp);
                }
            }
        }
        strcat(result_file,"\n");
    }

	// 需要输出的内容
//	char * result_file = (char *)"5\nflavor1 1\nflavor2 1\nflavor3 1\nflavor4 2\nflavor5 2\n\n1 flavor1 1 flavor2 1 flavor3 1 flavor4 2 flavor5 2";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(result_file, filename);
}

void readInfo(char * info[MAX_INFO_NUM])
{
    int temp_flavor = -1;
    int flavor_number = 0;
    sscanf(info[0], "%d %d", &physical_machine.cpu, &physical_machine.memory);    //物理机信息
    sscanf(info[2], "%d", &flavor_number);    //需要预测的个数
    for(int i = 3; i < 3 + flavor_number; i++)//需要预测的种类
    {
        sscanf(info[i], "flavor%d", &temp_flavor);
        flavor_type_to_predict[temp_flavor] = true;
    }
    strcpy(resources_to_optimize, info[3 + flavor_number + 1]);
    strptime(info[3 + flavor_number + 3], "%Y-%m-%d", &predict_start_time);       //开始时间
    strptime(info[3 + flavor_number + 4], "%Y-%m-%d", &predict_end_time);//结束时间
//    predict_day = static_cast<int>(
//   (static_cast<long>(mktime(&predict_end_time)) - static_cast<long>(mktime(&predict_start_time))) / (60 * 60 * 24));       //计算预测天数
    predict_day = diffDay(predict_end_time, predict_start_time) + 1;
}

void readData(char * data[MAX_INFO_NUM], int &data_num)
{
    char temp_time[50];
    FlavorDemand temp_history_demand = {0};
    //记录第一行训练数据，同时认为这一天为训练开始时间
    sscanf(data[0],"%*[a-z0-9-]\tflavor%d\t%s", &temp_history_demand.type, temp_time);
    strptime(temp_time, "%Y-%m-%d", &temp_history_demand.create_time);
    history_demand[temp_history_demand.type].push_back(temp_history_demand);
    train_start_time = temp_history_demand.create_time;
    train[temp_history_demand.type][diffDay(temp_history_demand.create_time, train_start_time)] += 1;
    //记录训练数据
    for(int i = 1; i < data_num - 1; i++)
    {
        sscanf(data[i],"%*[a-z0-9-]\tflavor%d\t%s", &temp_history_demand.type, temp_time);
        strptime(temp_time, "%Y-%m-%d", &temp_history_demand.create_time);
        history_demand[temp_history_demand.type].push_back(temp_history_demand);
        train[temp_history_demand.type][diffDay(temp_history_demand.create_time, train_start_time)] += 1;
    }
    //记录最后一行训练数据，同时认为这一天为训练结束时间
    sscanf(data[data_num - 1],"%*[a-z0-9-]\tflavor%d\t%s", &temp_history_demand.type, temp_time);
    strptime(temp_time, "%Y-%m-%d", &temp_history_demand.create_time);
    history_demand[temp_history_demand.type].push_back(temp_history_demand);
    train[temp_history_demand.type][diffDay(temp_history_demand.create_time, train_start_time)] += 1;
    train_end_time = temp_history_demand.create_time;

    train_day = diffDay(train_end_time, train_start_time) + 1;
}

int diffDay(struct tm end, struct tm start)
{
    //同一年就用结构体里的年日期减
    if(end.tm_year == start.tm_year)
    {
        return end.tm_yday - start.tm_yday;
    }
    else
    {
        return static_cast<int>((mktime(&end) - mktime(&start)) / (24 * 60 * 60));
    }
}
vector<double> ta;
vector<double> ArmaData;
int p=7,q=7,pp=17;//一定注意p，q的取值是通过数据计算后，估计出来的。
vector<double> pre;
void predictDay()
{
    ArmaData = sumAllFlavor();

    ta = LeastSquares(ArmaData,pp);
    while(ta.empty())
    {
        --pp;
        cout << "--pp" << endl;
        ta = LeastSquares(ArmaData,pp);
    }
    cout<<"根据AR模型得到的参数ta个数:  "<<ta.size()<<endl;
    for(int i=0;i<ta.size();i++){
        cout<<"ta["<<i<<"] = "<<ta[i]<<endl;
    }

    //残差
    vector<double> bias = getBiasSeries(ArmaData,ta,pp);
    /**
    for(int i=0;i<bias.size();i++){
        cout<<"var["<<i<<"] = "<<bias[i]<<endl;
    }
    **/
    vector<double> ab = getParm_ab(ArmaData,bias,p,q,pp);

    vector<double> a(ab.begin(),ab.begin()+p);
    vector<double> b(ab.begin()+p,ab.begin()+p+q);
    cout<<"参数a个数:  "<<a.size()<<endl;
    for(int i=0;i<a.size();i++){
        cout<<"a["<<i<<"] = "<<a[i]<<endl;
    }
    cout<<"参数b个数:  "<<b.size()<<endl;
    for(int i=0;i<b.size();i++){
        cout<<"b["<<i<<"] = "<<b[i]<<endl;
    }

//    calPQ_N(ArmaData,bias,a,b,p,q);

//    cout << "!!!!!" << endl;
//    cout<< "predict: " <<predict_fun(ArmaData,bias,a,b,p,q,train_day + 1)<<endl;

//    for(int i = 0; i < predict_day; ++i)
//    {
//        pre.push_back(predict_fun(ArmaData,bias,a,b,p,q,train_day + 1 + i));
//    }
//    double psum = fabs(accumulate(pre.begin(), pre.end(), 0));

    double psum = predict_fun(ArmaData,bias,a,b,p,q,train_day + predict_day);

    vector<double> sumEach = sumEachFlavor();
//    double aaa[] = {0.020993702,0.064380686,0.027291812,0.012596221,0.151154654,0.069279216,0.028691393,0.254023793,0.083974808,0.013995801,0.078376487,0.046885934,0.021693492,0.097270819,0.029391183};
//    vector<double> sumEach = sumLastDayFlavor(7);
    double sumAll = accumulate(sumEach.begin(), sumEach.end(), 0);
    for(int flavor = 1; flavor < 16; flavor++)
    {
        if (flavor_type_to_predict[flavor])
        {
            vec_predict_demand[flavor] = static_cast<int>(sumEach[flavor] / sumAll * psum * ((0.4 * rand()/RAND_MAX) + 0.8));
//            vec_predict_demand[flavor] = static_cast<int>(aaa[flavor] * psum );
//            if(sumEach[flavor] / sumAll < 0.01)
//            {
//                vec_predict_demand[flavor] = static_cast<int>(5.0 * rand() / RAND_MAX);
//            }
            if(vec_predict_demand[flavor] < 0)
            {
                vec_predict_demand[flavor] = 0;
            }
        }
    }
}

vector<double> sumAllFlavor()
{
    vector<double> sumAll;
    sumAll.resize(train_day);
    for(int i = 0; i < train_day; ++i)
    {
        for(int flavor = 1; flavor < 16; ++flavor)
        {
            sumAll[i] += trainMA[flavor][i];
        }
    }

    return sumAll;
}

vector<double> sumEachFlavor()
{
    vector<double> sumEach;
    sumEach.resize(16);

    for(int i = 0; i < train_day; ++i)
    {
        for(int flavor = 0; flavor < 16; ++flavor)
        {
            sumEach[flavor] += train[flavor][i];
            allFlavorNum += train[flavor][i];
        }
    }

    return sumEach;
}

vector<double> sumLastDayFlavor(int n)
{
    vector<double> sumEach;
    sumEach.resize(16);

    for(int i = train_day-n; i < train_day; ++i)
    {
        for(int flavor = 0; flavor < 16; ++flavor)
        {
            sumEach[flavor] += train[flavor][i];
            allFlavorNum += train[flavor][i];
        }
    }

    return sumEach;
}

vector<vector<double>> trainMA;
int day_ma = 7;
void dataMoveAvg()
{
    trainMA.resize(16);
    for (int i = 0; i < 16; ++i)
    {
        trainMA[i].resize(train_day);
    }
//    train
    for(int flavor = 1; flavor < 16; ++flavor)
    {
        for(int MAday = day_ma; MAday < train_day; ++MAday)
        {
            for (int i = MAday - 7; i < MAday; ++i)
            {
                if(train[flavor][i] != 0)
                {
                    int a = 0;
                }
                trainMA[flavor][MAday - day_ma] += train[flavor][i];
            }
            trainMA[flavor][MAday - day_ma] /= 1.0 * day_ma;//这里记录第一个数据是前7天的平滑
        }
    }
    train_day -= day_ma;
}