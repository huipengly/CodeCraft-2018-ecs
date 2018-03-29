#include "myBPNN.h"

FullConnectionLayer::FullConnectionLayer(int inputSize, int outputSize)
{
	this->inputSize = inputSize;
	this->outputSize = outputSize;
	for (int i = 0; i < this->inputSize; ++i)
	{
		vector<double> tempWeight;	//临时存储一个输入到所有输出的权重
		for (int j = 0; j < this->outputSize; ++j)
		{
			tempWeight.push_back(1.0 * rand() / RAND_MAX * 0.2 - 0.1);
		}
		W.push_back(tempWeight);
		tempWeight.clear();
	}
	b.resize(this->outputSize);
	output.resize(this->outputSize);
}

void FullConnectionLayer::forward(vector<vector<double>> intputArray)
{
	input = intputArray;
	
	vector<vector<double>> tempOutputMatrix;//函数输出是一个二维矩阵，实际输出只是一个向量，用临时变量转换一下
	tempOutputMatrix = matrix_multiply(W, input);
	output = tempOutputMatrix.front();
	for (auto itOutput = output.begin(), itB = b.begin(); itOutput != output.end(); ++itOutput, ++itB)
	{
		*itOutput += *itB;
	}
}

void FullConnectionLayer::backward(vector<double> deltaArray)
{
	//delta = 
}

