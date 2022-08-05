/**
 * @file alloc.h
 * @author ghy (ghy_mike@163.com)
 * @brief  空间配置器的实现代码
 *         添加了一级空间配置器
 * @version 1.0
 * @date 2022-08-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _ALLOC_
#define _ALLOC_

#include <cstdlib>
#include <cstring>
#include <new>

#include "../excetdef.h"
#include "../Iterator/iterator.h"

namespace GHYSTL{

    /*-------------------------------------- 第一级配置器-------------------------------------------*/
    // 一级配置器是对堆做处理的，不需要线程控制
    template<int inst>
    class malloc_alloc_template
    {
    public:
        static void* allocate(size_t n){
            void* result = std::malloc(n);

            //分配空间失败，就通过 omm_malloc 的方式分配空间
            //会释放掉空间进程占用的空间
            if(result == nullptr) result = oom_malloc(n);

            return (result);
        }

        static void deallocate(void* p, size_t){ std::free(p);};

        static void* reallocate(void* p, size_t, size_t new_size){
            void* result = std::realloc(p, new_size);

            if(result == nullptr) result = oom_realloc(p, new_size);
            return (result);
        }

        // 等价于 (*set_malloc_handler(void (*f)))
        // 等价于 (*set_malloc_handler(void (*f)))()
        // C++ 编译器会自动去掉没有用的括号
        static void *set_malloc_handler(void (*f)()){
            void (*old)() = malloc_alloc_oom_handler;
            malloc_alloc_oom_handler = f;
            return (old);
        }

    private:
        static void* oom_malloc(size_t);
        static void* oom_realloc(void*, size_t);
        static void (*malloc_alloc_oom_handler)();
    };
    
    template<int inst>
    void (*malloc_alloc_template<inst>::malloc_alloc_oom_handler)() = nullptr;

    template<int inst>
    void* malloc_alloc_template<inst>::oom_malloc(size_t n){
        void (*my_malloc_handler)();
        void* result;
        for(;;){
            my_malloc_handler = malloc_alloc_oom_handler;
            if(my_malloc_handler == nullptr) THROW_BAD_ALLOC;
            (*my_malloc_handler)();
            result = std::malloc(n);
            if(result) return (result); 
        } 
    }

    template<int inst>
    void* malloc_alloc_template<inst>::oom_realloc(void* p, size_t n){
        void (*my_malloc_handler)();
        void* result;
        for(;;){
            my_malloc_handler = malloc_alloc_oom_handler;
            if(my_malloc_handler == nullptr) THROW_BAD_ALLOC;
            (*my_malloc_handler)();
            result = std::realloc(p, n);
            if(result) return (result);
        }
    }
    
    //相当于 宏定义
    enum {_ALIGN = 8}; // 自由链表节点个数的上调边界
    enum {_MAX_BYTES = 128}; // 自由链表的最大上限
    enum {_NFREELIST = _MAX_BYTES / _ALIGN}; // 自由链表的个数
    enum {_NOBJS = 20}; // 每次扩充分配节点个数


    /*---------------------------------------------- 第二级空间配置器 ------------------------------------------------*/
    // 二级配置器是要声明线程的开关，因为有自由链表的存在
    template<bool threads, int inst>
    class default_alloc_template
    {
    private:

        union obj 
        {
            // 自由链表节点
            union obj* free_list_next; // 指向下一个区块
            char client[1]; // 储存本块内存的首地址
        };

        // volatile: 每次都从内存中取值；编译器不可以（合并、消除）优化；保证volatile变量之间的顺序性
        // 16个自由链表维护8、16、24、.....128的内存区域 
        static obj* volatile free_list[_NFREELIST];

        static char *start_free; // 内存池的起始地址
        static char *end_free;;  // 内存池的结束地址
        static size_t heap_size; // 申请堆空间附加值大小


        //根据申请的块的大小，获取 自由链表 编号
        static size_t FREELIST_INDEX(size_t bytes){
            return ((bytes + _ALIGN - 1)/ _ALIGN - 1);
        }

        // 根据 bytes,上调到8的倍数
        static size_t ROUND_UP(size_t bytes){
            return ((bytes + _ALIGN -1) & ~(_ALIGN - 1));
        }

        // 返回一个大小为n的对象，并可能加入大小为 n 的其他区块到 自由链表中
        static void *refill(size_t bytes);

        // 从内存池中取空间给 free list 使用，条件不允许时，会调整 nblock
        // 配置一大块空间，可容纳 _NOBJS 个大小为size的块
        static char *chunk_alloc(size_t bytes, size_t& nobjs);
    
    public:
        // 分配大小为 bytes 的空间， n > 0
       static void *allocate(size_t bytes);
        //  释放 p 指向的大小为 bytes 的空间, p 不能为 0
       static void deallocate(void *ptr, size_t bytes);
        // 重新分配空间，接受三个参数，参数一为指向新空间的指针，参数二为原来空间的大小，参数三为申请空间的大小
       static void *reallocate(void *ptr, size_t old_size, size_t new_size);
    };


    // 默认为 0 号 一级配置器
    typedef malloc_alloc_template<0> malloc_alloc;

    // 默认为 0 号 二级配置器，多线程开启
    typedef default_alloc_template<true, 0> default_alloc;
}
#endif