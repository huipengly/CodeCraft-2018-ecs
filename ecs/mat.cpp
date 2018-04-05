
#include "mat.h"
#include "matrix.h"
/**
 * a = inv(t(x) _*_ x) _*_ t(x) _*_ Y
 * t(x) : 对x球转置
 * r(x) : 对x求逆矩
 * -*- : 矩阵乘法
 * inv(x): 矩阵的逆
 */

/**
 *矩阵转置
 */
vector<vector<double> > t(vector<vector<double> > x){
    //x的装置矩阵
    vector<vector<double> > tx;
    //tx初始化便于直接访问下标,这是原矩阵的转置的形式
    for(int i=0;i<x[0].size();i++){
        vector<double> tmp(x.size(),0);
        tx.push_back(tmp);
    }

    for(int i=0;i<x.size();i++){
        for(int j=0;j<x[0].size();j++){
            tx[j][i] = x[i][j];
        }
    }
    return tx;
}

/**
 *矩阵乘法
 */
vector<vector<double> > mulMat(vector<vector<double> > tx, vector<vector<double> > x){
    vector<vector<double> > res;
    //初始化结果矩阵的格式row(tx) X col(x)
    for(int i=0;i<tx.size();i++){
        vector<double> tmp(x[0].size(),0);
        res.push_back(tmp);
    }

//    cout<<res.size()<<" "<<res[0].size()<<endl;
    for(int i=0;i<tx.size();i++){
        for(int j=0;j<x[0].size();j++){
            for(int k=0;k<x.size();k++){
               res[i][j] += tx[i][k] * x[k][j];
            }
        }
    }
    return res;
}

/**
 *矩阵的行列式，行列变化为上三角矩阵
 */

double det(vector<vector<double> > x){
	//只有一个元素
	//if(x.size() == 1 && x[0].size() == 1) return x[0][0];
	
	double det = 1;
	//交换数组指定的两行，即进行行变换（具体为行交换）
    int iter = 0;  //记录行变换的次数（交换）
    for(int i=0;i<x.size();i++){
        if(x[i][i] == 0){
        	for(int j=i+1;j<x.size();j++){
	        	if(x[j][i] != 0){
	                swap(x[i],x[j]);//交换两行
    	            iter ++;
    	        }
    	    }
        }
        for(int k=i+1;k<x.size();k++){
            double yin = -1 * x[k][i] / x[i][i] ;
            for(int u=0; u<x[0].size(); u++){
                x[k][u] = x[k][u] + x[i][u] * yin;
            }
        }
    }
	
	/**
   	cout<<"上三角矩阵："<<endl;
   	for(int i=0;i<x.size();i++){
        for(int j=0;j<x[0].size();j++){
            cout<<x[i][j]<<" ";
        }
        cout<<endl;
    }**/
	for(int i=0;i<x.size();i++){//求对角线的积 即 行列式的值
      det = det * x[i][i];
	}
	//行变换偶数次符号不变
	if(iter%2 == 1)  det= -det;

	return det;
}

/**
 *删除矩阵的第r行，第c列
 */
vector<vector<double> > delMat(vector<vector<double> > x,int r,int c){
	vector<vector<double> > Ax;
	for(int i=0;i<x.size();i++){
		vector<double> tmp;
		for(int j=0;j<x[0].size();j++){
			if(i != r && j != c) tmp.push_back(x[i][j]);			
		}
		if(i != r) Ax.push_back(tmp);
	}	
	return Ax;
}


/**
 *求矩阵的伴随矩阵
 */
vector<vector<double> > A(vector<vector<double> > x){
	vector<vector<double> > tmp(x),res;
	
	//tx初始化便于直接访问下标,这是原矩阵的转置的形式
    for(int i=0;i<x.size();i++){
        vector<double> tp(x[0].size(),0);
        res.push_back(tp);
    }
    
	for(int i=0;i<x.size();i++){
		for(int j=0;j<x[0].size();j++){
			tmp = x;
			tmp = delMat(tmp,i,j);
			res[i][j] = ((i+j)%2==0?1:-1) * det(tmp);
			
		}
	}
	return res;
}


/**
 *矩阵的逆
 */
vector<vector<double> > old_inv(vector<vector<double> > x){
	vector<vector<double> > res = A(x);
	double dets = det(x);
	for(int i=0;i<res.size();i++){
		for(int j=0;j<res[0].size();j++){
			res[i][j] /= dets;
		}
	}
	return res;
}

