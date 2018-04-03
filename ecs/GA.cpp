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
const int variation_rate = 1;           //变异率
const int max_generations = 10;         //进化代数

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

    value();
    for(int i = 0; i < max_generations; ++i)
    {
        //计算各个基因适值

//    //轮盘赌
        roulette_build();
//    cout << roulette_choose() << endl;

        //交叉
        for (int i = 0; i < population_size / 2; i++)
        {
            //FIXME:会报错
            order_crossover(roulette_choose(), roulette_choose(), children);
        }

        //变异
//    children = population;
        variation();

        //子代取代父代
        population = children;
        children.clear();

        //计算新子代适值
        value();
        roulette.clear();
    }

    //TODO:保留历史最优，最优保留遗传
    //从最后一代中选择输出
    vector<double>::iterator result;
    result = max_element(utilization.begin(), utilization.end());
    int temp_dis = distance(utilization.begin(), result);
//    utilization.ma

    //输出
    vector<int> temp_use(16,0);
    vector<int> temp_cut_positon_for_outputs;
    gene_deconding(population[temp_dis], temp_cut_positon_for_outputs);
    int outputs_counter = 0;
    if(temp_cut_positon_for_outputs.empty())
    {
        vector<int> temp_outputs;
        copy(population[temp_dis].begin(), population[temp_dis].end(), back_inserter(temp_outputs));
        outputs.push_back(temp_use);
        for (int &temp_output : temp_outputs)
        {
            outputs[outputs_counter][flavors_to_place[temp_output]] += 1;
        }
    }
    else
    {
        for(auto it = temp_cut_positon_for_outputs.begin(); it != temp_cut_positon_for_outputs.end(); ++it)
        {
            vector<int> temp_outputs;
            if (it == temp_cut_positon_for_outputs.begin())//0到第一个切点
            {
                copy(population[temp_dis].begin(), population[temp_dis].begin() + *it, back_inserter(temp_outputs));
                outputs.push_back(temp_use);
                for (int &temp_output : temp_outputs)
                {
                    outputs[outputs_counter][flavors_to_place[temp_output]] += 1;
                }
                outputs_counter++;
            } else
            {
                copy(population[temp_dis].begin() + *(it - 1), population[temp_dis].begin() + *it,
                     back_inserter(temp_outputs));
                outputs.push_back(temp_use);
                for (int &temp_output : temp_outputs)
                {
                    outputs[outputs_counter][flavors_to_place[temp_output]] += 1;
                }
                outputs_counter++;
            }

            if (it == temp_cut_positon_for_outputs.end() - 1)//最后一个切点到末尾
            {
                vector<int> temp_use2(16, 0);
                temp_outputs.clear();
                //            temp_use.clear();
                copy(population[temp_dis].begin() + *it, population[temp_dis].end(), back_inserter(temp_outputs));
                outputs.push_back(temp_use2);
                for (int &temp_output : temp_outputs)
                {
                    outputs[outputs_counter][flavors_to_place[temp_output]] += 1;
                }
                outputs_counter++;
            }
        }
    }

//    int i = 0;
}

