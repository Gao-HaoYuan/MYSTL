#include "../containers_seqence/stack.h"

#include <iostream>

using namespace::GHYSTL;

int main()
{
	stack<int> test;
	std::cout << "************************压栈测试************************" << std::endl;
	std::cout << "压入元素：";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << i << ",";
		test.push(i);
	}
	std::cout << std::endl << std::endl << "栈顶元素：";
	std::cout << test.top() << std::endl << std::endl;
	std::cout << "栈的长度：" << test.size() << std::endl;
	std::cout << std::endl << std::endl;


	std::cout << "************************弹栈栈测试************************" << std::endl << std::endl;
	std::cout << "依次弹出：" << std::endl << std::endl;
	for (int i = 0; i < 4; ++i)
	{
		std::cout << "\t" << test.top() << std::endl << std::endl;
		test.pop();
	}
	std::cout << "栈的长度：" << test.size() << std::endl;
	std::cout << std::endl << std::endl;
	std::cout << "************************测试结束************************" << std::endl;

	system("pause");
	return 0;
}