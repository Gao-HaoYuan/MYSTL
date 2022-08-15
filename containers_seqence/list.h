/**
 * @file list.h
 * @author ghy (ghy_mike@163.com)
 * @brief list
 * 
 * @version 1.0
 * @date 2022-08-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _LIST_H_
#define _LIST_H_

#include "../algorithm/algorithm.h"

namespace GHYSTL{

/************************************************ 链表结点 ************************************************************/


// 声明链表结点
template<typename value_type_>
struct list_node;


template<typename value_type_>
struct list_base_node{
    typedef list_node<value_type_> *link_type;
    
    link_type prev;
    link_type next;

    list_base_node(link_type prev = nullptr, link_type next = nullptr) : prev(prev), next(next) {}

    
};

template<typename value_type_>
struct list_node : list_base_node<value_type_>
{
    typedef value_type_                         value_type;
    typedef list_node<value_type_>              *link_type;
    typedef list_base_node<value_type_>         base_type;

    value_type data; // 数据

    list_node(link_type prev = nullptr, link_type next = nullptr) : base_type(prev, next), data() {}

    list_node(link_type prev, link_type next, const value_type& val) : base_type(prev, next), data(val) {}
};


/************************************************ 链表迭代器 ************************************************************/

template<typename value_type_>
class list_const_iterator
{
public:
    template<typename value_type, typename alloc>
    friend class list; // 友元类， 使用这个友元让list直接访问 get_node 函数

    typedef GHYSTL::bidirectional_iterator_tag                  iterator_category;
    typedef value_type_                                         value_type;
    typedef const value_type*                                   pointer;
    typedef const value_type&                                   reference;
    typedef ptrdiff_t                                           difference_type;

    typedef list_const_iterator<value_type>                     self;
    typedef list_node<value_type>*                              link_type;
    typedef size_t                                              size_type;

protected:
    link_type node;
    
    link_type get_node() { return node; }

public:

    list_const_iterator(link_type node = nullptr) : node(node) {}

    list_const_iterator(const self& x) : node(x.node) {}

    self& operator=(const self& x){
        node = x.node;
        return *this;
    }

    bool operator==(const self& x) const { return node == x.node; }
    bool operator!=(const self& x) const { return !operator==(x); }

    reference operator*() const { return node->data; }
    pointer operator->() const { return &(operator*()); }

    self& operator++(){
        node = node->next;
        return *this;
    }

    self operator++(int){
        self ret = *this;
        ++*this;
        return ret;
    }

    self& operator--(){
        node = node->prev;
        return *this;
    } 

    self operator--(int){
        self ret = *this;
        --*this;
        return ret;
    }  
};


template<typename value_type_>
class list_iterator : public list_const_iterator<value_type_>
{
public:
    typedef GHYSTL::bidirectional_iterator_tag          iterator_category;
    typedef value_type_                                 value_type;
    typedef value_type*                                 pointer;
    typedef value_type&                                 reference;
    typedef ptrdiff_t                                   difference_type;


    typedef list_iterator<value_type>               self;
    typedef list_const_iterator<value_type>         iterator_base;
    typedef list_node<value_type>*                  link_type;
    typedef size_t                                  size_type;


    list_iterator(const link_type node = nullptr) : iterator_base(node) {}

    list_iterator(const self& x) : iterator_base(x.node) {}

    self& operator=(const self& x){
        this->node = x.node;
        return *this;
    }

    bool operator==(const self& x) const { return this->node == x.node; }

    bool operator!=(const self& x) const { return !(operator==(x)); }

    reference operator*() const { return this->node->data; }

    pointer operator->() const { return &(operator*()); }

    self& operator++(){
        this->node = this->node->next;
        return *this;
    } 

    self operator++(int){
        self ret = *this;
        ++*this;
        return ret;
    }

    self& operator--(){
        this->node = this->node->prev;
        return *this;
    }

    self operator--(int){
        self ret = *this;
        --*this;
        return ret;
    }
};


/************************************************ 链表 list ************************************************************/
template <typename value_type_, typename Alloc = GHYSTL::allocator<value_type_>>
class list
{
public:
    typedef value_type_             value_type;
    typedef value_type*             pointer;
    typedef const value_type*       const_pointer;
    typedef value_type&             reference;
    typedef const value_type&       const_reference;
    typedef size_t                  size_type;
    typedef ptrdiff_t               difference_type;

