#include "../containers_seqence/priority_queue.h"

#include <iostream>

using namespace::GHYSTL;


void p_queue_print(GHYSTL::priority_queue<int> p)
{
  while (!p.empty())
  {
    std::cout << " " << p.top();
    p.pop();
  }
  std::cout << std::endl;
}

//  queue 的遍历输出
#define QUEUE_COUT(q) do {                       \
    std::string q_name = #q;                     \
    std::cout << " " << q_name << " :";          \
    queue_print(q);                              \
} while(0)

// priority_queue 的遍历输出
#define P_QUEUE_COUT(p) do {                     \
    std::string p_name = #p;                     \
    std::cout << " " << p_name << " :";          \
    p_queue_print(p);                            \
} while(0)

#define QUEUE_FUN_AFTER(con, fun) do {           \
  std::string fun_name = #fun;                   \
  std::cout << " After " << fun_name << " :\n";  \
  fun;                                           \
  QUEUE_COUT(con);                               \
} while(0)

#define P_QUEUE_FUN_AFTER(con, fun) do {         \
  std::string fun_name = #fun;                   \
  std::cout << " After " << fun_name << " :\n";  \
  fun;                                           \
  P_QUEUE_COUT(con);                             \
} while(0)

void priority_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[------------- Run container test : priority_queue -------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 1,2,3,4,5 };
  GHYSTL::vector<int> v1(5);
  GHYSTL::priority_queue<int> p1;
  GHYSTL::priority_queue<int> p2(5);
  GHYSTL::priority_queue<int> p3(5, 1);
  GHYSTL::priority_queue<int> p4(a, a + 5);
  GHYSTL::priority_queue<int> p5(v1);
  GHYSTL::priority_queue<int> p6(std::move(v1));
  GHYSTL::priority_queue<int> p7(p2);
  GHYSTL::priority_queue<int> p8(std::move(p2));
  GHYSTL::priority_queue<int> p9;
  p9 = p3;
  GHYSTL::priority_queue<int> p10;
  p10 = std::move(p3);
  GHYSTL::priority_queue<int> p11{ 1,2,3,4,5 };
  GHYSTL::priority_queue<int> p12;
  p12 = { 1,2,3,4,5 };

  P_QUEUE_FUN_AFTER(p1, p1.push(1));
  P_QUEUE_FUN_AFTER(p1, p1.push(5));
  P_QUEUE_FUN_AFTER(p1, p1.push(3));
  P_QUEUE_FUN_AFTER(p1, p1.pop());
  P_QUEUE_FUN_AFTER(p1, p1.emplace(7));
  P_QUEUE_FUN_AFTER(p1, p1.emplace(2));
  P_QUEUE_FUN_AFTER(p1, p1.emplace(8));

  while (!p1.empty())
  {
    P_QUEUE_FUN_AFTER(p1, p1.pop());
  }
  P_QUEUE_FUN_AFTER(p1, p1.swap(p4));
  P_QUEUE_FUN_AFTER(p1, p1.clear());

  std::cout << "[------------- End container test : priority_queue -------------]" << std::endl;
}

int main()
{
	priority_test();
	return 0;
}