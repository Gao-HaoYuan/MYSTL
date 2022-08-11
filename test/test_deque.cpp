#include "../containers_seqence/deque.h"

#include <iostream>

using namespace GHYSTL;

int main(){
	/***************************************************************************************/
	/***************************************************************************************/
	std::cout << "************************初始化、前插、后插测试************************" << std::endl;
	std::cout << std::endl;
	deque<int> test(1); // 初始化
	test.push_back(2); // 后插
	test.push_back(3); // 后插
	test.push_back(4); // 后插
	test.push_front(0); // 前插
	test.push_front(-1); // 前插

	std::cout << "当前元素：";
	for (deque<int>::iterator iter = test.begin_(); iter != test.end_(); ++iter){
		std::cout << *iter << ",";
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "长度：" << test.size() << std::endl;
	std::cout << std::endl;




	/***************************************************************************************/
	/***************************************************************************************/
	std::cout << "************************前删、后删测试************************" << std::endl;
	std::cout << std::endl;
	test.pop_front(); // 前删
	test.pop_back(); // 后删
	std::cout << "当前元素：";
	for (deque<int>::iterator iter = test.begin_(); iter != test.end_(); ++iter){
		std::cout << *iter << ",";
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "长度：" << test.size() << std::endl;
	std::cout << std::endl;




	/***************************************************************************************/
	/***************************************************************************************/
	std::cout << "************************清空测试************************" << std::endl;
	std::cout << std::endl;
	test.clear(); // 前删
	std::cout << "当前元素：";
	for (deque<int>::iterator iter = test.begin_(); iter != test.end_(); ++iter){
		std::cout << *iter << ",";
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "长度：" << test.size() << std::endl;
	std::cout << std::endl;




	/***************************************************************************************/
	/***************************************************************************************/
	std::cout << "************************跨主控节点后插测试************************" << std::endl;
	std::cout << std::endl;
	std::cout << "缓冲区默认大小为512字节，一个int占4字节，512 / 4 = 128" << std::endl << std::endl;
	std::cout << "当插入的元素量 > 128 时就会跨主控节点" << std::endl;
	std::cout << std::endl;
	test.clear(); // 前删
	for (int i = 0; i < 150; i++){
		test.push_back(i);
	}
	std::cout << "当前元素：";
	for (deque<int>::iterator iter = test.begin_(); iter != test.end_(); ++iter){
		std::cout << *iter << ",";
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "长度：" << test.size() << std::endl;
	std::cout << std::endl;




	/***************************************************************************************/
	/***************************************************************************************/
	std::cout << "************************跨主控节点前插测试************************" << std::endl;
	std::cout << std::endl;
	std::cout << "缓冲区默认大小为512字节，一个int占4字节，512 / 4 = 128" << std::endl << std::endl;
	std::cout << "当插入的元素量 > 128 时就会跨主控节点" << std::endl;
	std::cout << std::endl;
	test.clear(); // 前删
	for (int i = 0; i < 150; i++){
		test.push_front(i);
	}
	std::cout << "当前元素：";
	for (deque<int>::iterator iter = test.begin_(); iter != test.end_(); ++iter){
		std::cout << *iter << ",";
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "长度：" << test.size() << std::endl;
	std::cout << std::endl;

	std::cout << "************************测试结束************************" << std::endl;
	std::cout << std::endl;
	system("pause");
	return 0;
}