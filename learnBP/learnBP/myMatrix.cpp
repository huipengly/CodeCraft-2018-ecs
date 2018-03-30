#include "myMatrix.h"


vector<vector<double>> matrixMultiply(vector<vector<double>> matrixA, vector<vector<double>> matrixB)
{
	int rowA = matrixA.size();			//matrixA������
	int colA = matrixA[0].size();		//matrixA������  
	int rowB = matrixB.size();			//matrixB������  
	int colB = matrixB[0].size();		//matrixB������  
	vector<vector<double>>  resultMatrix;
	if (colA != rowB)//�������arrA�����������ھ���arrB���������򷵻ؿ�  
	{
		return resultMatrix;
	}
	else
	{
		//���ý������Ĵ�С����ʼ��ΪΪ0  
		resultMatrix.resize(rowA);
		for (int i = 0; i < rowA; ++i)
		{
			resultMatrix[i].resize(colB);
		}

		//�������
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
	int rowA = matrixA.size();			//matrixA������
	int colA = matrixA[0].size();		//matrixA������
	int rowB = vectorB.size();			//vectorB������
	int colB = 1;						//vectorB������
	vector<double>  resultVector;
	if (colA != rowB)//�������arrA�����������ھ���arrB���������򷵻ؿ�  
	{
		return resultVector;
	}
	else
	{
		//���ý������Ĵ�С����ʼ��ΪΪ0
		resultVector.resize(rowA);

		//�������
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
	int rowA = matrixA.size();			//matrixA������
	int colA = matrixA[0].size();		//matrixA������  
	int rowB = 1;						//vectorB������  
	int colB = vectorB.size();			//vectorB������  
	vector<double>  resultVector;
	if (colA != rowB)//�������arrA�����������ھ���arrB���������򷵻ؿ�  
	{
		return resultVector;
	}
	else
	{
		//���ý������Ĵ�С����ʼ��ΪΪ0
		resultVector.resize(rowA);

		//�������
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



//����ת��
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

//������ӦԪ����ӣ����������������ȣ��򷵻ؿվ���
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