//
// Created by huipeng on 2018/3/12.
//

#ifndef ECS_FLAVORINFO_H
#define ECS_FLAVORINFO_H

//把各个flavor的信息提前写好
struct FlaverInfo
{
    int type;
    int cpu;
    int memory;
    int disk;
};

FlaverInfo flaver_info[16] =
        {{0},
        {1,1,1024},
        {2,1,2048},
        {3,1,4096},
        {4,2,2048},
        {5,2,4096},
        {6,2,8192},
        {7,4,4096},
        {8,4,8192},
        {9,4,16384},
        {10,8,8192},
        {11,8,16384},
        {12,8,32768},
        {13,16,16384},
        {14,16,32768},
        {15,16,65536},
         };



#endif //ECS_FLAVORINFO_H
