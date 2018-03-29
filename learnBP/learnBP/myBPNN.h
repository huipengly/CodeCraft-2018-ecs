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
	vector<vector<double>> input;		//����
	vector<double> output;				//�����
	vector<vector<double>> W;			//Ȩ�ؾ���
	vector<double> b;					//ÿ������ڵ����ӵ�ƫ�����Ȩ��
	vector<vector<double>> delta;
};

class MyBPNN
{

};

#endif //__MYBPNN_H_