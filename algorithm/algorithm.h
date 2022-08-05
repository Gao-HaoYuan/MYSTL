/*
*   算法的接口文件，包含base中的算法，提供新的特性
*    
*   base 文件的算法：equal、fill、fill_n、iter_sawp、max、min、
*   lexicographical_compare、mismatch、copy 、copy_backward 
*
*   b本文件的算法：adjacent_find、count、find、for_each、generate、transform
* 
*/

#ifndef GHYSTL_ALGORITHM_H_
#define GHYSTL_ALGORITHM_H_

#include "iterator.h"
#include "algo_base.h"
#include "algo_numeric.h"

namespace GHYSTL{

    /*****************************************************************************************/
    // adjacent_find
    // 找出第一组满足条件的相邻元素。这里所谓的条件，在版本一种指的是“两元素相等”
	// 在版本二中，允许用户指定一个二元运算，二元运算的操作数分别是相邻的第一个元素和第二个元素
    /*****************************************************************************************/
    template<typename ForwardIterator>
    inline ForwardIterator
    adjacent_find(ForwardIterator first, ForwardIterator last){
        if(first != last){
            for(ForwardIterator next = first; ++next != last; first = next){
                if(*first == *next){
                    return first;
                }
            }
        }
        return last;
    }
    
    // compare: 用户自定义的操作
    template<typename ForwardIterator, typename compare>
    inline ForwardIterator
    adjacent_find(ForwardIterator first, ForwardIterator last, const compare& comp){
        if(first != last){
            for(ForwardIterator next = first; ++next != last; first = next){
                if(comp(*first, *next)){
                    return first;
                }
            }
        }
        return last;
    }
    
    
    /*****************************************************************************************/
    // count
    // 运用equality操作符，将 [ first, last ) 区间内的每一个元素拿来和指定值 value 做比较
    // 返回与 value 相等的元素个数
    /*****************************************************************************************/
    template<typename InputIterator, typename T>
    inline GHYSTL::iter_dif_t<InputIterator> 
    count(InputIterator first, InputIterator last, const T& value){
        GHYSTL::iter_dif_t<InputIterator> counter = 0;
        for(; first != last; ++first){
            if(*first == value) ++counter;
        }
        return (counter);
    }

    // compare: 用户自定义的操作
    template<typename InputIterator, typename compare>
    inline GHYSTL::iter_dif_t<InputIterator> 
    count_if(InputIterator first, InputIterator last, const compare& comp){
        GHYSTL::iter_dif_t<InputIterator> counter = 0;
        for(; first != last; ++first){
            if(comp(first)) ++counter;
        }
        return (counter);
    }
    
    /*****************************************************************************************/
    // find
    // 根据equality操作符，循序查找 [ first, last ) 内所有元素，找出第一个匹配“等同条件”者，如果找到，
	// 返回 InputIterator 指向该元素，反则返回迭代器的last
    /*****************************************************************************************/
    template<typename InputIterator, typename T>
    inline InputIterator 
    find(InputIterator first, InputIterator last, const T& value){
        for(; first != last && *first != value;){
            ++first
        }
        return (first);
    }

    // Pred: 用户自定义的操作
    template<typename InputIterator, typename Pred>
    inline InputIterator 
    find_if(InputIterator first, InputIterator last, const Pred& pred){
        for(; first != last && (!pred(*first));){
            ++first
        }
        return (first);
    }
    
    
    /*****************************************************************************************/
    // for_each
    // 将仿函数 f 实施于 [ first, last ) 区间内的每一个元素上。
	// f 不可以改变元素内容，因为 first，last 都是 InputIterator ，不保证接受赋值行为
    /*****************************************************************************************/
    template<typename InputIterator, typename Function>
    inline Function
    for_each(InputIterator first, InputIterator last, const Function& fun){
        for(; first != last; ++first) fun(*first);
        return (fun);
    }
    
    /*****************************************************************************************/
    // generate
    // 将仿函数 f 实施于 [ first, last ) 区间内的每一个元素上。
	// 将仿函数 gen 的元素结果填写在 [ first, last ) 区间内的所有元素上，填写使用了迭代器的 assignment 操作符
    /*****************************************************************************************/
    template <typename ForwardIterator, typename Function>
    inline void
    generate(ForwardIterator first, ForwardIterator last, const Function& fun){
        for (; first != last; ++first) *first = fun();
    }
    
    template<typename OutputIterator, typename Dif, typename Function>
    inline OutputIterator
    generate_n(OutputIterator first, Dif n, const Function& fun){
        for(; n > 0; --n) *first++ = fun();
        return (first);
    }

    /*****************************************************************************************/
    // transform
    // 将函数对象 op 作用于 [ first, last ) 中的每一个元素，并以其结果产生出一个新序列
    /*****************************************************************************************/
    template<typename InputIterator, typename OutputIterator, typename Function>
    inline OutputIterator
    transform(InputIterator first, InputIterator last, OutputIterator dest, const Function &fun){
        for(; first != last; ++first, ++dest) *dest = fun(*first);
        return (dest);
    }

    /* 
	* 将函数对象 binary_op 作用于一对元素上
	* 其中一个元素来源于 [ first1, last1 )，另一个元素来源于 [ first2, last2)
	* 在 result 开始的位置依次存放结果
	* 如果第二个序列的长度小于第一个序列，结果未知
	*/
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Function>
    inline OutputIterator
    transform(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, 
                OutputIterator dest, const Function &fun){
        for(; first1 != last1; ++first1, ++first2, ++dest) 
            *dest = fun(*first1, *first2);
        return (dest);
    }
}
#endif
