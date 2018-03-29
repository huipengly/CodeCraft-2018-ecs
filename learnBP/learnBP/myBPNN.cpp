#include "myBPNN.h"

myBPNN::myBPNN(vector<int> layers)
{
	layerNumber = layers.size();
	for (int i = 0; i < layerNumber; ++i)
	{
		this->layers.push_back(Layer(i, layers[i]));
		nodeNumber += layers[i];
	}
	this->layers.back().nodes.pop_back();//最后输出的恒1节点去除
	for (int i = 0; i < layerNumber - 1; ++i)
	{
		connections.clear();
		vector<Node>* nodes = &this->layers[i].nodes;
		for (auto itNode = nodes->begin(); itNode != nodes->end(); ++itNode)
		{
			vector<Node>* nextLayerNodes = &this->layers[i + 1].nodes;
			for (auto itNextNode = nextLayerNodes->begin(); itNextNode != nextLayerNodes->end(); ++itNextNode)
			{
				//添加连接两端的节点信息
				connections.push_back(Connection(&(*itNode), &(*itNextNode)));
			}
		}
		for (auto conn : connections)
		{
			//添加节点的连接信息
			conn.downstreamNode->appendDownstreamConnection(conn);
			conn.upstreamNode->appendUpstreamConnection(conn);
		}
	}
}


myBPNN::~myBPNN()
{
}

void myBPNN::train(vector<vector<double>> labels, vector<vector<double>> dataset, double rate, int iteration)
{
	for (int i = 0; i < iteration; ++i)
	{
		for (size_t j = 0; j < dataset.size(); ++j)
		{
			trainOneSample(labels[j], dataset[j], rate);
		}
	}
}


void myBPNN::trainOneSample(vector<double> label, vector<double> sample, double rate)
{
	predict(sample);
	calculateDelta(label);
	update_weight(rate);
}

void myBPNN::calculateDelta(vector<double> label)
{
	vector<Node> outputNodes = layers.back().nodes;
	auto itNode = outputNodes.begin();
	for (auto itLabel = label.begin(); itLabel != label.end(); ++itLabel, ++itNode)
	{
		itNode->calculateOutputLayerDelta(*itLabel);
	}
	for (auto itLayer = layers.begin() + 1; itLayer != layers.end() - 1; ++itLayer)
	{
		for (auto node : itLayer->nodes)
		{
			node.calculateHiddenLayerDelta();
		}
	}
}

void myBPNN::update_weight(double rate)
{
	for (Layer layer : layers)
	{
		for (Node node : layer.nodes)
		{
			for (Connection conn : node.downstream)
			{
				conn.updateWeight(rate);
			}
		}
	}
}

void myBPNN::calculateGradient()
{
	for (auto layer : layers)
	{
		for (auto node : layer.nodes)
		{
			for (auto conn : node.downstream)
			{
				conn.calculateGradient();
			}
		}
	}
}

void myBPNN::getGradient(vector<double> lable, vector<double> sample)
{
	predict(sample);
	calculateDelta(lable);
	calculateGradient();
}

vector<double> myBPNN::predict(vector<double> sample)
{
	/*
	根据输入的样本预测输出值
	sample: 数组，样本的特征，也就是网络的输入向量
	*/
	vector<double> pre;
	layers[0].setOutput(sample);
	for (auto itLayer = layers.begin() + 1; itLayer != layers.end(); ++itLayer)
	{
		itLayer->calculateOutput();//一层一层计算输出
	}
	for (auto it = layers.back().nodes.begin(); it != layers.back().nodes.end(); ++it)
	{
		pre.push_back(it->getOutput());//记录输出层的值
	}
	return pre;
}

Node::Node(int layerIndex, int nodeIndex)
{
	this->layerIndex = layerIndex;
	this->nodeIndex = nodeIndex;
	output = 0;
	delta = 0;
}

Node::~Node()
{

}

void Node::setOutput(double output)
{
	this->output = output;
}

void Node::appendDownstreamConnection(Connection connection)
{
	downstream.push_back(connection);
}

void Node::appendUpstreamConnection(Connection connection)
{
	upstream.push_back(connection);
}

void Node::calculateOutput()
{
	double output_without_sigmoid = 0;
	for (auto us : upstream)
	{
		auto upstreamNodeOutput = us.upstreamNode->getOutput();
		auto upstreamWeight = us.getWeight();
		output_without_sigmoid += upstreamNodeOutput * upstreamWeight;
	}
	output = Sigmoid(output_without_sigmoid);
}

void Node::calculateHiddenLayerDelta()
{
	double downstream_delta = 0;
	for (auto ds : downstream)
	{
		auto downstreamsNodeDelta = ds.upstreamNode->getOutput();
		auto downstreamsWeight = ds.getWeight();
		downstream_delta += downstreamsNodeDelta * downstreamsWeight;
	}
	delta = output * (1 - output) * downstream_delta;
}

void Node::calculateOutputLayerDelta(double label)
{
	delta = output * (1 - output) * (label - output);
}

void Node::printInfo()
{
	//cout << layerIndex << '-' << nodeIndex << ": output : " << output << ", delta : " << delta
}

double Node::getOutput()
{
	return output;
}

double Node::getDelta()
{
	return delta;
}

Connection::Connection(Node * upstreamNode, Node * downstreamNode)
{
	if (NULL != upstreamNode)
	{
		this->upstreamNode = upstreamNode;
	}
	else
	{
		this->upstreamNode = NULL;
	}
	if (NULL != downstreamNode)
	{
		this->downstreamNode = downstreamNode;
	}
	else
	{
		this->downstreamNode = NULL;
	}
	weight = 1.0 * rand() / RAND_MAX * 0.2 - 0.1;	//初始权重是一个很小的随机数
	gradient = 0;
}

void Connection::calculateGradient()
{
	gradient = downstreamNode->getDelta() * upstreamNode->getOutput();
}

void Connection::updateWeight(double rate)
{
	calculateGradient();
	weight += rate * gradient;
}

double Connection::getWeight()
{
	return weight;
}

double Connection::getGradient()
{
	return gradient;
}

Layer::Layer(int LayerIndex, int nodeNumber)
{
	this->layerIndex = LayerIndex;
	for (int i = 0; i < nodeNumber; ++i)
	{
		nodes.push_back(Node(LayerIndex, i));
	}
	nodes.push_back(Node(LayerIndex, nodeNumber));
	nodes.back().setOutput(1);					//最后添加一个输出恒为1的节点
}

Layer::~Layer()
{
}

void Layer::setOutput(vector<double> data)
{
	for (size_t i = 0; i < data.size(); ++i)
	{
		nodes[i].setOutput(data[i]);
	}
}

void Layer::calculateOutput()
{
	for (size_t i = 0; i < nodes.size() - 1; ++i)
	{
		nodes[i].calculateOutput();
	}
}

double Sigmoid(double x)
{
	return 1.0 / (1.0 + exp(-x));
}

double dSigmoid(double y)
{
	return y * (1.0 - y);
}

double dtanh(double y)
{
	y = tanh(y);
	return 1.0 - y * y;
}
