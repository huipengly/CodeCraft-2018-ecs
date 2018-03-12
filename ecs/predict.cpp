#include "predict.h"
#include <stdio.h>
#include <iostream>
#include <ctime>
#include "cstring"

using namespace std;

//输入信息
PhysicalMachine physical_machine;               //物理机信息
bool flavor_type_to_predict[16] = {false};     //需要预测的虚拟机类型，共15中，需要预测的为true。0代表flavor0， 1代表flavor1
char resources_to_optimize[4];                 //0为优化cpu，1为优化memory
int predict_day = 0;                            //预测时间范围
struct tm predict_start_time = {0};             //预测开始时间
struct tm predict_end_time = {0};                      //预测结束时间
int info_status = 0;

//你要完成的功能总入口
void predict_server(char * info[MAX_INFO_NUM], int info_num, char * data[MAX_DATA_NUM], int data_num, char * filename)
{
    readData(data, data_num);

	// 需要输出的内容
	char * result_file = (char *)"5\nflavor1 1\nflavor2 1\nflavor3 1\nflavor4 2\nflavor5 2\n\n1 flavor1 1 flavor2 1 flavor3 1 flavor4 2 flavor5 2";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(result_file, filename);
}

void readInfo(char * info[MAX_INFO_NUM], int info_num)
{

}

void readData(char * data[MAX_INFO_NUM], int data_num)
{
    int temp_flavor = -1;
    int flavor_number = 0;
    char *tempstring = nullptr;
    sscanf(data[0], "%d %d", &physical_machine.cpu, &physical_machine.memory);                      //物理机信息
    sscanf(data[2], "%d", &flavor_number);                                                          //需要预测的个数
    for(int i = 3; i < 3 + flavor_number; i++)                                                      //需要预测的种类
    {
        sscanf(data[i], "flavor%d", &temp_flavor);
        flavor_type_to_predict[temp_flavor] = true;
    }
    strcpy(resources_to_optimize, data[3 + flavor_number + 1]);
    strptime(data[3 + flavor_number + 3], "%Y-%m-%d %H:%M:%S", &predict_start_time);                //开始时间
    strptime(data[3 + flavor_number + 4], "%Y-%m-%d %H:%M:%S", &predict_end_time);                  //结束时间
    predict_day = (mktime(&predict_end_time) - mktime(&predict_start_time)) / (60 * 60 * 24);       //计算预测天数
}