// files_test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#pragma  warning(disable:4996)
void main_x(char *fie);
void main_o();
void print_file_byte_by_byte(char* filename)
{
	FILE* pf;
	pf = fopen(filename, "rb");
	int i = fgetc(pf);
	while (i != EOF)
	{
		printf("%c", i);
		i = fgetc(pf);
	}
	fclose(pf);
}

void print_file_by_string(char * filename,int num)
{
	FILE* pf;
	char buff[10];
	pf = fopen(filename, "rb");
	fgets(buff,num,pf);
	printf(buff);
	fclose(pf);
}
int get_file_size(char *file_name)
{
	//FILE* pf;
	FILE* file_handle = fopen(file_name,"rb");
	
	//获取当前读取文件的位置 进行保存
	//unsigned int current_read_position=ftell( file_handle );
	int file_size;
	//printf("before:%d\n",file_size);
	fseek( file_handle,0,SEEK_END );
	//获取文件的大小  ftell 用于得到文件位置指针当前位置相对于文件首的偏移字节数。
	file_size=ftell( file_handle );
	//恢复文件原来读取的位置
	//fseek( file_handle,current_read_position,SEEK_SET );
	fclose(file_handle);
	return file_size;
	
}
void creatfile(char * filename,char buff[])
{
	FILE* pf = fopen(filename, "wb");
	fprintf(pf, "This is\n testing for\n fprintf...\n");
	//fprintf(pf, buff);

}
void segmentation(char * filename)
{

	main_x(filename);
}


int main()
{
    //std::cout << "Hello World!\n"; 
	char filename[] = {"D:\\TTT\\test.pdf"};
	char buffer[] = {"jdfsf\ngfg\nfdfg"};
	main_o();
	//segmentation(filename);
	//printf("This is\n testing for\n fprintf...\n");
	//creatfile(filename,buffer);
	//printf("the file length: %d bytes\n", get_file_size(filename));
	//print_file_byte_by_byte(filename);
	//print_file_by_string(filename,5);

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件


