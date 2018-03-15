//
// Created by huipengly on 2018/3/14.
//

#include "GA.h"
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <cstring>
#include <iostream>

using namespace std;

extern FlaverInfo flaver_info[16];
//std::random_device rd;
int resouces_needed = 0;                //需要的最小资源，根据优化量来算
vector<int> flavors_to_place;           //把每个flavor的数目拆成需求序列，序列每一个元素为一个需求
vector<vector<int>> population;         //种群基因
vector<int> gene;                       //遗传算法基因，顺序序列
vector<double> utilization;          //种群利用率
vector<double> fitness_value;        //适值

void genetic_algorithm(vector<int> vec_predict_demand, vector<vector<int>> &outputs, int population_size)
{
//    std::mt19937 g(rd());

    if(strcmp(resources_to_optimize, "CPU") != 0)
    {
        for(auto it = vec_predict_demand.begin(); it != vec_predict_demand.end(); ++it)
        {
            resouces_needed += *it * flaver_info[it - vec_predict_demand.begin()].cpu;
        }
    }
    else if (strcmp(resources_to_optimize, "MEM") != 0)
    {
        for(auto it = vec_predict_demand.begin(); it != vec_predict_demand.end(); ++it)
        {
            resouces_needed += *it * flaver_info[it - vec_predict_demand.begin()].memory;
        }
    }

    //数目拆成序列
    for(auto it = vec_predict_demand.begin(); it != vec_predict_demand.end(); ++it)
    {
        for(int i = 0; i < *it; ++i)
        {
            flavors_to_place.push_back(static_cast<int &&>(it - vec_predict_demand.begin()));
        }
    }

    //产生一个随机序列
    for(int i = 0; i < population_size; ++i)
    {
        for (int j = 0; j < static_cast<int>(flavors_to_place.size()); j++)
        {
            gene.push_back(j);
        }
        random_shuffle(gene.begin(), gene.end());
        population.push_back(gene);
        gene.clear();
    }

    value();

    int i = 0;
}

void value()
{
    //计算每一个的利用率
    vector<int> temp_cut_positon;
    for(auto it = population.begin(); it != population.end(); ++it)
    {
//        int i = static_cast<int>(distance(population.begin(), it));
        temp_cut_positon.clear();
        utilization.push_back(gene_deconding(*it, temp_cut_positon));
    }

    vector<double>::iterator min_utilization = min_element(utilization.begin(), utilization.end());
    //计算适值
    for(auto it_u = utilization.begin(); it_u != utilization.end(); ++it_u)
    {
        fitness_value.push_back(*it_u - *min_utilization + 0.01 * rand() / RAND_MAX);
    }

}

//物理机数目就是cut_position.size()
double gene_deconding(vector<int> gene, vector<int> &cut_positon)
{
    int cpu_sum(0), mem_sum(0);
    for(auto it = gene.begin(); it != gene.end(); ++it)
    {
        int temp_distance = static_cast<int>(distance(gene.begin(), it));
        cpu_sum += flaver_info[flavors_to_place[*it]].cpu;
        mem_sum += flaver_info[flavors_to_place[*it]].memory;
        if((cpu_sum > physical_machine.cpu) || (mem_sum > physical_machine.memory))
        {
            //如果超过物理机，则认为上一个就是切点。从上一个点重新计算切点
            cut_positon.push_back(temp_distance - 1);
            --it;
            cpu_sum = mem_sum = 0;
        }
    }

    if(strcmp(resources_to_optimize, "CPU") != 0)
    {
//        cut_positon.push_back(123);
        return static_cast<double>(resouces_needed)/((cut_positon.size()+1) * physical_machine.cpu);
    }
    else if (strcmp(resources_to_optimize, "MEM") != 0)
    {
        return (resouces_needed)/(cut_positon.size() * physical_machine.memory);
    }
    else
    {
        return -1;
    }
}

