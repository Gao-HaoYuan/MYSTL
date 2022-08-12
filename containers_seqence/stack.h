/**
 * @file stack.h
 * @author ghy (ghy_mike@163.com)
 * @brief stack的实现
 * 
 * 
 * @version 1.0
 * @date 2022-08-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _STACK_H_
#define _STACK_H_

#include "deque.h"

namespace GHYSTL{

template<typename value_type_, typename Container = GHYSTL::deque<value_type_>>
class stack{
public:
    typedef Container                                   Container_type;
    //底层容器的型别
    typedef typename Container::value_type              value_type;
    typedef typename Container::size_type               size_type;
    typedef typename Container::reference               reference;
    typedef typename Container::const_reference         const_reference;

    static_assert(GHYSTL::is_same<value_type_, value_type>::value,
                        "the value_type of Container should be same witeh T");

private:
    Container_type c_stack; //用底层容器实现 stack

public:
    /*----------------------------------------------- 构造、复制、移动函数 ----------------------------------------------------*/
    stack() = default;

    explicit stack(size_type n) : c_stack(n) {}

    stack(size_type n, const value_type& value) : c_stack(n, value) {}

    template<typename IIter>
    stack(IIter first, IIter last) : c_stack(first, last) {}

    stack(std::initializer_list<value_type> lst) : c_stack(lst.begin(), lst.end()) {}


    stack(const Container& c) : c_stack(c) {} 
    //  noexcept 默认是 noexcept(true) 不抛错
    //  is_nothrow_move_constructible 判断容器是否有构造函数
    stack(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value) 
        : c_stack(std::move(c)) {}


    stack(const stack& rhs) : c_stack(rhs.c_stack) {}
    stack(stack&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value)
        : c_stack(std::move(rhs.c_stack)) {}

    stack& operator=(const stack& rhs){
        c_stack = rhs.c_stack;
        return *this;
    }

    stack& operator=(stack&& rhs) noexcept(std::is_nothrow_move_assignable<Container>::value){
        c_stack = std::move(rhs.c_stack);
        return *this;
    }

    stack& operator=(std::initializer_list<value_type> lst){
        c_stack = lst;
        return *this;
    }

    ~stack() = default;

    /*------------------------------------------------ 元素访问 --------------------------------------------------------*/

    reference       top() { return c_stack.back(); }
    const_reference top() const { return c_stack.back(); }

    bool empty() const noexcept { return c_stack.empty(); }
    size_type size() const noexcept { return c_stack.size(); }

    /*------------------------------------------------ 插入删除元素 --------------------------------------------------------*/
    template<typename ... types>
    void emplace(types && ... args){
        c_stack.emplace_back(std::forward<types>(args)...);
    }

    void push(const value_type& value){
        c_stack.push_back(value);
    }   

    void push(value_type&& value){
        c_stack.push_back(std::move(value));
    }

    void pop(){
        c_stack.pop_back();
    }

    void clear(){
        while (!empty())
            pop();
        
    }

public:
    // 下面的比较运算会调用 这里的操作
    friend bool operator==(const stack& lhs, const stack& rhs){
        return lhs.c_stack == rhs.c_stack;
    }

    friend bool operator<(const stack& lhs, const stack& rhs){
        return lhs.c_stack < rhs.c_stack;
    }

    void swap(stack & rhs) noexcept(noexcept(GHYSTL::swap(c_stack, rhs.c_stack))){
        GHYSTL::swap(c_stack, rhs.c_stack);
    }
};

/*--------------------------------------------- 重载比较操作符 ------------------------------------------------------*/
template <class value_type, class Container>
bool operator==(const stack<value_type, Container>& lhs, const stack<value_type, Container>& rhs){
  return lhs == rhs;
}

template <class value_type, class Container>
bool operator!=(const stack<value_type, Container>& lhs, const stack<value_type, Container>& rhs){
  return !(lhs == rhs);
}

template <class value_type, class Container>
bool operator<(const stack<value_type, Container>& lhs, const stack<value_type, Container>& rhs){
  return lhs < rhs;
}

template <class value_type, class Container>
bool operator>(const stack<value_type, Container>& lhs, const stack<value_type, Container>& rhs){
  return rhs < lhs;
}

template <class value_type, class Container>
bool operator<=(const stack<value_type, Container>& lhs, const stack<value_type, Container>& rhs)
{
  return !(rhs < lhs);
}

template <class value_type, class Container>
bool operator>=(const stack<value_type, Container>& lhs, const stack<value_type, Container>& rhs){
  return !(lhs < rhs);
}

// 重载 swap
template <class value_type, class Container>
void swap(stack<value_type, Container>& lhs, stack<value_type, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))){
  lhs.swap(rhs);
}

} // namespace GHYSTL
#endif