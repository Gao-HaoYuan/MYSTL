/**
 * @file iterator.h
 * @author ghy (ghy_mike@163.com)
 * @brief  定义了部分迭代器和相关的函数
 *         添加了基类迭代器
 * 
 * @version 1.0
 * @date 2022-08-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once
#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include "../util/type_traits.h"

/*
*这个文件用于迭代器的设计
*包含一些全局函数、模板、结构体
*/

namespace GHYSTL{
    //////////////////////////////////////////////////////////////////////////////////////
    // 迭代器一共包括五种
    //////////////////////////////////////////////////////////////////////////////////////
    // Input Iterator               只读
    // Output Iterator              只写
    // Forward Iterator             读写，但只能单向移动，每次只能移动一步
    // Bidirectional Iterator       读写，双向移动，每次只能移动一步
    // Random Access Iterator       随机访问，随意移动指定步数，支持原生指针的全部能力
    //////////////////////////////////////////////////////////////////////////////////////
    // 迭代器的从属关系，子类可以接收父类类型的算法，但是效率可能不好
    //              Input Iterator
    //                    ↑ 
    //             Forward Iterator
    //                    ↑
    //          Bidirectional Iterator
    //                    ↑
    //          Random Access Iterator
    /////////////////////////////////////////////////////////////////////////////////////

    // 可能没用了
    template <class T, class Distance> struct input_iterator
    {
        typedef input_iterator_tag  iterator_category;
        typedef T                   value_type;
        typedef Distance            difference_type;
        typedef T*                  pointer;
        typedef T&                  reference;
    };

    struct output_iterator
    {
        typedef output_iterator_tag iterator_category;
        typedef void                value_type;
        typedef void                difference_type;
        typedef void                pointer;
        typedef void                reference;
    };

    template <class T, class Distance> struct forward_iterator
    {
        typedef forward_iterator_tag    iterator_category;
        typedef T                       value_type;
        typedef Distance                difference_type;
        typedef T*                      pointer;
        typedef T&                      reference;
    };
    
    template <class T, class Distance> struct bidirectional_iterator
    {
        typedef bidirectional_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef Distance                    difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    template <class T, class Distance> struct random_access_iterator
    {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef Distance                    difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };




    // --------------- 定义迭代器原型 ---------------
    //
    // using 和 typedef 等价，后期改用 using
    // iterator 原型
    template<typename Category,
            typename T,
            typename Distance = ptrdiff_t,
            typename Pointer = T*,
            typename Reference = T&>
    struct iterator_base
    {
        /**
         * 迭代器类型，分为五类：
         * - input _iterator：只读
         * - output _iterator：只写
         * - forward _iterator：前向访问，支持读写
         * - bidirectional _iterator：双向访问，支持读写
         * - random access _iterator：随机访问，支持读写
         */
        using iterator_category = Category;

        /**
         * 迭代器所指对象类型；
         */
        using value_type = T;

        /**
         * 两个迭代器之间的距离；
         * 可以用来表示容器的最大容量；
         */
        using difference_type = Distance;

        /**
         * 指向迭代器所指之物的指针；
         */
        using pointer = Pointer;

        /**
         * 迭代器所指之物是否允许改变；
         * 对迭代器解引用的时候，返回的是左值还是右值；
         */
        using reference = Reference;
    };

    /**
     * 萃取迭代器的 iterator_category
     */
    template<typename Iterator>
    inline typename GHYSTL::iterator_traits<Iterator>::iterator_category
        iterator_category(const Iterator&){
            using category = typename iterator_traits<Iterator>::iterator_category;
            return category();
        }

    /**
     * 萃取迭代器的 value_type
     */
    // template<typename Iterator>
    // inline typename GHYSTL::iterator_traits<Iterator>::value_type
    //     value_type(const Iterator& iter){
    //         return GHYSTL::iterator_traits<Iterator>::value_type();
    //     }

    template<typename Iterator>
    inline typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator &) {
        return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
    }


