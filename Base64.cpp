#define _CRT_SECURE_NO_WARNINGS 1
#include "Base64.h"
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<algorithm>
#include<string>
using namespace std;

//ͳ���ļ����ַ��ĸ���
size_t Size(const char* filename)
{
	int size = 0;
	FILE *fp;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		printf("Open file failed!\n");
		exit(1);
	}

	size = 0;
	while (fgetc(fp) != EOF)
	{
		size++;
	}

	fclose(fp);
	return size;
}

//���ļ��ж�ȡ�ַ�
char* ReadFromFile(FILE* fp,int size)
{
	char* data = (char*)malloc(sizeof(char)*size);
	fread(data, 1, size, fp);
	return data;
}

//������д���ļ���
void WriteToFile(const char* str, FILE* fin)
{
	assert(fin);
	assert(str);

	fwrite(str, 1, strlen(str), fin);
}

//Base64 ����(����ʱ�ַ���8bit��ʾ)
void Base64_encode(const char* filename)
{
	//����һ�������ļ�������д��
	string encode = filename;
	size_t pos = encode.rfind('.');//�ҵ���һ��'.'���ֵ�λ��
	assert(pos != string::npos);//npos��string����Ϊ��֤����������Ч�±��ֵ
	//substr��������һ���½��ĳ�ʼ��Ϊstring������Ӵ��Ŀ�������
	encode = encode.substr(0, pos);
	encode += ".encode";

	FILE* fin = fopen(encode.c_str(), "wb");
	assert(fin);
	FILE* fout = fopen(filename, "rb");
	assert(fout);
	
	int data_len = Size(filename);
	char* data = ReadFromFile(fout,data_len);

	int prepare = 0;
	int ret_len;//�ռ�Ĵ�С
	int temp = 0;
	char *ret = NULL;//retָ�򿪱ٵĿռ�
	char *f = NULL;//��ret��ֵ����f��f����������
	int tmp = 0;//��¼���ݵĳ���
	char changed[4];

	//���ٿռ�Ĵ�С(���ٶ��ٸ�3�ֽڵĿռ�)
	ret_len = data_len / 3;
	temp = data_len % 3;
	if (temp > 0)
	{
		//�����3֮����ʣ��˵�����迪��һ���ֽ�
		ret_len += 1;
	}
	ret_len = ret_len * 4 + 1;
	ret = (char *)malloc(ret_len);
	if (ret == NULL)
	{
		printf("No enough memory.\n");
		exit(0);
	}
	memset(ret, 0, ret_len);

	f = ret;
	//ÿ�ζ��������ַ����б��룬ֱ�����е��ַ������б���
	while (tmp < data_len)
	{
		temp = 0;//��ʾÿ�α�����ַ���
		prepare = 0;
		memset(changed, '\0', 4);
		while (temp < 3)
		{

			if (tmp >= data_len)
			{
				break;
			}

			//prepare<<8��ÿ���ַ�ռ8��bit��Դ�ı���ÿ����3���ַ���ÿ�λ�֮ǰ��������8λ����֤3���ַ��õ�һ�������Ʊ���
			//data[tmp]&0xFF����֤���ɵ��ַ�����8bitλ
			prepare = ((prepare << 8) | (data[tmp] & 0xFF));
			tmp++;
			temp++;
		}
		//��ֹ����while�˳�ʱ������ַ�������3����prepare����Ҫ�չ�3���ַ���λ��
		prepare = (prepare << ((3 - temp) * 8));

		//4��forѭ������3���ַ����ڵ�ʱ�򣬾Ϳ��Խ�����4���ַ�
		for (int i = 0; i < 4; i++)
		{
			if (temp < i)
			{
				changed[i] = 0x40;
			}
			else
			{
				//0x3F��6bit��ʾһ���ַ�
				changed[i] = (prepare >> ((3 - i) * 6)) & 0x3F;
			}
			*f = base[changed[i]];
			f++;
		}
	}
	*f = '\0';

	/*printf("%s\n", ret);*/

	WriteToFile(ret, fin);

	fclose(fin);
	fclose(fout);
}

//ת������(�����ַ���λ��)
static char find_pos(char ch)
{
	//strrchr(str,ch)�����ҳ�str�ַ��������һ�γ��ֵ��ַ�ch���ֵĵ�ַ��Ȼ�󽫵�ַ����
	char *ptr = (char*)strrchr(base, ch);
	return (ptr - base);
}

//Base64 ����
void Base64_decode(const char *filename)
{
	string unencode = filename;
	//�ҵ���һ��'.'���ֵ�λ��
	size_t pos = unencode.rfind('.');
	assert(pos != string::npos);//npos��string����Ϊ��֤����������Ч�±��ֵ
	//substr��������һ���½��ĳ�ʼ��Ϊstring������Ӵ��Ŀ�������
	unencode = unencode.substr(0, pos);
	unencode += ".unencode";

	//�Զ��ķ�ʽ�򿪱����ļ�����д�ķ�ʽ�򿪽����ļ�
	FILE* fin = fopen(unencode.c_str(), "wb");
	assert(fin);
	FILE* fout = fopen(filename, "rb");
	assert(fout);

	int data_len = Size(filename);
	char* data = ReadFromFile(fout,data_len);

	//�ж��ٵ��ĸ��ַ������Ϊ�����ַ�
	int ret_len = (data_len / 4) * 3;//���뿪�ٿռ�Ĵ�С
	int equal_count = 0;//�Ⱥŵĸ���
	char *ret = NULL;
	char *f = NULL;
	int tmp = 0;
	int temp = 0;
	int prepare = 0;

	if (*(data + data_len - 1) == '=')
	{
		equal_count += 1;
	}
	if (*(data + data_len - 2) == '=')
	{
		equal_count += 1;
	}
	
	//Ϊ��4���ַ�ת3���ַ���ʱ���ֽڽ��ж���
	switch (equal_count)
	{
	case 0:
		ret_len += 4;//3 + 1 [1 for NULL]
		break;
	case 1:
		ret_len += 4;//Ceil((6*3)/8)+1
		break;
	case 2:
		ret_len += 3;//Ceil((6*2)/8)+1
		break;
	}
	ret = (char *)malloc(ret_len);
	if (ret == NULL)
	{
		printf("No enough memory.\n");
		exit(0);
	}
	memset(ret, 0, ret_len);

	f = ret;
	while (tmp < (data_len - equal_count))
	{
		temp = 0;
		prepare = 0;
		while (temp < 4)
		{
			if (tmp >= (data_len - equal_count))
			{
				break;
			}
			prepare = (prepare << 6) | (find_pos(data[tmp]));
			temp++;
			tmp++;
		}
		prepare = prepare << ((4 - temp) * 6);
		for (int i = 0; i<3; i++)
		{
			if (i == temp)
			{
				break;
			}
			*f = (char)((prepare >> ((2 - i) * 8)) & 0xFF);
			f++;
		}
	}
	*f = '\0';
	/*printf("%s\n", ret);*/

	WriteToFile(ret, fin);
	
	fclose(fin);
	fclose(fout);
}

int main(int argc, char* argv[])
{
	Base64_encode("abc.txt");
	Base64_decode("abc.encode");

	//printf("%d\n", Size("abc.encode"));
	//printf("%d\n", Size("abc.unencode"));

	/*system("pause");*/
	return 0;
}