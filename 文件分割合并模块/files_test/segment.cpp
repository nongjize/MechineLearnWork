#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 6   // 定义分割块数
//https://blog.csdn.net/qq_28261343/article/details/51838578
//分割
void main_x(char * filename)//D:\TTT
{
	FILE* fsrc = fopen(filename, "rb");  // 源文件
	FILE* div = fopen("D:\\TTT\\div.txt", "w");  // 存入分割条目的信息

	if (fsrc == NULL || div == NULL)
	{
		perror("打开错误：");
		return;
	}
	fseek(fsrc, 0, SEEK_END);
	int fLen = ftell(fsrc);  // 文件长度
	//printf("文件长度：%d\n", fLen);
	int blockLen = fLen / N;   // 每一块的长度
	//printf("blockLen:%d\n", blockLen);
	FILE* ftmp;  // 临时文件，
	for (int i = 0; i < N; i++)  // 按块分割
	{
		char tName[20];
		char tdir[60] = "D:\\TTT\\";
		sprintf(tName, "部分%d.tmp", i + 1);//生成文件名
		strcat(tdir, tName);  //产生临时目录
		//printf("%s\n", tdir);
		ftmp = fopen(tdir, "wb");  // 生成临时文件
		if (ftmp == NULL)
		{
			perror("产生文件出错：");
			break;
		}
		fputs(tdir, div); // 写入文件名
		fputc('\n', div);

		int offset = i * blockLen; //计算偏移量
		fseek(fsrc, offset, SEEK_SET);
		int count = 0;  //统计写入ftmp的数量
		if (i == N - 1) blockLen = fLen - blockLen * (N - 1);  //最后一块的长度
		while (count < blockLen && !feof(fsrc))
		{

			fputc(fgetc(fsrc), ftmp);
			count++;
		}
		printf("count:%d\n", count);
		fclose(ftmp);
	}

	fclose(fsrc);
	fclose(div);

	system("pause");
}

//合并
void main_o()
{
	FILE* fdest = fopen("D:\\1_new.pdf", "wb"); //合并生成的文件
	FILE* div = fopen("D:\\TTT\\div.txt", "r");  // 读取已分割部分的目录
	if (fdest == NULL || div == NULL)
	{
		perror("打开文件出错");
		return;
	}

	char tempName[60];
	FILE* tempFile;
	// 循环读出temp文件路径，并进行文件的合并
	while (fgets(tempName, 60, div)) // fgets读取到字符串时返回非0，否则返回0
	{

		tempName[strlen(tempName) - 1] = '\0'; // 去掉最后一个\n
		tempFile = fopen(tempName, "rb");
		if (tempFile == NULL)
		{
			printf("打开文件%s失败,", tempName);
			perror("出错原因");
			return;
		}

		// feof判断文件结尾
		// 当文件内部的位置指针指向文件结束时，并不会立即设置FILE结构中的文件结束标识，
		// 只有再执行一次读文件操作，才会设置结束标志，此后调用feof()才会返回为真。

		// 问题代码
		//while (!feof(tempFile))  // 问题代码,每次合并多写入一个EOF（-1）
		//{
		   // fputc(fgetc(tempFile), fdest); // 将部分文件写入合并的文件
		//}

		printf("正在合并%s到新文件\n", tempName);
		// 修正后的代码
		int ch = fgetc(tempFile);//在用feof前先读取一个字符

		//判断你上面读取的ch是否为结束符，feof在读取到EOF才会返回1，
		//即若ch为EOF，则while循环不会进入，而当ch为EOF的前一次读取时，while循环仍会继续。
		while (!feof(tempFile))
		{
			fputc(ch, fdest); //写入字符
			ch = fgetc(tempFile);//读取下一个字符 如果是EOF,那么进入下一次循环前就会停止
		}

		fclose(tempFile);
	}
	fclose(fdest);
	fclose(div);

	system("pause");
}