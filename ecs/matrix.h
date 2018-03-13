//
// Created by codecraft on 3/13/18.
//

#ifndef ECS_MATRIX_H
#define ECS_MATRIX_H

#include <cmath>

bool Gauss(float *A, float *B, int n);  //求逆
bool matrix_mul(double* A, int A_rows, int A_cols, double* B, int B_rows, int B_cols, double *result);//矩阵乘法
void matrix_trans(double *M, int M_rows, int M_cols,  double *m_trans);//转置

#endif //ECS_MATRIX_H