    typedef list<value_type_, Alloc>               self;
    typedef list_node<value_type>*                  link_type;

    typedef Alloc           allocator_type;
    typedef Alloc           data_alloc;
    typedef typename allocator_type::template rebind<list_node<value_type>>::other              node_alloc;
    typedef typename allocator_type::template rebind<list_base_node<value_type>>::other         base_alloc;

    typedef list_iterator<value_type>                       iterator;
    typedef list_const_iterator<value_type>                 const_iterator;
    typedef GHYSTL::reverse_iterator<iterator>              reverse_iterator;
    typedef GHYSTL::reverse_iterator<const_iterator>        const_reverse_iterator;

    allocator_type get_allocator() const { return allocator_type(); }

private:
  link_type node; // 类内可以访问私有变量

public:
    /*-------------------------------------------- 构造函数 ------------------------------------------------*/
    list() { empty_init(); }

    template<typename Iter,
                typename = typename GHYSTL::enable_if<is_iterator<Iter>::value, void>::type>
    list(Iter first, Iter last) : list(){
        assign(first, last);
    }

    explicit list(const size_type n) : list() { resize(n); }

    list(const size_type n, const value_type& val) : list() {
        insert(begin(), n, val);
    }

    list(std::initializer_list<value_type>& lst) : list() {
        assign(lst.begin(), lst.end());
    }

    list(const self& x) : list() { insert(begin(), x.begin(), x.end()); }

    list(self&& x) : list() { GHYSTL::swap(node, x.node); } // 通过调用默认构造函数，创建空结点，然后交换

    self& operator=(const self& x){
        if(this != &x){
            assign(x.begin(), x.end());
        }
        return *this;
    }

    self& operator=(self&& x) noexcept {
        clear();
        splice(end(), x);
        return *this;
    }

    self& operator=(const std::initializer_list<value_type>& lst){
        assign(lst.begin(), lst.end());
        return *this;
    }

    ~list(){
        if(node){
            clear();
            base_alloc::deallocate(node);
            node = nullptr;
        }
    }

public:
    /*-------------------------------------------- 常规操作函数 ------------------------------------------------*/

    void swap(self& x) noexcept { GHYSTL::swap(node, x.node); }

    iterator erase(const_iterator pos){
        GHYSTL_DEBUG(pos != cend());
        const_iterator next = pos;
        ++next;
        erase_imple(pos);
        return make_iter(next);
    }

    iterator erase(const_iterator first, const_iterator last){
        while (first != last)
            first = erase(first);
        return make_iter(last);
    }

    void resize(const size_type new_size){
        size_type n = size();
        if(n < new_size){
            for(; n != new_size; ++n)
                insert_imple(end());
        }
        else{
            for(; n != new_size; --n)
                pop_back();
        }
    }

    void resize(const size_type new_size, const value_type& val){
        size_type n = size();
        if(n < new_size){
            insert(end(), n - new_size, val);
        }
        else{
            for(; n != new_size; --n)
                pop_back();
        }
    }

    size_type max_size() const { return size_type(-1); }

    void clear();
    void reverse();
    void remove(const value_type& val);

    template<typename Pred>
    void remove_if(const Pred& pred){
        iterator ed = end();
        for(iterator bg = begin(); bg != ed; )
            if(pred(*bg))
                bg = erase(bg);
            else
                ++bg;
    }

    void unique() { unique(GHYSTL::equal_to<value_type>()); }

    template<typename Comp>
    void unique(const Comp& cmp){ // 删除链表得重复元素，前提是链表是有序得
        iterator first = begin();
        iterator last = end();

        if(first == last) return;

        for(iterator next = first; ++next != last; ){
            if(cmp(*next, *first))
                erase(next);
            else
                first = next;
            next = first;
        }
    }

    void sort() { sort(GHYSTL::less<value_type>()); }
    
    template<typename Comp>
    void sort(const Comp& cmp){
        merge_sort(begin(), end(), size(), cmp);
    }
   
public:
    /*-------------------------------------------- 插入删除 push、pop、emplace ------------------------------------------------*/

