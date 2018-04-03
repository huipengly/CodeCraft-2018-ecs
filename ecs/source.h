//
// Created by huipengly on 2018/4/3.
//

#ifndef AR_ARMA_MODEL_SOURCE_H
#define AR_ARMA_MODEL_SOURCE_H

#include "head.h"

vector<double> getAutoCov(vector<double> data);
vector<double> getAutoCor(vector<double> data);
vector<double> getBiasCor(vector<double> AutoCor);

#endif //AR_ARMA_MODEL_SOURCE_H
