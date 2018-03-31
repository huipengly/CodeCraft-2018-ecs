//
// Created by huipengly on 2018/3/26.
//

#include "myMinMaxScaler.h"

myMinMaxScaler::myMinMaxScaler()
{
    rangeMin = 0;
    rangeMax = 1;
}

void myMinMaxScaler::setRange(double min, double max)
{
    rangeMin = min;
    rangeMax = max;
}

vector<double> myMinMaxScaler::fitTransform(vector<double> originData)
{
    vector<double> transData;

    auto result = minmax_element(originData.begin(), originData.end());
    dataMin = *result.first;
    dataMax = *result.second;

    for(auto data : originData)
    {
        transData.push_back((data - dataMin) / (dataMax - dataMin) * (rangeMax - rangeMin));
    }

    return transData;
}

vector<double> myMinMaxScaler::fitTransform(double* originData, int size)
{
    vector<double> transData;
    vector<double> vecOriginData;

    for (int i = 0; i < size; ++i)
    {
        vecOriginData.push_back(originData[i]);
    }

    auto result = minmax_element(vecOriginData.begin(), vecOriginData.end());
    dataMin = *result.first;
    dataMax = *result.second;

    for(auto data : vecOriginData)
    {
        transData.push_back((data - dataMin) / (dataMax - dataMin) * (rangeMax - rangeMin));
    }

    return transData;
}

vector<double> myMinMaxScaler::inverse_transform(vector<double> transedData)
{
    vector<double> originData;

    for(auto Data : transedData)//Data是缩小过的
    {
        originData.push_back((Data * (dataMax - dataMin))/(rangeMax - rangeMin) + dataMin);
    }

    return originData;
}

double myMinMaxScaler::inverse_transform(double transedData)
{
    return (transedData * (dataMax - dataMin))/(rangeMax - rangeMin) + dataMin;
}
