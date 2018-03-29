#include "myBPNN.h"

FullConnectionLayer::FullConnectionLayer(int inputSize, int outputSize)
{
	this->inputSize = inputSize;
	this->outputSize = outputSize;
	for (int i = 0; i < this->inputSize; ++i)
	{
		vector<double> tempWeight;	//��ʱ�洢һ�����뵽���������Ȩ��
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
	
	vector<vector<double>> tempOutputMatrix;//���������һ����ά����ʵ�����ֻ��һ������������ʱ����ת��һ��
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

