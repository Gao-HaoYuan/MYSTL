/**
 * @file deque.h
 * @author ghy (ghy_mike@163.com)
 * @brief   deque 实现
 * 
 *  
 * @version 1.0
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _DEQUE_H_
#define _DEQUE_H_

#ifndef DEQUE_MAP_INIT_SIZE
#define DEQUE_MAP_INIT_SIZE 8
#endif

#include "../algorithm/algorithm.h"
#include "vector.h"

namespace GHYSTL{

template <class T>
struct deque_buf_size
{
    static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
};

template<typename value_type_>
class deque_const_iterator : public GHYSTL::iterator_base<random_access_iterator_tag, value_type_>
{
public:
    typedef GHYSTL::random_access_iterator_tag      iterator_category;
    typedef value_type_                             value_type;
    typedef const value_type*                       pointer;
    typedef const value_type&                       reference;
    typedef size_t                                  size_type;
    typedef ptrdiff_t                               difference_type;

    typedef const pointer*                          map_pointer;
    typedef deque_const_iterator<value_type>        self;

    // 迭代器所含成员数据
    pointer         cur;    // 指向所在缓冲区的当前元素
    pointer         first;  // 指向所在缓冲区的头部
    pointer         last;   // 指向所在缓冲区的尾部
    map_pointer     node;   // 缓冲区所在节点

    static const size_type buffer_size = deque_buf_size<value_type>::value;

    /*-------------------------------------- 构造函数 ---------------------------------------------*/

    deque_const_iterator() : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {} //默认构造函数

    deque_const_iterator(pointer v, map_pointer node) 
            : cur(v), first(*node), last(*node + buffer_size), node(node) {} // v 缓冲区当前结点，缓冲区所在的结点

    deque_const_iterator(const self& rhs) // 复制构造函数
        : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node){}

    deque_const_iterator(self&& rhs) // 移动构造函数
        : cur(std::move(rhs.cur)), first(std::move(rhs.first)), last(std::move(rhs.last)), node(std::move(rhs.node)){
            rhs.cur = nullptr;
            rhs.first = nullptr;
            rhs.last = nullptr;
            rhs.node = nullptr;
        }


    self& operator=(const self& rhs){
        if(this != &rhs){
            cur = rhs.cur;
            first = rhs.first;
            last = rhs.last;
            node = rhs.node;
        }
        return *this;
    }

    bool operator==(const self& x) const { return cur == x.cur; }
    
    bool operator!=(const self& x) const { return !(*this == x); }

    bool operator<(const self& x) const { return node == x.node ? (cur < x.cur) : (node < x.node); }

    bool operator<=(const self& x) const { return !(x < *this); }

    bool operator>(const self& x) const { return x < *this; }

    bool operator>=(const self& x) const { return !(*this < x); }

    void set_node(map_pointer new_node){
        node = new_node;
        first = *new_node;
        last = first + buffer_size;
    }

    reference operator*() const { return (*cur); }

    pointer operator->() const { return cur; }

    reference operator[](difference_type off) const { return *(*(this + off)); }

    self& operator++(){
        ++cur;

        if(cur == last){
            set_node(node + 1);
            cur = first;
        }
        return *this;
    }

    self operator++(int){
        self ret = *this;
        operator++();
        return ret;
    }

    self& operator--(){
        if(cur == first){
            set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }

    self operator--(int){
        self ret = *this;
        operator--();
        return ret;
    }

    self& operator+=(const difference_type n){
        const auto offset = n + (cur - first);
        if(offset >= 0 && offset < static_cast<difference_type>(buffer_size)){
            cur += n;
        }else{
            const auto node_offset = offset > 0 ? offset / static_cast<difference_type>(buffer_size) 
                                        : static_cast<difference_type>((-offset -1) / buffer_size) - 1;
            set_node(node + node_offset);
            cur = first + (offset - node_offset * static_cast<difference_type>(buffer_size));
        }
        return *this;
    }

    self operator+(const difference_type n) const {
        self tmp = *this;
        return tmp += n; 
    }

    self& operator-=(const difference_type n){
        return *this += -n;
    }

    self operator-(const difference_type n) const { 
        self tmp = *this;
        return tmp -= n; 
    }

    difference_type operator-(const self& x) const {
        return static_cast<difference_type> (buffer_size) * (node - x.node)
                + (cur - first) - (x.cur - x.first);
    }
};


template<typename value_type_>
class deque_iterator : public deque_const_iterator<value_type_>
{
public:
    typedef GHYSTL::random_access_iterator_tag      iterator_category;
    typedef value_type_                             value_type;
    typedef value_type*                             pointer;
    typedef value_type&                             reference;
    typedef size_t                                  size_type;
    typedef ptrdiff_t                               difference_type;

