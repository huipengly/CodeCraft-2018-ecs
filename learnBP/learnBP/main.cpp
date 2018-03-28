#include <iostream>
#include <random>
#include "myBPNN.h"

using namespace std;

int main()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	for (int n = 0; n < 10; n++)
		std::cout << mt()/static_cast<double>(mt.max()) * 0.2 -0.1 << std::endl;
	vector<int> a = { 1, 10, 1 };
	myBPNN bp(a);
	return 0;
}