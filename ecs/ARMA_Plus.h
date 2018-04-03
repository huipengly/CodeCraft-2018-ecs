//
// Created by huipengly on 2018/4/3.
//

#ifndef ECS_ARMA_PLUS_H
#define ECS_ARMA_PLUS_H

int Calculate_pq(vector<double> data);
void formatData(vector<double> data,int p);
vector<double> LeastSquares(vector<double> data,int p);
vector<double> getBiasSeries(vector<double> data,vector<double> a,int p);
void formatData(vector<double> data,vector<double> bias,int p, int q,int pp);
vector<double> getParm_ab(vector<double> data,vector<double> bias,int p, int q,int pp);
int calPQ_N(vector<double> data,vector<double> data_var,vector<double> a,vector<double> b,int p,int q);
double predict_fun(vector<double> &data,vector<double> &data_var,vector<double> a,vector<double> b,int p,int q,int k);

#endif //ECS_ARMA_PLUS_H
