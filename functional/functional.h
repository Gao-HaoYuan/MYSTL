/**
 * @file functional.h
 * @author ghy (ghy_mike@163.com)
 * @brief 算术类（Arthmetic）仿函数的实现
 *        包含函数对象与哈希函数
 * 
 *         增加哈希仿函数
 * 
 * @version 1.0
 * @date 2022-08-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once
#ifndef GHYSTL_FUNCTIONAL_H_
#define GHYSTL_FUNCTIONAL_H_

#include <cstddef>

namespace GHYSTL{

    /*----------------------------------------------  定义一元函数的参数型别和常规运算 ---------------------------------------------------*/ 
    template<class Arg, class Result>
    struct unary_function
    {
        typedef Arg     argumet_type;
        typedef Result  result_type;
    };


    // 定义二元函数的参数型别和返回值型别
    template<class Arg1, class Arg2, class Result>
    struct binary_function
    {
        typedef Arg1     first_argumet_type;
        typedef Arg2     second_argumet_type;
        typedef Result   result_type;
    };

    // 函数对象：加法
    template<class T>
    struct plus : public binary_function<T,T,T>
    {
        T operator()(const T& x, const T& y) const { return x + y;}
    };
    
    // 函数对象：减法
    template<class T>
    struct minus : public binary_function<T,T,T>
    {
        T operator()(const T& x, const T& y) const { return x - y;}
    };

    // 函数对象：乘法
    template<class T>
    struct multiplies : public binary_function<T,T,T>
    {
        T operator()(const T& x, const T& y) const { return x * y;}
    };

    // 函数对象：除法
    template<class T>
    struct divide : public binary_function<T,T,T>
    {
        T operator()(const T& x, const T& y) const { return x / y;}
    };

    // 函数对象：取模
    template<class T>
    struct modulus : public binary_function<T,T,T>
    {
        T operator()(const T& x, const T& y) const { return x % y;}
    };

    // 函数对象：取反
    template<class T>
    struct negate : public unary_function<T,T>
    {
        T operator()(const T& x) const { return -x;}
    };
    
    // 加法验证同元素
    template<class T>
    T identity_element(plus<T>){ return T(0);}

    // 乘法验证同元素
    template<class T>
    T identity_element(multiplies<T>){ return T(1);}

    // 函数对象：等于
    template<class T>
    struct equal_to : public binary_function<T,T,bool>
    {
        bool operator()(const T& x, const T& y) const { return x == y;}
    };

    // 函数对象：不等于
    template<class T>
    struct not_equal_to : public binary_function<T,T,bool>
    {
        bool operator()(const T& x, const T& y) const { return x != y;}
    };

    // 函数对象：大于
    template<class T>
    struct greater : public binary_function<T,T,bool>
    {
        bool operator()(const T& x, const T& y) const { return x > y;}
    };

    // 函数对象：小于
    template<class T>
    struct less : public binary_function<T,T,bool>
    {
        bool operator()(const T& x, const T& y) const { return x < y;}
    };

    // std 标准库的源码，以后再改进把，这次就先这样了
    // 注意 -> decltype(static_cast<_Ty1&&>(_Left)  意思就是返回值是这个类型 decltype 可以获取表达式类型

    // template<>
	// struct less<void>
	// {	// transparent functor for operator<
	// typedef int is_transparent;

	// template<class _Ty1,
	// 	class _Ty2>
	// 	constexpr auto operator()(_Ty1&& _Left, _Ty2&& _Right) const
	// 	-> decltype(static_cast<_Ty1&&>(_Left)
	// 		< static_cast<_Ty2&&>(_Right))
	// 	{	// transparently apply operator< to operands
	// 	return (static_cast<_Ty1&&>(_Left)
	// 		< static_cast<_Ty2&&>(_Right));
	// 	}
	// };

    // 函数对象：大于等于
    template<class T>
    struct greater_equal : public binary_function<T,T,bool>
    {
        bool operator()(const T& x, const T& y) const { return x >= y;}
    };

    // 函数对象：小于等于
    template<class T>
    struct less_equal : public binary_function<T,T,bool>
    {
        bool operator()(const T& x, const T& y) const { return x <= y;}
    };
     
    // 函数对象：逻辑与
    template<class T>
    struct logical_and : public binary_function<T,T,bool>
    {
        bool operator()(const T& x, const T& y) const { return x && y;}
    };

    // 函数对象：逻辑或
    template<class T>
    struct logical_or : public binary_function<T,T,bool>
    {
        bool operator()(const T& x, const T& y) const { return x || y;}
    };

    // 函数对象：逻辑非
    template<class T>
    struct logical_not : public unary_function<T,bool>
    {
        bool operator()(const T& x) const { return !x;}
    };

    // 证同函数: 不改变元素，返回自身
    template<class T>
    struct identity : public unary_function<T, const T&>
    {
        const T& operator()(const T& x) const { return x;}
    };

    // 选择函数：接受一个pair，返回第一个元素
    template<class Pair>
    struct selectfirst : public unary_function<Pair, typename Pair::first_type>
    {
        const typename Pair::first_type& operator()(const Pair& x) const{
            return x.first;
        }
    };

    // 选择函数：接受一个pair，返回第二个元素
    template<class Pair>
    struct selectsecond : public unary_function<Pair, typename Pair::second_type>
    {
        const typename Pair::second_type& operator()(const Pair& x) const{
            return x.second;
        }
    };

    //投射函数：返回第一个参数
    template<class Arg1, class Arg2>
    struct  projectfirst : public binary_function<Arg1, Arg2, Arg1>
    {
        Arg1 operator()(const Arg1& x, const Arg2&) const {return x;}
    };

    //投射函数：返回第二个参数
    template<class Arg1, class Arg2>
    struct  projectsecond : public binary_function<Arg1, Arg2, Arg2>
    {
        Arg2 operator()(const Arg1&, const Arg2& y) const {return y;}
    };
    
/******************************************************************************************/
// 哈希函数对象
// 留着写
template<typename key>
struct hash { };

// hash 函数
template<>
struct hash<std::string>
{
    size_t operator()(const std::string& str) const {
        size_t h = 0;
        for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
            h = (h << 2) + *iter; //逐位hash
        return h;
    }
};

inline size_t hash_string(const char* s){
    size_t h = 0;
    for(; *s; ++s)
        h = (h << 2) + *s;
    return h;
}

// 特化版本 字符串 hash
template <>
struct hash<char *>
{
  size_t operator()(char *s) const { return hash_string(s); }
};

template <>
struct hash<const char *>
{
  size_t operator()(const char *s) const { return hash_string(s); }
};


// 特化版本 普通 hash 
template <>
struct hash<char>
{
  size_t operator()(char x) const { return x; }
};

template <>
struct hash<signed char>
{
  size_t operator()(signed char x) const { return x; }
};

template <>
struct hash<unsigned char>
{
  size_t operator()(unsigned char x) const { return x; }
};

template <>
struct hash<short>
{
  size_t operator()(short x) const { return x; }
};

template <>
struct hash<unsigned short>
{
  size_t operator()(unsigned short x) const { return x; }
};

template <>
struct hash<int>
{
  size_t operator()(int x) const { return x; }
};

template <>
struct hash<unsigned int>
{
  size_t operator()(unsigned int x) const { return x; }
};

template <>
struct hash<long>
{
  size_t operator()(long x) const { return x; }
};

template <>
struct hash<unsigned long>
{
  size_t operator()(unsigned long x) const { return x; }
};


}
#endif