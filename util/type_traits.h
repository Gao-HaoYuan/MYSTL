/**
 * @file type_traits.h
 * @author ghy (ghy_mike@163.com)
 * @brief  萃取类型、迭代器， 以及一些用 struct 定义的小工具
 *         修改了 true_type 、 false_type的表示方式
 *         增加了 判断工具
 * @version 1.0  
 * @date 2022-08-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once
#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_

#include <cstddef>

/*
* 萃取传入的T类型的类型特性
* 
* 类模板专用化： https://docs.microsoft.com/en-us/previous-versions/c401y1kb(v=vs.140)?redirectedfrom=MSDN
* POD解释： https://blog.csdn.net/a627088424/article/details/48595525
*/

// --------------- 定义constructor、destructor函数的trivial类型 --------------

/**
 * 对于某数据的以下四种函数：
 * - 默认构造函数(default constructor)
 * - 拷贝构造函数(copy constructor)
 * - 赋值函数(copy assignment operator)
 * - 析构函数(destructor)
 *
 * 如果它们至少满足以下三个条件之一：
 * - 显式(explict)定义了这四种函数
 * - 类里有非静态非POD的数据成员
 * - 有基类
 *
 * 那么它们为non-trivial函数;
 * 如果以上四个函数都是trivial函数，那么这个对象属于POD类型。
 *
 * 由于c++中基本内置类型都是POD类型，因此我们一般讨论class、struct和union是否为POD类型
 *
 * 对于POD类型的数据，进行构造、析构、拷贝和赋值时，不需要调用它们的constructor、destructor函数，可以直接采用malloc()、memcpy()来提高效率。
 */

namespace GHYSTL{

    /*----------------------------------------------- 辅助类型的结构体 -----------------------------------------------*/

    // 标记迭代器的类型
    struct input_iterator_tag{};
    struct output_iterator_tag{};
    struct forward_iterator_tag : public input_iterator_tag{};
    struct bidirectional_iterator_tag : public forward_iterator_tag{};
    struct random_access_iterator_tag : public bidirectional_iterator_tag{};

    // 真假类型 基类
    template <typename val_type, val_type val>
    struct integral_constant
    {
        static constexpr val_type value = val;

        typedef val_type value_type;
        typedef integral_constant<value_type, val> type; 

        // 实现对象类型转化，将类对象隐式转换成 value_type 类型   
        // cout<< integral_constant <<endl; 就是调用这个函数
        constexpr operator value_type() const noexcept { return (value); }

        // 重载括号运算符
        constexpr value_type operator()() const noexcept { return (value); }
    };

    // 真假类型
    typedef integral_constant<bool, true>   true_type;
    typedef integral_constant<bool, false>  false_type;

    // 判断真假
    template <bool val>
    using bool_constant = integral_constant<bool, val>;

    template<bool val>
    struct bool_type : bool_constant<val>{};

    // 可变参数模板
    template<typename...>
    struct param_tester{
        typedef void type;
        typedef true_type is_typedef;
    };

    // 使用结构体传入可变参数，返回 结构体类型，与 void 做区别
    // 可以识别传进来的参数是否有效 （SFINAE），无效的话就忽略
    // 参考：https://cloud.tencent.com/developer/ask/sof/94673
    template<typename... types>
    using void_t = typename param_tester<types...>::type;
    
    // 使用结构体传入可变参数，并返回真值
    // 这个可能会用不上，因为和后面定义重复了
    template<typename... types>
    using typedef_t = typename param_tester<types...>::is_typedef;

    // =void，缺省参数，仅用于判断类型
    template<bool val, typename = void>
    struct enable_if{
    };

    template<typename type_>
    struct enable_if<true, type_>
    {
        typedef type_ type;
    };
    
    template<bool test, typename type = void>
    using enable_if_t  = typename enable_if<test, type>::type;
    

    /*-------------------------------------------- 类型的榨汁机 -----------------------------------------------------*/
    template<typename type>
    struct type_traits
    {
        typedef false_type has_trivial_default_constructor; // 是否有构造函数
        typedef false_type has_trivial_copy_constructor; // 是否有复制构造函数
        typedef false_type has_trivial_assignmen_operator; // 是否有复制函数
        typedef false_type has_trivial_destructor; // 是否有析构函数

        // 是否是POD类型，就是 C++ 内置类型 或者 C结构体类型
        // 有POD类型，必有trivial函数（上面的四个函数）
        typedef false_type is_POD_type; 
    };
    
    // 类模板专用化
    template <> struct  type_traits<char>
    {
        typedef true_type has_trivial_default_constructor; 
        typedef true_type has_trivial_copy_constructor; 
        typedef true_type has_trivial_assignmen_operator; 
        typedef true_type has_trivial_destructor; 
        typedef true_type is_POD_type; 
    };

