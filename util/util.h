/*
* 工具类
* 包括 move, forward, swap 等函数，以及 pair 等
*/

#ifndef GHYSTL_UTIL_H_
#define GHYSTL_UTIL_H_
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
    };

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