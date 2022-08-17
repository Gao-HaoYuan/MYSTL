/**
 * @file util.h
 * @author ghy (ghy_mike@163.com)
 * @brief  定义了复制函数、pair工具
 * @version 1.0
 * @date 2022-08-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once
#ifndef _UTIL_H_
#define _UTIL_H_
namespace GHYSTL
{
    template<class T1, class T2>
    struct pair
    {
        typedef T1 first_type;
        typedef T2 second_type;

        T1 first;
        T2 second;

        pair() : first(T1()), second(T2()){}
        pair(const T1& a, const T2& b) : first(a), second(b){}

        pair(const pair<T1, T2>& rhs) = default;
        pair(pair<T1, T2>&& rhs) = default;

        pair& operator=(const pair<T1, T2>& rhs) {
            if(this != &rhs){
                first = rhs.first;
                second = rhs.second;
            }
            
            return *this;
        }

        pair& operator=(pair<T1, T2>&& rhs) {
            if (this != &rhs){
                first = std::move(rhs.first);
                second = std::move(rhs.second);
            }

            return *this;
        }

        ~pair() = default;

        void swap(pair<T1, T2>& other) {
            if (this != &other) {
                std::swap(first, other.first);
                std::swap(second, other.second);
            }
        }

    };

    // 重载比较操作符 
    template <class Ty1, class Ty2>
    bool operator==(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    template <class Ty1, class Ty2>
    bool operator<(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
        return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
    }

    template <class Ty1, class Ty2>
    bool operator!=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
        return !(lhs == rhs);
    }

    template <class Ty1, class Ty2>
    bool operator>(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
        return rhs < lhs;
    }

    template <class Ty1, class Ty2>
    bool operator<=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
        return !(rhs < lhs);
    }

    template <class Ty1, class Ty2>
    bool operator>=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
        return !(lhs < rhs);
    }

    // 重载的 swap
    template <class Ty1, class Ty2>
    void swap(pair<Ty1, Ty2>& lhs, pair<Ty1, Ty2>& rhs) {
        lhs.swap(rhs);
    }

    // 全局函数，让两个数据成为一个 pair
    template <class Ty1, class Ty2>
    pair<Ty1, Ty2> make_pair(Ty1&& first, Ty2&& second) {
        return pair<Ty1, Ty2>(std::forward<Ty1>(first), std::forward<Ty2>(second));
    }

/*---------------------------------------------------- 工具函数 --------------------------------------------------*/

    // 交换函数
    template<class T>
    void swap(T& a, T& b){
        T c(a); a=b; b=c;
    }

    // 复制函数
    // __cdecl和__stdcall都是参数从右到左入栈
    // 参考 https://blog.csdn.net/W_Y2010/article/details/95480357
    void * __cdecl memmove(void *dst, const void *src, size_t count){
        void * ret = dst;

        // dst和src区域没有重叠
        if( dst <= src || (char*)dst >= ((char*)src + count)){
            while (count--)
            {
                *(char*)dst = *(char *)src;
                dst = (char*)dst + 1;
                src = (char*)src + 1;
            }
        }else{
            
            // dst和src区域重叠, 从尾部开始拷贝，避免数据冲突
            dst = (char*)dst + count - 1;
            src = (char*)src + count - 1;

            while (count--)
            {
                *(char*)dst = *(char *)src;
                dst = (char*)dst - 1;
                src = (char*)src - 1;
            }
        }

        // 返回 dst 的首地址
        return ret;
    }

    // 复制函数
    void * __cdecl memcpy(void *dst, const void *src, size_t count){
        void * ret = dst;
        //没有处理dst和src区域是否重叠的问题
        while (count--)
        {
            *(char*)dst = *(char *)src;
            dst = (char*)dst + 1;
            src = (char*)src + 1;
        }
        return ret;
    }   
    
} // namespace GHYSTL
#endif