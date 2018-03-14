#ifndef __PREDICT_H__
#define __PREDICT_H__

#include "lib_io.h"
#include <ctime>
#include "matrix.h"

//物理机信息
class PhysicalMachine
{
public:
    int cpu;
    int memory;
    int disk;
};

class FlavorDemand
{
public:
    int type;
    struct tm create_time;
};

void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename);
void readInfo(char * info[MAX_INFO_NUM]);
void readData(char * data[MAX_INFO_NUM], int &data_num);

#endif