    typedef pointer*                                map_pointer;
    typedef deque_iterator<value_type>              self;
    typedef deque_const_iterator<value_type>        iterator_base;

    
    // 迭代器所含成员数据
    pointer         cur;    // 指向所在缓冲区的当前元素
    pointer         first;  // 指向所在缓冲区的头部
    pointer         last;   // 指向所在缓冲区的尾部
    map_pointer     node;   // 缓冲区所在节点

    static const size_type buffer_size = deque_buf_size<value_type>::value;

    deque_iterator() : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {} //默认构造函数

    deque_iterator(pointer v, map_pointer node) 
            : cur(v), first(*node), last(*node + buffer_size), node(node) {} // v 缓冲区当前结点，缓冲区所在的结点

    deque_iterator(const self& rhs) // 复制构造函数
        : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node){}

    deque_iterator(self&& rhs) // 移动构造函数
        : cur(std::move(rhs.cur)), first(std::move(rhs.first)), last(std::move(rhs.last)), node(std::move(rhs.node)){
            rhs.cur = nullptr;
            rhs.first = nullptr;
            rhs.last = nullptr;
            rhs.node = nullptr;
        }

    void iter_swap(self &right) noexcept {
        GHYSTL::iter_swap(this->cur,    (right.cur));
        GHYSTL::iter_swap(this->first,  (right.first)); 
        GHYSTL::iter_swap(this->last,   (right.last)); 
        GHYSTL::iter_swap(this->node,   (right.node)); 
    }

    bool operator==(const self& x) const { return cur == x.cur; }
    
    bool operator!=(const self& x) const { return !(*this == x); }

    bool operator<(const self& x) const { return node == x.node ? (cur < x.cur) : (node < x.node); }

    bool operator<=(const self& x) const { return !(x < *this); }

    bool operator>(const self& x) const { return x < *this; }

    bool operator>=(const self& x) const { return !(*this < x); }

    self& operator=(const self& rhs){
        if(this != &rhs){
            cur = rhs.cur;
            first = rhs.first;
            last = rhs.last;
            node = rhs.node;
        }
        return *this;
    }

    void set_node(map_pointer new_node){
        node = new_node;
        first = *new_node;
        last = first + buffer_size;
    }

    reference operator*() const { return (*cur); }

    pointer operator->() const { return cur; }

    reference operator[](difference_type off) const { return *(*(this + off)); }

    self& operator++(){
        ++cur;

        if(cur == last){
            set_node(node + 1);
            cur = first;
        }
        return *this;
    }

    self operator++(int){
        self ret = *this;
        operator++();
        return ret;
    }

    self& operator--(){
        if(cur == first){
            set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }

    self operator--(int){
        self ret = *this;
        operator--();
        return ret;
    }

    self& operator+=(const difference_type n){
        const auto offset = n + (cur - first);
        if(offset >= 0 && offset < static_cast<difference_type>(buffer_size)){
            cur += n;
        }else{
            const auto node_offset = offset > 0 ? offset / static_cast<difference_type>(buffer_size) 
                                        : static_cast<difference_type>((-offset -1) / buffer_size) - 1;
            set_node(node + node_offset);
            cur = first + (offset - node_offset * static_cast<difference_type>(buffer_size));
        }
        return *this;
    }

    self operator+(const difference_type n) const {
        self tmp = *this;
        return tmp += n; 
    }

    self& operator-=(const difference_type n){
        return *this += -n;
    }

    self operator-(const difference_type n) const { 
        self tmp = *this;
        return tmp -= n; 
    }

    difference_type operator-(const self& x) const {
        return static_cast<difference_type> (buffer_size) * (node - x.node)
                + (cur - first) - (x.cur - x.first);
    }
};


template<typename value_type_, typename Alloc = GHYSTL::allocator<value_type_>, typename = void>
class deque{
public:
    typedef     value_type_                             value_type;
    typedef     value_type*                             pointer;
    typedef     const value_type*                       const_pointer;
    typedef     value_type&                             reference;
    typedef     const value_type&                       const_reference;
    typedef     size_t                                  size_type;
    typedef     ptrdiff_t                               differene_type;
    typedef     pointer*                                map_type;
    typedef     const pointer*                          const_map_type;

    typedef     deque_iterator<value_type>                          iterator;
    typedef     deque_const_iterator<value_type>                    const_iterator;
    typedef     GHYSTL::reverse_iterator<iterator>                  reverse_iterator;
    typedef     GHYSTL::reverse_iterator<const_iterator>            const_reverse_iterator;

    typedef     deque<value_type, Alloc, void>                                   self;
    typedef     Alloc                                                            allocator_type;
    typedef     Alloc                                                            alloc;
    typedef     typename allocator_type::template rebind<pointer>::other        map_alloc; // rebind是一个模板，要特别声明

