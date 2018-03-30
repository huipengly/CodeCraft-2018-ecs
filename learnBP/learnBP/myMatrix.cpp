#include "myMatrix.h"


vector<vector<double>> matrixMultiply(vector<vector<double>> matrixA, vector<vector<double>> matrixB)
{
	int rowA = matrixA.size();			//matrixA的行数
	int colA = matrixA[0].size();		//matrixA的列数  
	int rowB = matrixB.size();			//matrixB的行数  
	int colB = matrixB[0].size();		//matrixB的列数  
	vector<vector<double>>  resultMatrix;
	if (colA != rowB)//如果矩阵arrA的列数不等于矩阵arrB的行数。则返回空  
	{
		return resultMatrix;
	}
	else
	{
		//设置结果矩阵的大小，初始化为为0  
		resultMatrix.resize(rowA);
		for (int i = 0; i < rowA; ++i)
		{
			resultMatrix[i].resize(colB);
		}

		//矩阵相乘
		for (int i = 0; i < rowA; ++i)
		{
			for (int j = 0; j < colB; ++j)
			{
				for (int k = 0; k < colA; ++k)
				{
					resultMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
				}
			}
		}
	}
	return resultMatrix;
}

vector<double> matrixMultiply(vector<vector<double>> matrixA, vector<double> vectorB)
{
	int rowA = matrixA.size();			//matrixA的行数
	int colA = matrixA[0].size();		//matrixA的列数
	int rowB = vectorB.size();			//vectorB的行数
	int colB = 1;						//vectorB的列数
	vector<double>  resultVector;
	if (colA != rowB)//如果矩阵arrA的列数不等于矩阵arrB的行数。则返回空  
	{
		return resultVector;
	}
	else
	{
		//设置结果矩阵的大小，初始化为为0
		resultVector.resize(rowA);

		//矩阵相乘
		for (int i = 0; i < rowA; ++i)
		{
			for (int k = 0; k < colA; ++k)
			{
				resultVector[i] += matrixA[i][k] * vectorB[k];
			}
		}
	}
	return resultVector;
}

vector<double> matrixMultiply(vector<double> vectorB, vector<vector<double>> matrixA)
{
	int rowA = matrixA.size();			//matrixA的行数
	int colA = matrixA[0].size();		//matrixA的列数  
	int rowB = 1;						//vectorB的行数  
	int colB = vectorB.size();			//vectorB的列数  
	vector<double>  resultVector;
	if (colA != rowB)//如果矩阵arrA的列数不等于矩阵arrB的行数。则返回空  
	{
		return resultVector;
	}
	else
	{
		//设置结果矩阵的大小，初始化为为0
		resultVector.resize(rowA);

		//矩阵相乘
		for (int j = 0; j < colA; ++j)
		{
			for (int k = 0; k < colB; ++k)
			{
				resultVector[k] += vectorB[k] * matrixA[k][j];
			}
		}
	}
	return resultVector;
}

vector<vector<double>> matrixMultiply(vector<double> arrA, vector<double> arrB)
{

}

vector<vector<double>> matrixMultiply(double x, vector<vector<double>> arrB)
{

}

vector<vector<double>> matrixMultiply(double x, vector<double> arrB)
{

}



//矩阵转置
vector<vector<double>> matrixTranspose(vector<vector<double>> matrix)
{
	vector<vector<double>> matrixT;
	matrixT.resize(matrix.size());
	for (auto itM = matrixT.begin(); itM != matrixT.end(); ++itM)
	{
		itM->resize(matrix[0].size());
	}

	for (size_t i = 0; i < matrix.size(); ++i)
	{
		for (size_t j = 0; j < matrix[0].size(); ++j)
		{
			matrixT[j][i] = matrix[i][j];
		}
	}

	return matrixT;
}

//向量对应元素相加，如果两个向量不想等，则返回空矩阵
vector<double> vectorAdd(vector<double> vecA, vector<double> vecB)
{
	vector<double> result;
	if (vecA.size() == vecB.size())
	{
		for (auto itA = vecA.begin(), itB = vecB.begin(); itA != vecA.end(); ++itA, ++itB)
		{
			result.push_back(*itA + *itB);
		}
	}
	return result;
}