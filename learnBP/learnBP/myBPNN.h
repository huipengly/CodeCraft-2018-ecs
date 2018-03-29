#ifndef __MYBPNN_H_
#define __MYBPNN_H_

#include <vector>
#include "myMatrix.h"

using namespace std;

typedef int FuncType(int, int);

class FullConnectionLayer
{
public:
	FullConnectionLayer(int intputSize, int outputSize);
	void forward(vector<vector<double>> intputArray);
	void backward(vector<double> deltaArray);
protected:
	int inputSize;
	int outputSize;
	vector<vector<double>> input;		//输入
	vector<double> output;				//输出项
	vector<vector<double>> W;			//权重矩阵
	vector<double> b;					//每个输出节点连接的偏置项的权重
	vector<vector<double>> delta;
};

class MyBPNN
{

};

#endif //__MYBPNN_H_