#include "../containers_associative/unordered_map.h"
#include "../containers_seqence/vector.h"

#include <iostream>

using namespace GHYSTL;

// pair 的宏定义
#define PAIR    GHYSTL::pair<const int, int>  // map 的 key 不能修改， 所以 map 中我声明的是 const类型

// map 的遍历输出
#define MAP_COUT(m) do { \
    std::string m_name = #m; \
    std::cout << " " << m_name << " :"; \
    for (auto it : m)    std::cout << " <" << it.first << "," << it.second << ">"; \
    std::cout << std::endl; \
} while(0)

// map 的函数操作
#define MAP_FUN_AFTER(con, fun) do { \
    std::string str = #fun; \
    std::cout << " After " << str << " :" << std::endl; \
    fun; \
    MAP_COUT(con); \
} while(0)

// 输出容器调用函数的值
#define FUN_VALUE(fun) do {                              \
  std::string fun_name = #fun;                           \
  std::cout << " " << fun_name << " : " << fun << "\n";  \
} while(0)

// map 的函数值
#define MAP_VALUE(fun) do { \
    std::string str = #fun; \
    auto it = fun; \
    std::cout << " " << str << " : <" << it.first << "," << it.second << ">\n"; \
} while(0)

void unordered_map_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[-------------- Run container test : unordered_map -------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  GHYSTL::vector<PAIR> v;
  for (int i = 0; i < 5; ++i)
    v.push_back(PAIR(5 - i, 5 - i));
  GHYSTL::unordered_map<int, int> um1;
  GHYSTL::unordered_map<int, int> um2(520);
  GHYSTL::unordered_map<int, int> um3(520, GHYSTL::hash<int>());
  GHYSTL::unordered_map<int, int> um4(520, GHYSTL::hash<int>(), GHYSTL::equal_to<int>());
  GHYSTL::unordered_map<int, int> um5(v.begin(), v.end());
  GHYSTL::unordered_map<int, int> um6(v.begin(), v.end(), 100);
  GHYSTL::unordered_map<int, int> um7(v.begin(), v.end(), 100, GHYSTL::hash<int>());
  GHYSTL::unordered_map<int, int> um8(v.begin(), v.end(), 100, GHYSTL::hash<int>(), GHYSTL::equal_to<int>());
  GHYSTL::unordered_map<int, int> um9(um5);
  GHYSTL::unordered_map<int, int> um10(std::move(um5));
  GHYSTL::unordered_map<int, int> um11;
  um11 = um6;
  GHYSTL::unordered_map<int, int> um12;
  um12 = std::move(um6);
  GHYSTL::unordered_map<int, int> um13{PAIR(1,1), PAIR(2,3), PAIR(3,3)};
  GHYSTL::unordered_map<int, int> um14;
  um14 = { PAIR(1,1),PAIR(2,3),PAIR(3,3) };

  MAP_FUN_AFTER(um1, um1.emplace(PAIR(1, 1)));
  MAP_FUN_AFTER(um1, um1.emplace_hint(um1.begin(), PAIR(1, 2)));
  MAP_FUN_AFTER(um1, um1.insert(PAIR(2, 2)));
  MAP_FUN_AFTER(um1, um1.insert(um1.end(), PAIR(3, 3)));
  MAP_FUN_AFTER(um1, um1.insert(v.begin(), v.end()));
  MAP_FUN_AFTER(um1, um1.erase(um1.begin()));
  MAP_FUN_AFTER(um1, um1.erase(um1.begin(), um1.find(3)));
  MAP_FUN_AFTER(um1, um1.erase(1));
  std::cout << std::boolalpha;
  FUN_VALUE(um1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(um1.size());
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.max_size());
  FUN_VALUE(um1.bucket(1));
  FUN_VALUE(um1.bucket_size(um1.bucket(5)));
  MAP_FUN_AFTER(um1, um1.clear());
  MAP_FUN_AFTER(um1, um1.swap(um7));
  MAP_VALUE(*um1.begin());
  FUN_VALUE(um1.at(1));
  FUN_VALUE(um1[1]);
  std::cout << std::boolalpha;
  FUN_VALUE(um1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(um1.size());
  FUN_VALUE(um1.max_size());
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.max_size());
  FUN_VALUE(um1.bucket(1));
  FUN_VALUE(um1.bucket_size(um1.bucket(1)));
  MAP_FUN_AFTER(um1, um1.reserve(1000));
  FUN_VALUE(um1.size());
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.bucket_size(1));
  FUN_VALUE(um1.bucket_size(2));
  FUN_VALUE(um1.bucket_size(3));
  MAP_FUN_AFTER(um1, um1.rehash(150));
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.count(1));
  MAP_VALUE(*um1.find(3));
  auto first = *um1.equal_range(3).first;
  auto second = *um1.equal_range(3).second;
  std::cout << " um1.equal_range(3) : from <" << first.first << ", " << first.second
    << "> to <" << second.first << ", " << second.second << ">" << std::endl;
  FUN_VALUE(um1.load_factor());

  std::cout << "[-------------- End container test : unordered_map -------------]" << std::endl;
}