void value()
{
    utilization.clear();
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
            cut_positon.push_back(temp_distance);
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
void order_crossover(int parents1, int parents2, vector<vector<int>> &children)
{
    vector<int> child1, child2;
    child1 = population[parents1];
    child2 = population[parents2];

    //交换变量为start开始到end前一个，最后一个量不进行交换
    int crossover_start = static_cast<int>(floor(1.0 * population[parents1].size() * rand() / RAND_MAX));
    int crossover_end = static_cast<int>(floor(1.0 * population[parents1].size() * rand() / RAND_MAX));

    if(crossover_start > crossover_end)
    {
        swap(crossover_start, crossover_end);
    }

    //交换变异段
    auto it1 = child1.begin();
    auto it2 = child2.begin();
    swap_ranges(it1 + crossover_start, it1 + crossover_end, it2 + crossover_start);

    //临时基因变量，存储的是从变异尾端到基因尾端再加上一段完整的父代基因，用来顺序加入新基因
    //填入剩余序列
    //子代1
    vector<int> temp_gene11;
    copy(population[parents1].begin() + crossover_end, population[parents1].end(), back_inserter(temp_gene11));
    copy(population[parents1].begin(), population[parents1].end(), back_inserter(temp_gene11));
    auto it_temp_gene = temp_gene11.begin();
    vector<int> temp_crossover_part;
    copy(child1.begin() + crossover_start, child1.begin() + crossover_end, back_inserter(temp_crossover_part));
    sort(temp_crossover_part.begin(), temp_crossover_part.end());
    for(auto it = (child1.begin() + crossover_end); it != child1.end(); ++it)
    {
//        int diss = distance(it, child1.end());
        while(true)
        {
//            bool aaaa = binary_search(child1.begin() + crossover_start, child1.begin() + crossover_end, *it_temp_gene);
            //binary_search函数需要排序才能搜索
            if(!binary_search(temp_crossover_part.begin(), temp_crossover_part.end(), *it_temp_gene))
            {
                break;
            }
            ++it_temp_gene;
        }
        *it = *it_temp_gene;
        ++it_temp_gene;
    }
    for(auto it = child1.begin(); it != (child1.begin() + crossover_start); ++it)
    {
//        int diss = distance(it, child1.end());
        while(true)
        {
//            bool aaaa = binary_search(child1.begin() + crossover_start, child1.begin() + crossover_end, *it_temp_gene);
            if(!binary_search(child1.begin() + crossover_start, child1.begin() + crossover_end, *it_temp_gene))
            {
                break;
            }
            ++it_temp_gene; //如果有跟交叉部分重复的，则继续向下找
        }
        *it = *it_temp_gene;
        ++it_temp_gene;     //处理完一个，继续向下处理
    }
    //子代2
     vector<int> temp_gene12;
    copy(population[parents2].begin() + crossover_end, population[parents2].end(), back_inserter(temp_gene11));
    copy(population[parents2].begin(), population[parents2].end(), back_inserter(temp_gene11));
    it_temp_gene = temp_gene11.begin();
    temp_crossover_part.clear();
    copy(child2.begin() + crossover_start, child2.begin() + crossover_end, back_inserter(temp_crossover_part));
    sort(temp_crossover_part.begin(), temp_crossover_part.end());
    for(auto it = (child2.begin() + crossover_end); it != child2.end(); ++it)
    {
        while(true)
        {
            if(!binary_search(temp_crossover_part.begin(), temp_crossover_part.end(), *it_temp_gene))
            {
                break;
            }
            ++it_temp_gene;
        }
        *it = *it_temp_gene;
        ++it_temp_gene;
    }
    for(auto it = child2.begin(); it != (child2.begin() + crossover_start); ++it)
    {
        while(true)
        {
            if(!binary_search(child2.begin() + crossover_start, child2.begin() + crossover_end, *it_temp_gene))
            {
                break;
            }
            ++it_temp_gene; //如果有跟交叉部分重复的，则继续向下找
        }
        *it = *it_temp_gene;
        ++it_temp_gene;     //处理完一个，继续向下处理
    }

    //将交叉好的保存到子代
    children.push_back(child1);
    children.push_back(child2);
}

//变异点间倒序变异
void variation()
{
    for(int i = 0; i < static_cast<int>(children.size()); ++i)
    {
        double rand_num = 1.0 * rand() / RAND_MAX;

        if (rand_num < variation_rate)
        {
            //为什么迭代器不能.size？
            int variation_start = static_cast<int>(floor(1.0 * children[i].size() * rand() / RAND_MAX));
            int variation_end = static_cast<int>(floor(1.0 * children[i].size() * rand() / RAND_MAX));

            if(variation_start > variation_end)
            {
                swap(variation_start, variation_end);
            }

            reverse(children[i].begin() + variation_start, children[i].begin() + variation_end);
        }
    }
}