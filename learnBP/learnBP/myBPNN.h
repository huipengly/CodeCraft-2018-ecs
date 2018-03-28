#ifndef _MYBPNN_H_
#define _MYBPNN_H_

#include <iostream>
#include <vector>

using namespace std;
class Connection;//����ط����ܻ�����bug
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
	void train(vector<vector<double>>, vector<vector<double>> dataset, double rate, int iteration);		//ѵ��
	void trainOneSample(vector<double> label, vector<double> sample, double rate);							//ѵ������һ������
	void calculateDelta(vector<double> label);//label��ѵ�������е�����һ����labels������ѵ������
	void update_weight(double rate);
	void calculateGradient();
	void getGradient(vector<double> lable, vector<double> sample);
	vector<double> predict(vector<double> sample);
	void printInfo();
protected:
	vector<Connection> connections;
	vector<Layer> layers;
	int layerNumber;		//����
	int nodeNumber;			//�ܽڵ���

};

class Node
{
public:
	Node(int layerIndex, int nodeIndex);
	~Node();
	void setOutput(double output);							//�趨���ֵ������������
	void appendDownstreamConnection(Connection* connection);	//���һ�������νڵ������
	void appendUpstreamConnection(Connection* connection);		//���һ�������νڵ������
	void calculateOutput();								//����ڵ�����
	void calculateHiddenLayerDelta();						//��������ڵ����
	void calculateOutputLayerDelta(double label);						//���������ڵ����
	void printInfo();									//��ӡ�ڵ���Ϣ
	double getOutput();
	double getDelta();
	vector<Connection*> downstream;
	vector<Connection*> upstream;
protected:
	int layerIndex;		//�ڵ����ڲ���
	int nodeIndex;		//�ڵ���
	double output;		//�ڵ����
	double delta;		//�ڵ����
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
	Node* upstreamNode;		//��һ�����ӽڵ�
	Node* downstreamNode;	//��һ�����ӽڵ�
protected:
	double weight;			//Ȩ��
	double gradient;		//�ݶ�
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