/*
* 算术类（Arthmetic）仿函数的实现
* 这个头文件包含了 mystl 的函数对象与哈希函数
*/

#ifndef GHYSTL_FUNCTIONAL_H_
#define GHYSTL_FUNCTIONAL_H_

#include <cstddef>

namespace GHYSTL{

    // 定义一元函数的参数型别和返回值型别
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
        typedef Arg1     argumet_type;
        typedef Arg2     argumet_type;
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









}
#endif