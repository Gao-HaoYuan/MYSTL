/**
 * @file algorithm.h
 * @author ghy (ghy_mike@163.com)
 * @brief  算法的封装和实现
 * 
 * @version 1.0
 * @date 2022-08-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once
#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_


#include "algo_numeric.h"

namespace GHYSTL{

    /*****************************************************************************************/
    // adjacent_find
    // 找出第一组满足条件的相邻元素。这里所谓的条件，在版本一种指的是“两元素相等”
	// 在版本二中，允许用户指定一个二元运算，二元运算的操作数分别是相邻的第一个元素和第二个元素
    /*****************************************************************************************/
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

    template<typename ForwardIterator>
    inline ForwardIterator
    adjacent_find(ForwardIterator first, ForwardIterator last){
        return GHYSTL::adjacent_find(first, last, equal_to<ForwardIterator>());
    }
     
    /*****************************************************************************************/
    // count
    // 运用equality操作符，将 [ first, last ) 区间内的每一个元素拿来和指定值 value 做比较
    // 返回与 value 相等的元素个数
    /*****************************************************************************************/
    // compare: 用户自定义的操作
    template<typename InputIterator, typename compare>
    inline GHYSTL::iter_dif_t<InputIterator> 
    count_if(InputIterator first, InputIterator last, const compare& comp){
        iter_dif_t<InputIterator> counter = 0;
        for(; first != last; ++first){
            if(comp(*first)) ++counter;
        }
        return (counter);
    }

    template<typename InputIterator, typename T>
    inline GHYSTL::iter_dif_t<InputIterator> 
    count(InputIterator first, InputIterator last, const T& value){
        iter_dif_t<InputIterator> counter = 0;
        for(; first != last; ++first){
            if(*first == value) ++counter;
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
            ++first;
        }
        return (first);
    }

    // Pred: 用户自定义的操作
    template<typename InputIterator, typename Pred>
    inline InputIterator 
    find_if(InputIterator first, InputIterator last, const Pred& pred){
        for(; first != last && (!pred(*first));){
            ++first;
        }
        return (first);
    }

    template<typename FIter1, typename FIter2, typename Pred>
    inline FIter1 _find_end_imple(FIter1 first1, FIter1 last1, FIter2 first2, FIter2 last2, const Pred& pred,
                                GHYSTL::forward_iterator_tag, GHYSTL::forward_iterator_tag){
        FIter1 result = last1;
        FIter1 new_result;
        while (true)
        {
            new_result = GHYSTL::search(first1, first2, first2, last2, pred);

            if(new_result == last1){
                return result;
            }else{
                result = new_result;
                first1 = ++new_result;
            }
        }
        
        return last1;
    }

    template<typename BIter1, typename BIter2, typename Pred>
    inline BIter1 _find_end_imple(BIter1 first1, BIter1 last1, BIter2 first2, BIter2 last2, const Pred& pred,
                                GHYSTL::bidirectional_iterator_tag, GHYSTL::bidirectional_iterator_tag){
        BIter1 rresult = GHYSTL::search(last1, first1, last2, first2, pred);

        if(rresult == first1){
            return last1;
        }else{
            BIter1 result = rresult.base();
            GHYSTL::advance(result, -distance(first2, last2));
            return result;
        }
    }

    template<typename Iter1, typename Iter2, typename Pred>
    inline Iter1 find_end(Iter1 first1, Iter2 last1, Iter2 first2, Iter2 last2, const Pred& pred){
        return GHYSTL::_find_end_imple(first1, last1, first2, last2, pred, 
                                iter_cate_t<Iter1>(), iter_cate_t<Iter2>());
    }

    template<typename Iter1, typename Iter2>
    inline Iter1 find_end(Iter1 first1, Iter2 last1, Iter2 first2, Iter2 last2){
        return GHYSTL::_find_end_imple(first1, last1, static_cast<Iter1>(first2), static_cast<Iter1>(last2), 
                    equal_to<Iter1>(), iter_cate_t<Iter1>(), iter_cate_t<Iter2>());
    }


    template<typename FIter1, typename FIter2, typename Pred>
    inline FIter1 find_first_of(FIter1 first1, FIter1 last1, FIter2 first2, FIter2 last2, const Pred& pred){
        for(; first1 != last1; ++first1){
            for(FIter2 cur = first2; cur != last2; ++cur){
                if(!pred(*first1, *cur)) return first1;
            }
        }
        return last1;
    }

    template<typename FIter1, typename FIter2>
    inline FIter1 find_first_of(FIter1 first1, FIter1 last1, FIter2 first2, FIter2 last2){
        return GHYSTL::find_first_of(first1, last1, 
                        static_cast<FIter1>(first2), static_cast<FIter1>(last2), equal_to<FIter1>());
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
    OutputIterator
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


    template<typename IIter1, typename IIter2, typename OIter, typename Comp>
    inline OIter merge(IIter1 first1, IIter1 last1, IIter2 first2, IIter2 last2, OIter dest, const Comp& cmp){
        if(first1 != last1 && first2 != last2){
            for(;;){
                if(cmp(*first2, *first1)){
                    *dest++ = *first2;
                    if(++first2 == last2) break;
                }else{
                    *dest++ = *first1;
                    if(++first1 == last1) break;
                }
            }
        }

        dest = GHYSTL::copy(first2, last2, dest);
        return (GHYSTL::copy(first1, last1, dest));
    }

    template<typename IIter1, typename IIter2, typename OIter>
    inline OIter merge(IIter1 first1, IIter1 last1, IIter2 first2, IIter2 last2, OIter dest){
        return (GHYSTL::merge(first1, last1, first2, last2, dest, less<IIter1>()));
    }

    template<typename Iter, typename value_type>
    inline Iter remove(Iter first, Iter last, const value_type& val){
        first = GHYSTL::find(first, last, val);
        if(first != last){
            for(Iter next = first; ++next != last;){
                if(*next != val) *first++ = std::move(*next);//移动语义, 很方便，让后面的值失效了
            }
        }
        return first;
    }

    template<typename Iter, typename OIter, typename value_type>
    inline OIter remove_copy(Iter first, Iter last, OIter dest, const value_type& val){
        for(;first != last; ++first){
            if(*first != val) *dest++ = *first;
        }
        return dest;
    }

    template<typename Iter, typename Pred>
    inline Iter remove_if(Iter first, Iter last, const Pred& pred){
        first = GHYSTL::find_if(first, last, pred);
        if(first != last){
            for(Iter next = first; ++next != last;){
                if(!pred(*next)) *first++ = std::move(*next);
            }
        }
        return first;
    }

    template<typename IIter, typename OIter, typename Pred>
    inline OIter remove_copy_if(IIter first, IIter last, OIter dest, const Pred& pred){
        for(; first != last; ++first){
            if(!pred(*first)) *dest++ = std::move(*first);
        }
        return dest;
    }

    template<typename FIter, typename value_type>
    inline void replace(FIter first, FIter last, const value_type& old_val, const value_type& new_val){
        for(; first != last; ++first){
            if(*first == old_val) *first = new_val;
        }
    }

    template<typename FIter, typename OIter, typename value_type>
    inline OIter replace_copy(FIter first, FIter last, OIter dest, const value_type& old_val, const value_type& new_val){
        for(; first != last; ++first, ++dest){
            *dest = *first == old_val ? new_val : *first;
        }
        return dest;
    }

    template<typename FIter, typename Pred, typename value_type>
    inline void replace_if(FIter first, FIter last, const Pred& pred, const value_type& val){
        for(; first != last; ++first)
            if(pred(*first)) *first = val;
    }

    template<typename FIter, typename OIter, typename Pred, typename value_type>
    inline void replace_copy_if(FIter first, FIter last, OIter dest, const Pred& pred, const value_type& val){
        for(; first != last; ++first, ++dest)
            *dest = pred(*first) ? val : *first;
    }

    template<typename BIter>
    inline void reverse(BIter first, BIter last){
        for(; first != last && first != --last; ++first)
            GHYSTL::iter_swap(first, last);
    }

    template<typename BIter, typename OIter>
    inline OIter reverse_copy(BIter first, BIter last, OIter dest){
        for(; first != last;) *dest++ = *--last;
        return dest;
    }

    template<typename FIter>
    inline void _rotate_imple(FIter first, FIter middle, FIter last,
                                GHYSTL::forward_iterator_tag){
        for(FIter mid = middle;;){
            GHYSTL::iter_swap(first++, mid++);
            if(first == middle){
                if(mid == last) return;
                middle = mid;
            }else if(mid == last){
                mid = middle;
            }
        }
    }

    template<typename BIter>
    inline void _rotate_imple(BIter first, BIter middle, BIter last,
                                GHYSTL::bidirectional_iterator_tag){
        GHYSTL::reverse(first, middle);
        GHYSTL::reverse(middle, last);
        GHYSTL::reverse(first, last);
    }

    template<typename Iter>
    inline void rotate(Iter first, Iter middle, Iter last){
        GHYSTL::_rotate_imple(first, middle, last, iter_cate_t<Iter>());
    }

    template<typename FIter, typename OIter>
    inline void rotate_copy(FIter first, FIter middle, FIter last, OIter dest){
        dest = GHYSTL::copy(middle, last, dest);
        reutrn (GHYSTL::copy(first, middle, dest));
    }

    template<typename FIter1, typename FIter2>
    inline FIter2 swap_ranges(FIter1 first1, FIter1 last1, FIter2 first2){
        for(; first1 != last1; ++first1) GHYSTL::iter_swap(first1, first2);
        return first2;
    }

    // 去掉容器里的重复值
    template<typename FIter, typename OIter, typename Pred>
    inline OIter unique_copy(FIter first, FIter last, OIter dest, const Pred& pred){
        if(first != last){
            GHYSTL::iter_val_t<FIter> val = *first;
            for(++first; first != last; ++first){
                if(!pred(*first, val)){
                    *dest++ = std::move(val);
                    val = *first;
                }
            }
            *dest++ = std::move(val); 
        }
        return dest;
    }

    template<typename FIter, typename OIter>
    inline OIter unique_copy(FIter first, FIter last, OIter dest){
        return GHYSTL::unique_copy(first, last, dest, equal_to<FIter>());
    }

    template<typename FIter, typename Pred>
    inline FIter unique(FIter first, FIter last, const Pred& pred){
        first = GHYSTL::adjacent_find(first, last, pred);
        if(first != last){
            FIter next = first;
            for(++next; ++next != last;){
                if(!pred(*first, *next)) *++first = std::move(*next);    
            }
            // 去掉最后一个重复的元素，或者作为 end()迭代器返回
            ++first;
        }
        return first;
    }

    template<typename FIter>
    inline FIter unique(FIter first, FIter last){
        return GHYSTL::unique(first, last, equal_to<FIter>());
    }

    template<typename FIter, typename value_type, typename Comp>
    inline FIter lower_bound(FIter first, FIter last, const value_type& val, const Comp& cmp){
        GHYSTL::iter_dif_t<FIter> len = distance(first, last);
        GHYSTL::iter_dif_t<FIter> half;
        FIter mid;

        while (len > 0)
        {
            half = (size_t)len >> 1;
            mid = first;
            GHYSTL::advance(mid, half);
            // 当 first mid len， 这种情况不是跳出条件  
            // first(mid) len 这种才是跳出条件， 此时，++mid = len, 是第一个不满足表达式 cmp(*mid, val) 的值.
            // 注意下面 val 和 mid 的顺序区别
            if(cmp(*mid, val)){ 
                first = ++mid;
                len = len - half -1;
            }else{
                len = half;
            }
        }
        
        return first;
    }

    template<typename FIter, typename value_type>
    inline FIter lower_bound(FIter first, FIter last, const value_type& val){
        return GHYSTL::lower_bound(first, last, val, less<FIter>());
    }

    template<typename FIter, typename value_type, typename Comp>
    inline FIter upper_bound(FIter first, FIter last, const value_type& val, const Comp& cmp){
        GHYSTL::iter_val_t<FIter> len = distance(first, last);
        GHYSTL::iter_val_t<FIter> half;
        FIter mid;

        while (len > 0)
        {
            half = (size_t)len >> 1;
            mid = first;
            GHYSTL::advance(mid, half);

            if(cmp(val, *mid)){
                len = half;
            }else{
                // 当 first mid len， 这种情况不是跳出条件  
                // first(mid) len 这种才是跳出条件， 此时，++mid = len, 是第一个不满足表达式 cmp(val， *mid) 的值
                // 注意上面 val 和 mid 的顺序区别
                first = ++mid;
                len = len - half - 1;
            }
        }
        return first;
    }

    template<typename FIter, typename value_type>
    inline FIter upper_bound(FIter first, FIter last, const value_type& val){
        return GHYSTL::upper_bound(first, last, val, less<FIter>());
    }

    template<typename FIter, typename value_type, typename Comp>
    inline bool binary_search(FIter first, FIter last, const value_type& val, const Comp& cmp){
        FIter result = GHYSTL::lower_bound(first, last, val, cmp);

        //  result != last      ->      *result>=val
        // !cmp(*result, val)   ->      *result<=val 
        return (result != last) && (!cmp(*result, val));
    }

    template<typename FIter, typename value_type>
    inline bool binary_search(FIter first, FIter last, const value_type& val){
        return GHYSTL::binary_search(first, last, val, less<FIter>());
    }

    // 后缀反转， 可以用于全排列， 返回容器的下一个组合
    template<typename BIter, typename Comp>
    inline bool next_permutation(BIter first, BIter last, const Comp& cmp){
        BIter before = last;
        if(first == last || first == --before) return false; // 集合为空或者只有一个元素就返回false

        for(BIter next = before ;; next = before){
            if(cmp(*--before, *next)){ // 找到第一个不满足这个表示的位置进行反转
                BIter instead = last;
                for(--instead; !(cmp(*before, *instead)); --instead) ; // 在后缀序列中，找到第一个大于当前before的数

                GHYSTL::iter_swap(before, instead);// 和before交换， 使交换后的序列仍然是递减的
                GHYSTL::reverse(next, last); // 反转数列，让后缀序列变成 递增 的，如果next，指向最后一个元素，是不会反转的
                return true;
            }

            if(before == first){ // 如果是递减的数列，就直接反转，返回false
                GHYSTL::reverse(first, last);
                return false;
            }
        }
    }

    template<typename BIter>
    inline bool next_permutation(BIter first, BIter last){
        return GHYSTL::next_permutation(first, last, less<BIter>());
    }


    //前缀反转，就是 cmp 函数和后缀反转了一下， 返回容器的上一个组合
    template<typename BIter, typename Comp>
    inline bool prev_permutation(BIter first, BIter last, const Comp& cmp){
        BIter before = last;// 注意 bofore 是 前缀--
        if(first == last || first == --before) return false; // 集合为空或者只有一个元素就返回false

        for(BIter next = before;; next = before){
            if(cmp(*next, *--before)){ // 注意 bofore 是 前缀--
                BIter instead = last;
                for(--instead; !(cmp(*instead, *before)); --instead) ;// 在后缀序列中，找到第一个小于当前before的数

                GHYSTL::iter_swap(before, instead);
                GHYSTL::reverse(next, last);// 反转数列，让后缀序列变成 递减 的，如果next，指向最后一个元素，是不会反转的
                return true;
            }

            if(before == first){// 如果是递增的数列，就直接反转，返回false
                GHYSTL::reverse(first, last);
                return false;
            }
        }
    }

    template<typename BIter>
    inline bool prev_permutation(BIter first, BIter last){
        return GHYSTL::prev_permutation(first, last, less<BIter>());
    }

    template<typename FIter1, typename FIter2, typename Pred>
    inline void _match_suffixes(FIter1 &last1, FIter2& last2, const Pred& pred, 
                                GHYSTL::forward_iterator_tag, GHYSTL::forward_iterator_tag){
        // forward_iterator 能读能写，但是不能向后偏移
    }

    template<typename FIter1, typename FIter2, typename Pred>
    inline void _match_suffixes(FIter1 &last1, FIter2& last2, const Pred& pred,
                                GHYSTL::bidirectional_iterator_tag, GHYSTL::bidirectional_iterator_tag){
        for(; pred(*--last1, *--last2);) ;
        ++last1;
        ++last2; 
    }

    template<typename Iter, typename value_type, typename Pred>
    inline Iter _find_fn2(Iter first, Iter last, const value_type& val, const Pred& pred){
        for(; first != last; ++first)
            if(pred(*first, val)) break;

        return first;    
    }

    template<typename Iter, typename value_type, typename Pred>
    inline GHYSTL::iter_dif_t<Iter> _count_fn2(Iter first, Iter last, const value_type& val, const Pred& pred){
        iter_dif_t<Iter> counter;
        for(; first != last; ++first)
            if(pred(*first, val)) ++counter;

        return counter;    
    }

    // 目的就是为了匹配两个迭代器的元素个数是否匹配
    template<typename Iter1, typename Iter2, typename Pred>
    inline bool _match_count(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2, const Pred& pred){
        GHYSTL::_match_suffixes(last1, last2, pred, iter_cate_t<Iter1>(), iter_cate_t<Iter2>()); // 先从后面匹配

        Iter1 skip;
        for(Iter1 cur= first1; cur != last1; ++cur){
            if(GHYSTL::_find_fn2(first1, cur, *cur, pred) == cur){ // 如果第一个容器中有两个值一样的元素，跳过
                GHYSTL::iter_dif_t<Iter2> count2 = _count_fn2(first2, last2, *cur, pred); // 统计和第一个容器第 cur 个元素相同的个数
                if(!count2) return false;

                skip = cur;
                GHYSTL::iter_dif_t<Iter1> count1 = _count_fn2(++skip, last1, *cur, pred) + 1; // 统计第一个容器有几个和 cur 的值相同的元素
                if(count1 != count2) return false;
            }
        }

        return true;
    }


    template<typename FIter1, typename FIter2, typename Pred>
    inline bool _is_permutation_imple(FIter1 first1, FIter1 last1, FIter2 first2, const Pred& pred){
        for(; first1 != last1; ++first1, ++first2){
            if( !pred(*first1, first2)){
                FIter2 last2 = first2;
                GHYSTL::advance(last2, distance(first1, last1));
                return GHYSTL::_match_count(first1, last1, first2, last2, pred);
            }
        }

        return true;
    }

    template<typename RIter1, typename RIter2, typename Pred>
    inline bool _is_permutation_imple(RIter1 first1, RIter1 last1, RIter2 first2, RIter2 last2,const Pred& pred,
                                        GHYSTL::random_access_iterator_tag, GHYSTL::random_access_iterator_tag){
       if(last1 - first1 != last2 - first2) return false;
       return GHYSTL::_is_permutation_imple(first1, last1, first2, pred);
    }

    template<typename FIter1, typename FIter2, typename Pred>
    inline bool _is_permutation_imple(FIter1 first1, FIter1 last1, FIter2 first2, FIter2 last2,const Pred& pred,
                                        GHYSTL::forward_iterator_tag, GHYSTL::forward_iterator_tag){
        for(; first1 != last1 && first2 != last2; ++first1, ++first2){
            if(!pred(*first1, *first2)){
                if(GHYSTL::distance(first1, last1) != GHYSTL::distance(first2, last2))
                    return false;
                else 
                    return GHYSTL::_match_count(first1, last1, first2, last2, pred);
            }      
        }

        return (first1 == last1 && first2 == last2);
    }
    
    template<typename Iter1, typename Iter2, typename Pred>
    inline bool is_permutation(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2, const Pred& pred){
        return GHYSTL::_is_permutation_imple(first1, last1, first2, last2, pred,
                                        iter_cate_t<Iter1>(), iter_cate_t<Iter2>());
    }

    template<typename Iter1, typename Iter2>
    inline bool is_permutation(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2){
        return GHYSTL::_is_permutation_imple(first1, last1, first2, last2, GHYSTL::equal_to<Iter1>());
    }

    //判断是否是一个容器是否是另一个容器的组合
    template<typename Iter1, typename Iter2>
    inline bool is_permutation(Iter1 first1, Iter1 last1, Iter2 first2){
        return GHYSTL::_is_permutation_imple(first1, last1, first2, GHYSTL::equal_to<Iter1>());
    }
    
    //插入一个元素的时候，堆要进行调整
    template<typename RIter, typename Dif, typename value_type, typename Comp>
    inline void _push_heap_imple(RIter first, Dif index, value_type&& val, const Comp &cmp){
        for(Dif par_index = (size_t)(index - 1) >> 1; index && cmp(*(first + par_index), val); 
                                par_index = (size_t)(index - 1) >> 1){
            *(first + index) = std::move(*(first + par_index));
            index = par_index;
        }
        *(first + index) = std::move(val);
    }

    
    template<typename RIter, typename Comp> 
    inline void push_heap(RIter first, RIter last, const Comp& cmp){
        // 先插入到 last 位置， 此时 last = last + 1， 然对 last-1（插入的元素）进行调整
        GHYSTL::_push_heap_imple(first, iter_dif_t<RIter> (last - first -1), std::move(*(last - 1)), cmp);
    }

    template<typename RIter>
    inline void push_heap(RIter first, RIter last){
        GHYSTL::push_heap(first, last, less<RIter>());
    }

    template<typename RIter, typename Dif, typename value_type, typename Comp>
    inline void _heapify(RIter first, Dif index, const Dif len, value_type&& val, const Comp &cmp){
        Dif right_index = (size_t)(index << 1) + 2;
        for(; right_index < len; ){
            if(cmp(*(first + right_index), *(first + (right_index - 1))))
                --right_index; // 左节点比右节点小(大)
            if(cmp(val, *(first + right_index))){
                *(first + index) = std::move(*(first + right_index));
            }else{
                *(first + index) = std::move(val);
                return;
            }
            index = right_index;
            right_index = (size_t)(index << 1) + 2;
        }

        if(right_index == len && cmp(val, *first + (--right_index))){//存在左节点，堆是完全二茶树
            index = right_index;
        } 
        *(first + index) = std::move(val);
    }

    template<typename RIter, typename Comp>
    inline void _pop_heap_imple(RIter first, RIter last, const Comp& cmp){
        GHYSTL::iter_val_t<RIter> val = std::move(*--last);// 堆底元素放到堆顶进行调整
        *last = std::move(*first); // 堆定的元素出堆, 把堆顶元素放到最后
        GHYSTL::_heapify(first, iter_dif_t<RIter>(0), iter_dif_t<RIter>(last - first), std::move(val), cmp);
    }

    template<typename RIter>
    inline void pop_heap(RIter first, RIter last){
        GHYSTL::_pop_heap_imple(first, last, less<RIter>());
    }

    template<typename RIter>
    inline void sort_heap(RIter first, RIter last){
        GHYSTL::sort_heap(first, last, less<RIter>());
    }

    template<typename RIter, typename Comp>
    inline void sort_heap(RIter first, RIter last, const Comp& cmp){
        // 把堆顶元素放到最后，前面的继续进行调整
        for(; last - first >= 2; --last) GHYSTL::_pop_heap_imple(first, last, cmp);
    }

    template<typename RIter, typename Dif, typename Comp>
    inline void _make_heap_imple(RIter first, Dif len, const Comp& cmp){
        GHYSTL::iter_val_t<RIter> val;
        //先从一半的子树开始调整，然后向前迭代
        for(Dif index = (size_t)(len - 1) >> 1; index >= 0; --index){
            val = std::move(*(first + index));
            GHYSTL::_heapify(first, index, len, std::move(val), cmp);
        }
    }

    template <typename RIter, typename Comp>
    inline void make_heap(RIter first, RIter last, const Comp &cmp) { 
        const GHYSTL::iter_dif_t<RIter> len = last - first;
        if(len >= 2)
            GHYSTL::_make_heap_imple(first, len, cmp);
    }

    template <typename RIter>
    void make_heap(RIter first, RIter last) { 
        GHYSTL::make_heap(first, last, less<RIter>());
    }

    // partial_sort
    // 对整个序列做部分排序，保证较小的 N 个元素以递增顺序置于[first, first + N)中
    template<typename RIter, typename Comp>
    inline void partial_sort(RIter first, RIter middle, RIter last, const Comp& cmp){
        GHYSTL::iter_dif_t<RIter> len = last - first;
        if(!len) return;
        else if(len == 1){
            GHYSTL::iter_swap(first, min_element(first, last, cmp));
            return;
        }

        GHYSTL::make_heap(first, middle, cmp);
        for(RIter cur = middle; cur != last; ++cur){
            if(cmp(*cur, *first)){ // 每次都把前面最大的提取走
                GHYSTL::iter_val_t<RIter> val = std::move(*cur);
                *cur = std::move(*first);
                GHYSTL::_heapify(first, iter_dif_t<RIter>(0), len, std::move(val), cmp);
            }
        }
        GHYSTL::sort_heap(first, middle, cmp);
    }

    template<typename RIter>
    inline void patial_sort(RIter first, RIter middle, RIter last){
        GHYSTL::partial_sort(first, middle, last, less<RIter>());
    }

    // partial_sort_copy
    // 行为与 partial_sort 类似，不同的是把排序结果复制到 result 容器中
    template<typename IIter, typename RIter, typename Comp>
    inline RIter partial_sort_copy(IIter first, IIter last, 
                                RIter dest_first, RIter dest_last, const Comp& cmp){
        RIter result = GHYSTL::copy(first, last, dest_first);
        GHYSTL::partial_sort(dest_first, result, result, cmp);
        return result;
    }

    template<typename IIter, typename RIter>
    inline RIter partial_sort_copy(IIter first, IIter last, 
                                RIter dest_first, RIter dest_last){
        return GHYSTL::partial_sort_copy(first, last, dest_first, dest_last, less<IIter>());
    }


    // 插入排序， 内层循环， 寻找有序区间的插入点
    template<typename BIter, typename value_type, typename Comp>
    inline void _unguarded_linear_insert(BIter last, value_type val, const Comp& cmp){
        for(BIter prev = GHYSTL::prev(last); cmp(val, *prev); last = prev, --prev){
            *last = std::move(*prev);
        }
        *last = std::move(val);
    }

    // 插入排序，外层循环
    template<typename BIter, typename Comp>
    inline void _unguarded_linear_insert(BIter first, BIter last, const Comp& cmp){
       for(BIter i = first; i != last; ++i)
            GHYSTL::_unguarded_linear_insert(i, std::move(*i), cmp);
    }

    // 插入排序的极端手法
    template<typename BIter, typename Comp>
    inline void _liner_insert(BIter first, BIter last, const Comp& cmp){
        iter_val_t<BIter> val = std::move(*last);
        if(cmp(val, *first)){
            GHYSTL::copy_backward(first, last, last+1);
            *first = std::move(val);
        }else{
            GHYSTL::_unguarded_linear_insert(last, std::move(val), cmp);
        }
    }

    // 插入排序
    template<typename BIter, typename Comp>
    inline void _insert_sort_imple(BIter first, BIter last, const Comp& cmp){
        for(BIter cur = first; ++cur != last;){
            GHYSTL::_liner_insert(first, cur, cmp);
        }
    }

    //插入排序
    template<typename BIter, typename Comp>
    inline void _insert_sort(BIter first, BIter last, const Comp& cmp){
        if(last == first) return;
        GHYSTL::_insert_sort_imple(first, last, cmp);
    }


    // 返回三个数中的 中间值
    template<typename value_type, typename Comp>
    inline const value_type &_get_piovt(const value_type& a, const value_type& b, 
                                        const value_type& c, const Comp& cmp){
        if (cmp(a, b)) {
            if (cmp(b, c))
                return (b);
            else if (cmp(a, c))
                return (c);
            else
                return (a);
        } 
        else if (cmp(a, c))
            return (a);
        else if (cmp(b, c))
            return (c);
        else
            return (b);
    }

    // 一次快排
    template <typename RIter, typename Comp>
    inline RIter _mid_partition(RIter first, RIter last, const Comp &cmp) {
        GHYSTL::iter_val_t<RIter> piovt = _get_piovt(*first, *(first + (size_t)(last - first) >> 1),
                                                        *(last - 1), cmp);
        for(;;++first){
            for(; cmp(*first, piovt); ++first) ;

            for(--last; cmp(piovt, *last); --last) ;

            if(!(first < last)) return first;
            GHYSTL::iter_swap(first, last);
        }

        return first;
    }

    // 快排 + 堆排， 因为最后使用了if else， 所以是分块排序的，最后有一块是无序的
    template<typename RIter, typename Comp>
    inline void _introsort_imple(RIter first, RIter last, 
                        size_t depth_limit, const Comp& cmp){
        for(; last - first > sort_threshold;){// 元素大于 32 个的时候，进行排序
            if(depth_limit-- == 0){ // 规定的快排的深度，深度为0时 使用堆排序
                GHYSTL::_make_heap_imple(first, iter_dif_t<RIter>(last - first), cmp);
                GHYSTL::sort_heap(first, last, cmp);
                return;
            }

            RIter cur = _mid_partition(first, last, cmp); //一次快排

            if(cur - first > last - cur){
                _introsort_imple(cur, last, depth_limit, cmp);
                last = cur;
            }else{
                _introsort_imple(first, cur, depth_limit, cmp);
                first = cur;
            }
        }
    }

    // 都是插入排序
    template<typename RIter, typename Comp>
    inline void _final_insert_sort(RIter first, RIter last, const Comp& cmp){
        if(last - first > sort_threshold){
            GHYSTL::_insert_sort(first, first + sort_threshold, cmp);
            GHYSTL::_unguarded_linear_insert(first + sort_threshold, last, cmp);
        }else
            GHYSTL::_insert_sort(first, last, cmp);
    }

    template<typename RIter, typename Comp>
    inline void sort(RIter first, RIter last, const Comp& cmp){
        if(last - first > sort_threshold)   // 先排序好一部分
            GHYSTL::_introsort_imple(first, last, size_t(_lg2((size_t)(last - first))) * 1.5, cmp);
        GHYSTL::_final_insert_sort(first, last, cmp); // 再使用插入排序，详细看我的草稿纸把
    }

    template<typename RIter>
    inline void sort(RIter first, RIter last){
        GHYSTL::sort(first, last, less<RIter>());
    }

    template<typename RIter, typename value_type, typename Comp>
    inline GHYSTL::pair<RIter, RIter> equal_range(RIter first, RIter last, 
                    const value_type& val, const Comp& cmp){
        iter_dif_t<RIter> len = last - first;
        iter_dif_t<RIter> half;
        RIter middle;
        while (len > 0)
        {
            half = (size_t)len >> 1;
            middle = first;
            GHYSTL::advance(first, half);
            
            if(cmp(*middle, val)){
                half = middle + 1;
                len = len - half - 1;
            }else if(cmp(val, *middle)){
                len = half;
            }else{ // val == *middle
                RIter left = GHYSTL::lower_bound(first, middle, val, cmp);
                RIter right = GHYSTL::upper_bound(middle + 1, last, val, cmp);
                return (pair<RIter, RIter>(left, right));
            }
        }

        return (pair<RIter, RIter>(first, last));
    }

    template<typename RIter, typename value_type>
    inline GHYSTL::pair<RIter, RIter> equal_range(RIter first, RIter last, const value_type& val){
        return GHYSTL::equal_range(first, last, val, less<RIter>());
    }

    // 使用缓冲区旋转容器
    template<typename BIter1, typename BIter2, typename Dif>
    inline BIter1 _rotate_adaptive(BIter1 first, BIter1 middle, BIter1 last, Dif len1, Dif len2,
                        BIter2 buf, Dif buf_size){
        BIter2 buf_end;
        if(len1 > len2 && len2 < buf_size){
            buf_end = GHYSTL::copy(middle, last, buf);
            GHYSTL::copy_backward(first, middle, last);
            return GHYSTL::copy(buf, buf_end, first);
        }else if(len1 < buf_size){
            GHYSTL::copy(first, middle, buf);
            GHYSTL::copy(middle, last, first);
            return GHYSTL::copy_backward(buf, buf_end, last);
        }else{
            GHYSTL::rotate(first, middle, last);
            advance(first, len2);
            return first;
        }
    }

    template<typename BIter1, typename BIter2, typename OIter, typename Comp>
    inline void _merge_backward(BIter1 first1, BIter1 last1, BIter2 first2, BIter2 last2, 
                    OIter dest, const Comp& cmp){
        if(first1 != last1 && first2 != last2){
            for(--last1, --last2;;){
                if(cmp(*last2, *last1)){ //把大的数放后面
                    *--dest = *last1;
                    if(last1 == first1) break;
                    --last1;
                }else{
                    *--dest = *last2;
                    if(last2 == first2) break;
                    --last2;
                }
            }
        }
        dest =GHYSTL::copy_backward(first2, last2, dest);
        GHYSTL::copy_backward(first1, last1, dest);
    }

    template<typename BIter, typename Dif, typename Pointer, typename Comp>
    inline void _merge_adaptive(BIter first, BIter middle, BIter last, Dif len1, Dif len2,
                        Pointer buf, Dif buf_size, const Comp& cmp){
        if(len1 <= len2 && len1 <= buf_size){
            BIter buf_end  = GHYSTL::copy(first, middle, buf);
            GHYSTL::merge(buf, buf_end, middle, last, first, cmp);
        }else if(len2 <= buf_size){
            BIter buf_end = GHYSTL::copy(middle, last, buf);
            GHYSTL::_merge_backward(first, middle, buf, buf_end, last, cmp);
        }else{
            BIter first_cut = first;
            BIter second_cut = middle;
            Dif len_11 = 0;
            Dif len_22 = 0;

            if(len1 > len2){
                len_11 = (size_t) len2 >> 1;
                GHYSTL::advance(first_cut, len_11);
                second_cut = GHYSTL::lower_bound(middle, last, *first_cut, cmp);
                len_22 = GHYSTL::distance(middle, second_cut);
            }else{
                len_22 = (size_t) len1 >> 1;
                GHYSTL::advance(second_cut, len_22);
                first_cut = GHYSTL::upper_bound(first, middle, *second_cut, cmp);
                len_11 = GHYSTL::distance(first, first_cut);
            }

            BIter new_middle = GHYSTL::_rotate_adaptive(first_cut, middle, second_cut,len1 - len_11, len2,
                                        buf, buf_size);

            GHYSTL::_merge_adaptive(first, first_cut, new_middle, len_11, len_22,
                                        buf, buf_size, cmp);

            GHYSTL::_merge_adaptive(new_middle, second_cut, last, len1 - len_11, len2 - len_22,
                                        buf, buf_size, cmp);

        }
    }

    template<typename BIter, typename Comp>
    inline void _inplace_merge_imple(BIter first, BIter middle, BIter last, const Comp& cmp){
        GHYSTL::iter_dif_t<BIter> len1 = distance(first, middle);
        GHYSTL::iter_dif_t<BIter> len2 = distance(middle, last);

        GHYSTL::temporary_buffer<iter_val_t<BIter>> buf((len1 < len2 ? len1 : len2));
        GHYSTL::_merge_adaptive(first, middle, last, len1, len2, 
                            buf.begin(), iter_dif_t<BIter>(buf.size()), cmp);
    }

    template<typename BIter, typename Comp>
    inline void inplace_merge(BIter first, BIter middle, BIter last, const Comp& cmp){
        if(first == middle || middle == last) return;
        GHYSTL::_inplace_merge_imple(first, middle, last, cmp);
    }

    template<typename BIter>
    inline void inplace_merge(BIter first, BIter middle, BIter last){
        GHYSTL::inplace_merge(first, middle, last, less<BIter>());
    }

    // 归并排序
    template<typename BIter, typename Dif, typename Pointer, typename Comp>
    inline void _stable_sort_imple(BIter first, BIter last, Pointer buf, Dif buf_size, const Comp& cmp){
        Dif len = GHYSTL::distance(first, last);
        if(len > sort_threshold){
            Dif len1 = (size_t) len >> 1;
            Dif len2 = len - len1;

            BIter middle = first;
            GHYSTL::advance(middle, len1);
            GHYSTL::_stable_sort_imple(first, middle, buf, buf_size, cmp);
            GHYSTL::_stable_sort_imple(middle, last, buf, buf_size, cmp);
            GHYSTL::_merge_adaptive(first, middle, last, len1, len2, buf, buf_size, cmp);
        }else
            GHYSTL::_insert_sort(first, last, cmp);
    }

    template<typename BIter, typename Comp>
    inline void stable_sort(BIter first, BIter last, const Comp& cmp){
        GHYSTL::iter_dif_t<BIter> len = distance(first, last);
        if(len > 1){
            GHYSTL::iter_dif_t<BIter> len1 = (size_t) len >> 1;
            GHYSTL::iter_dif_t<BIter> len2 = len - len1;

            GHYSTL::temporary_buffer<iter_val_t<BIter>> buf(len1 < len2 ? len1 : len2);
            GHYSTL::_stable_sort_imple(first, last, buf.begin(), iter_dif_t<BIter>(buf.size()), cmp);
        }else
            GHYSTL::_insert_sort(first, last, cmp);
    }

    // 归并排序
    template<typename BIter>
    inline void stable_sort(BIter first, BIter last){
        GHYSTL::stable_sort(first, last, less<BIter>());
    }

    // nth_element
    // 对序列重排，使得所有小于第 n 个元素的元素出现在它的前面，大于它的出现在它的后面  
    template<typename RIter, typename Comp>
    inline void nth_element(RIter first, RIter nth, RIter last, const Comp& cmp){
        RIter cut;
        while (last - first > 3)
        {
            cut = GHYSTL::_mid_partition(first, last, cmp);//进行一次快排

            if(cut <= nth)
                first = cut;
            else
                last = cut;
        }
        
        GHYSTL::_insert_sort(first, last, cmp);
    }
    
    template<class RIter>
    inline void nth_element(RIter first, RIter nth, RIter last){
        GHYSTL::nth_element(first, nth, last, less<RIter>());
    }
}
#endif
