/*
*  数值算法
*/

#ifndef GHYSTL_ALGO_NUMERIC_H_
#define GHYSTL_ALGO_NUMERIC_H_

#include "util.h"
#include "functional.h"
#include "iterator.h"

namespace GHYSTL{

    /*****************************************************************************************/
    // accumulate
    // 计算 init 和 [ first, last )内所有元素的总和
    /*****************************************************************************************/
    // 必须提供 init ，因为当 [ first, last )为空时仍能获得一个明确定义的值
    // 如果希望计算 [ first, last )中所有数值的总和，应该将 init 设为 0
    template<typename iterator, typename T>
    inline T 
    accumulate(iterator first, iterator last,  T init){
        for(; first != last; ++first) init += *first;
        return init;
    }

    template<typename iterator, typename T, typename Function>
    inline T 
    accumulate(iterator first, iterator last, T init, const Function& fun){
        for(; first != last; ++first) init = fun(init, *first);
        return init;
    }


    /*****************************************************************************************/
    // ajacent_difference 
    // 来计算[ first, last ] 中相邻元素的差额
    /*****************************************************************************************/
    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator
    ajacent_difference(InputIterator first, InputIterator last, OutputIterator result){
        if (first != last){
            *result = *first; // 记录第一个元素
            for(InputIterator prev = first; ++first != last; ++prev){
                *++result = *first - *prev;
            }
            *++result = *last - *first; // 记录最后一个元素
        }
        return result;
    }

    template<typename InputIterator, typename OutputIterator, typename binaryOp>
    inline OutputIterator
    ajacent_difference(InputIterator first, InputIterator last, OutputIterator result,  const binaryOp& binary_op){
        if (first != last){
            *result = *first; // 记录第一个元素
            for(InputIterator prev = first; ++first != last; ++prev){
                *++result = binary_op(*first - *prev);
            }
            *++result = binary_op(*last - *first); // 记录最后一个元素
        }
        return result;
    }

    /*****************************************************************************************/
    // inner_product 
    // 计算[ first1, last1 ] 和 [ fisrt2, first2 + ( last1 - fisrt1 ) ] 的一般内积（generalied inner product）
    // 必须提供初值，确保 [ fisrt, last ] 为空时，仍有明确定义
    /*****************************************************************************************/
    template<typename InputIterator1, typename InputIterator2, typename T>
    inline T 
    inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init){
		    for(;first1 != last1; ++first1, ++first2) init = init + (*first1) * (*first2);
        return init;
	  }

    // 根据用户自定义的函数：binary_op1、binary_op2，计算内积
    template<typename InputIterator1, typename InputIterator2, typename T, 
                  typename Fun1, typename Fun2>
    inline T 
    inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, 
                  T init, const Fun1& fun1,  const Fun2& fun2){
		    for(;first1 != last1; ++first1, ++first2) init = fun1(init, fun2(*first1, *first2));
        return init;
	  }

    
    /*****************************************************************************************/
    // partial_sum 
    // 计算局部总和，它会将 *first 赋值给 *result ，将 *first和 *(first + 1)的和赋值给 *( result + 1 ),
    // 以此类推，注意，result 可以等于 first，这使得我们得以完成就地计算。
    /*****************************************************************************************/
    template<typename InputIterator, typename OutputIterator>
	  OutputIterator 
    partial_sum(InputIterator first, InputIterator last, OutputIterator result){
        if(first != last){
            *result = *first;
            for(InputIterator prev = first; ++first != last; ++prev){
                *++result = *prev + *first ;
            }
            **++result = *first + *last;
        }
        return result ;
    }

    template<typename InputIterator, typename OutputIterator, typename binaryOp>
	  OutputIterator 
    partial_sum(InputIterator first, InputIterator last, OutputIterator result, const binaryOp& fun){
        if(first != last){
            *result = *first;
            for(InputIterator prev = first; ++first != last; ++prev){
                *++result = fun(*prev, *first) ;
            }
            **++result = fun(*first, *last);
        }
        return result ;
    }

    /*****************************************************************************************/
    // power 
    // 计算某数的 n 次幂, 快速幂的方法实现
    /*****************************************************************************************/
    template<class T, class Integer>
    inline T power(T x, Integer n){ // 接口函数
          return power(x, n, GHYSTL::multiplies<T>());
    }

    template<class T, class Integer, class MonoidOperation>
    T power(T x, Integer n, MonoidOperation op){
        if(n == 0)
            return GHYSTL::identity_element(op);
        else
            while ((n & 1) == 0) // n 是偶数，就直接把 x 相乘，n 除以 2, n 如果是奇数就跳过
            {
               n >>= 1;
               x = op(x, x);
            }

            T result = x;
            // 如果 n = 1， 那么说明上面的循环，已经计算处结果了， n再右移一位就是 0， 下面循环就不执行
            n >> = 1; 

            while (n)  // 等价于 n != 0， n 为奇数，上面的循环没有执行
            {
              x = op(x, x);
              if((n & 1) == 1) // 如果是奇数，说明乘到了最后，result * x 就是最终结果
                  result = op(result, x);
              n >>= 1;
            }
            
        return result;     
    }
}
#endif