    static const size_type buffer_size = deque_buf_size<value_type>::value;

private:
    iterator     begin;  //第一个元素
    iterator     end;   //最后一个元素
    map_type     map;    //主控中心
    size_type    map_size;   // map 内的指针数目

public:
    /*---------------------------------------------------- 构造函数和常规函数 ------------------------------------------------------*/

    deque() { copy_n_default(0, value_type()); }

    explicit deque(const size_type n)  { copy_n_default(n, value_type(0)); } 

    deque(const size_type n, const value_type& val) { copy_n_default(n, val); } 

    deque(const std::initializer_list<value_type>& lst) : deque(lst.begin(), lst.end()) {} 

    template<typename Iter,
            typename = typename enable_if<is_iterator<Iter>::value, void>::type>
    deque(Iter bg, Iter ed){
        // copy_init(bg, ed, GHYSTL::iterator_category(bg)); // 注意萃取器的使用 
        // copy_init(bg, ed, GHYSTL::iter_cate_t<Iter>()); 
        copy_init(bg, ed, typename GHYSTL::iterator_traits<Iter>::iterator_category()); 
    }

    deque(const self& x) { copy_init(x.begin(), x.end(), GHYSTL::forward_iterator_tag()); } // 拷贝构造函数

    deque(self&& x) noexcept : 
        begin(std::move(x.begin)), end(std::move(x.end)), map(x.map), map_size(x.map_size){ // 移动构造函数
        x.map = nullptr;
        x.map_size = 0;
    }

    self& operator=(const self& x);

    self& operator=(self&& x);

    self& operator=(const std::initializer_list<value_type>& lst){
        assign(lst.begin(), lst.end());
        return (*this);
    }

    ~deque(){
        if(map != nullptr){
            for (auto cur = map; cur < map + map_size; ++cur){
                alloc::deallocate(*cur, buffer_size);
                *cur = nullptr;
            }
            map = nullptr;
            map_size = 0;
        }
    }

    void swap(self& x) noexcept {
        if(this != x){
            GHYSTL::swap(map, x.map);
            GHYSTL::swap(begin, x.begin);
            GHYSTL::swap(end, x.end);
            GHYSTL::swap(map_size, x.map_size);
        }
    }

    void shrink_to_fit() noexcept {
        // 保留数据部分
        for (auto cur = map; cur < begin.node; ++cur){
            alloc::deallocate(*cur, buffer_size);
            *cur = nullptr;
        }
        for (auto cur = end.node + 1; cur < map + map_size; ++cur){
            alloc::deallocate(*cur, buffer_size);
            *cur = nullptr;
        }

        map = begin.node;
        map_size = GHYSTL::distance(begin.node, end.node);
    }

    void resize(size_type new_size) { resize(new_size, value_type()); }

    void resize(size_type new_size, const value_type& value){
       const auto len = size();
        if (new_size < len){
            erase(begin + new_size, end);
        }else{
            insert(end, new_size - len, value);
        }
    }

    bool empty() const noexcept { return begin == end; }

    allocator_type get_allocater() const { return allocator_type(); }


    iterator erase(iterator bg, iterator ed){
        if (bg == begin && ed == end){
            clear();
            return end;
        }
        else{
            const size_type len = ed - bg;
            const size_type elems_before = end - begin;

            if (elems_before < ((size() - len) / 2)){
                GHYSTL::copy_backward(begin, bg, ed);
                auto new_begin = begin + len;
                alloc::destroy(begin.cur, new_begin.cur);
                begin = new_begin;
            }
            else
            {
                GHYSTL::copy(ed, end, bg);
                auto new_end = end - len;
                alloc::destroy(new_end.cur, end.cur);
                end = new_end;
            }
            return begin + elems_before;
        }
    }

    iterator erase(iterator pos) { 
        auto next = pos;
        ++next;
        const size_type elems_before = pos - begin;
        if (elems_before < (size() / 2)) //前面的少，移动前面的
        {
            GHYSTL::copy_backward(begin, pos, next);
            pop_front();
        }
        else{ // 后面的少移动后面的
            GHYSTL::copy(next, end, pos);
            pop_back();
        }

        return begin + elems_before;
    }

    // 清空 deque 元素，空间保留
    void clear(){
        // 因为底层的 destr 是对指针操作，iteartor的 ++ 重载了，指针不行
        // 所以要分开释放
        for (map_type cur = begin.node + 1; cur < end.node; ++cur){
            // 清空 deque 元素，但没释放空间
            alloc::destroy(*cur, *cur + buffer_size);
        }

        if (begin.node != end.node)
        { // 有两个以上的缓冲区
            alloc::destroy(begin.cur, begin.last);
            alloc::destroy(end.first, end.cur);
        }
        else
        {
            alloc::destroy(begin.cur, begin.last);
        }
        shrink_to_fit();
        end = begin;
    }

