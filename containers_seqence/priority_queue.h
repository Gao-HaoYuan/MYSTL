/**
 * @file priority_queue.h
 * @author ghy (ghy_mike@163.com)
 * @brief heap(堆)的实现
 * 
 * 
 * @version 1.0
 * @date 2022-08-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include "vector.h"

namespace GHYSTL{

template<typename value_type_, typename Container = GHYSTL::vector<value_type_>,
             typename Compare = GHYSTL::less<typename Container::value_type>>

class priority_queue{
public:
    typedef Container                               container_type;
    typedef Compare                                 value_compare;
    // 使用底层容器的型别
    typedef typename Container::value_type          value_type;
    typedef typename Container::size_type           size_type;
    typedef typename Container::reference           reference;
    typedef typename Container::const_reference     const_reference;

    static_assert(std::is_same<value_type_, value_type>::value,
                "the value_type of Container should be same with T");

private:
    container_type      c_heap;  // vector 实现堆
    value_compare       cmp;    // 权值比较 

public:

    /***************************************** 构造函数 *******************************************/
    priority_queue() = default;

    priority_queue(const Compare& c) : c_heap(), cmp(c) {}

    explicit priority_queue(size_type n) : c_heap(n){
        GHYSTL::make_heap(c_heap.begin(), c_heap.end(), cmp);
    }

    priority_queue(size_type n, const value_type& value) : c_heap(n, value){
        GHYSTL::make_heap(c_heap.begin(), c_heap.end(), cmp);
    }

    template<typename IIter>
    priority_queue(IIter first, IIter last) : c_heap(first, last) {
        GHYSTL::make_heap(c_heap.begin(), c_heap.end(), cmp);
    }

    priority_queue(std::initializer_list<value_type> lst) : c_heap(lst) {
        GHYSTL::make_heap(c_heap.begin(), c_heap.end(), cmp);
    }

    priority_queue(const Container& c) : c_heap(c) {
        GHYSTL::make_heap(c_heap.begin(), c_heap.end(), cmp);
    }

    priority_queue(Container&& c) : c_heap(std::move(c)) {
        GHYSTL::make_heap(c_heap.begin(), c_heap.end(), cmp);
    }

    priority_queue(const priority_queue& rhs) : c_heap(rhs.c_heap), cmp(rhs.cmp){
        GHYSTL::make_heap(c_heap.begin(), c_heap.end(), cmp);
    }

    priority_queue(priority_queue&& rhs) : c_heap(std::move(rhs.c_heap)), cmp(rhs.cmp){
        GHYSTL::make_heap(c_heap.begin(), c_heap.end(), cmp);
    }

    priority_queue& operator=(const priority_queue& rhs){
        c_heap = rhs.c_heap;
        cmp = rhs.cmp;
        GHYSTL::make_heap(c_heap.begin(), c_heap.end(), cmp);
        return *this;
    }

    priority_queue& operator=(priority_queue&& rhs){
        c_heap = std::move(rhs.c_heap);
        cmp = rhs.cmp;
        GHYSTL::make_heap(c_heap.begin(), c_heap.end(), cmp);
        return *this;
    }

    priority_queue& operator=(std::initializer_list<value_type> lst){
        c_heap = lst;
        cmp = value_compare();
        GHYSTL::make_heap(c_heap.begin(), c_heap.end(), cmp);
        return *this;
    }

    ~priority_queue() = default;


    /***************************************** 元素访问 *******************************************/

    const_reference top() const { return c_heap.front(); }

    bool empty() const noexcept { return c_heap.empty(); }

    size_type size() const noexcept { return c_heap.size(); } 

    /***************************************** 插入删除 *******************************************/

    template<typename ... types>
    void emplace(types && ... args){
        c_heap.emplace_back(std::forward<types>(args)...);
        GHYSTL::make_heap(c_heap.begin(), c_heap.end(), cmp);
    }

    void push(const value_type& value){
        c_heap.push_back(value);
        GHYSTL::make_heap(c_heap.begin(), c_heap.end(), cmp);
    }

    void push(value_type&& value){
        c_heap.push_back(std::move(value));
        GHYSTL::make_heap(c_heap.begin(), c_heap.end(), cmp);
    }

    void pop(){
        GHYSTL::pop_heap(c_heap.begin(), c_heap.end(), cmp);
        c_heap.pop_back();
    }

    void clear(){
        while (!empty())
            pop();
    }
    
public:
    void swap(priority_queue& rhs) noexcept(noexcept(GHYSTL::swap(c_heap, rhs.c_heap))
                                                     && noexcept(GHYSTL::swap(cmp, rhs.cmp)))
    {
        GHYSTL::swap(c_heap, rhs.c_heap);
        GHYSTL::swap(cmp, rhs.cmp);
    }

    friend bool operator==(const priority_queue& lhs, const priority_queue& rhs){
        return lhs.c_heap == rhs.c_heap;
    }
    friend bool operator!=(const priority_queue& lhs, const priority_queue& rhs){
        return lhs.c_heap != rhs.c_heap;
    }
};

 /***************************************** 重载比较操作符 *******************************************/
template <class value_type, class Container, class Compare>
bool operator==(priority_queue<value_type, Container, Compare>& lhs,
                priority_queue<value_type, Container, Compare>& rhs)
{
  return lhs == rhs;
}

template <class value_type, class Container, class Compare>
bool operator!=(priority_queue<value_type, Container, Compare>& lhs,
                priority_queue<value_type, Container, Compare>& rhs)
{
  return lhs != rhs;
}

// 重载 mystl 的 swap
template <class value_type, class Container, class Compare>
void swap(priority_queue<value_type, Container, Compare>& lhs, 
          priority_queue<value_type, Container, Compare>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

}// namespace GHYSTL
#endif