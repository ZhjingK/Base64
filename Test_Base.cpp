#define _CRT_SECURE_NO_WARNINGS 1
#include"Base.h"
#include "stdafx.h"
#include <gtest/gtest.h>
#include<io.h>

//ͳ���ļ����ַ��ĸ���
TEST(File, Num)
{
	Base64 b1;
	EXPECT_EQ(b1.Size("abc.txt"),240);
}

//���ļ��ж�ȡ�ַ�
TEST(File, Char)
{
	Base64 b2;
	FILE* fp = fopen("abc.txt", "r");
	int data_len = b2.Size("abc.txt");
	char* data = b2.ReadFromFile(fp, data_len);
	EXPECT_STREQ(data,"Whether sixty or sixteen");
}

//������д���ļ���
TEST(File, Write)
{
	char* data = "Whether sixty or sixteen";
	FILE* fp = fopen("abc.txt", "wb");
	Base64 b3;
	b3.WriteToFile(data, fp);
	fclose(fp);
	FILE* fd = fopen("abc.txt", "rb");
	int size = b3.Size("abc.txt");
	char* data2 = b3.ReadFromFile(fp, size);
	EXPECT_STREQ(data, data2);
}

//����ǰ�жϱ����ļ��Ƿ����
TEST(File, CodeName)
{
	bool test = _access("F:\C++\Test_Base\Test_Base\abc.encode", 0);
	EXPECT_TRUE(test);
}

//�ļ�����
TEST(File, Code)
{
	Base64 b4;
	b4.Base64_encode("abc.txt");
	int size1 = b4.Size("abc.txt");
	int size2 = b4.Size("abc.encode");
	
	//conditionΪ��
	//_access()�����ж��ļ��Ƿ���ڲ��Ƿ���ж�Ӧ��Ȩ�ޣ�������Ȩ�޷���0����������Ȩ�޷���1
	EXPECT_TRUE(_access("F:\C++\Test_Base\Test_Base\abc.encode",0));
	EXPECT_GT(size2, size1);
}

//����֮ǰ�жϽ����ļ����Ƿ����
TEST(File, DecodeName)
{
	bool test = _access("F:\C++\Test_Base\Test_Base\abc.unencode", 0);
	EXPECT_TRUE(test);
}

//�ļ�����
TEST(File, Decode)
{
	Base64 b5;
	b5.Base64_decode("abc.encode");

	int size1 = b5.Size("abc.txt");
	int size2 = b5.Size("abc.unencode");

	EXPECT_TRUE(_access("F:\C++\Test_Base\Test_Base\abc.unencode", 0));
	EXPECT_EQ(size2, size1);
}

int _tmain(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	int a = RUN_ALL_TESTS();

	system("pause");
	return 0;
}