    void pop_front() { 
        GHYSTL_DEBUG(!empty());

        if (begin.cur != begin.last - 1){
            alloc::destroy(begin.cur);
            ++begin.cur;
        }
        else // 如果相等，说明这个 buffer 里面只有一个元素了
        {
            alloc::destroy(begin.cur);
            ++begin;
            destroy_buffer(begin.node - 1, begin.node - 1);
        }
    }


    void pop_back() { 
        GHYSTL_DEBUG(!empty());
        
        if (end.cur != end.first)
        {
            alloc::destroy(end.cur);
            --end.cur;
        }
        else
        {
            alloc::destroy(end.cur);
            --end;
            destroy_buffer(end.node + 1, end.node + 1);
        } 
    }

    // assign 重新赋值
    void assign(size_type n, const value_type& value)
    {
        if (n > size())
        {
            GHYSTL::fill(begin(), end(), value);
            insert(end(), n - size(), value);
        }
        else
        {
            erase(begin() + n, end());
            GHYSTL::fill(begin(), end(), value);
        }
    }

    void assign(const std::initializer_list<value_type>& lst){
        assign(lst.begin(), lst.end());
    }

    template<typename Iter>
    typename enable_if<is_iterator<Iter>::value, void>::type assign(Iter bg, Iter ed){
        assign(bg, ed, GHYSTL::iterator_category(bg));
    }

    // 初始化列表 赋值
    template<typename IIter>
    void assign(IIter first, IIter last, GHYSTL::input_iterator_tag)
    {
        auto first1 = begin();
        auto last1 = end();
        for (; first != last && first1 != last1; ++first, ++first1){
            *first1 = *first;
        }

        if (first1 != last1){
            erase(first1, last1);
        }else {
            insert_dispatch(end, first, last, GHYSTL::iter_cate_t<IIter>());
        }
    }

    // 初始化列表 赋值
    template <class FIter>
    void assign(FIter first, FIter last, GHYSTL::forward_iterator_tag)
    {  
        const size_type len1 = size();
        const size_type len2 = GHYSTL::distance(first, last);
        if (len1 < len2){
            auto next = first;
            GHYSTL::advance(next, len1);
            GHYSTL::copy(first, next, begin);
            insert_dispatch(end, next, last, GHYSTL::iterator_category(first));
        } else {
            erase(GHYSTL::copy(first, last, begin), end); // 先复制，后删除
        }
    }

    // 迭代器相关操作
    iterator begin_() noexcept { return begin; }
    iterator end_()   noexcept { return end; }

    const_iterator begin_() const noexcept { return const_iterator(begin); }
    const_iterator end_()   const noexcept { return const_iterator(end); }

    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend()   const noexcept { return end(); }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    reverse_iterator rend()   noexcept { return reverse_iterator(begin()); }

    const_reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rend()   const noexcept { return reverse_iterator(begin()); }

    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend()   const noexcept { return rend(); }


    // 访问元素相关操作 
    reference operator[](const size_type n){ 
        GHYSTL_DEBUG(n < size());
        return begin[n];
    }
    const_reference operator[](const size_type n) const { 
        GHYSTL_DEBUG(n < size());
        return begin[n];
    }

    reference at(const size_type n) { 
        THROW_OUT_OF_RANGE_IF(!(n < size()), "deque<T>::at() subscript out of range");
        return (*this)[n];
     }

    const_reference at(size_type n) const{
        THROW_OUT_OF_RANGE_IF(!(n < size()), "deque<T>::at() subscript out of range");
        return (*this)[n]; 
    }

    reference front() {
        GHYSTL_DEBUG(!empty());
        return *begin();
    }

    const_reference front() const { 
        GHYSTL_DEBUG(!empty());
        return *begin();
    }

    reference back() { 
        GHYSTL_DEBUG(!empty());
        return *(end() - 1);
    }
    const_reference back() const  { 
        GHYSTL_DEBUG(!empty());
        return *(end() - 1);
    }

    size_type size()     const { return end - begin; }
    size_type max_size() const { return static_cast<size_type>(-1); }

    /*---------------------------------------------------------- 插入元素 -------------------------------------------------------*/

    iterator insert(iterator position, const value_type& value){
        if (position.cur == begin.cur){
            push_front(value);
            return begin;
        }
        else if (position.cur == end.cur){
            push_back(value);
            auto tmp = end;
            --tmp;
            return tmp;
        }
        else{
            return insert_aux(position, value);
        }
    }

    iterator insert(iterator pos, const size_type n, const value_type& val){
        if (pos.cur == begin.cur)
        {
            require_capacity(n, true);
            auto new_begin = begin - n;
            GHYSTL::fill_n(new_begin, n, val);
            begin = new_begin;
        }
        else if (pos.cur == end.cur)
        {
            require_capacity(n, false);
            auto new_end = end + n;
            GHYSTL::fill_n(end, n, val);
            end = new_end;
        }
        else
        {
            fill_insert(pos, n, val);
        }
    }