    void push_front(const value_type& val){ insert_imple(begin(), val); }
    
    void push_front(value_type && val){
        insert_imple(begin(), std::forward<value_type>(val));
    }

    void push_back(const value_type& val) { insert_imple(end(), val); }

    void push_back(value_type&& val){
        insert_imple(end(), std::forward<value_type>(val));
    }

    template<typename ... types>
    void emplace_front(types && ... args){
        insert_imple(begin(), std::forward<types>(args)...);
    }

    template<typename ... types>
    void emplace_back(types && ... args){
        insert_imple(end(), std::forward<types>(args)...);
    }

    template<typename ... types>
    iterator emplace(const_iterator pos, types && ... args){
        insert_imple(pos, std::forward<types>(args)...);
        return make_iter(--pos);
    }

    void pop_front() { erase(begin()); }
    void pop_back() { erase(--end()); }

    /*-------------------------------------------- 插入操作 insert ------------------------------------------------*/

    iterator insert(const_iterator pos, const value_type& val){
        insert_imple(pos, val);
        return make_iter(--pos);
    }

    iterator insert(const_iterator pos, value_type&& val){
        return emplace(pos, std::move(val));
    }

    void insert(const_iterator pos, const size_type n, const value_type& val){
        const_iterator prev = pos;
        if(prev == cbegin()){
            insert_n(pos, n, val);
            return begin();
        }
        else{
            --prev;
            insert_n(pos, n, val);
            return make_iter(++prev);
        }
    }

    template<typename Iter>
    typename enable_if<is_iterator<Iter>::value, iterator>::type 
    insert(const_iterator pos, Iter first, Iter last){
        const_iterator prev = pos;
        if(prev == cbegin()){
            insert_range(pos, first, last, GHYSTL::iterator_category(first));
            return begin();
        }
        else{
            --prev;
            insert_range(pos, first, last, GHYSTL::iterator_category(first));
            return make_iter(++prev);
        }
    }

    iterator insert(const_iterator pos, const std::initializer_list<value_type>& lst){
        return insert(pos, lst.begin(), lst.en());
    }


    /*-------------------------------------------- 分配操作 assign ------------------------------------------------*/

    template<typename Iter>
    typename enable_if<is_iterator<Iter>::value, void>::type
    assign(Iter first, Iter last){
        assign_imple(first, last, GHYSTL::iterator_category(first));
    }

    void assign(const std::initializer_list<value_type>& lst){
        assign(lst.begin(), lst.end());
    }

    void assign(size_type n, const value_type& val){
        iterator bg = begin();
        iterator ed = end();

        for(; bg != ed && n != 0; --n, ++bg)
            resueNode(bg, val);
        for(; n != 0; val)
            insert_imple(bg, val);
        
        earse(bg, ed);
    }

    /*--------------------------------------------  链接两个 list splice ------------------------------------------------*/
    void splice(const_iterator pos, self& x){
        if(x.empty())
            transfer(pos, x.begin(), x.end());
    }

    void splice(const_iterator pos, self&& x) { splice(pos, (self&) x); }


    void splice(const_iterator pos, const_iterator tar){
        const_iterator last = tar;
        if(pos != tar)
            transfer(pos, tar, ++last);
    }

    void splice(const_iterator pos, const_iterator first, const_iterator last){
        if(first != pos)
            transfer(pos, first, last);
    }

/*-------------------------------------------- 有序合并操作 merge ------------------------------------------------*/
    void merge(self& x) { merge(x, GHYSTL::less<value_type>()); }

    void merge(self&& x) { merge((self &)x, GHYSTL::less<value_type>()); }

    template<typename Compare>
    void merge(self && lst, const Compare& cmp){
        merge((self &)lst, cmp);
    }

