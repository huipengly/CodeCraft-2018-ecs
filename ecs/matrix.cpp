//
// Created by codecraft on 3/13/18.
//

#include "matrix.h"

bool Gauss(double *A, double *B, int n)
{
    int i, j, k;
    double max, temp;
    double t[90000];                //临时矩阵
    //将A矩阵存放在临时矩阵t[n][n]中
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            t[i*n+j] = A[i*n+j];
        }
    }
    //初始化B矩阵为单位阵
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            B[i*n+j] = (i == j) ? (float)1 : 0;
        }
    }
    for (i = 0; i < n; i++)
    {
        //寻找主元
        max = t[i*n+i];
        k = i;
        for (j = i + 1; j < n; j++)
        {
            if (fabs(t[j*n+i]) > fabs(max))
            {
                max = t[j*n+i];
                k = j;
            }
        }
        //如果主元所在行不是第i行，进行行交换
        if (k != i)
        {
            for (j = 0; j < n; j++)
            {
                temp = t[i*n+j];
                t[i*n+j] = t[k*n+j];
                t[k*n+j] = temp;
                //B伴随交换
                temp = B[i*n+j];
                B[i*n+j] = B[k*n+j];
                B[k*n+j] = temp;
            }
        }
        //判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵
        if (t[i*n+i] == 0)
        {

            return false;
        }
        //消去A的第i列除去i行以外的各行元素
        temp = t[i*n+i];
        for (j = 0; j < n; j++)
        {
            t[i*n+j] = t[i*n+j] / temp;        //主对角线上的元素变为1
            B[i*n+j] = B[i*n+j] / temp;        //伴随计算
        }
        for (j = 0; j < n; j++)        //第0行->第n行
        {
            if (j != i)                //不是第i行
            {
                temp = t[j*n+i];
                for (k = 0; k < n; k++)        //第j行元素 - i行元素*j列i行元素
                {
                    t[j*n+k] = t[j*n+k] - t[i*n+k] * temp;
                    B[j*n+k] = B[j*n+k] - B[i*n+k] * temp;
                }
            }
        }
    }
    return true;
}

bool matrix_mul(double* A, int A_rows, int A_cols, double* B, int B_rows, int B_cols, double *result)
{
    if(A_cols != B_rows)    //A的列数要等于B的行数
    {
        return false;
    }

    for(int a_rows = 0; a_rows < A_rows; a_rows++)
    {
        for(int b_cols = 0; b_cols < B_cols; b_cols++)
        {
            for(int a_cols = 0; a_cols < A_cols; a_cols++)
            {
                result[a_rows*B_cols + b_cols] += A[a_rows*A_cols + a_cols] * B[a_cols*B_cols + b_cols];
            }
        }
    }
    return true;
}

void matrix_trans(double *M, int M_rows, int M_cols,  double *m_trans)
{
    for(int rows = 0; rows < M_cols; rows++)
    {
        for(int cols = 0; cols < M_rows; cols++)
        {
            m_trans[rows*M_rows + cols] = M[cols*M_cols + rows];
        }
    }
}