vector<vector<double> > inv(vector<vector<double> > x)
{
	int size = static_cast<int>(x.size());
	auto *A = new double[size * size]();
	auto *B = new double[size * size]();

	for (int i = 0; i < size; ++i)
	{
		for(int j = 0; j < size; ++j)
		{
			A[i * size + j] = x[i][j];
			cout << A[i * size + j] << " ";
		}
		cout <<  ';' << endl;
	}

	if(!Gauss(A, B, size))
    {
        return vector<vector<double>>();
    }

	vector<vector<double>> res;
	res.resize(size);
	for(int i = 0; i < size; ++i)
	{
		res[i].resize(size);
	}

	for (int i = 0; i < size; ++i)
	{
		for(int j = 0; j < size; ++j)
		{
			res[i][j] = B[i * size + j];
		}
	}

	return res;
}

//bool Gauss(double *A, double *B, int n)
//{
//	int i, j, k;
//	double max, temp;
//	double t[90000];                //临时矩阵
//	//将A矩阵存放在临时矩阵t[n][n]中
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < n; j++)
//		{
//			t[i*n+j] = A[i*n+j];
//		}
//	}
//	//初始化B矩阵为单位阵
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < n; j++)
//		{
//			B[i*n+j] = (i == j) ? (float)1 : 0;
//		}
//	}
//	for (i = 0; i < n; i++)
//	{
//		//寻找主元
//		max = t[i*n+i];
//		k = i;
//		for (j = i + 1; j < n; j++)
//		{
//			if (fabs(t[j*n+i]) > fabs(max))
//			{
//				max = t[j*n+i];
//				k = j;
//			}
//		}
//		//如果主元所在行不是第i行，进行行交换
//		if (k != i)
//		{
//			for (j = 0; j < n; j++)
//			{
//				temp = t[i*n+j];
//				t[i*n+j] = t[k*n+j];
//				t[k*n+j] = temp;
//				//B伴随交换
//				temp = B[i*n+j];
//				B[i*n+j] = B[k*n+j];
//				B[k*n+j] = temp;
//			}
//		}
//		//判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵
//		if (t[i*n+i] == 0)
//		{
//
//			return false;
//		}
//		//消去A的第i列除去i行以外的各行元素
//		temp = t[i*n+i];
//		for (j = 0; j < n; j++)
//		{
//			t[i*n+j] = t[i*n+j] / temp;        //主对角线上的元素变为1
//			B[i*n+j] = B[i*n+j] / temp;        //伴随计算
//		}
//		for (j = 0; j < n; j++)        //第0行->第n行
//		{
//			if (j != i)                //不是第i行
//			{
//				temp = t[j*n+i];
//				for (k = 0; k < n; k++)        //第j行元素 - i行元素*j列i行元素
//				{
//					t[j*n+k] = t[j*n+k] - t[i*n+k] * temp;
//					B[j*n+k] = B[j*n+k] - B[i*n+k] * temp;
//				}
//			}
//		}
//	}
//	return true;
//}


/**
 *合并两个行相同的矩阵
 */ 
vector<vector<double> > ConRows(vector<vector<double> > x, vector<vector<double> > y){
	//行相同，添加列
	for(int i=0;i<y.size();i++){
		for(int j=0;j<y[0].size();j++){
			x[i].push_back(y[i][j]);
		}
	}
	return x;
}

/**
 *合并两个列相同的矩阵
 */ 
vector<vector<double> > ConCols(vector<vector<double> > x, vector<vector<double> > y){
	//列相同，添加行
	for(int i=0;i<y.size();i++){
		vector<double> row;
		for(int j=0;j<y[0].size();j++){
			row.push_back(y[i][j]);
		}
		x.push_back(row);
	}
	return x;
}






/**
 *测试矩阵运算成功
 */
void test_Mat(){
    vector<vector<double> > data,tdata,res,Ax;
    //data = getdata();
	double x[] = {2,1,-1,2,1,0,1,-1,1};
	
    for(int i=0;i<3;i++){
        vector<double> tmp;
        data.push_back(tmp);
        for(int j=0;j<3;j++){
            data[i].push_back(x[i*3+j]);
        }
    }
	
	/**
    tdata = t(data);

    for(int i=0;i<tdata.size();i++){
        for(int j=0;j<tdata[0].size();j++){
            cout<<tdata[i][j]<<" ";
        }
        cout<<endl;
    }

    res = mulMat(tdata,data);
    for(int i=0;i<res.size();i++){
        for(int j=0;j<res[0].size();j++){
            cout<<res[i][j]<<" ";
        }
        cout<<endl;
    }
	
	cout<<det(data)<<endl;
	*/
	
	//Ax = inv(data);
	cout<<det(data)<<endl;
	cout<<"逆矩阵:"<<endl;
	for(int i=0;i<Ax.size();i++){
        for(int j=0;j<Ax[0].size();j++){
            cout<<Ax[i][j]<<" ";
        }
        cout<<endl;
    }
}

/**
int main()
{

    vector<double> data;


    test_Mat();
    return 0;
}

**/



