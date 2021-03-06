//
// Created by huipengly on 2018/3/14.
//

#ifndef GA_H
#define GA_H

#include <vector>
#include "predict.h"

using namespace std;

extern PhysicalMachine physical_machine;
extern char *resources_to_optimize;

//ga排列预测出的机子。
//输入分别为：预测需求（第n个表示flavor[n]的数目），物理机型号，排列信息（[m][n]，m号物理机第n个安排），种群大小
void genetic_algorithm(vector<int> vec_predict_demand, vector<vector<int>> &outputs, int population_size);
double gene_deconding(vector<int> gene, vector<int> &cut_positon);
void value();                   //计算整个种群适值
void roulette_build();          //轮盘构造
int roulette_choose();          //轮盘赌选择一个数
void order_crossover(int parents1, int parents2, vector<vector<int>> &children);
void variation();               //变异点间倒序变异


#endif //ECS_GA_H