    /**
     * 萃取迭代器的 difference_type
     */
    // template<typename Iterator>
    // inline typename GHYSTL::iterator_traits<Iterator>::difference_type
    //     difference_type(const Iterator& iter){
    //         return GHYSTL::iterator_traits<Iterator>::difference_type();
    //     }

    template<typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator &) {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    /**
     * 萃取迭代器的 pointer
     */
    template<typename Iterator>
    inline typename GHYSTL::iterator_traits<Iterator>::pointer
        pointer(const Iterator&){
            return GHYSTL::iterator_traits<Iterator>::pointer();
        }

    /**
     * 萃取迭代器的 reference
     */
    template<typename Iterator>
    inline typename GHYSTL::iterator_traits<Iterator>::reference
        reference(const Iterator&){
            return GHYSTL::iterator_traits<Iterator>::reference();
        }



    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 类模板：迭代器距离的计算
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // template <class T(迭代器类型)， class Distance>
    // inline void _distance(T first, T last, Distance& n)
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //                          distance
    //                              |
    //                              |-----------------------判断迭代器类型
    //           Input_Iterator     ⬇   Random_Access_Iterator
    //          -------------------------------------------------
    //          |                                               |
    //          |                                               |
    //          ⬇                                               |
    //_distance(..., input_iterator_tag)                        |    
    //  while(first!=last){++first;++n;}              _distance(..., random_access_iterator_tag)
    //                                                       n+=last-first;
    ///////////////////////////////////////////////////////////////////////////////////////////////

    // random_access_iterator只用于连续内存，所有可以直接用 last-first
    // 其他的可以用于不连续的内存，所以只能一步一步的循环
    // n 为传出参数
    template <typename InputIterator>
    inline typename GHYSTL::iterator_traits<InputIterator>::difference_type
    _distance(InputIterator first, InputIterator last, GHYSTL::input_iterator_tag){
        typename GHYSTL::iterator_traits<InputIterator>::difference_type n = 0;
        while(first != last){++first; ++n;}
        return (n);
    }

    // n 为传出参数
    template <typename InputIterator>
    inline typename GHYSTL::iterator_traits<InputIterator>::difference_type
    _distance(InputIterator first, InputIterator last, GHYSTL::random_access_iterator_tag){
        return (last - first);
    }

    template <typename InputIterator>
    inline typename GHYSTL::iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last){
        // 通过参数传递自动推导迭代器类型
        return _distance(first, last, GHYSTL::iterator_category(first));
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // 类模板：迭代器向前或者向后移动 n 个元素
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // template <class T(迭代器类型)， class Distance>
    // inline void advance(T& i , T last, Distance n)
    ///////////////////////////////////////////////////////////////////////////////////////////////
    //                           advance
    //                              |
    //                              |-----------------------判断迭代器类型
    //           Input_Iterator     ⬇   Random_Access_Iterator
    //          --------------------------------------------------------------
    //          |                              |                              |
    //          |       Bidirectional_Iterator |                              |
    //          ⬇                              |                              |
    //_advance(..., input_iterator_tag)        |                              |    
    //  while(n--)++i;                         |            _advance(..., random_access_iterator_tag)
    //                                         |                            i += n;
    //                                         |
    //                        _advance(..., bidirectional_iterator_tag) 
    //                                    if(n>=0) while (n--)  ++i;
    //                                    else while (n++)  --i;
    ///////////////////////////////////////////////////////////////////////////////////////////////                             

    template <typename InputIterator, typename Distance>
    inline void _advance(InputIterator& i, Distance n, GHYSTL::input_iterator_tag){
        while(n--) ++i;
    }

    template <typename BidirectionalIterator, typename Distance>
    inline void _advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag){
        if(n>=0) 
            while (n--)  ++i;
        else 
            while (n++)  --i;
    }
    
    template <typename RandomAccessIterator, typename Distance>
    inline void _advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag){
        i += n;
    }
    
    template <typename InputIterator, typename Distance>
    inline void advance(InputIterator& i, Distance n){
        _advance(i, n, GHYSTL::iterator_category(i));
    }         
}
#endif