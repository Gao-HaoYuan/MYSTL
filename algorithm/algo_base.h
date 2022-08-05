/*
*   equal、fill、fill_n、iter_sawp、max、min、
*   lexicographical_compare、mismatch、copy 、copy_backward 算法的实现
*/

#ifndef GHYSTL_ALGO_BASE_H_
#define GHYSTL_ALGO_BASE_H_

#include <cstring>

#include "iterator.h"
#include "type_traits.h"
#include "util.h"

namespace GHYSTL{
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
        for(;firs != last; ++first){
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
    // iter_swap
    // 将两个迭代器所指对象对调
    /*****************************************************************************************/
    template<class ForwardIterator1 ,class ForwardIterator2>
    inline void 
    iter_swap(ForwardIterator1 first, ForwardIterator2 last){
        GHYSTL::swap(*first, *last);
    }


    /*****************************************************************************************/
    // max
    // 取二者中的较大值，语义相等时保证返回第一个参数
    /*****************************************************************************************/
    template<class T>
    inline const T& 
    max(const T& a, const T& b){
        return a>b ? a:b;
    }

    //compare: 用户自定义的比较函数
    //compare值为真，返回第一个参数，假返回第二个参数
    template<class T, class compare>
    inline const T& 
    max(const T& a, const T& b, compare comp){
        return comp(a,b) ? a:b;
    }


    /*****************************************************************************************/
    // min 
    // 取二者中的较小值，语义相等时保证返回第一个参数
    /*****************************************************************************************/
    template<class T>
    inline const T& 
    min(const T& a, const T& b){
        return a<b ? a:b;
    }

    //compare: 用户自定义的比较函数
    //compare值为真，返回第二个参数，假返回第一个参数
    template<class T, class compare>
    inline const T& 
    min(const T& a, const T& b, compare comp){
        return comp(a,b) ? b:a;
    }


    /*****************************************************************************************/
    // lexicographical_compare
    // 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
    // (1)如果第一序列的元素较小，返回 true ，否则返回 false
    // (2)如果到达 last1 而尚未到达 last2 返回 true
    // (3)如果到达 last2 而尚未到达 last1 返回 false
    // (4)如果同时到达 last1 和 last2 返回 false
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
    // mismatch
    // 平行比较两个序列，找到第一处失配的元素，返回一对迭代器，分别指向两个序列中失配的元素
    /*****************************************************************************************/
    template<class InputIterator1, class InputIterator2>
    inline GHYSTL::pair<InputIterator1, InputIterator2> 
    mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2){
        while (first1 != last1 && *first1 == *first2)
        {
            ++first1;
            ++first2;
        }

        return GHYSTL::pair<InputIterator1, InputIterator2>(first1,first2);
    }

    //compare: 用户自定义比较操作
    template<class InputIterator1, class InputIterator2, class compare>
    inline GHYSTL::pair<InputIterator1, InputIterator2> 
    mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, compare comp){
        while (first1 != last1 && comp(*first1, *first2))
        {
            ++first1;
            ++first2;
        }

        return GHYSTL::pair<InputIterator1, InputIterator2>(first1,first2);
    }


    /*****************************************************************************************/
    // copy
    // 把 [first, last)区间内的元素拷贝到 [result, result + (last - first))内
    /*****************************************************************************************/
    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator 
    __copy(InputIterator first, InputIterator last, OutputIterator result, GHYSTL::true_type){
        using Distance = GHYSTL::iterator_traits<InputIterator>::difference_type;
        Distance dist = 0;
        GHYSTL::distance(first, last, dist);
        GHYSTL::memmove(first, last, sizeof(first) * dist);
        GHYSTL::advance(result, dist);
        return result;
    }

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator 
    __copy(InputIterator first, InputIterator last, OutputIterator result, GHYSTL::false_type){
        using Distance = GHYSTL::iterator_traits<InputIterator>::difference_type;
        while (first != last)
        {
            *result = *first;
            ++result;
            ++first;
        }
        return result;
    }

    template<typename InputIterator, typename OutputIterator, typename T>
    inline OutputIterator 
    _copy(InputIterator first, InputIterator last, OutputIterator result, T*){
        using is_POD = GHYSTL::_type_traits<T>::is_POD_type;
        return __copy(first, last, result, is_POD());
    }

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator 
    copy(InputIterator first, InputIterator last, OutputIterator result){
        return _copy(first, last, result, GHYSTL::value_type(first));
    }

    /* copy 算法针对const char* 版本的特化版 */
	inline char* 
    copy(const char* first, const char* last, char* result)
	{
		GHYSTL::memmove(result, first, last - first) ;
		return result + (last - first) ;
	}

	/* copy 算法针对const wchar_t* 版本的特化版 */
	inline wchar_t* 
    copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
	{
		GHYSTL::memmove(result, first, sizeof(wchar_t) * (last - first)) ;
		return result + (last - first) ;
	}


    /*****************************************************************************************/
    // copy_backward
    // 将 [first, last)区间内的元素拷贝到 [result - (last - first), result)内
    /*****************************************************************************************/

    //支持链表
    template<typename BidirectionalIterator1, typename BidirectionalIterator2>
    inline BidirectionalIterator2
    __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
                    BidirectionalIterator2 result, GHYSTL::bidirectional_iterator_tag){
        while (first != last)
        {
            *--result == *--last;
        }
        return result;
    }

    template<typename BidirectionalIterator1, typename BidirectionalIterator2>
    inline BidirectionalIterator2
    __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
                    BidirectionalIterator2 result, GHYSTL::random_access_iterator_tag){
        for(auto n = last - first; n>0; --n){
            *--result == *--last;
        }
        return result;
    }
    
    template<typename BidirectionalIterator1, typename BidirectionalIterator2>
    inline BidirectionalIterator2
    _copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
                    BidirectionalIterator2 result){
        return __copy_backward(first, last, result, GHYSTL::iterator_category(first));
    }

    template<typename BidirectionalIterator1, typename BidirectionalIterator2>
    inline BidirectionalIterator2
    copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
                    BidirectionalIterator2 result){
        return _copy_backward(first, last, result);
    }

    /* copy_backward 算法针对const char* 版本的特化版 */
    inline char* 
    copy_backward(const char *first, const char *last, char *result) {
        GHYSTL::memmove(result - (last - first), first, (last - first));
        return result - (last - first);
    }

    /* copy_backward 算法针对const wchar_t* 版本的特化版 */
    inline wchar_t* 
    copy_backward(const wchar_t *first, const wchar_t *last, wchar_t *result) {
        size_t len = sizeof(wchar_t) * (last - first);
        GHYSTL::memmove(result - (last - first), first, len);
        return result - (last - first);
    }
}
#endif