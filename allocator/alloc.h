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
#include <iostream>

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
        
        // 这是一个函数
        // 返回值类型 一个函数指针
        // (*p)() 是一个函数指针的写法， 这里 p 就是 set_malloc_handler(void (*f)())
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

    // 默认为 0 号 一级配置器
    typedef malloc_alloc_template<0> malloc_alloc;
    
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
    
    //参数初始化 和 函数定义
    template<bool threads, int inst>
    char* default_alloc_template<threads, inst>::start_free = nullptr;

    template<bool threads, int inst>
    char* default_alloc_template<threads, inst>::end_free = nullptr;

    template<bool threads, int inst>
    size_t default_alloc_template<threads, inst>::heap_size = 0;

    template<bool threads, int inst>
    typename default_alloc_template<threads, inst>::
    obj* volatile default_alloc_template<threads, inst>::free_list[_NFREELIST] ={
        nullptr, nullptr, nullptr, nullptr, 
        nullptr, nullptr, nullptr, nullptr, 
        nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr
    };

    template<bool threads, int inst>
    void* default_alloc_template<threads, inst>::allocate(size_t bytes){
        if(bytes > (size_t)_MAX_BYTES){
            return (malloc_alloc::allocate(bytes));
        }

        // 选择 自由链表 编号
        obj* volatile *my_list = free_list + FREELIST_INDEX(bytes);
        obj* result = *my_list;

        // 没有可用的 自由链表， 重新填充 自由链表
        if(result == nullptr){
            void *r = refill(ROUND_UP(bytes));
            return r;
        }

        //调整freelist, 将list后面的空间前移，返回list所指的空间
        *my_list = result->free_list_next;
        return result; // 返回分配的空间的地址
    }

    template<bool threads, int inst>
    void default_alloc_template<threads, inst>::deallocate(void *ptr, size_t bytes){
        if(bytes > (size_t)_MAX_BYTES){
            malloc_alloc::deallocate(ptr, bytes);
            return;
        }

        obj * node = static_cast<obj*>(ptr);
        obj* volatile *my_list = free_list + FREELIST_INDEX(bytes);
        node->free_list_next = *my_list;
        *my_list = node;
    }

    // 重新分配空间
    template<bool threads, int inst>
    void* default_alloc_template<threads, inst>::
    reallocate(void *ptr, size_t old_size, size_t new_size){
        deallocate(ptr, old_size);
        ptr = allocate(new_size);

        return ptr;
    }

    //返回一个大小为n的对象，并且有时候会为适当的freelist增加节点
    //假设bytes已经上调为8的倍数
    template<bool threads, int inst>
    void * default_alloc_template<threads, inst>::refill(size_t bytes){
        // 记录获得的区块数量
        size_t nobjs = _NOBJS;

        // 从内存池中取 nobjs 个区块作为 自由链表 的新节点
        char *chunk = chunk_alloc(bytes, nobjs);

        // 如果只有一个区块，就把这个区块返回给调用者，free list 没有增加新节点
        if(nobjs == 1){
            return chunk;
        }

        // 否则把一个区块给调用者，剩下的纳入 free list 作为新节点
        obj* volatile *my_list = free_list + FREELIST_INDEX(bytes);
        obj *result = (obj*)(chunk);

        obj *current_obj , *next_obj;

        // 剩下的空间分离出来
        *my_list = next_obj = (obj*)(chunk + bytes);

        for(size_t i=1;;++i){
            current_obj = next_obj;
            if( nobjs -1 == i){
                current_obj->free_list_next = nullptr;
                break;
            }
            next_obj = (obj*)((char*)next_obj + bytes);
            current_obj->free_list_next = next_obj;
        }

        return result;
    }

    //内存池(一大块空闲的空间) bytes已经上调为8的倍数
    template <bool threads, int inst>
    char* default_alloc_template<threads, inst>::chunk_alloc(size_t bytes, size_t& nobjs){
        char* result;
        size_t bytes_need = bytes * nobjs;
        size_t bytes_left = end_free - start_free;

        if(bytes_left >= bytes_need){
            result = start_free;
            start_free += bytes_need;
            return result;
        } else if(bytes_left >= bytes){ // 提供不了 nobjs 个块的内存
            nobjs = bytes_left / bytes;
            bytes_need = nobjs * bytes;
            result = start_free;
            start_free = start_free + bytes_need;
            return result; 
        }else{ // 一个 bytes 大小的内存也提供不了
            // 每次申请两倍的内存 从堆中
            size_t bytes_to_get = 2 * bytes_need + ROUND_UP(heap_size >> 4);

            // 如果内存池还有剩余，把剩余的空间加入到 free list 中
            // 注意此时 bytes_left 小于 bytes， 所以会把剩余的内存池存到 前面的链表中         
            if(bytes_left > 0){
                obj* volatile *my_list = free_list + FREELIST_INDEX(bytes_left);
                ((obj*)start_free)->free_list_next = *my_list;
                *my_list = (obj*)start_free;
            }

            start_free = (char*)malloc(bytes_to_get);

            if(nullptr == start_free){//堆的内存不足
                obj* volatile *my_list, *p;

                // 在自由链表中寻找空间, 且区块足够大的 free list
               for(size_t i = bytes; i <= _MAX_BYTES; i += _ALIGN){
                   my_list = free_list + FREELIST_INDEX(i);
                   p = *my_list;
                   if(p){
                       *my_list = p->free_list_next;
                       start_free = (char*)p;
                       end_free = start_free + i;
                       return chunk_alloc(bytes, nobjs);
                   }
               }

               end_free = nullptr; //没有内存可用
               start_free = (char*)malloc_alloc::allocate(bytes_to_get); 
            }

            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(bytes, nobjs);
        }
    }

    // 默认为 0 号 二级配置器，多线程开启
    typedef default_alloc_template<true, 0> default_alloc;

}// end of namesapce
#endif