    template <> struct  type_traits<signed char>
    {
        typedef true_type has_trivial_default_constructor; 
        typedef true_type has_trivial_copy_constructor; 
        typedef true_type has_trivial_assignmen_operator; 
        typedef true_type has_trivial_destructor; 
        typedef true_type is_POD_type; 
    };

    template <> struct  type_traits<unsigned char>
    {
        typedef true_type has_trivial_default_constructor; 
        typedef true_type has_trivial_copy_constructor; 
        typedef true_type has_trivial_assignmen_operator; 
        typedef true_type has_trivial_destructor; 
        typedef true_type is_POD_type; 
    };

    template <> struct  type_traits<wchar_t>
    {
        typedef true_type has_trivial_default_constructor; 
        typedef true_type has_trivial_copy_constructor; 
        typedef true_type has_trivial_assignmen_operator; 
        typedef true_type has_trivial_destructor; 
        typedef true_type is_POD_type; 
    };
    
    template <> struct  type_traits<short>
    {
        typedef true_type has_trivial_default_constructor; 
        typedef true_type has_trivial_copy_constructor; 
        typedef true_type has_trivial_assignmen_operator; 
        typedef true_type has_trivial_destructor; 
        typedef true_type is_POD_type; 
    };

    template <> struct  type_traits<unsigned short>
    {
        typedef true_type has_trivial_default_constructor; 
        typedef true_type has_trivial_copy_constructor; 
        typedef true_type has_trivial_assignmen_operator; 
        typedef true_type has_trivial_destructor; 
        typedef true_type is_POD_type; 
    };

    template <> struct  type_traits<int>
    {
        typedef true_type has_trivial_default_constructor; 
        typedef true_type has_trivial_copy_constructor; 
        typedef true_type has_trivial_assignmen_operator; 
        typedef true_type has_trivial_destructor; 
        typedef true_type is_POD_type; 
    };

    template <> struct  type_traits<unsigned int>
    {
        typedef true_type has_trivial_default_constructor; 
        typedef true_type has_trivial_copy_constructor; 
        typedef true_type has_trivial_assignmen_operator; 
        typedef true_type has_trivial_destructor; 
        typedef true_type is_POD_type; 
    };

    template <> struct type_traits<long>
	{
		typedef true_type    has_trivial_default_constructor;
		typedef true_type    has_trivial_copy_constructor;
		typedef true_type    has_trivial_assignment_operator;
		typedef true_type    has_trivial_destructor;
		typedef true_type    is_POD_type;
	};

	template <> struct type_traits<unsigned long>
	{
		typedef true_type    has_trivial_default_constructor;
		typedef true_type    has_trivial_copy_constructor;
		typedef true_type    has_trivial_assignment_operator;
		typedef true_type    has_trivial_destructor;
		typedef true_type    is_POD_type;
	};

    template <> struct type_traits<long long>
	{
		typedef true_type    has_trivial_default_constructor;
		typedef true_type    has_trivial_copy_constructor;
		typedef true_type    has_trivial_assignment_operator;
		typedef true_type    has_trivial_destructor;
		typedef true_type    is_POD_type;
	};

    template <> struct type_traits<unsigned long long>
	{
		typedef true_type    has_trivial_default_constructor;
		typedef true_type    has_trivial_copy_constructor;
		typedef true_type    has_trivial_assignment_operator;
		typedef true_type    has_trivial_destructor;
		typedef true_type    is_POD_type;
	};

	template <> struct type_traits<float>
	{
		typedef true_type    has_trivial_default_constructor;
		typedef true_type    has_trivial_copy_constructor;
		typedef true_type    has_trivial_assignment_operator;
		typedef true_type    has_trivial_destructor;
		typedef true_type    is_POD_type;
	};

	template <>struct type_traits<double>
	{
		typedef true_type    has_trivial_default_constructor;
		typedef true_type    has_trivial_copy_constructor;
		typedef true_type    has_trivial_assignment_operator;
		typedef true_type    has_trivial_destructor;
		typedef true_type    is_POD_type;
	};

	template <>struct type_traits<long double>
	{
		typedef true_type    has_trivial_default_constructor;
		typedef true_type    has_trivial_copy_constructor;
		typedef true_type    has_trivial_assignment_operator;
		typedef true_type    has_trivial_destructor;
		typedef true_type    is_POD_type;
	};


