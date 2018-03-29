#include "myMatrix.h"

vector<vector<double>> matrix_multiply(vector<vector<double>> arrA, vector<vector<double>> arrB)
{
	//����arrA������  
	int rowA = arrA.size();
	//����arrA������  
	int colA = arrA[0].size();
	//����arrB������  
	int rowB = arrB.size();
	//����arrB������  
	int colB = arrB[0].size();
	vector<vector<double>>  res;
	if (colA != rowB)//�������arrA�����������ھ���arrB���������򷵻ؿ�  
	{
		return res;
	}
	else
	{
		//���ý������Ĵ�С����ʼ��ΪΪ0  
		res.resize(rowA);
		for (int i = 0; i < rowA; ++i)
		{
			res[i].resize(colB);
		}

		//�������  
		for (int i = 0; i < rowA; ++i)
		{
			for (int j = 0; j < colB; ++j)
			{
				for (int k = 0; k < colA; ++k)
				{
					res[i][j] += arrA[i][k] * arrB[k][j];
				}
			}
		}
	}
	return res;
}