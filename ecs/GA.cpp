//
// Created by huipengly on 2018/3/14.
//

#include "GA.h"
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

void genetic_algorithm(vector<int> vec_predict_demand, vector<vector<int>> outputs, int population_size)
{
    std::random_device rd;
    std::mt19937 g(rd());

    vector<int> flavors_to_place;   //把每个flavor的数目拆成需求序列，序列每一个元素为一个需求
    vector<vector<int>> population; //种群基因
    vector<int> gene;               //遗传算法基因，顺序序列
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
        for (int j = 0; j < flavors_to_place.size(); j++)
        {
            gene.push_back(j);
        }
        shuffle(gene.begin(), gene.end(), g);
        population.push_back(gene);
    }

    int i = 0;
}

