#include "../containers_associative/unordered_set.h"

#include <iostream>

// 遍历输出容器
#define COUT(container) do {                             \
  std::string con_name = #container;                     \
  std::cout << " " << con_name << " :";                  \
  for (auto it : container)                              \
    std::cout << " " << it;                              \
  std::cout << "\n";                                     \
} while(0)

// 输出容器调用函数后的结果
#define FUN_AFTER(con, fun) do {                         \
  std::string fun_name = #fun;                           \
  std::cout << " After " << fun_name << " :\n";          \
  fun;                                                   \
  COUT(con);                                             \
} while(0)

// 输出容器调用函数的值
#define FUN_VALUE(fun) do {                              \
  std::string fun_name = #fun;                           \
  std::cout << " " << fun_name << " : " << fun << "\n";  \
} while(0)

using namespace GHYSTL;

void unordered_set_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[-------------- Run container test : unordered_set -------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 5,4,3,2,1 };
  GHYSTL::unordered_set<int> us1;
  GHYSTL::unordered_set<int> us2(520);
  GHYSTL::unordered_set<int> us3(520, GHYSTL::hash<int>());
  GHYSTL::unordered_set<int> us4(520, GHYSTL::hash<int>(), GHYSTL::equal_to<int>());
  GHYSTL::unordered_set<int> us5(a, a + 5);
  GHYSTL::unordered_set<int> us6(a, a + 5, 100);
  GHYSTL::unordered_set<int> us7(a, a + 5, 100, GHYSTL::hash<int>());
  GHYSTL::unordered_set<int> us8(a, a + 5, 100, GHYSTL::hash<int>(), GHYSTL::equal_to<int>());
  GHYSTL::unordered_set<int> us9(us5);
  GHYSTL::unordered_set<int> us10(std::move(us5));
  GHYSTL::unordered_set<int> us11;
  us11 = us6;
  GHYSTL::unordered_set<int> us12;
  us12 = std::move(us6);
  GHYSTL::unordered_set<int> us13{ 1,2,3,4,5 };
  GHYSTL::unordered_set<int> us14;
  us13 = { 1, 2, 4, 5};

  FUN_AFTER(us1, us1.emplace(1));
  FUN_AFTER(us1, us1.emplace_hint(us1.end(), 2));
  FUN_AFTER(us1, us1.insert(5));
  FUN_AFTER(us1, us1.insert(us1.begin(), 5));
  FUN_AFTER(us1, us1.insert(a, a + 5));
  FUN_AFTER(us1, us1.erase(us1.begin()));
  FUN_AFTER(us1, us1.erase(us1.begin(), us1.find(3)));
  FUN_AFTER(us1, us1.erase(1));
  std::cout << std::boolalpha;
  FUN_VALUE(us1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.bucket_count());
  FUN_VALUE(us1.max_size());
  FUN_VALUE(us1.bucket(1));
  FUN_VALUE(us1.bucket_size(us1.bucket(5)));
  FUN_AFTER(us1, us1.clear());
  FUN_AFTER(us1, us1.swap(us7));
  FUN_VALUE(*us1.begin());
  std::cout << std::boolalpha;
  FUN_VALUE(us1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.max_size());
  FUN_VALUE(us1.bucket_count());
  FUN_AFTER(us1, us1.reserve(1000));
  FUN_VALUE(*us1.begin(us1.bucket(1)));
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.bucket_count());
  FUN_VALUE(us1.bucket_size(1));
  FUN_VALUE(us1.bucket_size(2));
  FUN_VALUE(us1.bucket_size(3));
  FUN_AFTER(us1, us1.rehash(150));
  FUN_VALUE(us1.bucket_count());
  FUN_VALUE(us1.count(1));
  FUN_VALUE(*us1.find(3));
  auto first = *us1.equal_range(3).first;
  auto second = *us1.equal_range(3).second;
  std::cout << " us1.equal_range(3) : from " << first << " to " << second << std::endl;
  FUN_VALUE(us1.load_factor());

  std::cout << "[-------------- End container test : unordered_set -------------]" << std::endl;
}

void unordered_multiset_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[------------ Run container test : unordered_multiset ----------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 5,4,3,2,1 };
  GHYSTL::unordered_multiset<int> us1;
  GHYSTL::unordered_multiset<int> us2(520);
  GHYSTL::unordered_multiset<int> us3(520, GHYSTL::hash<int>());
  GHYSTL::unordered_multiset<int> us4(520, GHYSTL::hash<int>(), GHYSTL::equal_to<int>());
  GHYSTL::unordered_multiset<int> us5(a, a + 5);
  GHYSTL::unordered_multiset<int> us6(a, a + 5, 100);
  GHYSTL::unordered_multiset<int> us7(a, a + 5, 100, GHYSTL::hash<int>());
  GHYSTL::unordered_multiset<int> us8(a, a + 5, 100, GHYSTL::hash<int>(), GHYSTL::equal_to<int>());
  GHYSTL::unordered_multiset<int> us9(us5);
  GHYSTL::unordered_multiset<int> us10(std::move(us5));
  GHYSTL::unordered_multiset<int> us11;
  us11 = us6;
  GHYSTL::unordered_multiset<int> us12;
  us12 = std::move(us6);
  GHYSTL::unordered_multiset<int> us13{ 1,2,3,4,5 };
  GHYSTL::unordered_multiset<int> us14;
  us14 = { 1,2,3,4,5 };

  FUN_AFTER(us1, us1.emplace(1));
  FUN_AFTER(us1, us1.emplace_hint(us1.end(), 2));
  FUN_AFTER(us1, us1.insert(5));
  FUN_AFTER(us1, us1.insert(us1.begin(), 5));
  FUN_AFTER(us1, us1.insert(a, a + 5));
  FUN_AFTER(us1, us1.erase(us1.begin()));
  FUN_AFTER(us1, us1.erase(us1.begin(), us1.find(3)));
  FUN_AFTER(us1, us1.erase(1));
  std::cout << std::boolalpha;
  FUN_VALUE(us1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.bucket_count());
  FUN_VALUE(us1.max_size());
  FUN_VALUE(us1.bucket(1));
  FUN_VALUE(us1.bucket_size(us1.bucket(5)));
  FUN_AFTER(us1, us1.clear());
  FUN_AFTER(us1, us1.swap(us7));
  FUN_VALUE(*us1.begin());
  std::cout << std::boolalpha;
  FUN_VALUE(us1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.max_size());
  FUN_VALUE(us1.bucket_count());
  FUN_AFTER(us1, us1.reserve(1000));
  FUN_VALUE(*us1.begin(us1.bucket(1)));
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.bucket_count());
  FUN_VALUE(us1.bucket_size(1));
  FUN_VALUE(us1.bucket_size(2));
  FUN_VALUE(us1.bucket_size(3));
  FUN_AFTER(us1, us1.rehash(150));
  FUN_VALUE(us1.bucket_count());
  FUN_VALUE(us1.count(1));
  FUN_VALUE(*us1.find(3));
  auto first = *us1.equal_range(3).first;
  auto second = *us1.equal_range(3).second;
  std::cout << " us1.equal_range(3) : from " << first << " to " << second << std::endl;
  FUN_VALUE(us1.load_factor());


  std::cout << "[------------ End container test : unordered_multiset ----------]" << std::endl;
}


int main() {    
    // unordered_set_test();
    unordered_multiset_test();
    return 0;
}