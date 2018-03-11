#include "predict.h"
#include <stdio.h>
#include <iostream>
#include <ctime>
#include "cstring"

using namespace std;

//输入信息
PhysicalMachine physical_machine;   //物理机信息
bool flavor_type_to_predict[15] = {false};    //需要预测的虚拟机类型，共15中，需要预测的为true
int resources_to_optimize = -1;     //0为优化cpu，1为优化memory
int predict_day = 0;//预测时间范围
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
    char *temp_line_string;
    int temp_flavor = -1;
//    int flavor_number = 0;
    for(int i = 0; i < data_num; i++)
    {
        temp_line_string = strsep(data, "\n"); //FIXME:研究怎么能在windows下编译成功
        switch(info_status)
        {
            case 0://读取物理机信息
                sscanf(temp_line_string, "%d %d", &physical_machine.cpu, &physical_machine.memory);
                info_status = 1;
                break;
            case 1://跳过第一个空行
                info_status = 2;
                break;
            case 2://flavor数目
//                flavor_number = sscanf(temp_line_string, "%d", flavor_number);
                info_status = 3;
                break;
            case 3://读取需要预测的flavor类型
                if(strcmp(temp_line_string, "\n") != 0)
                {
                    sscanf(temp_line_string, "flavor%d", &temp_flavor);
                    flavor_type_to_predict[temp_flavor] = true;
                    break;
                }
                info_status = 4;
                break;
            case 4:
                if(strcmp(temp_line_string, "CPU"))
                {
                    resources_to_optimize = 0;
                }
                else if(strcmp(temp_line_string, "MEM"))
                {
                    resources_to_optimize = 1;
                }
                info_status = 5;
                break;
            case 5:
                info_status = 6;
                break;
            case 6://时间读取，暂时空着
//                sscanf(temp_line_string, "%d-%d-%d", );
                break;
        }
    }
}