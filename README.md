# MYSTL
自己实现的STL库

参考了其他开源的项目

完成的组件： 算法、空间配置器、配接器、仿函数、迭代器
  

实现了vector 并测试通过，基于vector 实现了 heap（priority_queue）

实现了deque 并测试通过，基于deque实现了 stack、queue

实现了list 并测试通过，单链表不写了，就是增加删除节点的时候，更改一下节点的连接顺序

实现了红黑树、哈希表

基于红黑树和哈希表，实现了map、set、multi_map、multi_set，以及 unordered_map、unordered_set、unordered_multimap、unordered_multiset

实现了 string， 对 char* 、 wchar_t、char16_t、char32_t 的封装


STL库就写到了这里了，项目只是一个简单的 demo， 有很多不足的地方，比如哈希表现在大多采用 vector + 一个单链表的形式，而我使用的是一个 vector + n个单链表。
非常感谢 git 开源的大佬项目，让我学习到了很多，虽然我对自己代码进行了测试，但肯定还存在没有发现的 bug，后续有机会的话，会对项目中的 bug 进行修复和升级完善。
