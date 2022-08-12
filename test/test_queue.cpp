#include "../containers_seqence/queue.h"

#include <iostream>

using namespace::GHYSTL;

int main()
{

	queue<int> test;
	std::cout << "************************压入测试************************" << std::endl << std::endl;
	std::cout << "queue只能从后端弹出" << std::endl << std::endl;
	std::cout << "压入元素：";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << i << ",";
		test.push(i);
	}
	std::cout << std::endl << std::endl << "前端元素：";
	std::cout << test.front() << std::endl;
	std::cout << std::endl <<  "后端元素：";
	std::cout << test.back() << std::endl << std::endl;
	std::cout << "栈的长度：" << test.size() << std::endl;
	std::cout << std::endl << std::endl;


	std::cout << "************************弹出测试************************" << std::endl << std::endl;
	std::cout << "queue只能从前端弹出" << std::endl;
	for (int i = 0; i < 4; ++i)
	{
		std::cout << std::endl << "前端元素：";
		std::cout << test.front() << std::endl;
		std::cout << std::endl << "后端元素：";
		std::cout << test.back() << std::endl << std::endl;
		std::cout << "栈的长度：" << test.size() << std::endl;
		std::cout << "-------------------" << std::endl;
		test.pop();
	}
	std::cout << std::endl << std::endl;

	system("pause");
	return 0;
}