    void fill_insert(iterator position, size_type n, const value_type& value);


    iterator insert(iterator pos, value_type&& val){
        if (pos.cur == begin.cur){
            emplace_front(std::move(val));
            return begin;
        }
        else if (pos.cur == end.cur){
            emplace_back(std::move(val));
            auto tmp = end;
            --tmp;
            return tmp;
        }
        else{
            return insert_aux(pos, std::move(val));
        }
    }

    template <class FIter>
    void copy_insert(iterator position, FIter first, FIter last, size_type n);

    template <class Iter>
    void insert_dispatch(iterator pos, Iter first, Iter last){
        insert_dispatch(pos, first, last, GHYSTL::iterator_category(first));
    }

    template <class IIter>
    void insert_dispatch(iterator pos, IIter first, IIter last, GHYSTL::input_iterator_tag);

    template <class FIter>
    void insert_dispatch(iterator pos, FIter first, FIter last, GHYSTL::forward_iterator_tag);

    /*------------------------------------------ 插入元素的基本调用 ----------------------------------------------------*/
    void push_front(const value_type& value);
    void push_back(const value_type& value);

    void push_front(value_type&& value) { emplace_front(std::move(value)); }
    void push_back(value_type&& value)  { emplace_back(std::move(value)); }

    template<typename ... types>
    iterator emplace(iterator pos, types&& ...args){
        if (pos.cur == begin.cur){
            emplace_front(std::forward<types>(args)...);
            return begin;
        }
        else if (pos.cur == end.cur){
            emplace_back(std::forward<types>(args)...);
            return end - 1;
        }
        return insert_aux(pos, std::forward<types>(args)...);
    }

    // 在头部就地构建元素
    template<typename ... Args>
    void emplace_front(Args&& ...args){
        if (begin.cur != begin.first){
            alloc::construct(begin.cur - 1, std::forward<Args>(args)...);
            --begin.cur;
        }
        else{
            require_capacity(1, true);
            try{
                --begin;
                alloc::construct(begin.cur, std::forward<Args>(args)...);
            }
            catch (...)
            {
                ++begin;
                throw;
            }
        }
    }

    // 在尾部就地构建元素
    template<typename ... Args>
    void emplace_back(Args&& ...args){
        if (end.cur != end.last - 1)
        {
            alloc::construct(end.cur, std::forward<Args>(args)...);
            ++end.cur;
        }
        else{
            require_capacity(1, false);
            alloc::construct(end.cur, std::forward<Args>(args)...);
            ++end;
        }
    }


private:
    /*----------------------------------------------- 底层空间构造和初始化 -----------------------------------------------------*/
    map_type create_map(size_type size){
        map_type mp = map_alloc::allocate(size);
        for(size_type i = 0; i < size; ++i)
            *(mp + i) = nullptr;
        return mp;
    }

    void create_buffer(map_type nstart, map_type nfinish){
        map_type cur;
        try{
            for(cur = nstart; cur <= nfinish; ++cur)
                *cur = alloc::allocate(buffer_size);
        }catch(...){
            while (cur != nstart)
            {
                --cur;
                alloc::deallocate(*cur, buffer_size);
                *cur = nullptr;
            }
            throw;
        }

    }

    void destroy_buffer(map_type nstart, map_type nfinish){
        for(map_type n = nstart; n <= nfinish; ++n){
            alloc::deallocate(*n, buffer_size);
            *n = nullptr;
        }
    }

    void map_init(size_type nElem){
        const size_type nodes = nElem / buffer_size + 1; //主控结点个数
        map_size = GHYSTL::max(static_cast<size_type>(DEQUE_MAP_INIT_SIZE), nodes + 2);

        try{
            map = create_map(map_size);
        }
        catch(...){
            map = nullptr;
            map_size = 0;
            throw;
        }

        // 让 nstart 和 nfinish 都指向 map_ 最中央的区域，方便向头尾扩充
        map_type nstart = map + ((map_size - nodes) >> 1);
        map_type nfinish = nstart + nodes - 1;

        try{
            create_buffer(nstart, nfinish);
        }
        catch(...){
            map_alloc::deallocate(map, map_size);
            map = nullptr;
            map_size = 0;
            throw;
        }

        begin.set_node(nstart);
        end.set_node(nfinish);
        begin.cur = begin.first;
        end.cur = end.first + (nElem % buffer_size);
    } 

    // 默认值初始化
    void copy_n_default(size_type n, const value_type& value){
        map_init(n);
        if(n){
            for(auto cur = begin.node; cur < end.node; ++cur){
                alloc::copy_construct(*cur, *cur + buffer_size, value);
            }
            alloc::copy_construct(end.first, end.cur, value);
        }
    }

