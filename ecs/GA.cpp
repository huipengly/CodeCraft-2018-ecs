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

extern FlaverInfo flavor_info[16];
//std::random_device rd;
int resouces_needed = 0;                //需要的最小资源，根据优化量来算
vector<int> flavors_to_place;           //把每个flavor的数目拆成需求序列，序列每一个元素为一个需求
vector<vector<int>> population;         //种群基因，遗传算法基因，顺序序列
vector<vector<int>> children;           //子种群
vector<double> utilization;             //种群利用率
vector<double> fitness_value;           //适值
vector<double> roulette;                //轮盘赌概率区间

void genetic_algorithm(vector<int> vec_predict_demand, vector<vector<int>> &outputs, int population_size)
{
//    std::mt19937 g(rd());

    if(strcmp(resources_to_optimize, "CPU") != 0)
    {
        for(auto it = vec_predict_demand.begin(); it != vec_predict_demand.end(); ++it)
        {
            resouces_needed += *it * flavor_info[it - vec_predict_demand.begin()].cpu;
        }
    }
    else if (strcmp(resources_to_optimize, "MEM") != 0)
    {
        for(auto it = vec_predict_demand.begin(); it != vec_predict_demand.end(); ++it)
        {
            resouces_needed += *it * flavor_info[it - vec_predict_demand.begin()].memory;
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
    vector<int> temp_gene;
    for(int i = 0; i < population_size; ++i)
    {
        for (int j = 0; j < static_cast<int>(flavors_to_place.size()); j++)
        {
            temp_gene.push_back(j);
        }
        random_shuffle(temp_gene.begin(), temp_gene.end());
        population.push_back(temp_gene);
        temp_gene.clear();
    }

    //计算各个基因适值
    value();

    //轮盘赌
    roulette_build();
    cout << roulette_choose() << endl;

    //交叉
    order_crossover(1, 2, children);

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
        cpu_sum += flavor_info[flavors_to_place[*it]].cpu;
        mem_sum += flavor_info[flavors_to_place[*it]].memory;
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
        return static_cast<double>(resouces_needed)/((cut_positon.size()+1) * physical_machine.cpu);
    }
    else if (strcmp(resources_to_optimize, "MEM") != 0)
    {
        return static_cast<double>(resouces_needed)/(cut_positon.size() * physical_machine.memory);
    }
    else
    {
        return -1;
    }
}

//构造轮盘
void roulette_build()
{
    double sum = accumulate(fitness_value.begin(), fitness_value.end(), 0.0);
    for(auto it = fitness_value.begin(); it != fitness_value.end(); ++it)
    {
        roulette.push_back(accumulate(fitness_value.begin(), it, 0.0) / sum);
    }
}

//轮盘赌选择
int roulette_choose()
{
    double rand_num = static_cast<double>(rand()) / RAND_MAX;
    return static_cast<int>(distance(roulette.begin(), upper_bound(roulette.begin(), roulette.end(), rand_num)));
    //upper_bound返回指向第一个大于给定值的元素的迭代器
}

//顺序交叉
void order_crossover(int parents1, int parents2, vector<vector<int>> children)
{
    vector<int> child1, child2;
    child1 = population[parents1];
    child2 = population[parents2];

    //交换变量为start开始到end前一个，最后一个量不进行交换
    int crossover_start = static_cast<int>(floor(1.0 * population.size() * rand() / RAND_MAX));
    int crossover_end = static_cast<int>(floor(1.0 * population.size() * rand() / RAND_MAX));

    if(crossover_start > crossover_end)
    {
        swap(crossover_start, crossover_end);
    }

    //交换变异段
    auto it1 = child1.begin();
    auto it2 = child2.begin();
    swap_ranges(it1 + crossover_start, it1 + crossover_end, it2 + crossover_start);

    //临时基因变量，存储的是从变异尾端到基因尾端再加上一段完整的父代基因，用来顺序加入新基因
    vector<int> temp_gene;
    copy(population[parents1].begin() + crossover_end, population[parents1].end(), back_inserter(temp_gene));
    copy(population[parents1].begin(), population[parents1].end(), back_inserter(temp_gene));
    auto it_temp_gene = temp_gene.begin();
    //填入剩余序列
    for(auto it = (child1.begin() + crossover_end); it != child1.end(); ++it)
    {
        //FIXME:any_of调用bug
        while(any_of(&population[parents1][crossover_start], &population[parents1][crossover_end], it_temp_gene));
        --it_temp_gene;
        it = it_temp_gene;
    }
//    for(auto it = child1.begin(); it != (child1.begin() + crossover_start); ++it)
//    {
//        while(any_of(population[parents1][crossover_start], population[parents1][crossover_end], it_temp_gene++));
//        --it_temp_gene;
//        it = it_temp_gene;
//    }
//    for(auto it = (child2.begin() + crossover_end); it != child2.end(); ++it)
//    {
//        while(any_of(population[parents2][crossover_start], population[parents2][crossover_end], it_temp_gene++));
//        --it_temp_gene;
//        it = it_temp_gene;
//    }
//    for(auto it = child2.begin(); it != (child2.begin() + crossover_start); ++it)
//    {
//        while(any_of(population[parents2][crossover_start], population[parents2][crossover_end], it_temp_gene++));
//        --it_temp_gene;
//        it = it_temp_gene;
//    }
}