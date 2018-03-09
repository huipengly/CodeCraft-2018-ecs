#include "predict.h"
#include <stdio.h>

//你要完成的功能总入口
void predict_server(char * info[MAX_INFO_NUM], char * data[MAX_DATA_NUM], int data_num, char * filename)
{
	// 需要输出的内容
	char * result_file = (char *)"5\nflavor1 1\nflavor2 1\nflavor3 1\nflavor4 2\nflavor5 2\n\n1 flavor1 1 flavor2 1 flavor3 1 flavor4 2 flavor5 2";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(result_file, filename);
}
