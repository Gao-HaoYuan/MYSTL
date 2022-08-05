#include "alloc.h"

namespace GHYSTL{
    //参数初始化 和 函数定义
    template<bool threads, int inst>
    char* default_alloc_template<threads, inst>::start_free = nullptr;

    template<bool threads, int inst>
    char* default_alloc_template<threads, inst>::end_free = nullptr;

    template<bool threads, int inst>
    size_t default_alloc_template<threads, inst>::heap_size = 0;

    template<bool threads, int inst>
    default_alloc_template<threads, inst>::
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

        // 没有可用的 自由链表， 重新填充 自由链表
        if(my_list == nullptr){
            void *r = refill(ROUND_UP(bytes));
            return r;
        }

        //调整freelist, 将list后面的空间前移，返回list所指的空间
        my_list = my_list -> free_list_next;
        return my_list;
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
            next_obj = (obj*)((char*)next_obj+bytes);
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
            start_free = start_free + bytes_need;
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
}// end of namesapce