    // 使用 初始化列表 初始化
    template<typename IIter>
    void copy_init(IIter first, IIter last, GHYSTL::input_iterator_tag){
        const size_type n = GHYSTL::distance(first, last);
        map_init(n);
        for(; first != last; ++first);
            emplace_back(*first);
    }

    // 使用 初始化列表 初始化
    template <class FIter>
    void copy_init(FIter first, FIter last, GHYSTL::forward_iterator_tag)
    {
        const size_type n = GHYSTL::distance(first, last);
        map_init(n);
        for (auto cur = begin.node; cur < end.node; ++cur)
        {
            auto next = first;
            GHYSTL::advance(next, buffer_size);
            alloc::copy_construct(first, next, *cur);
            first = next;
        }
        alloc::copy_construct(first, last, end.first);
    }

    void require_capacity(size_type n, bool front)
    {
        if (front && (static_cast<size_type>(begin.cur - begin.first) < n)) // 在前面插入
        {
            const size_type need_buffer = (n - (begin.cur - begin.first)) / buffer_size + 1;

            if (need_buffer > static_cast<size_type>(begin.node - map)){
                reallocate_map_at_front(need_buffer);
                return;
            }
            create_buffer(begin.node - need_buffer, begin.node - 1);
        }
        else if (!front && (static_cast<size_type>(end.last - end.cur - 1) < n)) // 在后面插入
        {
            const size_type need_buffer = (n - (end.last - end.cur - 1)) / buffer_size + 1;

            if (need_buffer > static_cast<size_type>((map + map_size) - end.node - 1)){
                reallocate_map_at_back(need_buffer);
                return;
            }
            create_buffer(end.node + 1, end.node + need_buffer);
        }
    }
    
    void reallocate_map_at_front(size_type need_buffer)
    {
        //开辟新的空间
        const size_type new_map_size = GHYSTL::max(map_size << 1, map_size + need_buffer + DEQUE_MAP_INIT_SIZE);
        map_type new_map = create_map(new_map_size);
        const size_type old_buffer = end.node - begin.node + 1;
        const size_type new_buffer = old_buffer + need_buffer;

        // 新的 map 中的指针指向原来的 buffer，并开辟新的 buffer
        auto new_begin = new_map + (new_map_size - new_buffer) / 2;
        auto new_mid = new_begin + need_buffer;
        auto new_end = new_mid + old_buffer;
        create_buffer(new_begin, new_mid - 1);
        for (auto begin_1 = new_mid, begin_2 = begin.node; begin_1 != new_end; ++begin_1, ++begin_2)
            *begin_1 = *begin_2;

        // 释放旧的 deque 内存
        map_alloc::deallocate(map, map_size);
        map = new_map;
        map_size = new_map_size;
        begin = iterator(*new_mid + (begin.cur - begin.first), new_mid);
        end = iterator(*(new_end - 1) + (end.cur - end.first), new_end - 1);
    }

    void reallocate_map_at_back(size_type need_buffer)
    {
        const size_type new_map_size = GHYSTL::max(map_size << 1, map_size + need_buffer + DEQUE_MAP_INIT_SIZE);
        map_type new_map = create_map(new_map_size);
        const size_type old_buffer = end.node - begin.node + 1;
        const size_type new_buffer = old_buffer + need_buffer;

        // 另新的 map 中的指针指向原来的 buffer，并开辟新的 buffer
        auto new_begin = new_map + ((new_map_size - new_buffer) / 2);
        auto new_mid = new_begin + old_buffer;
        auto new_end = new_mid + need_buffer;
        for (auto begin_1 = new_begin, begin_2 = begin.node; begin_1 != new_mid; ++begin_1, ++begin_2)
            *begin_1 = *begin_2;

        create_buffer(new_mid, new_end - 1);

        // 更新数据
        map_alloc::deallocate(map, map_size);
        map = new_map;
        map_size = new_map_size;
        begin = iterator(*new_begin + (begin.cur - begin.first), new_begin);
        end = iterator(*(new_mid - 1) + (end.cur - end.first), new_mid - 1);
    }

    template <class... Args>
    iterator insert_aux(iterator pos, Args&& ...args)
    {
        const size_type elems_before = pos - begin;
        value_type value_copy = value_type(std::forward<Args>(args)...);
        if (elems_before < (size() / 2))
        { // 在前半段插入
            emplace_front(front());
            auto front1 = begin;
            ++front1;
            auto front2 = front1;
            ++front2;
            pos = begin + elems_before;
            auto cur = pos;
            ++cur;
            GHYSTL::copy(front2, cur, front1);
        } else { // 在后半段插入
            emplace_back(back());
            auto back1 = end;
            --back1;
            auto back2 = back1;
            --back2;
            pos = begin + elems_before;
            GHYSTL::copy_backward(pos, back2, back1);
        }
        *pos = std::move(value_copy);
        return pos;
    }
};

