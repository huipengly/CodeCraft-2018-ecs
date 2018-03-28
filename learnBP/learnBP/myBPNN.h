#ifndef _MYBPNN_H_
#define _MYBPNN_H_

#include <iostream>
#include <vector>

using namespace std;

class myBPNN
{
public:
	myBPNN();
	~myBPNN();
	void train(int labels, vector<vector<double>> dataset, double rate, int iteration);		//ѵ��
	void trainOneSample(int labels, double sample, double rate);							//ѵ������һ������
	void calculateDelta(int label);
	void update_weight(double rate);
	void calculateGradient();
	void getGradient(int lable, double sample);
	double* predict(double sample);
	void printInfo();
protected:
	Connection connections;
	Layer layer;
	int layerNumber;
	int nodeNumber;

};

class Connection;//����ط����ܻ�����bug

class Node
{
public:
	Node(int layerIndex, int nodeIndex);
	Node();
	~Node();
	void setOutput(double output);							//�趨���ֵ������������
	bool appendDownstreamConnection(Connection connect);	//���һ�������νڵ������
	bool appendUpstreamConnection(Connection connect);		//���һ�������νڵ������
	double calculateOutput();								//����ڵ�����
	double calculateHiddenLayerDelta();						//��������ڵ����
	double calculateOutputLayerDelta();						//���������ڵ����
	void printInfo();									//��ӡ�ڵ���Ϣ
protected:
	int layerIndex;		//�ڵ����ڲ���
	int nodeIndex;		//�ڵ���
	double output;		//�ڵ����
	double delta;		//�ڵ����
};

class ConstNode : Node
{
public:
	ConstNode(int layerIndex, int nodeIndex);
};

class Layer
{
public:
	Layer(int LayerIndex, int nodeNumber);
	~Layer();
	void setOutput(vector<double> data);
	void calculateOutput();
	void printInfo();
protected:
	int layerIndex;
	vector<Node> nodes;
};

class Connection
{
public:
	Connection();
	~Connection();
	void calculateGradient();
	void getGradient();
	void updateWeight(double rate);
	void printInfo();
protected:
	Node* upstreamNode;		//��һ�����ӽڵ�
	Node* downstreamNode;	//��һ�����ӽڵ�
	double weight;			//Ȩ��
	double gradient;		//�ݶ�
};

class Connections
{
public:
	Connections();
	~Connections();
	void addConnection(Connection connection);
	void printInfo();
protected:
	vector<Connection> connectinos;
};

#endif // !_MYBPNN_H_