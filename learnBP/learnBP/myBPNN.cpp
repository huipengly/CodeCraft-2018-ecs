#include "myBPNN.h"



myBPNN::myBPNN()
{
}


myBPNN::~myBPNN()
{
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

//ConstNode::ConstNode(int layerIndex, int nodeIndex) : Node(layerIndex, nodeIndex)
//{
//	output = 1;
//}

Connection::Connection()
{
	weight = rand() / RAND_MAX * 0.2 - 0.1;	//初始权重是一个很小的随机数
	gradient = 0;
}

Layer::Layer(int LayerIndex, int nodeNumber)
{
	this->layerIndex = LayerIndex;
}

Layer::~Layer()
{
}
