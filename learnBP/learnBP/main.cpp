#include <iostream>
#include <random>
#include "myBPNN.h"

using namespace std;

int main()
{
	vector<double> x1 = { 0.1, 0.2 };
	vector<double> x2 = { 0.4, 1.3 };
	vector<double> x3 = { 1.2, 0.8 };
	vector<double> x4 = { 1.01, 1.2 };
	vector<vector<double>> x;
	x.push_back(x1);
	x.push_back(x2);
	x.push_back(x3);
	x.push_back(x4);

	vector<double> y1 = { 0 };
	vector<double> y2 = { 0 };
	vector<double> y3 = { 0 };
	vector<double> y4 = { 1 };
	vector<vector<double>> y;
	y.push_back(y1);
	y.push_back(y2);
	y.push_back(y3);
	y.push_back(y4);

	//std::random_device rd;
	//std::mt19937 mt(rd());
	//for (int n = 0; n < 10; n++)
	//	std::cout << mt()/static_cast<double>(mt.max()) * 0.2 -0.1 << std::endl;
	//vector<int> laybels = { 2, 2, 1 };
	//myBPNN bp(laybels);
	//bp.train(y, x, 0.1, 1000);
	//vector<double> pre_y = bp.predict(x4);
	//cout << "pre_y : " << pre_y.front() << endl;
	return 0;
}