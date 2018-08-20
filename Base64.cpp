#define _CRT_SECURE_NO_WARNINGS 1
#include "Base64.h"
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<algorithm>
#include<string>
using namespace std;

//统计文件中字符的个数
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

//从文件中读取字符
char* ReadFromFile(FILE* fp,int size)
{
	char* data = (char*)malloc(sizeof(char)*size);
	fread(data, 1, size, fp);
	return data;
}

//将数据写到文件中
void WriteToFile(const char* str, FILE* fin)
{
	assert(fin);
	assert(str);

	fwrite(str, 1, strlen(str), fin);
}

//Base64 编码(编码时字符是8bit表示)
void Base64_encode(const char* filename)
{
	//创建一个编码文件并往里写入
	string encode = filename;
	size_t pos = encode.rfind('.');//找到第一个'.'出现的位置
	assert(pos != string::npos);//npos是string定义为保证大于所有有效下标的值
	//substr函数返回一个新建的初始化为string对象的子串的拷贝对象
	encode = encode.substr(0, pos);
	encode += ".encode";

	FILE* fin = fopen(encode.c_str(), "wb");
	assert(fin);
	FILE* fout = fopen(filename, "rb");
	assert(fout);
	
	int data_len = Size(filename);
	char* data = ReadFromFile(fout,data_len);

	int prepare = 0;
	int ret_len;//空间的大小
	int temp = 0;
	char *ret = NULL;//ret指向开辟的空间
	char *f = NULL;//将ret的值赋给f，f进行向后遍历
	int tmp = 0;//记录数据的长度
	char changed[4];

	//开辟空间的大小(开辟多少个3字节的空间)
	ret_len = data_len / 3;
	temp = data_len % 3;
	if (temp > 0)
	{
		//如果除3之后，有剩余说明还需开辟一个字节
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
	//每次都是三个字符进行编码，直到所有的字符都进行编码
	while (tmp < data_len)
	{
		temp = 0;//表示每次编码的字符数
		prepare = 0;
		memset(changed, '\0', 4);
		while (temp < 3)
		{

			if (tmp >= data_len)
			{
				break;
			}

			//prepare<<8：每个字符占8个bit，源文本中每个是3个字符，每次或之前都向左移8位；保证3个字符得到一个二进制编码
			//data[tmp]&0xFF：保证生成的字符串是8bit位
			prepare = ((prepare << 8) | (data[tmp] & 0xFF));
			tmp++;
			temp++;
		}
		//防止上述while退出时，最后字符数不足3个，prepare还是要凑够3个字符的位置
		prepare = (prepare << ((3 - temp) * 8));

		//4次for循环，当3个字符存在的时候，就可以将其拆成4个字符
		for (int i = 0; i < 4; i++)
		{
			if (temp < i)
			{
				changed[i] = 0x40;
			}
			else
			{
				//0x3F：6bit表示一个字符
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

//转换算子(查找字符的位置)
static char find_pos(char ch)
{
	//strrchr(str,ch)函数找出str字符串中最后一次出现的字符ch出现的地址，然后将地址返回
	char *ptr = (char*)strrchr(base, ch);
	return (ptr - base);
}

//Base64 解码
void Base64_decode(const char *filename)
{
	string unencode = filename;
	//找到第一个'.'出现的位置
	size_t pos = unencode.rfind('.');
	assert(pos != string::npos);//npos是string定义为保证大于所有有效下标的值
	//substr函数返回一个新建的初始化为string对象的子串的拷贝对象
	unencode = unencode.substr(0, pos);
	unencode += ".unencode";

	//以读的方式打开编码文件，以写的方式打开解码文件
	FILE* fin = fopen(unencode.c_str(), "wb");
	assert(fin);
	FILE* fout = fopen(filename, "rb");
	assert(fout);

	int data_len = Size(filename);
	char* data = ReadFromFile(fout,data_len);

	//有多少的四个字符解码后为三个字符
	int ret_len = (data_len / 4) * 3;//解码开辟空间的大小
	int equal_count = 0;//等号的个数
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
	
	//为了4个字符转3个字符的时候字节进行对齐
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