    template<typename Compare>
    void merge(self & lst, const Compare& cmp);


public:
/*-------------------------------------------- 迭代器相关操作 ------------------------------------------------*/

bool empty() const { return node->next == node; }
size_type size() const { return GHYSTL::distance(begin(), end()); }

// 获取头尾迭代器
iterator begin() noexcept { return iterator(node->next); }
iterator end() noexcept { return iterator(node); }

const_iterator begin() const noexcept { return const_iterator(node->next); }
const_iterator end() const noexcept { return const_iterator(node); }

const_iterator cbegin() const noexcept { return begin(); }
const_iterator cend() const noexcept { return end(); }

reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

const_reverse_iterator crbegin() const noexcept { return rbegin(); }
const_reverse_iterator crend() const noexcept { return rend(); }


// 获取头尾值
reference front() { return *begin(); }
const_reference front() const { return *begin(); }

reference back() { return *(--end()); }
const_reference back() const {return *(--end()); }



private:
/*-------------------------------------------- 一些受保护得操作，供上面的函数调用 ------------------------------------------------*/
iterator make_iter(const_iterator iter) const {
    return iterator(iter.get_node());
}

template<typename ... types>
link_type create_node(types && ... args){
    link_type p = node_alloc::allocate();

    try{
        data_alloc::construct(std::addressof(p->data), std::forward<types>(args)...);
    }
    catch(...){
        node_alloc::deallocate(p);
        throw;
    }

    return p;
}

void destros_and_free_node(link_type ptr){
    node_alloc::destroy(&(ptr->data));
    node_alloc::deallocate(ptr);
}

void transfer(const_iterator pos, const_iterator first, const_iterator last){
    if(pos != last){
        first.get_node()->prev->next = last.get_node();
        last.get_node()->prev->next = pos.get_node();
        link_type tmp = pos.get_node()->prev;
        tmp->next = first.get_node();
        pos.get_node()->prev = last.get_node()->prev;
        last.get_node()->prev = first.get_node()->prev;
        first.get_node()->prev = tmp;
    }
}

void insert_n(const_iterator pos, size_type n, const value_type& val){
    for(; n != 0; --n)
        insert_imple(pos, val);
}

template<typename ... types> // 默认插在 pos 前面
void insert_imple(const_iterator pos, types && ... args){
    link_type cur = pos.get_node();
    link_type tmp = create_node(std::forward<types>(args)...);
    cur->prev->next = tmp;
    tmp->next = cur;
    tmp->prev = cur->prev;
    cur->prev = tmp;
}

template<typename Iter>
void insert_range(const_iterator pos, Iter first, Iter last, GHYSTL::input_iterator_tag){
    for(; first != last; ++first)
        insert_imple(pos, *first);
}

template<typename Iter>
void insert_range(const_iterator pos, Iter first, Iter last, GHYSTL::random_access_iterator_tag){
    size_type num = last - first;
    for(; num != 0; --num, ++first)
        insert_imple(pos, *first);
}

void erase_imple(const_iterator pos){
    link_type next_node = pos.get_node()->next;
    link_type prev_node = pos.get_node()->prev;

    prev_node->next = next_node;
    next_node->prev = prev_node;

    destros_and_free_node(pos.get_node());
}

template<typename Iter>
void assign_imple(Iter first, Iter last, GHYSTL::input_iterator_tag){
    iterator bg = this->begin();
    iterator ed = this->end();

    for(; bg != ed && first != last; ++first, ++bg)
        reuseNode(bg, *first);
    for(; first != last; ++first) // 第二个链表长
        insert_imple(bg, *first);
    
    erase(bg, ed);
}

template<typename Iter>
void assign_imple(Iter first, Iter last, GHYSTL::random_access_iterator_tag){
    size_type n = last - first;
    iterator bg = this->begin();
    iterator ed = this->end();

    for(; bg != ed && n != 0; --n, ++first, ++bg)
        this->reuseNode(bg, *first);
    for(; n != 0; --n, ++first)
        insert_imple(bg, *first);

    erase(bg, ed);
}

template<typename type>
void reuseNode(const_iterator pos, type && val){
    data_alloc::destroy(std::addressof(pos.get_node()->data));
    data_alloc::construct(std::addressof(pos.get_node()->data), std::forward<type>(val));
}

void empty_init(){
    node = (link_type)base_alloc::allocate();
    data_alloc::construct(std::addressof(node->data));
    node->next = node;
    node->prev = node;
}

template<typename Compare = GHYSTL::less<value_type>>
iterator merge_sort(iterator first, iterator last, size_type len, Compare& cmp = Compare());

}; // end of class

