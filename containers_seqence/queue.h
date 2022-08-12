/**
 * @file queue.h
 * @author ghy (ghy_mike@163.com)
 * @brief queue的实现
 * 
 * 
 * @version 1.0
 * @date 2022-08-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "deque.h"

namespace GHYSTL{

template<typename value_type_, typename Container = GHYSTL::deque<value_type_>>
class queue{
public: 
    typedef Container                                   container_type;
    // 使用底层容器的型别
    typedef typename Container::value_type              value_type;
    typedef typename Container::size_type               size_type;
    typedef typename Container::reference               reference;
    typedef typename Container::const_reference         const_reference;

    static_assert(GHYSTL::is_same<value_type_, value_type>::value, 
                    "the value_type of Container should be same with T");

private:
    container_type c_queue; //底层容器实现队列

public:
    /***************************************** 构造函数 *******************************************/

    queue() = default;

    explicit queue(size_type n) : c_queue(n) {}

    queue(size_type n, const value_type& value) : c_queue(n, value) {}

    template<typename IIter>
    queue(IIter first, IIter last) : c_queue(first, last) {}

    queue(std::initializer_list<value_type> lst) : c_queue(lst.begin(), lst.ebd()) {}

    queue(const Container& c) : c_queue(c) {}

    queue(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value)
        : c_queue(std::move(c)) {}
    
    queue(const queue& rhs) : c_queue(rhs.c_queue) {}

    queue(queue&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value)
        : c_queue(std::move(rhs.c_queue)){ }

    queue& operator=(const queue& rhs){
        c_queue = rhs.c_queue;
        return *this;
    }

    queue& operator=(queue&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value){
        c_queue = std::move(rhs.c_queue);
        return *this;
    }

    queue& operator=(std::initializer_list<value_type> lst){
        c_queue = lst;
        return *this;
    }

    ~queue() = default;

    /***************************************** 元素访问 *******************************************/
    reference           front()         { return c_queue.front(); }
    const_reference     front() const   { return c_queue.front(); } 

    reference           back()          { return c_queue.back(); }
    const_reference     back() const    { return c_queue.back(); }


    bool empty() const noexcept { return c_queue.empty(); }
    size_type size() const noexcept { return c_queue.size(); }


    /***************************************** 元素插入删除 *******************************************/
    template<typename ... types>
    void emplace(types && ... args){
        c_queue.emplace_back(std::forward<types>(args)...);
    }

    void push(const value_type& value){
        c_queue.push_back(value);
    }

    void push(value_type&& value){
        c_queue.emplace_back(std::move(value));
    }

    void pop(){
        c_queue.pop_front();
    }

    void clear(){
        while (!empty())
            pop();
    }

public:
    friend bool operator==(const queue& lhs, const queue& rhs) { return lhs.c_queue == rhs.c_queue; }
    friend bool operator< (const queue& lhs, const queue& rhs) { return lhs.c_queue<  rhs.c_queue; }

    void swap(queue& rhs) noexcept(noexcept(GHYSTL::swap(c_queue, rhs.c_queue))){
        GHYSTL::swap(c_queue, rhs.c_queue);
    }
};

/***************************************** 重载比较操作符 *******************************************/
template <class value_type, class Container>
bool operator==(const queue<value_type, Container>& lhs, const queue<value_type, Container>& rhs){
  return lhs == rhs;
}

template <class value_type, class Container>
bool operator!=(const queue<value_type, Container>& lhs, const queue<value_type, Container>& rhs){
  return !(lhs == rhs);
}

template <class value_type, class Container>
bool operator<(const queue<value_type, Container>& lhs, const queue<value_type, Container>& rhs){
  return lhs < rhs;
}

template <class value_type, class Container>
bool operator>(const queue<value_type, Container>& lhs, const queue<value_type, Container>& rhs){
  return rhs < lhs;
}

template <class value_type, class Container>
bool operator<=(const queue<value_type, Container>& lhs, const queue<value_type, Container>& rhs){
  return !(rhs < lhs);
}

template <class value_type, class Container>
bool operator>=(const queue<value_type, Container>& lhs, const queue<value_type, Container>& rhs){
  return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class value_type, class Container>
void swap(queue<value_type, Container>& lhs, queue<value_type, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))){
  lhs.swap(rhs);
}

}// namespace GHYSTL
#endif