/**
 * @file allocator.h
 * @author ghy (ghy_mike@163.com)
 * @brief  alloc(空间配置器)封装接口
 * @version 1.0
 * @date 2022-08-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef GHYSTL_ALLOCATOR_H_
#define GHYSTL_ALLOCATOR_H_

#include "alloc.h"
#include "../util/util.h"

namespace GHYSTL{
    template<class value_type_, class Alloc>
    class allocator_base{
    public:
        typedef value_type_                 value_type;
        typedef value_type*                 pointer;
        typedef const value_type*           const_pointer;
        typedef value_type&                 reference;
        typedef const value_type&           const_reference;
        typedef size_t                      size_type;
        typedef ptrdiff_t                   difference_type;
        typedef Alloc                       alloc;

        /*----------------------------------------------------- 空间分配和销毁 ---------------------------------------------------*/        

        inline static pointer allocate(){
            return static_cast<pointer>(alloc::allocate(sizeof(value_type)));
        }

        // 分配 n 个变量的内存，不是 n 个字节
        inline static pointer allocate(const size_type n){
            return (n ? static_cast<pointer>(alloc::allocate(sizeof(value_type) * n)) : nullptr);
        }

        inline static void deallocate(pointer ptr){
            if(ptr) alloc::deallocate(ptr, sizeof(value_type) * 1);
        }

        inline static void deallocate(pointer ptr, size_type n){
            if(ptr) alloc::deallocate(ptr, sizeof(value_type) * n);
        }

        // 最多可以存储多少个 该类型
        inline static size_type max_size(){
            return ((size_t)(-1) / sizeof(value_type));
        }

        /*--------------------------------------------------- 参数初始化构造器 -------------------------------------------------*/

        // 初始化多个参数
        template<typename... types>
        inline static void construct(pointer ptr, types&&...values){
            // new 关键字   的 placement new 用法，在开辟的内存上初始化
            // forward 完美转发，根据传进来的参数进行初始化
            // 如果是多个，会执行多次初始化
            // 可变参数展开，省略号要放在 括号外面展开， 递归除外
            new(ptr) value_type(std::forward<types>(values)...);
        }

        // 初始化为默认值
        inline static void construct(pointer ptr){
            new(ptr) value_type();
        }

        inline static pointer construct(pointer first, size_type n){
            // size_type 类型是 size_t，  是无符号的整形，不好小于 0
            for(; n !=0; --n, ++first) new(first) value_type();
            return (first);
        }

        inline static pointer construct(pointer first, pointer last){
            for(; first != last; ++first) new(first) value_type();
            return (first);
        }

        // 复制构造函数
        template<typename type>
        inline static void copy_construct(type* ptr, const type& val){
            copy_construct_imple(ptr, val, 
                            typename GHYSTL::type_traits<type>::has_trivial_copy_constructor());
        }

        inline static void copy_construct(pointer ptr, const value_type& val){
            copy_construct_imple(ptr, val, 
                            typename GHYSTL::type_traits<value_type>::has_trivial_copy_constructor());
        }

        // 移动构造函数
        inline static void copy_construct(pointer ptr, value_type&& val){ 
            new(ptr) value_type(std::move(val));
        }
        
        // 复制构造函数
        inline static pointer copy_construct(pointer first, pointer last, const value_type& val){
            for(; first != last; ++first) copy_construct(first, val);
            return first;
        }

        template<typename Iter>
        inline static pointer copy_construct(Iter first, Iter last, pointer dest){
            return copy_construct_imple(first, last, dest, 
                                        typename GHYSTL::type_traits<value_type>::has_trivial_copy_constructor());
        }

        template<typename Iter>
        inline static pointer copy_construct(Iter first, const size_type distance, pointer dest){
            return copy_construct_imple(first, distance, dest, 
                                        typename GHYSTL::type_traits<value_type>::has_trivial_copy_constructor());
        }

        inline static pointer copy_construct(pointer first, size_type distance, const value_type& val){
            for(; distance != 0; --distance, ++first) copy_construct(first, val);
            return first;
        }

        /*--------------------------------------------------- 析构函数 -------------------------------------------------*/

        template<typename type>
        inline static void destroy(type* ptr){
            destroy_impl(ptr,
                        typename GHYSTL::type_traits<value_type>::has_trivial_destructor());
        }

        inline static void destroy(pointer ptr){
            destroy_impl(ptr,
                        typename GHYSTL::type_traits<value_type>::has_trivial_destructor());
        }

        inline static void destroy(pointer first, pointer last){
            destroy_impl(first, last,
                    typename GHYSTL::type_traits<value_type>::has_trivial_destructor());
        }

    private:    
        /*-------------------------------------------------------基本的构造函数---------------------------------------------------------------------*/

        inline static void copy_construct_imple(pointer ptr, const value_type& val, GHYSTL::true_type){
            *ptr = val;
        }

        inline static void copy_construct_imple(pointer ptr, const value_type& val, GHYSTL::false_type){
            new(ptr) value_type(val);
        }

        template<typename type>
        inline static void copy_construct_imple(type* ptr, const type& val, GHYSTL::true_type){
            *ptr = val;
        }

        template<typename type>
        inline static void copy_construct_imple(type* ptr, const type& val, GHYSTL::false_type){
            new(ptr) type(val);
        }

        template<typename Iter>
        inline static pointer copy_construct_imple(Iter first, Iter last, pointer dest, GHYSTL::false_type){
            return copy_construct_imple_td(first, last, dest, 
                                    typename GHYSTL::iterator_traits<Iter>::iterator_category());// 声明成类型要加括号，默认初始化
        }

        template<typename Iter>
        inline static pointer copy_construct_imple_td(Iter first, Iter last, pointer dest, GHYSTL::input_iterator_tag){
            while(first != last) new(dest++) value_type(*first++);
            return dest;
        }

        template<typename Iter>
        inline static pointer copy_construct_imple_td(Iter first, Iter last, pointer dest, GHYSTL::random_access_iterator_tag){
            for(size_type distance = last - first; distance != 0; --distance, ++first, ++dest)
                new(dest) value_type(*first);
            return dest;
        }

        template<typename Iter>
        inline static pointer copy_construct_imple(Iter first, Iter last, pointer dest, GHYSTL::true_type){
            return copy_construct_imple_pod_d(first, last, dest, 
                                    typename GHYSTL::iterator_traits<Iter>::iterator_category());// 声明成类型要加括号，默认初始化
        }

        template<typename Iter>
        inline static pointer copy_construct_imple_pod_d(Iter first, Iter last, pointer dest, GHYSTL::input_iterator_tag){
            while(first != last) *dest++ = *first++;
            return dest;
        }

        template<typename Iter>
        inline static pointer copy_construct_imple_pod_d(Iter first, Iter last, pointer dest, GHYSTL::random_access_iterator_tag){
            // deque的空间可能不连续，这种情况不能直接 memcpy
            return copy_construct_imple_pod_d_m(first, last, dest,
                                                typename GHYSTL::is_mem_copy<Iter>());
        }

        template<typename Iter>
        inline static pointer copy_construct_imple_pod_d_m(Iter first, Iter last, pointer dest, GHYSTL::false_type){
            for(size_type distance = last - first; distance != 0; --distance, ++first, ++dest){
                *dest = *first;
            }
            return dest;
        }

        template<typename Iter>
        inline static pointer copy_construct_imple_pod_d_m(Iter first, Iter last, pointer dest, GHYSTL::true_type){
            const size_type distance = last - first;
            // first 是一个迭代器 要先获得迭代器里的指针，指针地址传过去
            GHYSTL::memcpy(dest, &*first, sizeof(value_type) * distance);
            return (dest + distance);
        }
        
        template<typename Iter>
        inline static pointer copy_construct_imple(Iter first, size_type distance, pointer dest, GHYSTL::false_type){
            for(; distance!=0; --distance, ++first, ++dest){
                new(dest) value_type(*first);
            }
            return dest;
        }

        template<typename Iter>
        inline static pointer copy_construct_imple(Iter first, const size_type distance, pointer dest, GHYSTL::true_type){
            return copy_construct_imple_memory(dest, first, distance, GHYSTL::is_mem_copy<Iter>());
        }

        template<typename Iter>
        inline static pointer copy_construct_imple_memory(pointer dest, Iter first, size_type distance, GHYSTL::true_type){
            GHYSTL::memcpy(dest, &*first, sizeof(value_type) * distance);
            return (dest + distance);
        }

        template<typename Iter>
        inline static pointer copy_construct_imple_memory(pointer dest, Iter first, size_type distance, GHYSTL::false_type){
            for(; distance!=0; --distance, ++first, ++dest){
                *dest = *first;
            }
            return dest;
        }

        /*----------------------------------------------------基本析构函数--------------------------------------------------*/

        static void destroy_impl(pointer ptr, GHYSTL::true_type){}
        static void destroy_impl(pointer ptr, GHYSTL::false_type){ ptr->~value_type();}

        template<typename type>
        inline static void destroy_impl(type* ptr, GHYSTL::true_type){}

        template<typename type>
        inline static void destroy_impl(type* ptr, GHYSTL::false_type){ ptr->~type();}

        inline static void destroy_impl(pointer first, pointer last, GHYSTL::true_type){}
        
        inline static void destroy_impl(pointer first, pointer last, GHYSTL::false_type){
            for(; first != last; ++first) first->~value_type();
        }
    };


    /*---------------------------------------------------- 对空间配置器的封装 --------------------------------------------------*/

    template<typename value_type_>
    class allocator 
                : public allocator_base<value_type_, default_alloc> // 绑定默认的二级配置器

    {
    public:
        typedef allocator_base<value_type_, default_alloc>               base_type;
        typedef typename base_type::value_type                           value_type;
        typedef typename base_type::pointer                              pointer;
        typedef typename base_type::const_pointer                        const_pointer;
        typedef typename base_type::reference                            reference;
        typedef typename base_type::const_reference                      const_reference;
        typedef typename base_type::size_type                            size_type;
        typedef typename base_type::difference_type                      difference_type;
        typedef typename base_type::alloc                                alloc;

        template<typename value_type>
        struct rebind
        {
            typedef allocator<value_type> other;
        }; 
    };


    template<typename value_type_>
    class simple_allocator 
                : public allocator_base<value_type_, malloc_alloc> // 绑定默认的一级配置器

    {
    public:
        typedef allocator_base<value_type_, default_alloc>               base_type;
        typedef typename base_type::value_typ                            value_type;
        typedef typename base_type::pointer                              pointer;
        typedef typename base_type::const_pointer                        const_pointer;
        typedef typename base_type::reference                            reference;
        typedef typename base_type::const_reference                      const_reference;
        typedef typename base_type::size_type                            size_type;
        typedef typename base_type::difference_type                      difference_type;
        typedef typename base_type::alloc                                alloc;

        template<typename value_type>
        struct rebind
        {
            typedef simple_allocator<value_type> other;
        }; 
    };


    template<typename value_type, typename Alloc = GHYSTL::allocator<value_type>>
    class temporary_buffer
    {
    public:
        typedef value_type*     pointer;
        typedef Alloc           alloc;

        temporary_buffer(size_t len) : len(len) { allocate_construct();}

        ~temporary_buffer(){
            alloc::destroy(first, first + len);
            alloc::deallocate(first, len);
        }

        size_t size() const { return (len); }

        pointer begin() { return (first); }

        pointer end() { return (first + len); }
    
    private:
        void allocate_construct(){
            while(len){
                try{
                    first = alloc::allocate(len);
                    alloc:construct(first, len);
                } catch(_BAD_ALLOC){
                    len >>= 1;
                }catch(...){
                    throw;
                }
            }
        }

    private:
        size_t len;
        pointer first;
    };
}
#endif