template <typename value_type, typename alloc>
template<typename Compare>
void list<value_type, alloc>::merge(self& lst, const Compare& cmp){
    iterator first1 = begin();
    iterator last1  = end();
    iterator first2 = lst.begin();
    iterator last2  = lst.end();

    if(first1 == last1)
        splice(end(), lst);
    else if(first2 == last2)
        return;
    else  
        for(;;){
            if(cmp(*first2, *first1)){
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
                if(first2 == last2)
                    return;
            }
            else{
                if(++first1 == last1)
                    if(first2 != last2)
                        transfer(last1, first2, last2);
                    return;
            }
        }
}

// 模板没必要写出来默认值 ----> Compare
// 参数的缺省值也没必要写出来 -----> cmp = Compare()
// 所有的默认值在类的声明处给出就行， 编译器会根据类名匹配自动在类里寻找对应的方法
template<typename value_type, typename alloc>
template<typename Compare>
typename list<value_type, alloc>::iterator list<value_type, alloc>::
merge_sort(iterator first, iterator last, size_type len, Compare& cmp){
    if(len >= 2){
        iterator med = first;
        size_type half = len >> 1;
        GHYSTL::advance(med, half);

        first = merge(first, med, half, cmp); // 合并排序的思想
        med = merge_sort(med, last, len - half, cmp); // 合并排序的思想

        iterator new_first = first; // 记录最小值，下面的链接操作是把较大的值接到后面
        if(cmp(*med, *first))
            new_first = med;
        
        for(; ;){
            if(cmp(*med, *first)){ // 后半段小，就把后半段插入到前面
                splice(first, med++);
                if(med == last)
                    return new_first;
            }else{
                if(++first == med) 
                    return new_first;
            }
        }

    }

    return first;
}

template<typename value_type, typename alloc>
void list<value_type, alloc>::clear(){
    for(link_type cur = node->next; cur != node; ){
        link_type tmp = cur->next;
        destros_and_free_node(cur);
        cur = tmp;
    }
    node->next = node;
    node->prev = node;
}

template<typename value_type, typename alloc>
void list<value_type, alloc>::remove(const value_type& val){
    auto first = begin();
    auto last = end();

    while(first != last){
        if(*first == val)
            first = erase(first);
        else
            ++first;
    }
}

template<typename value_type, typename alloc>
void list<value_type, alloc>::reverse(){
    link_type cur = node;
    do{
        auto next = cur->next;
        cur->next = cur->prev;
        cur->prev = next;
        cur = next;
    }while (cur != node);
}

/*********************************************** 重写了比较操作 *****************************************************/
template <typename value_type, typename alloc>
inline void swap(GHYSTL::list<value_type, alloc> &left,
                 GHYSTL::list<value_type, alloc> &right) noexcept
{
  left.swap(right);
}

template <typename value_type, typename alloc>
inline bool operator==(const GHYSTL::list<value_type, alloc> &left,
                       const GHYSTL::list<value_type, alloc> &right)
{
  return (GHYSTL::equal(left.begin(), left.end(), right.begin(), right.end()));
}

template <typename value_type, typename alloc>
inline bool operator!=(const GHYSTL::list<value_type, alloc> &left,
                       const GHYSTL::list<value_type, alloc> &right)
{
  return !(left == right);
}

template <typename value_type, typename alloc>
inline bool operator<(const GHYSTL::list<value_type, alloc> &left,
                      const GHYSTL::list<value_type, alloc> &right)
{
  return GHYSTL::lexicographical_compare(left.begin(), left.end(), right.begin(),
                                      right.end());
}

template <typename value_type, typename alloc>
inline bool operator<=(const GHYSTL::list<value_type, alloc> &left,
                       const GHYSTL::list<value_type, alloc> &right)
{
  return !(right < left);
}

template <typename value_type, typename alloc>
inline bool operator>(const GHYSTL::list<value_type, alloc> &left,
                      const GHYSTL::list<value_type, alloc> &right)
{
  return right < left;
}

template <typename value_type, typename alloc>
inline bool operator>=(const GHYSTL::list<value_type, alloc> &left,
                       const GHYSTL::list<value_type, alloc> &right)
{
  return !(left < right);
}

} // namespace GHYSTL
#endif