    // 指针类型 专用化
    template <typename type>
    struct type_traits<type*>
    {
        typedef true_type has_trivial_default_constructor; 
        typedef true_type has_trivial_copy_constructor; 
        typedef true_type has_trivial_assignmen_operator; 
        typedef true_type has_trivial_destructor; 
        typedef true_type is_POD_type; 
    };

    template <typename type>
    struct type_traits<const type*>
    {
        typedef true_type has_trivial_default_constructor; 
        typedef true_type has_trivial_copy_constructor; 
        typedef true_type has_trivial_assignmen_operator; 
        typedef true_type has_trivial_destructor; 
        typedef true_type is_POD_type; 
    };


    /*--------------------------------------------------- 迭代器萃取 ---------------------------------------------------*/
    template<typename, typename = void>
    struct iterator_traits_base
    {

    };

    template<typename Iterator>
    struct iterator_traits_base<Iterator, void_t<typename Iterator::iterator_category, typename Iterator::value_type,
                                                typename Iterator::difference_type, typename Iterator::pointer,
                                                typename Iterator::reference>>
    {
        typedef typename Iterator::iterator_category        iterator_category;
        typedef typename Iterator::value_type               value_type;
        typedef typename Iterator::difference_type          difference_type;
        typedef typename Iterator::pointer                  pointer;
        typedef typename Iterator::reference                reference;
        typedef false_type                                  memory_copy_tag;
    };

    template<typename Iterator>
    struct iterator_traits : iterator_traits_base<Iterator>
    {
        typedef false_type memory_copy_tag;
    };

    template<typename type>
    struct iterator_traits<type*>
    {
        typedef  GHYSTL::random_access_iterator_tag         iterator_category;
        typedef  type                                       value_type;
        typedef  ptrdiff_t                                  difference_type;
        typedef  type*                                      pointer;
        typedef  type&                                      reference;
        typedef  true_type                                  memory_copy_tag;
    };

    template<typename type>
    struct iterator_traits<const type*>
    {
        typedef  GHYSTL::random_access_iterator_tag         iterator_category;
        typedef  type                                       value_type;
        typedef  ptrdiff_t                                  difference_type;
        typedef  type*                                      pointer;
        typedef  type&                                      reference;
        typedef  true_type                                  memory_copy_tag;
    };
    
    // 迭代器类型 萃取
    template<typename Iterator>
    using iter_val_t = typename iterator_traits<Iterator>::value_type;

    template <typename Iter>
    using iter_dif_t = typename iterator_traits<Iter>::difference_type;

    template <typename Iter>
    using iter_cate_t = typename iterator_traits<Iter>::iterator_category;

    /*--------------------------------------------------- 工具 ---------------------------------------------------*/

    template<typename Iterator, typename = void>
    struct is_iterator : false_type
    {

    };

    template<typename Iterator>
    struct is_iterator<Iterator, 
                    void_t<typename iterator_traits<Iterator>::iterator_category>> : true_type
    {

    };

    template<typename type1, typename type2>
    struct is_same : false_type
    {

    };

    template<typename type>
    struct is_same<type, type> : true_type
    {

    };

    template<bool, typename type1, typename type2>
    struct If
    {
        typedef type2 type;
    };

     template<typename type1, typename type2>
    struct If<true, type1, type2>
    {
        typedef type1 type;
    };
    
    template<typename value_type, typename = void>
    struct is_type_pod : false_type
    {   
        
    };

    // value_type 是榨汁机 type_traits<value_type>
    template<typename value_type>
    struct is_type_pod <value_type, 
                        void_t<typename value_type::is_POD_tag>> : true_type
    {

    };
    
    template<typename value_type, typename = void>//is_POD_tag 是 true_type 类型， 该类型有一个 value 值和 type 类型
    struct  is_pod : bool_type<type_traits<value_type>::is_POD_tag::value 
                                        || If<is_type_pod<value_type>::value, 
                                                typename is_type_pod<value_type>::type,
                                                false_type>::type::value>
    {
        
    };
    
    template <typename Iterator, typename = void>
    struct is_typedef_mem_copy : false_type
    {
    };

    // Iterator 是榨汁机 iterator_traits<Iterator>
    template <typename Iterator>
    struct is_typedef_mem_copy<Iterator, 
                                void_t<typename Iterator::memory_copy_tag>> : true_type
    {
    };
    
    template<typename Iterator, typename = void>//is_POD_tag 是 true_type 类型， 该类型有一个 value 值和 type 类型
    struct  is_mem_copy : bool_type<iterator_traits<Iterator>::memory_copy_tag::value 
                                        || If<is_typedef_mem_copy<Iterator>::value, 
                                                typename is_typedef_mem_copy<Iterator>::type,
                                                false_type>::type::value>
    {
        
    };
}
#endif