#include "../containers_seqence/list.h"

#include <iostream>

using namespace GHYSTL;

int main()
{

	list<int> tst;
	int i = 0;
	tst.push_back(1); // 在list尾部插入1
	tst.push_back(2); // 在list尾部插入2
	tst.push_back(2); // 在list尾部插入2
	tst.push_back(2); // 在list尾部插入2
	tst.push_front(3); // 在list头部插入1

	std::cout << "-----------------插入元素----------------" << std::endl;
	std::cout << "在list尾部依次插入1、2、2、2,在list头部插入3" << std::endl;
	// 取得list的长度
	std::cout << "list的长度:" << tst.size() << std::endl;
	// 遍历list
	for (list<int>::iterator iter = tst.begin(); iter != tst.end(); iter++)
	{
		std::cout << "第" << i << "个元素：" << *iter << std::endl;
		i++;
	}

	std::cout << std::endl << std::endl << "-----------------列表去重----------------" << std::endl;
	// 列表去重
	tst.unique();
	// 取得list的长度
	std::cout << "list的长度:" << tst.size() << std::endl;
	// 遍历list
	i = 0;
	for (list<int>::iterator iter = tst.begin(); iter != tst.end(); iter++)
	{
		std::cout << "第" << i << "个元素：" << *iter << std::endl;
		i++;
	}

	std::cout << std::endl << std::endl << "-----------------删除值等于2的元素----------------" << std::endl;
	tst.remove(2);// 删除值等于2的元素
	// 取得list的长度
	std::cout << "list的长度:" << tst.size() << std::endl;
	// 遍历list
	i = 0;
	for (list<int>::iterator iter = tst.begin(); iter != tst.end(); iter++)
	{
		std::cout << "第" << i << "个元素：" << *iter << std::endl;
		i++;
	}

	std::cout << std::endl << std::endl << "-----------------清空list----------------" << std::endl;
	tst.clear(); // 清空
	// 取得list的长度
	std::cout << "list的长度:" << tst.size() << std::endl;
	// 遍历list
	i = 0;
	for (list<int>::iterator iter = tst.begin(); iter != tst.end(); iter++)
	{
		std::cout << "第" << i << "个元素：" << *iter << std::endl;
		i++;
	}

	system("pause");
	return 0;
}