#include <iostream>

#include "../containers_seqence/vector.h"

using namespace GHYSTL;

int main(int argc, char* argv[])
{
	vector<int> my_vector;
	std::cout << "size = " << my_vector.size() << "\t";
	std::cout << "capacity = " << my_vector.capacity() << std::endl;

	vector<int> test({2,2});
	std::cout << "size = " << test.size() << "\t";
	std::cout << "capacity = " << test.capacity() << std::endl;

	test.push_back(1);
	std::cout << "size = " << test.size() << "\t";
	std::cout << "capacity = " << test.capacity() << std::endl;

	test.push_back(2);
	std::cout << "size = " << test.size() << "\t";
	std::cout << "capacity = " << test.capacity() << std::endl;

	test.push_back(3);
	std::cout << "size = " << test.size() << "\t";
	std::cout << "capacity = " << test.capacity() << std::endl;

	test.pop_back();
	std::cout << "size = " << test.size() << "\t";
	std::cout << "capacity = " << test.capacity() << std::endl;

	std::cout << test[2] << std::endl;

	test.erase(test.begin() + 1, test.begin() + 2);
	test.erase(test.begin());
	for (vector<int>::iterator it = test.begin(); it != test.end(); ++it)
	{
		std::cout << *it << std::endl;
	}
	system("pause");
	return 0;
}