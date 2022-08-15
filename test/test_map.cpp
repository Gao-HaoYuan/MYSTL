// map test : 测试 map, multi_map 的接口与它们 insert 的性能

#include "../containers_associative/map.h"
#include "../containers_seqence/vector.h"

#include <iostream>

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

// map 的函数值
#define MAP_VALUE(fun) do { \
    std::string str = #fun; \
    auto it = fun; \
    std::cout << " " << str << " : <" << it.first << "," << it.second << ">\n"; \
} while(0)

void map_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[------------------ Run container test : map -------------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  GHYSTL::vector<PAIR> v;
  for (int i = 0; i < 5; ++i)
    v.push_back(PAIR(i, i));
  GHYSTL::map<int, int> m1;
  GHYSTL::map<int, int, GHYSTL::greater<int>> m2;
  GHYSTL::map<int, int> m3(v.begin(), v.end());  
  GHYSTL::map<int, int> m4(v.begin(), v.end());
  GHYSTL::map<int, int> m5(m3);
  GHYSTL::map<int, int> m6(std::move(m3));
  GHYSTL::map<int, int> m7;
  m7 = m4;
  GHYSTL::map<int, int> m8;
  m8 = std::move(m4);
  GHYSTL::map<int, int> m9{ PAIR(1,1),PAIR(3,2),PAIR(2,3) };
  GHYSTL::map<int, int> m10;
  m10 = { PAIR(1,1),PAIR(3,2),PAIR(2,3) };

  for (int i = 5; i > 0; --i)
  {
    MAP_FUN_AFTER(m1, m1.emplace(PAIR(i, i)));
  }
  MAP_FUN_AFTER(m1, m1.emplace_hint(m1.begin(), PAIR(0, 0)));
  MAP_FUN_AFTER(m1, m1.erase(m1.begin()));
  MAP_FUN_AFTER(m1, m1.erase(0));
  MAP_FUN_AFTER(m1, m1.erase(1));
  MAP_FUN_AFTER(m1, m1.erase(m1.begin(), m1.end()));
  for (int i = 0; i < 5; ++i)
  {
    MAP_FUN_AFTER(m1, m1.insert(PAIR(i, i)));
  }
  MAP_FUN_AFTER(m1, m1.insert(v.begin(), v.end()));
  MAP_FUN_AFTER(m1, m1.insert(m1.end(), PAIR(5, 5)));
  MAP_VALUE(*m1.find(3));
  MAP_VALUE(*m1.lower_bound(3));
  MAP_VALUE(*m1.upper_bound(2));
  auto first = *m1.equal_range(2).first;
  auto second = *m1.equal_range(2).second;
  std::cout << " m1.equal_range(2) : from <" << first.first << ", " << first.second
    << "> to <" << second.first << ", " << second.second << ">" << std::endl;
  MAP_FUN_AFTER(m1, m1.erase(m1.begin()));
  // MAP_FUN_AFTER(m1, m1.erase(1));
  MAP_FUN_AFTER(m1, m1.erase(m1.begin(), m1.find(3)));
  MAP_FUN_AFTER(m1, m1.clear());
  MAP_FUN_AFTER(m1, m1.swap(m9));
  MAP_VALUE(*m1.begin());
  MAP_VALUE(*m1.rbegin());
  std::cout<< m1[1] << std::endl;
  MAP_FUN_AFTER(m1, m1[1] = 3);
  std::cout<< m1.at(1) << std::endl;
  std::cout << std::boolalpha;
  std::cout<< m1.empty() << std::endl;
  std::cout << std::noboolalpha; // 将 bool 变量按照 false true 输出
  std::cout<< m1.size() << std::endl;
  std::cout<< m1.max_size() << std::endl;

  std::cout << "[------------------ End container test : map -------------------]" << std::endl;
}

void multi_map_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[---------------- Run container test : multi_map ----------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  GHYSTL::vector<PAIR> v;
  for (int i = 0; i < 5; ++i)
    v.push_back(PAIR(i, i));
  GHYSTL::multi_map<int, int> m1;
  GHYSTL::multi_map<int, int, GHYSTL::greater<int>> m2;
  GHYSTL::multi_map<int, int> m3(v.begin(), v.end());
  GHYSTL::multi_map<int, int> m4(v.begin(), v.end());
  GHYSTL::multi_map<int, int> m5(m3);
  GHYSTL::multi_map<int, int> m6(std::move(m3));
  GHYSTL::multi_map<int, int> m7;
  m7 = m4;
  GHYSTL::multi_map<int, int> m8;
  m8 = std::move(m4);
  GHYSTL::multi_map<int, int> m9{ PAIR(1,1),PAIR(3,2),PAIR(2,3) };
  GHYSTL::multi_map<int, int> m10;
  m10 = { PAIR(1,1),PAIR(3,2),PAIR(2,3) };

  for (int i = 5; i > 0; --i)
  {
    MAP_FUN_AFTER(m1, m1.emplace(PAIR(i, i)));
  }
  // m1.print_rb();
  MAP_FUN_AFTER(m1, m1.emplace_hint(m1.begin(), PAIR(0, 0)));
  MAP_FUN_AFTER(m1, m1.erase(m1.begin()));
  MAP_FUN_AFTER(m1, m1.erase(0));
  MAP_FUN_AFTER(m1, m1.erase(1));
  MAP_FUN_AFTER(m1, m1.erase(m1.begin(), m1.end()));
  for (int i = 0; i < 5; ++i)
  {
    MAP_FUN_AFTER(m1, m1.insert(PAIR(i, i)));
  }
  MAP_FUN_AFTER(m1, m1.insert(v.begin(), v.end()));
  MAP_FUN_AFTER(m1, m1.insert(PAIR(5, 5)));
  MAP_FUN_AFTER(m1, m1.insert(m1.end(), PAIR(5, 5)));
  std::cout<< m1.count(3) << std::endl;
  MAP_VALUE(*m1.find(3));
  MAP_VALUE(*m1.lower_bound(3));
  MAP_VALUE(*m1.upper_bound(2));
  auto first = *m1.equal_range(1).first;
  auto second = *m1.equal_range(1).second;
  std::cout << " m1.equal_range(1) : from <" << first.first << ", " << first.second
    << "> to <" << second.first << ", " << second.second << ">" << std::endl;
  MAP_FUN_AFTER(m1, m1.erase(m1.begin()));
  MAP_FUN_AFTER(m1, m1.erase(1));
  m1.print_rb();

  MAP_FUN_AFTER(m1, m1.erase(m1.begin(), m1.find(3)));
  MAP_FUN_AFTER(m1, m1.clear());
  MAP_FUN_AFTER(m1, m1.swap(m9));
  MAP_FUN_AFTER(m1, m1.insert(PAIR(3, 3)));
  MAP_VALUE(*m1.begin());
  MAP_VALUE(*m1.rbegin());
  std::cout << std::boolalpha;
  std::cout<< m1.empty() << std::endl;
  std::cout << std::noboolalpha;
  std::cout<< m1.size() << std::endl;
  std::cout<< m1.max_size() << std::endl;

  std::cout << "[---------------- End container test : multi_map ----------------]" << std::endl;
}

int main(){
  // map_test();
  multi_map_test();
  return 0;
}