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
	void forward(vector<double> intputArray);
	void backward(vector<double> deltaArray);
	void update(double learnRate);
protected:
	int inputSize;
	int outputSize;
	vector<double> input;					//输入，向量形式。1*n
	vector<double> output;					//输出，向量形式。1*m
	vector<vector<double>> inputMatrix;
	vector<vector<double>> outputMatrix;
	vector<vector<double>> W;				//权重矩阵
	vector<double> b;						//每个输出节点连接的偏置项的权重
	vector<double> delta;					//误差？
	vector<double> W_grad;
	vector<double> b_grad;
};

class MyBPNN
{

};

double sigmoid(double x);
double dSigmoid(double y);

#endif //__MYBPNN_H_