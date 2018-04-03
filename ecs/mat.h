//
// Created by huipengly on 2018/4/3.
//

#ifndef AR_ARMA_MODEL_MAT_H
#define AR_ARMA_MODEL_MAT_H
#include "head.h"

vector<vector<double> > t(vector<vector<double> > x);
vector<vector<double> > mulMat(vector<vector<double> > tx, vector<vector<double> > x);
double det(vector<vector<double> > x);
vector<vector<double> > delMat(vector<vector<double> > x,int r,int c);
vector<vector<double> > A(vector<vector<double> > x);
vector<vector<double> > inv(vector<vector<double> > x);
vector<vector<double> > ConRows(vector<vector<double> > x, vector<vector<double> > y);
vector<vector<double> > ConCols(vector<vector<double> > x, vector<vector<double> > y);
void test_Mat();
#endif //AR_ARMA_MODEL_MAT_H
