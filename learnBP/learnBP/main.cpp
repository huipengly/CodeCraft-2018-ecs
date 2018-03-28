#include <iostream>
#include <random>

using namespace std;

int main()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	for (int n = 0; n < 10; n++)
		std::cout << mt()/static_cast<double>(mt.max()) * 0.2 -0.1 << std::endl;
	return 0;
}