void unordered_multimap_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[----------- Run container test : unordered_multimap -----------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  GHYSTL::vector<PAIR> v;
  for (int i = 0; i < 5; ++i)
    v.push_back(PAIR(5 - i, 5 - i));
  GHYSTL::unordered_multimap<int, int> um1;
  GHYSTL::unordered_multimap<int, int> um2(520);
  GHYSTL::unordered_multimap<int, int> um3(520, GHYSTL::hash<int>());
  GHYSTL::unordered_multimap<int, int> um4(520, GHYSTL::hash<int>(), GHYSTL::equal_to<int>());
  GHYSTL::unordered_multimap<int, int> um5(v.begin(), v.end());
  GHYSTL::unordered_multimap<int, int> um6(v.begin(), v.end(), 100);
  GHYSTL::unordered_multimap<int, int> um7(v.begin(), v.end(), 100, GHYSTL::hash<int>());
  GHYSTL::unordered_multimap<int, int> um8(v.begin(), v.end(), 100, GHYSTL::hash<int>(), GHYSTL::equal_to<int>());
  GHYSTL::unordered_multimap<int, int> um9(um5);
  GHYSTL::unordered_multimap<int, int> um10(std::move(um5));
  GHYSTL::unordered_multimap<int, int> um11;
  um11 = um6;
  GHYSTL::unordered_multimap<int, int> um12;
  um12 = std::move(um6);
  GHYSTL::unordered_multimap<int, int> um13{ PAIR(1,1),PAIR(2,3),PAIR(3,3) };
  GHYSTL::unordered_multimap<int, int> um14;
  um14 = { PAIR(1,1),PAIR(2,3), PAIR(3,3) };

  MAP_FUN_AFTER(um1, um1.emplace(PAIR(1, 1)));
  MAP_FUN_AFTER(um1, um1.emplace_hint(um1.begin(), PAIR(1, 2)));
  MAP_FUN_AFTER(um1, um1.insert(PAIR(2, 2)));
  MAP_FUN_AFTER(um1, um1.insert(um1.end(), PAIR(3, 3)));
  MAP_FUN_AFTER(um1, um1.insert(v.begin(), v.end()));
  MAP_FUN_AFTER(um1, um1.erase(um1.begin()));
  MAP_FUN_AFTER(um1, um1.erase(um1.begin(), um1.find(3)));
  MAP_FUN_AFTER(um1, um1.erase(1));
  std::cout << std::boolalpha;
  FUN_VALUE(um1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(um1.size());
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.max_size());
  FUN_VALUE(um1.bucket(1));
  FUN_VALUE(um1.bucket_size(um1.bucket(5)));
  MAP_FUN_AFTER(um1, um1.clear());
  MAP_FUN_AFTER(um1, um1.swap(um7));
  MAP_VALUE(*um1.begin());
  std::cout << std::boolalpha;
  FUN_VALUE(um1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(um1.size());
  FUN_VALUE(um1.max_size());
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.max_size());
  FUN_VALUE(um1.bucket(1));
  FUN_VALUE(um1.bucket_size(um1.bucket(1)));
  MAP_FUN_AFTER(um1, um1.reserve(1000));
  FUN_VALUE(um1.size());
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.bucket_size(1));
  FUN_VALUE(um1.bucket_size(2));
  FUN_VALUE(um1.bucket_size(3));
  MAP_FUN_AFTER(um1, um1.rehash(150));
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.count(1));
  MAP_VALUE(*um1.find(3));
  auto first = *um1.equal_range(3).first;
  auto second = *um1.equal_range(3).second;
  std::cout << " um1.equal_range(3) : from <" << first.first << ", " << first.second
    << "> to <" << second.first << ", " << second.second << ">" << std::endl;
  FUN_VALUE(um1.load_factor());

  std::cout << "[----------- End container test : unordered_multimap -----------]" << std::endl;
}

int main(){

    // unordered_map_test();

    unordered_multimap_test();

    return 0;
}