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
	void train(int labels, vector<vector<double>> dataset, double rate, int iteration);		//训练
	void trainOneSample(int labels, double sample, double rate);							//训练其中一个样本
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

class Connection;//这个地方可能会引起bug

class Node
{
public:
	Node(int layerIndex, int nodeIndex);
	Node();
	~Node();
	void setOutput(double output);							//设定输出值，用于输入结点
	bool appendDownstreamConnection(Connection connect);	//添加一个到下游节点的连接
	bool appendUpstreamConnection(Connection connect);		//添加一个到上游节点的连接
	double calculateOutput();								//计算节点的输出
	double calculateHiddenLayerDelta();						//计算隐层节点误差
	double calculateOutputLayerDelta();						//计算输出层节点误差
	void printInfo();									//打印节点信息
protected:
	int layerIndex;		//节点所在层编号
	int nodeIndex;		//节点编号
	double output;		//节点输出
	double delta;		//节点误差
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
	Node* upstreamNode;		//上一个连接节点
	Node* downstreamNode;	//下一个连接节点
	double weight;			//权重
	double gradient;		//梯度
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