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

void FullConnectionLayer::forward(vector<double> intputArray)
{
	input = intputArray;
	inputMatrix.push_back(input);
	
	vector<vector<double>> W_inputT;//���������һ����ά����ʵ�����ֻ��һ������������ʱ����ת��һ��
	W_inputT = matrixMultiply(W, matrixTranspose(inputMatrix));
	output = vectorAdd(W_inputT.front(), b);
}

void FullConnectionLayer::backward(vector<double> deltaArray)//�������������
{
	vector<vector<double>> deltaArrayMatrix;
	deltaArrayMatrix.push_back(deltaArray);
	vector<vector<double>> tempDSigmiodInput(1);
	for (auto itInput = input.begin(); itInput != input.end(); ++itInput)
	{
		tempDSigmiodInput[0].push_back(dSigmoid(*itInput));
	}
	auto W_T = matrixTranspose(W);
	auto Ds_W_T = matrixMultiply(tempDSigmiodInput, W_T);
	auto deltaMatrix = matrixMultiply(Ds_W_T, deltaArrayMatrix);//���Ϊ1*n�ľ�����ȡΪ����
	delta = deltaMatrix.front();
	W_grad = matrixMultiply(deltaArrayMatrix, inputMatrix).front();
	b_grad = deltaArray;
}

void FullConnectionLayer::update(double learnRate)
{
	W = matrixAdd(W, matrixMultiply(learnRate, W_grad));
	b = vectorAdd(b, matrixMultiply(learnRate, b_grad));
}

double sigmoid(double x)
{
	return 1.0 / (1.0 + exp(-x));
}

double dSigmoid(double y)
{
	return y * (1.0 - y);
}