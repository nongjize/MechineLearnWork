#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 6   // ����ָ����
//https://blog.csdn.net/qq_28261343/article/details/51838578
//�ָ�
void main_x(char * filename)//D:\TTT
{
	FILE* fsrc = fopen(filename, "rb");  // Դ�ļ�
	FILE* div = fopen("D:\\TTT\\div.txt", "w");  // ����ָ���Ŀ����Ϣ

	if (fsrc == NULL || div == NULL)
	{
		perror("�򿪴���");
		return;
	}
	fseek(fsrc, 0, SEEK_END);
	int fLen = ftell(fsrc);  // �ļ�����
	//printf("�ļ����ȣ�%d\n", fLen);
	int blockLen = fLen / N;   // ÿһ��ĳ���
	//printf("blockLen:%d\n", blockLen);
	FILE* ftmp;  // ��ʱ�ļ���
	for (int i = 0; i < N; i++)  // ����ָ�
	{
		char tName[20];
		char tdir[60] = "D:\\TTT\\";
		sprintf(tName, "����%d.tmp", i + 1);//�����ļ���
		strcat(tdir, tName);  //������ʱĿ¼
		//printf("%s\n", tdir);
		ftmp = fopen(tdir, "wb");  // ������ʱ�ļ�
		if (ftmp == NULL)
		{
			perror("�����ļ�����");
			break;
		}
		fputs(tdir, div); // д���ļ���
		fputc('\n', div);

		int offset = i * blockLen; //����ƫ����
		fseek(fsrc, offset, SEEK_SET);
		int count = 0;  //ͳ��д��ftmp������
		if (i == N - 1) blockLen = fLen - blockLen * (N - 1);  //���һ��ĳ���
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

//�ϲ�
void main_o()
{
	FILE* fdest = fopen("D:\\1_new.pdf", "wb"); //�ϲ����ɵ��ļ�
	FILE* div = fopen("D:\\TTT\\div.txt", "r");  // ��ȡ�ѷָ�ֵ�Ŀ¼
	if (fdest == NULL || div == NULL)
	{
		perror("���ļ�����");
		return;
	}

	char tempName[60];
	FILE* tempFile;
	// ѭ������temp�ļ�·�����������ļ��ĺϲ�
	while (fgets(tempName, 60, div)) // fgets��ȡ���ַ���ʱ���ط�0�����򷵻�0
	{

		tempName[strlen(tempName) - 1] = '\0'; // ȥ�����һ��\n
		tempFile = fopen(tempName, "rb");
		if (tempFile == NULL)
		{
			printf("���ļ�%sʧ��,", tempName);
			perror("����ԭ��");
			return;
		}

		// feof�ж��ļ���β
		// ���ļ��ڲ���λ��ָ��ָ���ļ�����ʱ����������������FILE�ṹ�е��ļ�������ʶ��
		// ֻ����ִ��һ�ζ��ļ��������Ż����ý�����־���˺����feof()�Ż᷵��Ϊ�档

		// �������
		//while (!feof(tempFile))  // �������,ÿ�κϲ���д��һ��EOF��-1��
		//{
		   // fputc(fgetc(tempFile), fdest); // �������ļ�д��ϲ����ļ�
		//}

		printf("���ںϲ�%s�����ļ�\n", tempName);
		// ������Ĵ���
		int ch = fgetc(tempFile);//����feofǰ�ȶ�ȡһ���ַ�

		//�ж��������ȡ��ch�Ƿ�Ϊ��������feof�ڶ�ȡ��EOF�Ż᷵��1��
		//����chΪEOF����whileѭ��������룬����chΪEOF��ǰһ�ζ�ȡʱ��whileѭ���Ի������
		while (!feof(tempFile))
		{
			fputc(ch, fdest); //д���ַ�
			ch = fgetc(tempFile);//��ȡ��һ���ַ� �����EOF,��ô������һ��ѭ��ǰ�ͻ�ֹͣ
		}

		fclose(tempFile);
	}
	fclose(fdest);
	fclose(div);

	system("pause");
}