template<typename value_type, typename alloc, typename NoType>
inline void swap(GHYSTL::deque<value_type, alloc, NoType> & left,
                GHYSTL::deque<value_type, alloc, NoType> & right) noexcept {
    left.swap(right);
}

template<typename value_type, typename alloc, typename NoType>
typename deque<value_type, alloc, NoType>::self& 
    deque<value_type, alloc, NoType>::operator=(const self& x){
            deque tmp(x);
            assign(std::move(tmp));
            return *this;
}

template<typename value_type, typename alloc, typename NoType>
typename deque<value_type, alloc, NoType>::self& 
    deque<value_type, alloc, NoType>::operator=(self&& x){
            clear();
            begin = std::move(x.begin);
            end = std::move(x.end);
            x.map = nullptr;
            x.map_size = 0;
}

template<typename value_type, typename alloc, typename NoType>
void deque<value_type, alloc, NoType>::
fill_insert(iterator position, size_type n, const value_type& value){
        const size_type elems_before = position - begin;
        const size_type len = size();
        auto value_copy = value;
        
        if (elems_before < (len / 2)) // 前面的元素少
        {
            require_capacity(n, true);
            // 原来的迭代器可能会失效
            auto old_begin = begin;
            auto new_begin = begin - n;
            position = begin + elems_before;

            try
            {
                if (elems_before >= n){ // 插入点前的元素 比 n 大
                    auto begin_n = begin + n;
                    GHYSTL::copy(begin, begin_n, new_begin);
                    begin = new_begin;
                    GHYSTL::copy(begin_n, position, old_begin);
                    GHYSTL::fill_n(position - n, position, value_copy);
                } 
                else { // 插入点前的元素 比 n 小
                    GHYSTL::fill(GHYSTL::copy(begin, position, new_begin), begin, value_copy);
                    begin = new_begin;
                    GHYSTL::fill(old_begin, position, value_copy);
                }
            }
            catch (...)
            {
                if (new_begin.node != begin.node)
                destroy_buffer(new_begin.node, begin.node - 1);
                throw;
            }
        }
        else
        {
            require_capacity(n, false);
            // 原来的迭代器可能会失效
            auto old_end = end;
            auto new_end = end + n;
            const size_type elems_after = len - elems_before;
            position = end - elems_after;

            try
            {
                if (elems_after > n)
                {
                    auto end_n = end - n;
                    GHYSTL::copy(end_n, end, end);
                    end = new_end;
                    GHYSTL::copy_backward(position, end_n, old_end);
                    GHYSTL::fill(position, position + n, value_copy);
                }
                else {
                    GHYSTL::fill_n(end, position + n, value_copy);
                    GHYSTL::copy(position, end, position + n);
                    end = new_end;
                    GHYSTL::fill(position, old_end, value_copy);
                }
            }
            catch (...)
            {
                if(new_end.node != end.node)
                destroy_buffer(end.node + 1, new_end.node);
                throw;
            }
        }
    }

// 在头部插入元素
template <typename value_type, typename alloc, typename NoType>
void deque<value_type, alloc, NoType>::
push_front(const value_type& value){
  if (begin.cur != begin.first) { // 有空间
    alloc::construct(begin.cur - 1, value);
    --begin.cur;
  }
  else{ // 没空间
    require_capacity(1, true);
    try
    {
      --begin;
      alloc::construct(begin.cur, value);
    }
    catch (...)
    {
      ++begin;
      throw;
    }
  }
}

// 在尾部插入元素
template <typename value_type, typename alloc, typename NoType>
void deque<value_type, alloc, NoType>::
push_back(const value_type& value)
{
  if (end.cur != end.last - 1){
    alloc::construct(end.cur, value);
    ++end.cur;
  }
  else{
    require_capacity(1, false);
    alloc::construct(end.cur, value);
    ++end;
  }
}

