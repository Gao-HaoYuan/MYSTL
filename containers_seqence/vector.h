/**
 * @file vector.h
 * @author ghy (ghy_mike@163.com)
 * @brief vector 实现
 *         待添加：错误检查、迭代器没有提供返回变量的函数 等
 *          
 * 
 * @version 1.0
 * @date 2022-08-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "../algorithm/algorithm.h"

namespace GHYSTL{

    // --------------- vector的 const_iterator ---------------------------------

    template<typename value_type_>
    class vector_const_iterator : public GHYSTL::iterator_base<random_access_iterator_tag, value_type_>
    {
    public:
        typedef GHYSTL::random_access_iterator_tag      iterator_category;
        typedef value_type_                             value_type;
        typedef const value_type*                       pointer;
        typedef const value_type&                       reference;
        typedef ptrdiff_t                               difference_type;

        typedef GHYSTL::true_type                   memory_copy_tag;
        typedef vector_const_iterator<value_type>   self;

        vector_const_iterator(value_type *cur = nullptr) : cur(cur){ }

        vector_const_iterator(const self &x) : cur(x.cur){ }// 拷贝构造函数

        self& operator=(const self &x){ //赋值运算符
            cur = x.cur;
            return (*this);
        }

        bool operator==(const self &x) const { return (cur == x.cur);}

        bool operator!=(const self &x) const { return (!(operator==(x)));}

        bool operator<(const self &x) const { return (cur < x.cur);}

        bool operator<=(const self &x) const { return (cur <= x.cur);}

        bool operator>(const self &x) const { return (cur > x.cur);}

        bool operator>=(const self &x) const { return (cur >= x.cur);}

        reference operator*() const { return (*cur);}

        pointer operator->() const { return (&(operator*()));}

        reference operator[](difference_type off) const { return (*(cur + off));}

        self &operator++(){
            ++cur;
            return (*this);
        }

        self operator++(int){
            auto ret = *this;
            operator++();
            return (ret);
        }

        self &operator--(){
            --cur;
            return (*this);
        }

        self operator--(int){
            auto ret = *this;
            operator--();
            return (ret);
        }

        self operator+(const difference_type n) const {return self(cur + n);}

        self &operator+=(const difference_type n){
            cur += n;
            return (*this);
        }

        self operator-(const difference_type n) const {return self(cur - n);}

        difference_type operator-(const self& x) const {return (cur - x.cur);}

        self &operator-=(const difference_type n){
            cur -= n;
            return (*this);
        }

    protected:
        value_type *cur;
    };
    
   
    // --------------- vector的 iterator ---------------------------------

    template<class value_type_>
    class vector_iterator : public vector_const_iterator<value_type_>{
    public:
        typedef GHYSTL::random_access_iterator_tag      iterator_category;
        typedef value_type_                             value_type;
        typedef value_type*                             pointer;
        typedef value_type&                             reference;
        typedef ptrdiff_t                               difference_type;
        
        typedef GHYSTL::true_type                       memory_copy_tag;
        typedef vector_iterator<value_type>             self;

        typedef vector_const_iterator<value_type>       base_type;

        // 通过 using 调用父类成员变量
        // 基类迭代器 没有提供 get_cur 的函数
        // 通过 using 改变类变量 cur 的访问权限， 可以在类外部对 cur 操作
        using base_type::cur;

        vector_iterator(const pointer cur = nullptr) : base_type(cur){}

        vector_iterator(const self &x) : base_type(x.cur){}

        self& operator=(const self &x){
            cur = x.cur;
            return (*this);
        }

        bool operator==(const self &x) const { return (cur == x.cur);}

        bool operator!=(const self &x) const { return (!(operator==(x)));}

        bool operator<(const self &x) const { return (cur < x.cur);}

        bool operator<=(const self &x) const { return (cur <= x.cur);}

        bool operator>(const self &x) const { return (cur > x.cur);}

        bool operator>=(const self &x) const { return (cur >= x.cur);}

        reference operator*() { return (*cur);} // 加 const 会变成只读的函数，不能对 成员变量 进行修改

        pointer operator->()  { return (&(operator*()));}

        reference operator[](difference_type off) const { return (*(cur + off));}

        self &operator++(){
            ++cur;
            return (*this);
        }

        self operator++(int){
            auto ret = *this;
            operator++();
            return (ret);
        }

        self &operator--(){
            --cur;
            return (*this);
        }

        self operator--(int){
            auto ret = *this;
            operator--();
            return (ret);
        }

        self operator+(const difference_type n) const {return self(cur + n);}

        self &operator+=(const difference_type n){
            cur += n;
            return (*this);
        }

        self operator-(const difference_type n) const {return self(cur - n);}

        difference_type operator-(const self& x) const {return (cur - x.cur);}

        self &operator-=(const difference_type n){
            cur -= n;
            return (*this);
        }
    };

    // --------------- vector ---------------------------------

    template<class T, class Alloc = GHYSTL::allocator<T>>
    class vector
    {
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

        typedef vector_iterator<value_type>                 iterator;
        typedef vector_const_iterator<value_type>           const_iterator;
        typedef GHYSTL::reverse_iterator<iterator>          reverse_iterator;
        typedef GHYSTL::reverse_iterator<const_iterator>    const_reverse_iterator;

    private:
        typedef vector<value_type, Alloc>   self;
        typedef Alloc                       allocator_type;
        typedef Alloc                       alloc;

        pointer first;     //目前使用空间头
        pointer last;    //目前使用空间尾
        pointer end_storage; //目前可用空间的尾
    
    public:
        /*------------------------ ---------------- 构造函数  -------------------------------------------*/

        vector():first(nullptr), last(nullptr), end_storage(nullptr){};

        // 调用默认构造函数，对 first， last， end_storage初始化 
        explicit vector(const size_type n) : vector() { 
            first = alloc::allocate(n);
            alloc::construct(first, n);
            last = end_storage = first + n;
        }

        vector(const size_type n, const value_type &val) : vector() { 
            // 用 n 个 val 初始化
            first = alloc::allocate(n);
            end_storage = last = alloc::copy_construct(first, n, val);
        }


        vector(const std::initializer_list<T> &lst) : vector(lst.begin(), lst.end()){
            // 使用初始化列表 对 vector 进行初始化
        }
        
        // 如果不是迭代器，就没有 type 属性，这个 构造函数 就会实例化失败
        // 而且也没有其他匹配的模板，所以这个函数原型就不会产生
        // 上面这个函数调用它的时候，就会链接失败（编译期间不会报错）
        template<typename Iter, 
                typename = typename GHYSTL::enable_if<is_iterator<Iter>::value, void>::type>
        vector(Iter bg, Iter ed) : vector() {
            // 为迭代器分配元素
            assign_imple(bg, ed, GHYSTL::iterator_category(bg));
        }

        vector(const self& x){ // 拷贝构造函数
            first = alloc::allocate(x.size());
            last = alloc::copy_construct(x.first, x.last, first);
            end_storage = first + x.size();
        }

        vector(self&& x) noexcept : first(x.first), last(x.last), end_storage(x.end_storage){// 移动构造函数
            x.first = x.last = x.end_storage = nullptr;
        }

        self& operator=(const self& x); // 赋值运算符
        self& operator=(self&& x); // 移动运算符

        self& operator=(const std::initializer_list<value_type>& lst){
            assign(lst.begin(), lst.end());
            return *this;
        }

        ~vector(){
            alloc::destroy(first, last);
            alloc::deallocate(first);
        }

        /*------------------------ ---------------- 常规函数  -------------------------------------------*/
        size_type capacity() const noexcept { return (end_storage - first); }

        bool empty() const noexcept { return first == last; }

        reference operator[](const size_type n){
            GHYSTL_DEBUG(n < size()); 
            return *(first + n); 
        }

        const_reference operator[](const size_type n) const { 
            GHYSTL_DEBUG(n < size()); 
            return *(first + n); 
        }

        reference front(){
             GHYSTL_DEBUG(!empty());
            return (*first); 
        }

        const_reference front() const{
             GHYSTL_DEBUG(!empty());
            return (*first); 
        }

        reference back(){
            GHYSTL_DEBUG(!empty());
            return *(last - 1);
        }

        const_reference back() const{
             GHYSTL_DEBUG(!empty());
            return *(last - 1); 
        }

        void shrink_to_fit(){
            if(end_storage != last && !empty()){
                size_t len = this->size();
                pointer ptr = alloc::allocate(len);
                pointer new_last = alloc::copy_construct(first, last, ptr);
                deallocate_and_update_ptr(ptr, new_last, len);
            }
        }

        // 与另一个 vector 交换
        void swap(self& x) noexcept {
            std::swap(first, x.first);
            std::swap(last, x.last);
            std::swap(end_storage, x.end_storage);
        }

        allocator_type get_allocator() const {
            return allocator_type();
        }

        void reserve(const size_type n){ // 扩容
            if(n > capacity()){
                pointer ptr = alloc::allocate(n);
                pointer new_last = alloc::copy_construct(first, last, ptr);
                deallocate_and_update_ptr(ptr, new_last, n);
            }
        }
    
    public:
    /*------------------------ ---------------- 迭代器操作函数 -------------------------------------------*/
        iterator begin() { return iterator(first); }
        iterator end() { return iterator(last); }

        const_iterator begin() const { return const_iterator(first); }
        const_iterator end() const { return const_iterator(last); }

        // 不管是对象本身是不是常量，换回值都是 const_iterator
        const_iterator cbegin() const { return begin(); }
        const_iterator cend() const { return end(); }


        reverse_iterator rbegin() { return reverse_iterator(end()); }
        reverse_iterator rend() { return reverse_iterator(begin()); }

        const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

        const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
        const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

        size_type size() const {
            return (last - first);
        }

        reference at(const size_type n) noexcept {
            THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
            return *(first + n);
        }

        const_reference at(const size_type n) const noexcept {
            THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
            return *(first + n);
        }
        
        pointer data() noexcept { return (first); }
        const_pointer data() const noexcept { return (first); }

        iterator erase(iterator pos){
            GHYSTL_DEBUG(pos >= begin() && pos < end());

            GHYSTL::copy(pos + 1, end(), pos);
            --last;
            alloc::destroy(last);
            return pos;
        }

        iterator erase(iterator bg, iterator ed){
            GHYSTL_DEBUG(bg >= begin() && ed <= end() && !(ed < bg));

            iterator iter = GHYSTL::copy(ed, end(), bg);
            alloc::destroy(first + (iter - begin()), last);
            last = first + (iter  - begin());
            return bg;
        }

        void clear(){
            alloc::destroy(first, last);
            last = first;
        }
        
        void assign(const size_type n, const value_type& val){
            clear();
            insert(begin(), n, val);
        }

        template<typename Iter>
        typename enable_if<is_iterator<Iter>::value, void>::type assign(Iter bg, Iter ed){
            clear();
            assign_imple(bg, ed, typename GHYSTL::iterator_traits<Iter>::iterator_category());
        }

        void assign(const std::initializer_list<value_type>& lst){
            assign(lst.begin(), lst.end());
        }

        size_type max_size() const {
            return alloc::max_size();
        }

        void resize(const size_type n){
            if(n <= size()){
                alloc::destroy(first + n, last);
                last = first + n;
            }else if(n <= capacity()){
                alloc::construct(last, first + n);
                last = last + n;
            }else{
                pointer ptr = alloc::allocate(n);
                alloc::copy_construct(first, last, ptr);
                alloc::construct(ptr + size(), ptr + n);
                deallocate_and_update_ptr(ptr, ptr + n, n);
            }
        }
        
        void resize(const size_type n, const value_type& val){
            if(n <= size()){
                alloc::destroy(first + n, last);
                last = first + n;
            }else if(n <= capacity()){
                alloc::copy_construct(last, first + n, val);
                last = first + n;
            }else{
                pointer ptr = alloc::allocate(n);
                alloc::copy_construct(first, last, ptr);
                alloc::construct(ptr + size(), ptr + n, val);
                deallocate_and_update_ptr(ptr, ptr + n, val);
            }
        }

        iterator insert(iterator pos, value_type&& val){
            return emplace(pos, std::move(val));
        }
        
        iterator insert(iterator pos, const size_t n, const value_type& val);
        
        iterator insert(iterator pos, const value_type& val){
            return (insert(pos, 1, val));
        }

        template<typename Iter>
        typename enable_if<is_iterator<Iter>::value, iterator>::type insert(Iter pos, Iter bg, Iter ed){
            return insert_imple(pos, bg, ed, 
                                typename GHYSTL::iterator_traits<Iter>::iterator_category());
        }

        iterator insert(iterator pos, const std::initializer_list<value_type>& lst){
            return insert(pos, lst.begin(), lst.end());
        }

        void push_back(const value_type& val){
            if(last == end_storage){
                const size_type len = size() ? 2 * size() : 1;
                auto ptr = alloc::allocate(len);
                auto new_last = alloc::copy_construct(first, last, ptr);
                deallocate_and_update_ptr(ptr, new_last, len);
            }
            alloc::copy_construct(last++, val);
        }

        void push_back(value_type && val){
            emplace_back(std::forward<value_type>(val));
        }

        void pop_back() { alloc::destroy(--last); }

        template<typename ...types>
        void emplace_back(types && ... args){
            if(last == end_storage){
                const size_type len = size() ? 2 * size() : 1;
                auto ptr = alloc::allocate(len);
                auto new_last = alloc::copy_construct(first, last, ptr);
                deallocate_and_update_ptr(ptr, new_last, len);
            }
            alloc::construct(last++, std::forward<types>(args)...);
        }

        template<typename ... types>
        iterator emplace(iterator pos, types && ... args){
            size_type off = pos - begin();
            iterator middle = --end();
            emplace_back(std::forward<types>(args)...);
            GHYSTL::rotate(begin() + off, middle, end());
            return (begin() + off);
        }


    private:
        template<typename Iter>
        void assign_imple(Iter bg, Iter ed, GHYSTL::input_iterator_tag){
            for(; bg != ed; ++bg)
                push_back(*bg);
        }

        template<typename Iter>
        void assign_imple(Iter bg, Iter ed, GHYSTL::forward_iterator_tag){
            const size_t len = GHYSTL::distance(bg, ed);
            if(capacity() < len){
                alloc::deallocate(first, capacity());
                first = alloc::allocate(len);
                end_storage = first + len;
            }

            last = alloc::copy_construct(bg, ed, first);
        }

        template<typename Iter>
        iterator insert_imple(iterator pos, Iter bg, Iter ed, GHYSTL::input_iterator_tag){
            const size_type off = pos - begin();
            const size_type old_size = size();
            for(; bg != ed; ++bg)
                push_back(*bg);
            GHYSTL::rotate(begin() + off, begin() + old_size, end());
            return (begin() + off);
        }

        template<typename Iter>
        iterator inset_imple(iterator pos, Iter bg, Iter ed, GHYSTL::forward_iterator_tag){
            const size_type off = capacity();
            const size_type count = GHYSTL::distance(bg, ed);

            if(count <= static_cast<size_type>(capacity() - last())){
                GHYSTL::copy_backward(pos, end(), last + count);
                last += count;
                GHYSTL::copy(bg, ed, pos);
            }else{
                const size_type old_size = capacity();
                const size_type len = old_size + (old_size > count ? old_size : count);
                pointer ptr = alloc::allocate(len);
                pointer new_last = alloc::copy_construct(first, pos, ptr);
                new_last = alloc::copy_construct(bg, ed, new_last);
                new_last = alloc::copy_construct(first + off, last, new_last);
                deallocate_and_update_ptr(ptr, new_last, len);
            }
            return (begin() + off);
        }

        void deallocate_and_update_ptr(pointer new_first, pointer new_last, const size_type n){
            alloc::destroy(first, last);
            alloc::deallocate(first, capacity());

            first = new_first;
            last = new_last;
            end_storage = first + n;
        }
    };

    template<class T, class Alloc>
    typename vector<T, Alloc>::self& vector<T, Alloc>::operator=(const self& rhs){
        if (this != &rhs){
            const auto len = rhs.size();
            if (len > capacity()){ 
                vector tmp(rhs.begin(), rhs.end());
                swap(tmp);
            }
            else if (size() >= len){
                auto i = GHYSTL::copy(rhs.begin(), rhs.end(), begin());
                alloc::destroy(first + len, last);
                last = first + len;
            }
            else{ 
                GHYSTL::copy(rhs.begin(), rhs.begin() + size(), first);
                alloc::copy_construct(rhs.begin() + size(), rhs.end(), last);
                last = first + len;
            }
        }
        return *this;
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::self& vector<T, Alloc>::operator=(self&& x){
        deallocate_and_update_ptr(x.first, x.last, x.capacity());
        x.first = nullptr;
        x.last = nullptr;
        x.end_storage = nullptr;
        return *this;
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator pos, const size_t n, const T& x){
        if(!n) return pos; // n == 0

        difference_type off = pos - begin();
        
        if(static_cast<size_t>(end_storage - last) >= n){
            const size_type after_ele = end() - pos;
            if(n < after_ele){
                alloc::copy_construct(end()- n, end(), last);
                GHYSTL::copy_backward(pos, end() - n, last);
                GHYSTL::fill_n(pos, n, x);
            }else{
                alloc::copy_consruct(end(), n - after_ele, x);
                alloc::copy_consruct(pos, end(), last + (n - after_ele));
                GHYSTL::fill_n(pos, n, x);
            }
            last += n;
        }else{
            const size_type old_size = capacity();
            const size_type len = old_size + (old_size > n ? old_size : n);
            auto ptr = alloc::allocate(len);
            pointer new_last = alloc::copy_construct(first, pos, ptr);
            new_last = alloc::copy_construct(new_last, n, x);
            new_last = alloc::copy_cosntruct(first + off, end(), new_last);
            deallocate_and_update_ptr(ptr, new_last, len);
        }
        
        return (begin() + off);
    }
    
    template<typename value_type, typename alloc>
    inline void swap(GHYSTL::vector<value_type>& left, GHYSTL::vector<value_type>& right) noexcept {
        left.swap(right);
    }

    template<typename value_type, typename alloc>
    inline bool operator==(GHYSTL::vector<value_type>& left, GHYSTL::vector<value_type>& right){
        return (left.size() == right.size() && 
                GHYSTL::equal(left.begin(), left.end(), right.begin(), right.end()));
    }

    template<typename value_type, typename alloc>
    inline bool operator!=(GHYSTL::vector<value_type>& left, GHYSTL::vector<value_type>& right){
        return !(left == right);
    }

    template<typename value_type, typename alloc>
    inline bool operator<(GHYSTL::vector<value_type>& left, GHYSTL::vector<value_type>& right){
        return GHYSTL::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end());
    }

    template<typename value_type, typename alloc>
    inline bool operator<=(GHYSTL::vector<value_type>& left, GHYSTL::vector<value_type>& right){
        return !(right < left);
    }

    template<typename value_type, typename alloc>
    inline bool operator>(GHYSTL::vector<value_type>& left, GHYSTL::vector<value_type>& right){
        return right < left;
    }

    template<typename value_type, typename alloc>
    inline bool operator>=(GHYSTL::vector<value_type>& left, GHYSTL::vector<value_type>& right){
        return !(left < right);
    }
}
#endif