#ifndef _MYBPNN_H_
#define _MYBPNN_H_

#include <iostream>
#include <vector>

using namespace std;
class Connection;//这个地方可能会引起bug
//class Connections;
class Layer;
class Node;
double Sigmoid(double x);
double dSigmoid(double y);
double dtanh(double y);

class myBPNN
{
public:
	myBPNN(vector<int> layers);
	~myBPNN();
	void train(vector<vector<double>>, vector<vector<double>> dataset, double rate, int iteration);		//训练
	void trainOneSample(vector<double> label, vector<double> sample, double rate);							//训练其中一个样本
	void calculateDelta(vector<double> label);//label是训练样本中的其中一个，labels是所有训练样本
	void update_weight(double rate);
	void calculateGradient();
	void getGradient(vector<double> lable, vector<double> sample);
	vector<double> predict(vector<double> sample);
	void printInfo();
protected:
	vector<Connection> connections;
	vector<Layer> layers;
	int layerNumber;		//层数
	int nodeNumber;			//总节点数

};

class Node
{
public:
	Node(int layerIndex, int nodeIndex);
	~Node();
	void setOutput(double output);							//设定输出值，用于输入结点
	void appendDownstreamConnection(Connection* connection);	//添加一个到下游节点的连接
	void appendUpstreamConnection(Connection* connection);		//添加一个到上游节点的连接
	void calculateOutput();								//计算节点的输出
	void calculateHiddenLayerDelta();						//计算隐层节点误差
	void calculateOutputLayerDelta(double label);						//计算输出层节点误差
	void printInfo();									//打印节点信息
	double getOutput();
	double getDelta();
	vector<Connection*> downstream;
	vector<Connection*> upstream;
protected:
	int layerIndex;		//节点所在层编号
	int nodeIndex;		//节点编号
	double output;		//节点输出
	double delta;		//节点误差
};

class Layer
{
public:
	Layer(int LayerIndex, int nodeNumber);
	~Layer();
	void setOutput(vector<double> data);
	void calculateOutput();
	void printInfo();
	vector<Node> nodes;
protected:
	int layerIndex;
};

class Connection
{
public:
	Connection(Node* upstreamNode, Node* downstreamNode);
	~Connection() {};
	void calculateGradient();
	void updateWeight(double rate);
	void printInfo();
	double getWeight();
	double getGradient();
	Node* upstreamNode;		//上一个连接节点
	Node* downstreamNode;	//下一个连接节点
protected:
	double weight;			//权重
	double gradient;		//梯度
};

//class Connections
//{
//public:
//	Connections() {};
//	~Connections() {};
//	void addConnection(Connection connection);
//	void printInfo();
//	vector<Connection> connectinos;
//protected:
//};

#endif // !_MYBPNN_H_