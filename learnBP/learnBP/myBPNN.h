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
	vector<double> input;					//���룬������ʽ��1*n
	vector<double> output;					//�����������ʽ��1*m
	vector<vector<double>> inputMatrix;
	vector<vector<double>> outputMatrix;
	vector<vector<double>> W;				//Ȩ�ؾ���
	vector<double> b;						//ÿ������ڵ����ӵ�ƫ�����Ȩ��
	vector<double> delta;					//��
	vector<double> W_grad;
	vector<double> b_grad;
};

class MyBPNN
{

};

double sigmoid(double x);
double dSigmoid(double y);

#endif //__MYBPNN_H_