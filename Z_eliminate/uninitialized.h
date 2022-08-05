/*
*  容器初始化
*/

#ifndef GHYSTL_UNINITIALIZED_H
#define GHYSTL_UNINITIALIZED_H

#include "construct.h"
#include "algo_base.h"

namespace GHYSTL{

    /*
	*  未初始化的拷贝,在已获得的内存上创建一些元素
	*/

    // POD 版本的未初始化拷贝
    template<class InputIterator, class ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
                                            ForwardIterator result, GHYSTL::true_type){
        //从源src所指的内存地址的起始位置开始拷贝n个字节到目标dest所指的内存地址的起始位置中
        std::memcpy(result, first, (last - first)*sizeof(*first));
        return result + (last - first);
    }


    // 非 POD 版本的未初始化拷贝
    template<class InputIterator, class ForwardIterator>
    ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
                                            ForwardIterator result, GHYSTL::false_type){
        int i=0;
        for(;first != last; ++i){
            GHYSTL::construct(result + i, *first);
        }
        return (result + i);
    }

    template<class InputIterator, class ForwardIterator, class T>
    ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last,
                                            ForwardIterator result, T){
        typedef typename GHYSTL::_type_traits<T>::is_POD_type is_POD;
        return _uninitialized_copy_aux(first, last, result, is_POD);
    }

    template<class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
                                            ForwardIterator result){
        // 萃取迭代器类型
        return _uninitialized_copy(first, last, result, GHYSTL::value_type(result));
    }



    /*
	*  未初始化的拷贝，以某一特定值初始化
	*/
    template<class ForwardIterator, class T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                const T& value, GHYSTL::true_type){
        GHYSTL::fill(first, last, value);
    }

    template<class ForwardIterator, class T>
    void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
                                const T& value, GHYSTL::false_type){
        for(;first != last; ++first){
            GHYSTL::construct(first,value);
        }
    }
    
    template<class ForwardIterator, class T>
    void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value){
        typedef typename GHYSTL::_type_traits<T>::is_POD_type is_POD;
        _uninitialized_fill_aux(first, last, value, is_POD());
    }

    /*
	*  未初始化的拷贝，以某一特定数量n初始化
	*/
    template<class ForwardIterator, class size, class T>
    ForwardIterator uninitialized_fill_n_aux(ForwardIterator first, size n, cosnt T& x, GHYSTL::true_type){
        return GHYSTL::fill_n(first, n, x);
    }

    template<class ForwardIterator, class size, class T>
    ForwardIterator uninitialized_fill_n_aux(ForwardIterator first, size n, cosnt T& x, GHYSTL::false_type){
        for(int i = 0;i != n; ++i,++first){
            GHYSTL::construct((T*)first, x);
        }
        return first;
    }

    template<class ForwardIterator, class size, class T>
    ForwardIterator uninitialized_fill_n(ForwardIterator first, size n, cosnt T& x){
        typedef typename GHYSTL::_type_traits<T>::is_POD_type is_POD;
        return _uninitialized_fill_n_aux(first, n, x, is_POD());
    }
}
#endif