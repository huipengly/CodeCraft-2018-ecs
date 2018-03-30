#ifndef __MYMATRIX_H_
#define __MYMATRIX_H_

#include <vector>
using namespace std;

vector<vector<double>> matrixMultiply(vector<vector<double>> matrixA, vector<vector<double>> matrixB);
vector<double> matrixMultiply(vector<vector<double>> matrixA, vector<double> vectorB);
vector<double> matrixMultiply(vector<double> vectorB, vector<vector<double>> matrixA);
vector<vector<double>> matrixMultiply(vector<double> arrA, vector<double> arrB);
vector<vector<double>> matrixMultiply(double x, vector<vector<double>> arrB);
vector<vector<double>> matrixMultiply(double x, vector<double> arrB);
vector<vector<double>> matrixTranspose(vector<vector<double>> matrix);
vector<double> vectorAdd(vector<double> vecA, vector<double> vecB);


#endif // ! __MYMATRIX_H_
