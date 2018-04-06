#ifndef __PREDICT_H__
#define __PREDICT_H__

#include "lib_io.h"
#include <ctime>
#include <vector>

using namespace std;

//物理机信息
struct PhysicalMachine
{
//public:
    int cpu;
    int memory;
    int disk;
};

struct FlavorDemand
{
//public:
    int type;
    struct tm create_time;
};

//flaver信息
struct FlaverInfo
{
    int type;
    int cpu;
    int memory;
    int disk;
};

void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename);
void readInfo(char * info[MAX_INFO_NUM]);
void readData(char * data[MAX_INFO_NUM], int &data_num);
int diffDay(struct tm end, struct tm start);        //计算日期差
void predictDay();
vector<double> sumAllFlavor();
vector<double> sumEachFlavor();
vector<double> sumLast30DayFlavor();

#endif
