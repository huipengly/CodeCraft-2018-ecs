#ifndef __PREDICT_H__
#define __PREDICT_H__

#include "lib_io.h"

//物理机信息
class PhysicalMachine
{
public:
    int cpu;
    int memory;
    int disk;
};

void predict_server(char * info[MAX_INFO_NUM], int info_num, char * data[MAX_DATA_NUM], int data_num, char * filename);
void readInfo(char * info[MAX_INFO_NUM], int info_num);
void readData(char * data[MAX_INFO_NUM], int data_num);

#endif
