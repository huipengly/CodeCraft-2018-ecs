//
// Created by huipengly on 2018/3/26.
//

#ifndef LSTM_C_MYMINMAXSCALER_H
#define LSTM_C_MYMINMAXSCALER_H

#include <vector>
#include <algorithm>

using namespace std;

class myMinMaxScaler
{
protected:
    double rangeMin;
    double rangeMax;
    double dataMin;
    double dataMax;

public:
    myMinMaxScaler();
    void setRange(double min, double max);                          //设置范围
    vector<double> fitTransform(vector<double> originData);         //转换为0-1
    vector<double> fitTransform(double* originData, int size);         //转换为0-1
    vector<double> inverse_transform(vector<double> transedData);   //转换为原始数据
    double inverse_transform(double transedData);                   //转换单个为原始数据

};


#endif //LSTM_C_MYMINMAXSCALER_H
