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


//你要完成的功能总入口
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{
    myMinMaxScaler MinMaxScaler;

    srand(time(0));

    readInfo(info);

    readData(data, data_num);

    for(int flavor = 1; flavor < 16; flavor++)
    {
        if(flavor_type_to_predict[flavor])
        {
            BpNet testNet;
            double *mydata = train[flavor];
            int mydata_num = train_day;

            vector<double> myDataTrans = MinMaxScaler.fitTransform(mydata, mydata_num);

            vector<sample> sampleGroup;
            sampleGroup.resize(mydata_num - lookback);
            for (int i = 0; i < mydata_num - lookback; i++)
            {
                for (int j = 0; j < lookback; ++j)
                {
                    sampleGroup[i].in.push_back(myDataTrans[i + j]);
                }
                sampleGroup[i].out.push_back(myDataTrans[i + lookback]);
            }
            testNet.training(sampleGroup, 0.001);

            vector<sample> testGroupp[7];
            for (int i = 0; i < predict_day; ++i)
            {
                testGroupp[i].resize(1);
            }
            testGroupp[0].front().in.push_back(myDataTrans[mydata_num - lookback + 1]);
            testGroupp[0].front().in.push_back(myDataTrans[mydata_num - lookback + 2]);
            testGroupp[0].front().in.push_back(myDataTrans[mydata_num - lookback + 3]);
            testNet.predict(testGroupp[0]);
            testGroupp[1].front().in.push_back(myDataTrans[mydata_num - lookback + 2]);
            testGroupp[1].front().in.push_back(myDataTrans[mydata_num - lookback + 3]);
            testGroupp[1].front().in.push_back(testGroupp[0].front().out.front());
            testNet.predict(testGroupp[1]);
            testGroupp[2].front().in.push_back(myDataTrans[mydata_num - lookback + 3]);
            testGroupp[2].front().in.push_back(testGroupp[0].front().out.front());
            testGroupp[2].front().in.push_back(testGroupp[1].front().out.front());
            for (int i = 2; i < predict_day - 1; ++i)
            {
                testNet.predict(testGroupp[i]);
                testGroupp[i + 1].front().in.push_back(testGroupp[i-2].front().out.front());
                testGroupp[i + 1].front().in.push_back(testGroupp[i-1].front().out.front());
                testGroupp[i + 1].front().in.push_back(testGroupp[i].front().out.front());
            }
            testNet.predict(testGroupp[predict_day - 1]);
            for(auto tg :testGroupp)
            {
                vec_predict_demand_double[flavor] += tg.front().out.front();
            }

            vec_predict_demand_double[flavor] = MinMaxScaler.inverse_transform(vec_predict_demand_double[flavor]);//恢复原始大小
            vec_predict_demand[flavor] = static_cast<int>(ceil(vec_predict_demand_double[flavor] + 0.5));//取整
        }
    }

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