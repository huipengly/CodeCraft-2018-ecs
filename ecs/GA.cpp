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
#include <set>

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

void testPopulationOrder()
{
    for(auto it = population.begin(); it != population.end(); ++it)
    {
        set<int> popset(it->begin(), it->end());
        for (int i = 0; i < it->size(); ++i)
        {
            if (popset.find(i) == popset.end())
            {
                cout << "population order error" << endl;
            }
        }
    }
}

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
    for (int j = 0; j < static_cast<int>(flavors_to_place.size()); j++)
    {
        temp_gene.push_back(j);
    }
    for(int i = 0; i < population_size; ++i)
    {
        random_shuffle(temp_gene.begin(), temp_gene.end());
        population.push_back(temp_gene);
    }

//    testPopulationOrder();

    //计算初始化的各个基因适值
    value();
    for(int i = 0; i < max_generations; ++i)
    {

        //轮盘赌构造轮盘
        roulette_build();

        //交叉
        for (auto crossOverCounter = 0; crossOverCounter < population_size / 2 - 2; crossOverCounter++)
        {
            order_crossover(roulette_choose(), roulette_choose(), children);
        }

        //记录历史最优
        fitness_value;
        auto bestChild = max_element(utilization.begin(), utilization.end());
        children.push_back(population[distance(utilization.begin(), bestChild)]);
        children.push_back(population[distance(utilization.begin(), bestChild)]);

        testPopulationOrder();

        //变异
//    children = population;
        variation();

        //子代取代父代
        population = children;
        children.clear();

        testPopulationOrder();

        //计算新子代适值
        fitness_value.clear();
        value();
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
    //清除轮盘
    roulette.clear();
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
    //upper_bound返回指向第一个大于给定值的元素的迭代器
    int num = static_cast<int>(distance(roulette.begin(), upper_bound(roulette.begin(), roulette.end(), rand_num)));
    return num - 1;//返回第一个大于的，是在前一个到这个的区间里。
}

//顺序交叉
void order_crossover(int parents1, int parents2, vector<vector<int>> &children)
{
    vector<int> child1, child2;
    if((parents1 >= population.size()) || (parents2 >= population.size()))
    {
        int a = 0;
    }
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
    swap_ranges(next(child1.begin(), crossover_start), next(child1.begin(), crossover_end), next(child2.begin(), crossover_start));

    //set记录交换的片段
    set<int> swapPiece2(next(child1.begin(), crossover_start), next(child1.begin(), crossover_end));
    set<int> swapPiece1(next(child2.begin(), crossover_start), next(child2.begin(), crossover_end));

    //i是用来在当前段操作，j在历史基因上搜索
    for(int i = 0, j = 0; i < child1.size(); ++i)
    {
        if ((i >= crossover_start) && (i < crossover_end))
        {
            //交换段不处理
            continue;
        }
        else
        {
            //find()  ，返回给定值值得定位器，如果没找到则返回end()。
            if(swapPiece2.find(population[parents1][j]) == swapPiece2.end())
            {
                //没有在交换段则赋值
                child1[i] = population[parents1][j];
            }
            else
            {
                --i;//没有成功写入新的值，不移动当前片段指向
            }
            ++j;//验证一个历史基因，向后走一个
            if(j > child1.size())
            {
                cout << "error : ox j out of range" << endl;
            }
        }
    }

    vector<int> childTest;
    for(int i = 0; i < child1.size(); ++i)
    {
        childTest.push_back(i);
    }
    set<int> child1set(childTest.begin(), childTest.end());
    for(int i = 0; i < child1.size(); ++i)
    {
        if(child1set.find(i) == child1set.end())
        {
            cout << "ox error" << endl;
        }
    }

    //i是用来在当前段操作，j在历史基因上搜索
    for(int i = 0, j = 0; i < child2.size(); ++i)
    {
        if(j > population[parents2].size())
        {
            int b = 0;
        }
        if ((i >= crossover_start) && (i < crossover_end))
        {
            //交换段不处理
            continue;
        }
        else
        {
            //find()  ，返回给定值值得定位器，如果没找到则返回end()。
            if(swapPiece1.find(population[parents2][j]) == swapPiece1.end())
            {
                //没有在交换段则赋值
                child2[i] = population[parents2][j];
            }
            else
            {
                --i;//没有成功写入新的值，不移动当前片段指向
            }
            ++j;//验证一个历史基因，向后走一个
        }
    }

    for(int i = 0; i < child1.size(); ++i)
    {
        if(child1set.find(i) == child1set.end())
        {
            cout << "ox error" << endl;
        }
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