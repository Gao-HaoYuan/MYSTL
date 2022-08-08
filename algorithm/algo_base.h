/**
 * @file algo_base.h
 * @author ghy (ghy_mike@163.com)
 * @brief 算法的基类，包含大部分的常规算法
 *         equal、fill、fill_n、iter_sawp、max、min、
 *          lexicographical_compare、mismatch、copy 、copy_backward 算法的实现
 * 
 *          修改copy算法，增加容错
 * 
 * @version 1.0
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once
#ifndef _ALGO_BASE_H_
#define _ALGO_BASE_H_

#include "../allocator/allocator.h"
#include "../adapter/function_adapters.h"

namespace GHYSTL{
    constexpr size_t sort_threshold = 32;

    /*------------------------------------------ 8月5号新增的算法 ----------------------------------------------------*/
    
    template<typename Iter>
    inline Iter next(Iter cur, GHYSTL::iter_dif_t<Iter> dif){
        GHYSTL::advance(cur, dif);
        return cur;
    }

    template<typename Iter>
    inline Iter next(Iter cur){
        return (++cur); //重写的前缀 ++ 运算符
    }

    template<typename Iter>
    inline Iter prev(Iter cur, GHYSTL::iter_dif_t<Iter> dif){
        GHYSTL::advance(cur, dif); // 向前移动的迭代器只能是 bidirectional_iterator
        return cur;
    }

    template<typename Iter>
    inline Iter prev(Iter cur){
        return (--cur); //重写的前缀 ++ 运算符
    }

    /*****************************************************************************************/
    // iter_swap
    // 将两个迭代器所指对象对调
    /*****************************************************************************************/

    template<typename FIter, typename = void>
    struct has_mem_iter_swap : GHYSTL::false_type{};

    template<typename FIter>
    struct has_mem_iter_swap<FIter, // decltype获取表达式的类型，declval根据类型 生成右值引用类型
                            GHYSTL::void_t<decltype(std::declval<FIter>().iter_swap(FIter()))>> : GHYSTL::true_type{};
    
    template<class ForwardIterator1 ,class ForwardIterator2>
    inline void _iter_swap(ForwardIterator1 first, ForwardIterator2 last){
        GHYSTL::swap(*first, *last);
    }
    
    template<typename FIter>
    inline void _iter_swap_imple(FIter left, FIter right, GHYSTL::false_type){
        GHYSTL::_iter_swap(left, right);
    }

    template<typename FIter>
    inline void _iter_swap_imple(FIter left, FIter right, GHYSTL::true_type){
        left.iter_swap(right);
    }

    template<typename FIter1, typename FIter2>// 同类型迭代器，包含 iter_swap 函数
    inline void _iter_swap_same(FIter1 left, FIter2 right, GHYSTL::true_type){
        GHYSTL::_iter_swap_imple(left, right, has_mem_iter_swap<FIter1>());
    }

    template<typename FIter1, typename FIter2>
    inline void _iter_swap_same(FIter1 left, FIter2 right, GHYSTL::false_type){
        GHYSTL::_iter_swap(left, right);
    }

    template<typename FIter1, typename FIter2>
    inline void iter_swap(FIter1 left, FIter2 right){
        GHYSTL::_iter_swap_same(left, right, GHYSTL::is_same<FIter1, FIter2>());
    }

    /*****************************************************************************************/
    // max
    // 取二者中的较大值，语义相等时保证返回第一个参数
    /*****************************************************************************************/

    //compare: 用户自定义的比较函数
    //compare值为真，返回第一个参数，假返回第二个参数
    template<class T, class compare>
    inline const T& max(const T& a, const T& b, compare comp){
        return comp(b,a) ? a:b;
    }

    template<class T>
    inline const T& max(const T& a, const T& b){
        return max(a, b, less<T>());
    }

    template<typename Iter, typename Comp>
    inline GHYSTL::iter_val_t<Iter> max(Iter first, Iter last, const Comp& cmp){
        iter_val_t<Iter> val = *first;
        for(; ++first != last;){
            if(cmp(val, *first)) val = *first;
        }
        return val;
    }
    
    template<typename Iter>
    inline GHYSTL::iter_val_t<Iter> max(Iter first, Iter last){
        return (max(first, last, less<Iter>()));
    }

    template<typename FIter, typename Comp>
    inline FIter max_element(FIter first, FIter last, const Comp& cmp){
        FIter result = first;
        if(first != last){
            for(GHYSTL::iter_val_t<FIter> cur = *first; ++first != last;){
                if(cmp(cur, *first)){
                    cur = *first;
                    result = first;
                }
            }   
        }
        return result;
    }

    template<typename FIter>
    inline FIter max_element(FIter first, FIter last){
        return GHYSTL::max_element(first, last, less<FIter>());
    }

    /*****************************************************************************************/
    // min 
    // 取二者中的较小值，语义相等时保证返回第一个参数
    /*****************************************************************************************/

    //compare: 用户自定义的比较函数
    //compare值为真，返回第二个参数，假返回第一个参数
    template<class T, class compare>
    inline const T& min(const T& a, const T& b, compare comp){
        return comp(a,b) ? b:a;
    }

    template<class T>
    inline const T& min(const T& a, const T& b){
        return min(a,b,less<T>());
    }

    template<typename Iter, typename Comp>
    inline GHYSTL::iter_val_t<Iter> min(Iter first, Iter last, const Comp& cmp){
        iter_val_t<Iter> val = *first;
        for(; ++first != last;){
            if(cmp(*first, val)) val = *first;
        }
        return val;
    }

    template<typename Iter>
    inline GHYSTL::iter_val_t<Iter> min(Iter first, Iter last){
        return (min(first, last, less<Iter>()));
    }

    template<typename FIter, typename Comp>
    inline FIter min_element(FIter first, FIter last, const Comp& comp){
        FIter result = first;
        if(first != last){
            for(GHYSTL::iter_val_t<FIter> cur = *first; ++first != last;){
                if(cmp(*first, cur)){
                    cur = *first;
                    result = first;
                }
            }
        }
        return result;
    }

    template<typename FIter>
    inline FIter min_element(FIter first, FIter last){
        return GHYSTL::min_element(first, last, less<FIter>());
    }

    /*****************************************************************************************/
    // mismatch
    // 平行比较两个序列，找到第一处失配的元素，返回一对迭代器，分别指向两个序列中失配的元素
    /*****************************************************************************************/
    //compare: 用户自定义比较操作
    template<class InputIterator1, class InputIterator2, class compare>
    inline GHYSTL::pair<InputIterator1, InputIterator2> 
    mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, const compare& comp){
        while (first1 != last1 && comp(*first1, *first2))
        {
            ++first1;
            ++first2;
        }

        return pair<InputIterator1, InputIterator2>(first1,first2);
    }

    template<class InputIterator1, class InputIterator2>
    inline GHYSTL::pair<InputIterator1, InputIterator2> 
    mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2){
        // while (first1 != last1  && *first1 == *first2)
        // {
        //     ++first1;
        //     ++first2;
        // }

        // return pair<Iter1, Iter2>(first1,first2);
        return (mismatch(first1, last1, 
                    static_cast<InputIterator1>(first2), equal_to<InputIterator1>()));
    }

    template<class Iter1, class Iter2, class compare>
    inline GHYSTL::pair<Iter1, Iter2> 
    mismatch(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2, const compare& comp){
        while (first1 != last1 && first2 != last2 && comp(*first1, *first2))
        {
            ++first1;
            ++first2;
        }

        return pair<Iter1, Iter2>(first1,first2);
    }

    template<class Iter1, class Iter2>
    inline GHYSTL::pair<Iter1, Iter2> 
    mismatch(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2){
        // while (first1 != last1 && first2 != last2 && *first1 == *first2)
        // {
        //     ++first1;
        //     ++first2;
        // }

        // return pair<Iter1, Iter2>(first1,first2);

        return (mismatch(first1, last1, 
                            static_cast<Iter1>(first2),static_cast<Iter1>(last2), equal_to<Iter1>()));
    }

    /*****************************************************************************************/
    // copy
    // 把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
    // copy()方法会根据迭代器类型采取不同的策略：
    //  * - char* wchar_t*：采用memmove
    //  * - InputIterator：
    //  *      - 如果是（可强化为）RandomAccessIterator：采取distance > 0来判断是否结束
    //  *      - 如果不能，则采取 first != last 的方式判断是否结束
    //  * - T*：
    //  *      - 如果 has_trivial_assignment_operator 为 false type，则使用 RandomAccessIterator 的方式
    //  *      - 如果 has_trivial_assignment_operator 为 true type，则直接使用 memmove
    //  * - (const T*, T*) 同 T*
    //  *
    //  * 综上，copy尽可能的采用 memmove的方式进行复制，
    //  * 如果不能的话，判断迭代器是否为RandomAccessIterator，是的话则采用 (distance = last - first; distance > 0; distance--) 的方式进行迭代；
    //  * 实在不行，则使用 (;first != last; ++first)的方式进行比较
    /*****************************************************************************************/
    template<typename RandomAccessIterator, typename OutputIterator, typename Distance>
    inline OutputIterator
    _copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance *) {
        for (Distance distance = last - first; distance > 0; --distance, ++result, ++first) {
            *result = *first;
        }
        return result;
    }

    template<typename T>
    inline T *
    _copy_t(const T *first, const T *last, T *result, true_type) {
        memmove(result, first, sizeof(T) * (last - first));
        return result + (last - first);
    }

    template<typename T>
    inline T *
    _copy_t(const T *first, const T *last, T *result, false_type) {
        return _copy_d(first, last, result, static_cast<ptrdiff_t *>(nullptr));
    }

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator
    _copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
        for (; first != last; ++result, ++first) {
            *result = *first;
        }
        return result;
    }

    template<typename RandomAccessIterator, typename OutputIterator>
    inline OutputIterator
    _copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag) {
        return _copy_d(first, last, result, distance_type(first));
    }

    template<typename InputIterator, typename OutputIterator>
    struct copy_dispatch {
        OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
            return _copy(first, last, result, iterator_category(first));
        }
    };

    template<typename T>
    struct copy_dispatch<T *, T *> {
        T *operator()(T *first, T *last, T *result) {
            using operator_type = typename type_traits<T>::has_trivial_assignment_operator;
            return _copy_t(first, last, result, operator_type());
        }
    };

    template<typename T>
    struct copy_dispatch<const T *, T *> {
        T *operator()(T *first, T *last, T *result) {
            using operator_type = typename type_traits<T>::has_trivial_assignment_operator;
            return _copy_t(first, last, result, operator_type());
        }
    };

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator
    copy(InputIterator first, InputIterator last, OutputIterator result) {
        return copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
    }

    inline char *
    copy(char *first, char *last, char *result) {
        memmove(result, first, last - first);
        return result + (last - first);
    }

    inline wchar_t *
    copy(wchar_t *first, wchar_t *last, wchar_t *result) {
        memmove(result, first, sizeof(wchar_t) * (last - first));
        return result + (last - first);
    }


    /*****************************************************************************************/
    // set_union
    // 合并两个集合（有序）
    /*****************************************************************************************/

    template<typename IIter1, typename IIter2, typename OIter, typename Comp>
    inline OIter set_union(IIter1 first1, IIter1 last1, IIter2 first2, IIter2 last2, OIter dest, const Comp& cmp){
        if(first1 != last1 && first2 != last2){
            for(;;){
                if(cmp(*first1, *first2)){
                    *dest++ = *first1;
                    if(++first1 == last1) break;
                }else if(cmp(*first2, *first1)){
                    *dest++ = *first2;
                    if(++first2 == last2) break;
                }else{
                    *dest++ = *first1;
                    ++first1;
                    ++first2;
                    if( first1 == last1 || first2 == last2) break;
                }
            }
        }

        dest = GHYSTL::copy(first1, last1, dest);
        return (GHYSTL::copy(first2, last2, dest));
    }

    template<typename IIter1, typename IIter2, typename OIter>
    inline OIter set_union(IIter1 first1, IIter1 last1, IIter2 first2, IIter2 last2, OIter dest){
        return (GHYSTL::set_union(first1, last1, 
                static_cast<IIter1>(first2), static_cast<IIter1>(last2), dest, less<IIter1>()));
    }


    /*****************************************************************************************/
    // set_intersection
    // 交集（有序）
    /*****************************************************************************************/
    template<typename IIter1, typename IIter2, typename OIter, typename Comp>
    inline OIter set_intersection(IIter1 first1, IIter1 last1, IIter2 first2, IIter2 last2, OIter dest, const Comp& cmp){
        if(first1 != last1 && first2 != last2){
            for(;;){
                if(cmp(*first1, *first2)){
                    if(++first1 == last1) break;
                }else if(cmp(*first2, *first1)){
                    if(++first2 == last2) break;
                }else{
                    *dest++ = *first1;
                    ++first1;
                    ++first2;
                    if(first1 == last1 || first2 == last2) break;
                }
            }
        }
        return (dest);
    }

    template<typename IIter1, typename IIter2, typename OIter>
    inline OIter set_intersection(IIter1 first1, IIter1 last1, IIter2 first2, IIter2 last2, OIter dest){
        return (GHYSTL::set_intersection(first1, last1, 
                    static_cast<IIter1>(first2), static_cast<IIter1>(last2), dest, less<IIter1>()));
    }

    /*****************************************************************************************/
    // set_difference
    // 差集，A，B是两个集合，则所有属于A且不属于B的元素构成的集合（A,B集合有序）
    //
    //set_symmetric_difference
    //差集，A，B是两个集合，保留两个集合不一样的元素（A,B集合有序）
    /*****************************************************************************************/
    template<typename IIter1, typename IIter2, typename OIter, typename Comp>
    inline OIter set_difference(IIter1 first1, IIter1 last1, IIter2 first2, IIter2 last2, OIter dest, const Comp& comp){
        if(first1 != last1 && first2 != last2){
            for(;;){
                if(cmp(*first1, *first2)){
                    *dest++ = *first1;
                    if(++first1 == last1) break;
                }else if(cmp(*first2, *first1)){
                    if(++first2 == last2) break;
                }else{
                    ++first1;
                    ++first2;
                    if(first1 == last1 || first2 == last2) break;
                }
            }
        }
        return (GHYSTL::copy(first1, last1, dest));
    }

    template<typename IIter1, typename IIter2, typename OIter>
    inline OIter set_difference(IIter1 first1, IIter1 last1, IIter2 first2, IIter2 last2, OIter dest){
        return (GHYSTL::set_difference(first1, last1, 
                    static_cast<IIter1>(first2), static_cast<IIter1>(last2), dest, less<IIter1>()));
    }

    template<typename IIter1, typename IIter2, typename OIter, typename Comp>
    inline OIter set_symmetric_difference(IIter1 first1, IIter1 last1, IIter2 first2, IIter2 last2, OIter dest, const Comp& comp){
        if(first1 != last1 && first2 != last2){
            for(;;){
                if(cmp(*first1, *first2)){
                    *dest++ = *first1;
                    if(++first1 == last1) break;
                }else if(cmp(*first2, *first1)){
                    *dest++ = *first2;
                    if(++first2 == last2) break;
                }else{
                    ++first1;
                    ++first2;
                    if(first1 == last1 || first2 == last2) break;
                }
            }
        }
        dest = GHYSTL::copy(first2, last2, dest);
        return (GHYSTL::copy(first1, last1, dest));
    }

    template<typename IIter1, typename IIter2, typename OIter>
    inline OIter set_symmetric_difference(IIter1 first1, IIter1 last1, IIter2 first2, IIter2 last2, OIter dest){
        return (GHYSTL::set_difference(first1, last1, 
                    static_cast<IIter1>(first2), static_cast<IIter1>(last2), dest, less<IIter1>()));
    }

    /*****************************************************************************************/
    // _partion_imple
    // 以某种策略对集合进行分割
    /*****************************************************************************************/
    template<typename FIter, typename Pred>
    FIter _partion_imple(FIter first, FIter last, const Pred& pred, GHYSTL::forward_iterator_tag){
        for(; first != last && pred(*first);) ++first;

        if(first == last) return (first);

        for(FIter next = first; ++next != last;){
            if(pred(*next)) GHYSTL::iter_swap(first++, next);
        }

        return first;
    }

    template<typename BIter, typename Pred>
    BIter _partion_imple(BIter first, BIter last, const Pred& pred, GHYSTL::bidirectional_iterator_tag){
        for(;; ++first){
            for(; first != last && pred(*first);) ++first;
            if(first == last) break;

            for(; first != --last && (!pred(*last));) ;
            
            // if(first == last) break; 

            GHYSTL::iter_swap(first, last);
        }

        return first;
    }

    template<typename Iter, typename Pred>
    Iter partition(Iter first, Iter last, const Pred& pred){
        return (GHYSTL::_partion_imple(first, last, pred, iter_cate_t<Iter>()));
    }


    /*****************************************************************************************/
    // _search_imple
    // 判段一个容器是否和另一个容器相交, 是就返回 第一个相交的位置的迭代器， 不是返回 last1
    /*****************************************************************************************/
    template<typename FIter1, typename FIter2, typename Pred>
    inline FIter1 _search_imple(FIter1 first1, FIter1 last1, FIter2 first2, FIter2 last2,
                                const Pred& pred, GHYSTL::forward_iterator_tag, GHYSTL::forward_iterator_tag){
       for(; first1 != last1; ++first1){ // 典型的滑动窗口
           FIter1 cpf1 = first1;
           for(FIter2 cpf2 = first2; ; ++cpf1, ++cpf2){
               if(cpf2 == last2){
                   return first1;
               }else if(!pred(*cpf1, *cpf2)){
                   break;
               }
           }
       }
       return last1;
    }

    template<typename FIter1, typename FIter2, typename Pred>
    inline FIter1 _search_imple(FIter1 first1, FIter1 last1, FIter2 first2, FIter2 last2,
                const Pred& pred, GHYSTL::random_access_iterator_tag, GHYSTL::random_access_iterator_tag){
        GHYSTL::iter_dif_t<FIter1> count1 = last1 - first1;
        GHYSTL::iter_dif_t<FIter2> count2 = last2 - first2;

        for(; count2<=count1; --count1, ++first1){
            FIter1 cpf1 = first1;
            for(FIter2 cpf2 = first2; ; ++cpf1, ++cpf2){
                if(cpf2 == last2){
                    return (first1);
                }else if(!pred(*cpf1, *cpf2)){
                    break;
                }
            }
        }
        return last1;
    }

    template<typename FIter1, typename FIter2, typename Pred>
    inline FIter1 search(FIter1 first1, FIter1 last1, FIter2 first2, FIter2 last2,
                const Pred& pred){

        return (GHYSTL::_search_imple(first1, last1, first2, last2, pred, 
                                        iter_cate_t<FIter1>(), iter_cate_t<FIter2>()));
    }

    template<typename FIter1, typename FIter2>
    inline FIter1 search(FIter1 first1, FIter1 last1, FIter2 first2, FIter2 last2){

        return (GHYSTL::search(first1, last1, first2, last2,
                static_cast<FIter1>(first2), static_cast<FIter1>(last2), equal_to<FIter1>()));
    }

    // 容器一个 位置x 到 x+n 的区间 [x, x+n]全部等于 val， 返回x，否则返回 last 
    template<typename FIter, typename Dif, typename value_type, typename Pred>
    inline FIter _search_n_imple(FIter first, FIter last, Dif n, const value_type& val, const Pred& pred,
                                GHYSTL::random_access_iterator_tag){
        if(n <= 0) return first;
        GHYSTL::iter_dif_t<FIter> count = last - first;
        FIter result = first;
        for(Dif mark = n-1; mark < count; mark += n){
            if(pred(first[mark], val)){
                for(Dif cur = 0; ;++cur){
                    if(cur == n)
                        return result;
                    else if(!pred(result[cur], val)){
                        mark -=(n - cur -1);
                        break;
                    }
                } 
            }
            result = first + mark + 1;
        }
        return last;
    }

    template<typename FIter, typename Dif, typename value_type, typename Pred>
    inline FIter search_n(FIter first, FIter last, Dif n, const value_type& val, const Pred& pred){
        return GHYSTL::_search_n_imple(first, last, n, val, pred, iter_cate_t<FIter>());
    }

    template<typename FIter, typename Dif, typename value_type>
    inline FIter search_n(FIter first, FIter last, Dif n, const value_type& val){
        return GHYSTL::_search_n_imple(first, last, n, val, equal_to<FIter>());
    }

    template<typename IIter1, typename IIter2, typename Comp>
    inline bool includes(IIter1 first1, IIter1 last1, IIter2 first2, IIter2 last2, const Comp& comp){
        if(first1 != last1 && first2 != last2){
            for(;;){
                if(cmp(*first2, *first1)){
                    return false;
                }else if(cmp(*first1, *first2)){
                    if(++first1 == last1) break;
                }else{
                    ++first1;
                    ++first2;
                    if(first1 == last1 || first2 == last2) break;
                }
            }
        }

        return (first2 == last2);
    }

    template<typename IIter1, typename IIter2>
    inline bool includes(IIter1 first1, IIter1 last1, IIter2 first2, IIter2 last2){
        return GHYSTL::includes(first1, last1, 
                static_cast<IIter1>(first2), static_cast<IIter1>(last2), GHYSTL::less<IIter1>());
    }
    
    /*-----------------------------------------------------8月5号之前的版本----------------------------------------------------------------*/

    /*****************************************************************************************/
    // equal
    // 比较第一序列在 [first, last)区间上的元素值是否和第二序列相等
    /*****************************************************************************************/
    template<class InputIterator1, class InputIterator2>
    inline bool 
    equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2){
        for(; first1 != last1; ++first1, ++first2){
            if(*first1 != *first2){
                return false;
            }
        }
        //不考虑第二个序列多出来的部分
        return true;
    }

    // CustomerCompared: 用户自定义的比较函数
    template<class InputIterator1, class InputIterator2, class CustomerCompared>
    inline bool 
    equal(InputIterator1 first1, InputIterator1 last1, 
            InputIterator2 first2, InputIterator2 last2, CustomerCompared cus_comp){
        for(; first1 != last1; ++first1, ++first2){
            if(!cus_comp(*first1, *first2)){
                return false;
            }
        }
        //不考虑第二个序列多出来的部分
        return true;
    }


    /*****************************************************************************************/
    // fill
    // 为 [first, last)区间内的所有元素填充新值
    /*****************************************************************************************/
    template<class ForwardIterator, class T>
    inline void 
    fill(ForwardIterator first, ForwardIterator last, const T& value){
        for(;first != last; ++first){
            *first = value;
        }
    }

    // 为 字符 类型提供特化版本
    inline void 
    fill(char* first, char* last, const char& value){
        std::memset(first, static_cast<unsigned char>(value), last-first);
    }

    inline void 
    fill(wchar_t* first, wchar_t* last, const wchar_t& value){
        std::memset(first, static_cast<unsigned wchar_t>(value), (last-first) * sizeof(wchar_t));
    }


    /*****************************************************************************************/
    // fill_n
    // 从 first 位置开始填充 n 个值
    /*****************************************************************************************/
    template<class OutputIterator, class size, class T>
    inline OutputIterator 
    fill_n(OutputIterator first, size n, const T& value){
        for(;n > 0; --n, ++first){
            *first = value;
        }
        return first;
    }

    // 为 字符 类型提供特化版本
    template<class size>
    inline char* 
    fill_n(char* first, size n, const char& value){
        if (n > 0)
            std::memset(first, static_cast<unsigned char>(value), n);
        return first + n;
    }

    template<class size>
    inline wchar_t* 
    fill_n(wchar_t* first, size n, const wchar_t& value){
        if (n > 0)
            std::memset(first, static_cast<unsigned wchar_t>(value), n * sizeof(wchar_t));
        return first + n;
    }

    /*****************************************************************************************/
    // lexicographical_compare
    // 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
    // (1)如果第一序列的元素较小，返回 true ，否则返回 false
    // (2)如果到达 last1 而尚未到达 last2 返回 true
    // (3)如果到达 last2 而尚未到达 last1 返回 false
    // (4)如果同时到达 last1 和 last2 返回 false (同时到达说明相等)
    /*****************************************************************************************/ 
    template<class InputIterator1, class InputIterator2>
    inline bool 
    lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2){
        for(; first1 != last1 && first2 != last2; ++first1, ++first2){
            if(*first1 < *first2)
                return true;
            if(*first2 < *first1)
                return false;
        }
        // 如果第一个序列到达尾端，但是第二个序列仍有剩余，那么第一个序列小于第二个序列
        // 比较第一个序列是否小于第二个序列，小于返回true, 不小于返回 false
        return first1 == last1 && first2 != last2;
    }

    // compare：用户指定的大小比较方法
    template<class InputIterator1, class InputIterator2, class compare>
    inline bool 
    lexicographical_compare(InputIterator1 first1, InputIterator1 last1, 
                InputIterator2 first2, InputIterator2 last2, compare comp){
        for(; first1 != last1 && first2 != last2; ++first1, ++first2){
            if(comp(*first1, *first2))
                return true;
            if(comp(*first2, *first1))
                return false;
        }
        // 如果第一个序列到达尾端，但是第二个序列仍有剩余，那么第一个序列小于第二个序列
        // 比较第一个序列是否小于第二个序列，小于返回true, 不小于返回 false
        return first1 == last1 && first2 != last2;
    }

    // 为了增强效率，设计 const unsigned char* 的特化版本
    inline bool 
    lexicographical_compare(const unsigned char* first1, const unsigned char* last1,
                                const unsigned char* first2, const unsigned char* last2){
        const size_t len1 = last1 - first1;
        const size_t len2 = last2 - first2;
        // 先比较相同长度的部分, memcmp 返回值
        // 如果返回值 < 0，则表示 str1 小于 str2。
        // 如果返回值 > 0，则表示 str2 小于 str1。
        // 如果返回值 = 0，则表示 str1 等于 str2。
        const int result = std::memcmp(first1,first2,GHYSTL::min(len1,len2));
        // 若相等，则长度较长者视为比较大
        return result != 0 ? result < 0 : len1 < len2;
    }


    /*****************************************************************************************/
    // copy_backward
    // 将 [first, last)区间内的元素拷贝到 [result - (last - first), result)内
    /*****************************************************************************************/

    template<typename BidirectionalIterator1, typename BidirectionalIterator2, typename Distance>
    inline BidirectionalIterator2
    _copy_backward_d(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result,
                     Distance *) {
        for (Distance distance = last - first; distance > 0; --distance, --result, --last) {
            *result = *last;
        }
        return result;
    }

    template<typename T>
    inline T *
    _copy_backward_t(const T *first, const T *last, T *result, true_type) {
        size_t len = sizeof(T) * (last - first);
        memmove(result - (last - first), first, len);
        return result - (last - first);
    }

    template<typename T>
    inline T *
    _copy_backward_t(const T *first, const T *last, T *result, false_type) {
        return _copy_backward_d(first, last, result, static_cast<ptrdiff_t *>(nullptr));
    }

    template<typename BidirectionalIterator1, typename BidirectionalIterator2>
    inline BidirectionalIterator2
    _copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result,
                   bidirectional_iterator_tag) {
        for (; last != first; --last, --result) {
            *result = *last;
        }
        return result;
    }

    template<typename BidirectionalIterator1, typename BidirectionalIterator2>
    inline BidirectionalIterator2
    _copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result,
                   random_access_iterator_tag) {
        return _copy_backward_d(first, last, result, distance_type(first));
    }

    template<typename BidirectionalIterator1, typename BidirectionalIterator2>
    struct copy_backward_dispatch {
        BidirectionalIterator2
        operator()(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
            return _copy_backward(first, last, result, iterator_category(first));
        }
    };

    template<typename T>
    struct copy_backward_dispatch<T *, T *> {
        T *operator()(T *first, T *last, T *result) {
            using operator_type = typename type_traits<T>::has_trivial_assignment_operator;
            return _copy_backward_t(first, last, result, operator_type());
        }
    };

    template<typename T>
    struct copy_backward_dispatch<const T *, T *> {
        T *operator()(T *first, T *last, T *result) {
            using operator_type = typename type_traits<T>::has_trivial_assignment_operator;
            return _copy_backward_t(first, last, result, operator_type());
        }
    };

    template<typename BidirectionalIterator1, typename BidirectionalIterator2>
    inline BidirectionalIterator2
    copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
        return copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2>()(first, last, result);
    }

    inline char *
    copy_backward(char *first, char *last, char *result) {
        memmove(result - (last - first), first, (last - first));
        return result - (last - first);
    }

    inline wchar_t *
    copy_backward(wchar_t *first, wchar_t *last, wchar_t *result) {
        size_t len = sizeof(wchar_t) * (last - first);
        memmove(result - (last - first), first, len);
        return result - (last - first);
    }
}
#endif