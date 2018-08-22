#define _CRT_SECURE_NO_WARNINGS 1
#include"Base.h"
#include "stdafx.h"
#include <gtest/gtest.h>
#include<io.h>

//统计文件中字符的个数
TEST(File, Num)
{
	Base64 b1;
	EXPECT_EQ(b1.Size("abc.txt"),240);
}

//从文件中读取字符
TEST(File, Char)
{
	Base64 b2;
	FILE* fp = fopen("abc.txt", "r");
	int data_len = b2.Size("abc.txt");
	char* data = b2.ReadFromFile(fp, data_len);
	EXPECT_STREQ(data,"Whether sixty or sixteen");
}

//将数据写到文件中
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

//编码前判断编码文件是否存在
TEST(File, CodeName)
{
	bool test = _access("F:\C++\Test_Base\Test_Base\abc.encode", 0);
	EXPECT_TRUE(test);
}

//文件编码
TEST(File, Code)
{
	Base64 b4;
	b4.Base64_encode("abc.txt");
	int size1 = b4.Size("abc.txt");
	int size2 = b4.Size("abc.encode");
	
	//condition为假
	//_access()函数判断文件是否存在并是否具有对应的权限，存在有权限返回0，不存在无权限返回1
	EXPECT_TRUE(_access("F:\C++\Test_Base\Test_Base\abc.encode",0));
	EXPECT_GT(size2, size1);
}

//解码之前判断解码文件名是否存在
TEST(File, DecodeName)
{
	bool test = _access("F:\C++\Test_Base\Test_Base\abc.unencode", 0);
	EXPECT_TRUE(test);
}

//文件解码
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