template <typename value_type, typename alloc, typename NoType>
template <class FIter>
void deque<value_type, alloc, NoType>::
copy_insert(iterator position, FIter first, FIter last, size_type n)
{
  const size_type elems_before = position - begin;
  auto len = size();

  if (elems_before < (len / 2)){
    require_capacity(n, true);
    // 原来的迭代器可能会失效
    auto old_begin = begin;
    auto new_begin = begin - n;
    position = begin + elems_before;

    try
    {
      if (elems_before >= n){
        auto begin_n = begin + n;
        GHYSTL::copy(begin, begin_n, new_begin);
        begin = new_begin;
        GHYSTL::copy(begin_n, position, old_begin);
        GHYSTL::copy(first, last, position - n);
      }
      else{
        auto mid = first;
        GHYSTL::advance(mid, n - elems_before);
        GHYSTL::copy(first, mid, GHYSTL::copy(begin, position, new_begin));
        begin_ = new_begin;
        GHYSTL::copy(mid, last, old_begin);
      }
    }
    catch (...)
    {
      if(new_begin.node != begin.node)
        destroy_buffer(new_begin.node, begin.node - 1);
      throw;
    }
  }
  else
  {
    require_capacity(n, false);
    // 原来的迭代器可能会失效
    auto old_end = end;
    auto new_end = end + n;
    const auto elems_after = len - elems_before;
    position = end - elems_after;

    try
    {
      if (elems_after > n){
        auto end_n = end - n;
        GHYSTL::copy(end_n, end, end);
        end = new_end;
        GHYSTL::copy_backward(position, end_n, old_end);
        GHYSTL::copy(first, last, position);
      }
      else
      {
        auto mid = first;
        GHYSTL::advance(mid, elems_after);
        GHYSTL::copy(position, end, GHYSTL::copy(mid, last, end));
        end = new_end;
        GHYSTL::copy(first, mid, position);
      }
    }
    catch (...)
    {
      if(new_end.node != end.node)
        destroy_buffer(end.node + 1, new_end.node);
      throw;
    }
  }
}

template <typename value_type, typename alloc, typename NoType>
template <class IIter>
void deque<value_type, alloc, NoType>::
insert_dispatch(iterator position, IIter first, IIter last, GHYSTL::input_iterator_tag)
{
    if (last <= first)  return;

    const size_type n = GHYSTL::distance(first, last);
    const size_type elems_before = position - begin;

    if (elems_before < (size() / 2)){
        require_capacity(n, true);
    }else{
        require_capacity(n, false);
    }

    position = begin + elems_before;
    auto cur = --last;

    for (size_type i = 0; i < n; ++i, --cur)
    {
        insert(position, *cur);
    }
}

template <typename value_type, typename alloc, typename NoType>
template <class FIter>
void deque<value_type, alloc, NoType>::
insert_dispatch(iterator position, FIter first, FIter last, GHYSTL::forward_iterator_tag)
{
  if (last <= first)  return;

  const size_type n = GHYSTL::distance(first, last);

  if (position.cur == begin.cur)
  {
    require_capacity(n, true);
    auto new_begin = begin - n;

    try{
      GHYSTL::copy(first, last, new_begin);
      begin = new_begin;
    }
    catch (...){
      if(new_begin.node != begin.node)
        destroy_buffer(new_begin.node, begin.node - 1);
      throw;
    }
  }
  else if (position.cur == end.cur){
    require_capacity(n, false);
    auto new_end = end + n;
    try{
      GHYSTL::copy(first, last, end);
      end = new_end;
    }
    catch (...){
      if(new_end.node != end.node)
        destroy_buffer(end.node + 1, new_end.node);
      throw;
    }
  }
  else{
    copy_insert(position, first, last, n);
  }
}

/**-------------------------------------- 重写的比较运算符 ---------------------------------------------------------*/
template <typename value_type, typename alloc, typename NoType>
inline bool operator==(const GHYSTL::deque<value_type, alloc, NoType> &left,
                       const GHYSTL::deque<value_type, alloc, NoType> &right){
    return (left.size() == right.size() &&
          GHYSTL::equal(left.begin(), left.end(), right.begin(), right.end()));
}

template <typename value_type, typename alloc, typename NoType>
inline bool operator!=(const GHYSTL::deque<value_type, alloc, NoType> &left,
                       const GHYSTL::deque<value_type, alloc, NoType> &right){
    return !(left == right);
}

template <typename value_type, typename alloc, typename NoType>
inline bool operator<(const GHYSTL::deque<value_type, alloc, NoType> &left,
                      const GHYSTL::deque<value_type, alloc, NoType> &right){
  return GHYSTL::lexicographical_compare(left.begin(), left.end(), right.begin(),
                                      right.end());
}

template <typename value_type, typename alloc, typename NoType>
inline bool operator<=(const GHYSTL::deque<value_type, alloc, NoType> &left,
                       const GHYSTL::deque<value_type, alloc, NoType> &right){
  return !(right < left);
}

template <typename value_type, typename alloc, typename NoType>
inline bool operator>(const GHYSTL::deque<value_type, alloc, NoType> &left,
                      const GHYSTL::deque<value_type, alloc, NoType> &right){
  return right < left;
}

template <typename value_type, typename alloc, typename NoType>
inline bool operator>=(const GHYSTL::deque<value_type, alloc, NoType> &left,
                       const GHYSTL::deque<value_type, alloc, NoType> &right){
  return !(left < right);
}

} // namespace GHYSTL
#endif