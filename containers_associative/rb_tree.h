/**
 * @file rb_tree.h
 * @author ghy (ghy_mike@163.com)
 * @brief 红黑树的实现
 * 
 * @version 1.0
 * @date 2022-08-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include <iostream>
#include "../algorithm/algorithm.h"

namespace GHYSTL{

/******************************************* 红黑的节点 ***************************************/

//节点颜色
typedef bool rb_tree_color_type;
constexpr rb_tree_color_type red = false;
constexpr rb_tree_color_type black = true;


//红黑树节点
template<typename value_type>
struct  rb_tree_node;

template<typename value_type>
struct rb_tree_base_node
{
    typedef rb_tree_node<value_type>* link_type;

    rb_tree_color_type color; // 节点颜色
    link_type parent; // 父节点
    link_type left; // 左孩子
    link_type right; // 右孩子

    rb_tree_base_node(rb_tree_color_type color = red, link_type parent = nullptr,
                        link_type left = nullptr, link_type right = nullptr)
                            : color(color), parent(parent), left(left), right(right) {}
};


template<typename value_type>
struct rb_tree_node : public rb_tree_base_node<value_type>
{
    typedef rb_tree_node<value_type>*           link_type; // 节点链接
    typedef rb_tree_base_node<value_type>       base_node_type; // 继承的父类节点的成员属性

    value_type value; // 节点的值

    /*----------------------------------- 构造函数 ------------------------------------*/
    rb_tree_node(const value_type& val) : value(val) {}

    rb_tree_node(value_type&& val) : value(std::move(val)) {}

    rb_tree_node(rb_tree_color_type color = red, link_type parent = nullptr,
                    link_type left = nullptr, link_type right = nullptr)
                        : base_node_type(color, parent, left, right), value() {}

    rb_tree_node(const value_type& value, rb_tree_color_type color = red, link_type parent = nullptr,
                    link_type left = nullptr, link_type right = nullptr)
                        : base_node_type(color, parent, left, right), value(value) {}

    rb_tree_node(value_type&& value, rb_tree_color_type color = red, link_type parent = nullptr,
                    link_type left = nullptr, link_type right = nullptr)
                        : base_node_type(color, parent, left, right), value(std::move(value)) {}
                        
    /*----------------------------------- 最大最小值 ------------------------------------*/
    // 红黑树是一棵平衡二叉搜索树，左孩子比根节点小，右孩子比根节点大
    static link_type minimum(link_type root){  
        while(root->left->parent)
            root = root->left;
        return root;
    }

    static link_type maximum(link_type root){
    while(root->right->parent)
        root = root->right;
    return root;
    }

};

/******************************************* 红黑的迭代器 ***************************************/
template<typename value_type_>
class rb_tree_const_iterator : public GHYSTL::iterator_base<bidirectional_iterator_tag, value_type_>
{ 
public:
    template<typename traits>
    friend class rb_tree; // 定义为友元类的目的是访问 get_node 函数

    typedef GHYSTL::bidirectional_iterator_tag              iterator_category;
    typedef value_type_                                     value_type;
    typedef const value_type*                               pointer;
    typedef const value_type&                               reference;
    typedef ptrdiff_t                                       difference_type;

    typedef rb_tree_node<value_type>                node_type;
    typedef rb_tree_node<value_type>*               link_type;
    typedef rb_tree_const_iterator<value_type>      self;

protected:
        link_type node;
        
        link_type get_node() { return node; }

public:
    /*----------------------------------- 构造函数 ------------------------------------*/
    rb_tree_const_iterator() : node(nullptr) {} 

    rb_tree_const_iterator(link_type node) : node(node) {} 

    rb_tree_const_iterator(const self& x) : node(x.node) {}  
    
    // 红黑树是平衡树，要根据 value 排序，所以不能提供移动构造函数，否则会破坏树的有序性


    bool operator==(const self& x) const { return node == x.node; }

    bool operator!=(const self& x) const { return !(operator==(x)); }

    reference operator*() const { return node->value; }

    pointer operator->() const  { return &(operator*()); }

    self& operator=(const self& x){
        node = x.node;
        return *this;
    }

    /*----------------------------------- 重写运算符 ------------------------------------*/
    /*----------------*\
    |      nil         |
    |       |          |
    |       p          |
    |      / \         |
    |     x  nil       |
    |    / \           |
    |  nil nil         |
    \*----------------*/

    self& operator--(){
        if(node->left->parent) 
            // 树有左孩子，并且左孩子不是 nil (空节点)
            // 寻找上一个比 node 值小的节点
            node = node_type::maximum(node->left);
        else{ 
            // 树的左子树是 nil
            // 如果该节点是右孩子，就返回父节点
            // 如果该节点是左孩子，就返回到根节点的 nil 节点
            while (node->parent->parent && node->parent->left == node)
                node = node->parent;
            node = node->parent;
        }

        return *this;
    }

    self operator--(int){
        auto ret = *this;
        operator--();
        return ret;
    }

    self& operator++(){
        if(node->right->parent)
            node = node_type::minimum(node->right);
        else{
            while (node->parent->parent && node->parent->right == node)
                node = node->parent;
            node = node->parent;
        }

        return *this;
    }

    self operator++(int){
        auto ret = *this;
        operator++();
        return ret;
    }
};


template<typename value_type_>
class rb_tree_iterator : public rb_tree_const_iterator<value_type_>
{
public: // 声明成公有的,要不然萃取的时候拿不到    
    typedef GHYSTL::bidirectional_iterator_tag                  iterator_category;
    typedef value_type_                                         value_type;
    typedef value_type*                                         pointer;
    typedef value_type&                                         reference;
    typedef ptrdiff_t                                           difference_type;


    typedef rb_tree_const_iterator<value_type>              base_iterator;
    typedef typename base_iterator::node_type               node_type;
    typedef typename base_iterator::link_type               link_type;
    typedef rb_tree_iterator<value_type>                    self;

public:
    /*----------------------------------- 构造函数 ------------------------------------*/
    rb_tree_iterator() : base_iterator() {}

    rb_tree_iterator(link_type node) : base_iterator(node) {}

    rb_tree_iterator(const self& x) : base_iterator(x.node) {}

    reference operator*() const { return this->node->value; }

    pointer operator->() const { return &(operator*()); }

    self& operator=(const self& x){
        this->node = x.node;
        return *this;
    }

    /*----------------------------------- 重写运算符 ------------------------------------*/ 
    self& operator--(){
        if(this->node->left->parent)
            this->node = node_type::maximum(this->node->left);
        else{
            while(this->node->parent->parent && this->node->parent->left == this->node)
                this->node = this->node->parent;
            this->node = this->node->parent;
        }

        return *this;
    }

    self operator--(int){
        auto ret = *this;
        operator--();
        return ret;
    }

    self& operator++(){
        if(this->node->right->parent)
            this->node = node_type::minimum(this->node->right);
        else{
            while (this->node->parent->parent && this->node->parent->right == this->node)
                this->node = this->node->parent;
            this->node = this->node->parent;    
        }

        return *this;
    }

    self operator++(int){
        auto ret = *this;
        operator++();
        return ret;
    }
};



/******************************************* 红黑树 ***************************************/
template<typename traits>
class rb_tree{
protected:
    typedef rb_tree<traits>                     self;
    typedef typename traits::key_type           key_type;
    typedef typename traits::allocator_type     allocator_type;
    typedef typename traits::value_compare      value_compare; 
    typedef size_t                              size_type;


    typedef typename traits::value_type         value_type;
    typedef value_type*                         pointer;
    typedef const value_type*                   const_pointer;
    typedef value_type&                         reference;
    typedef const value_type&                   const_reference;
    typedef std::ptrdiff_t                      difference_type;

    typedef rb_tree_node<value_type>            node_type;
    typedef rb_tree_node<value_type>*           link_type;
    typedef rb_tree_base_node<value_type>       base_node_type;
    typedef rb_tree_base_node<value_type>*      base_link_type;


    typedef allocator_type          data_alloc;
    typedef typename allocator_type::template rebind<node_type>::other                  node_alloc;
    typedef typename allocator_type::template rebind<base_node_type>::other             base_alloc;
    
    enum{
        is_multi = traits::is_multi
    };


    typedef typename traits::key_compare    key_compare;
    typedef key_compare                     Compare;

    typedef rb_tree_const_iterator<value_type>  const_iterator;
    typedef typename GHYSTL::If<is_same<key_type, value_type>::value, 
                                    const_iterator, rb_tree_iterator<value_type>>::type iterator;


    typedef GHYSTL::reverse_iterator<iterator>              reverse_iterator;
    typedef GHYSTL::reverse_iterator<const_iterator>        const_reverse_iterator;


    typedef GHYSTL::pair<iterator, bool>                        pair_ib;
    typedef GHYSTL::pair<iterator, iterator>                    pair_ii;
    typedef GHYSTL::pair<const_iterator, const_iterator>        pair_cc;

private:
    link_type   nil;
    link_type   root;
    size_type   node_count;
    Compare     comp; // 类型是 key_compare

public:


    /************************************************* 构造函数 ********************************************************/

    rb_tree() : nil(create_nil()), root(nil), comp(), node_count(0) {}

    explicit rb_tree(const Compare& comp): nil(create_nil()), root(nil), comp(comp), node_count(0) {}

    rb_tree(const self& x) : nil(create_nil()), root(nil), comp(x.comp), node_count(0){
        if(x.get_root() != x.get_nil())
            root = copy_assign(root, x.get_root(), x.get_nil());
        
        node_count = x.node_count;
        nil->left = maximum(get_root());
        nil->right = minimum(get_root());
    }

    rb_tree(self&& x) : nil(x.nil), root(x.root), comp(x.comp), node_count(x.node_count){
        x.nil = x.root = nullptr;
        x.node_count = 0;
    }

    self& operator=(const self& x){
        if(this != &x){
            if(x.node_count != 0){
                clear();
                nil = create_nil();
                root = copy_assign(root, x.get_root(), x.get_nil());
                nil->left = maximum(get_root());
                nil->right = minimum(get_root());
            }
            node_count = x.node_count;
            comp = x.comp;
        }

        return *this;
    }

    self& operator=(self&& x){
        clear();
        nil = std::move(x.nil);
        root = std::move(x.root);
        comp = x.comp;
        node_count = x.node_count;

        x.nil = x.root = nullptr;
        x.node_count = 0;
        return *this;
    }

    ~rb_tree(){
        if(!empty())
            rb_destroy(get_root());
        destroy_nil();
    }

/*-------------------------------------------- 常规操作 -----------------------------------------------------*/
public:
    void clear(){
        if(!empty())
            rb_destroy(get_root());

        root = nil;
        nil->left = nil->right = nil;
        node_count = 0;
    }

    void swap(self& x) noexcept {
        GHYSTL::swap(nil, x.nil);
        GHYSTL::swap(root, x.root);
        GHYSTL::swap(comp, x.comp);
        GHYSTL::swap(node_count, x.node_count);
    }

    size_type size() const { return node_count; }

    bool empty() const { return !(node_count); }
    
    size_type count(const key_type& val) const{
        pair_cc range = equal_range(val);
        return GHYSTL::distance(range.first, range.second);
    }

    allocator_type get_allocator() const { return allocator_type(); }

    key_compare key_comp() const { return key_compare(); }
    
    value_compare value_comp() const { return value_compare(); }

    size_type max_size() const { return size_type(-1); }

    /*------------------------------- erase的几种形式 -----------------------------------*/

    iterator erase(const_iterator x){
        const_iterator temp = x;
        const_iterator succ = x;

        ++succ;

        if (x == begin())
            nil->right = succ.get_node();
        if (x == --end())
            nil->left = (--temp).get_node();

        rb_delete(x.get_node());
        return make_iter(succ);
    }

    size_type erase(const key_type &k){

        pair_ii range = equal_range(k);
        size_type count = 0;
        
        for (iterator first = range.first, last = range.second; first != last; ++count)
            first = erase(first);
        return (0);
    }

    iterator erase(const_iterator first, const_iterator last){
        for(; first != last; )
            first = erase(first);

        return make_iter(last);
    }

    /*--------------------------------- 查找操作 --------------------------------------*/

    const_iterator lower_bound(const key_type& key) const {
        return lower_bound_imple(key);
    }

    iterator lower_bound(const key_type& key) {
        return make_iter(lower_bound_imple(key));
    }

    const_iterator upper_bound(const key_type& key) const {
        return upper_bound_imple(key);
    }

    iterator upper_bound(const key_type& key){
        return make_iter(upper_bound_imple(key));
    }

    pair_cc equal_range(const key_type& key) const {
        return {lower_bound(key), upper_bound(key)};
    }
    
    pair_ii equal_range(const key_type& key) {
        return {lower_bound(key), upper_bound(key)};
    }

    const_iterator find(const key_type& key) const {
        return make_iter(find_imple(key));
    }

    /*--------------------------------- 插入操作 --------------------------------------*/

    template<bool multi = is_multi> // multi类型
    enable_if_t<multi, iterator> insert(const value_type& val){
        return insert_equal_imple(get_root(), val);
    }

    template <bool multi = is_multi>
    enable_if_t<!multi, pair_ib> insert(const value_type& value){
        return insert_unique_imple(get_root(), value);
    }

    template<bool multi = is_multi> // 不是 multi类型
    enable_if_t<!multi, pair_ib> insert(value_type&& value){
        return insert_unique_imple(get_root(), std::forward<value_type>(value));
    }

    template<bool multi = is_multi>
    enable_if_t<multi, iterator> insert(value_type&& val){
        return insert_equal_imple(get_root(), std::forward<value_type>(val));
    }
    
    template<typename Iter>
    void insert(Iter first, Iter last){
        for(; first != last; ++first)
            insert(*first);
    }

    void insert(const std::initializer_list<value_type>& lst){
        insert(lst.begin(), lst.end());
    }   

    iterator insert(const_iterator pos, value_type&& val){
        return emplace_hint(pos, std::forward<value_type>(val));
    }

    iterator insert(const_iterator pos, const value_type& val){
        return emplace_hint(pos, val);
    }

    template<bool multi = is_multi, typename ... types>
    enable_if_t<multi, iterator> emplace(types && ... args){
        return insert_equal_imple(get_root(), std::forward<types>(args)...);
    }

    template<bool multi = is_multi, typename ... types>
    enable_if_t<!multi, pair_ib> emplace(types && ... args){
        return insert_unique_imple(get_root(), std::forward<types>(args)...);
    }

    template<bool multi = is_multi, typename ... types>
    enable_if_t<!multi, iterator> emplace_hint(const_iterator pos, types && ... args);

    template<bool multi = is_multi, typename value_type>
    enable_if_t<!multi, iterator> emplace_hint(const_iterator pos, value_type&& value);

    template<bool multi = is_multi, typename ... types>
    enable_if_t<multi, iterator> emplace_hint(const_iterator pos, types && ... args);

/*-------------------------------------------- 迭代器相关操作 -----------------------------------------------------*/
public:
    void print_rb() { print(get_root()); }

    iterator end() { return iterator(nil); }
    iterator begin() { return iterator(nil->right); }

    const_iterator end() const { return const_iterator(nil); }
    const_iterator begin() const { return const_iterator(nil->right); }

    const_iterator cend() const { return end(); }
    const_iterator cbegin() const { return begin(); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }

    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

    const_reverse_iterator crend() const { return rend(); }
    const_reverse_iterator crbegin() const { return rbegin(); }

protected:
    void print(link_type root, int count_hight = 0){
        if(root != nil){
            print(root->right, count_hight + 1);
            for(auto n = count_hight; n != -1; --n)
                std::cout<<"\t";
            std::cout << root->value.first << "," << root->value.second;
            root->color ? std::cout<<"黑色"<<std::endl : std::cout<<"红色"<<std::endl;
            print(root->left, count_hight + 1);
            std::cout<<std::endl;
        }
    }


protected:
    const key_type& get_key(const value_type& val) const {
        return traits::key_of_value(val);
    }

    template<typename ... types>
    void construct_value(link_type ptr, types&& ... args){
        try{
            data_alloc::construct(std::addressof(ptr->value), std::forward<types>(args)...);
        }
        catch(...){
            node_alloc::deallocate(ptr);
            throw;
        }
    }

    link_type create_node(rb_tree_color_type color, link_type parent){
        
        link_type ptr = node_alloc::allocate();

        try{
            ptr->color = color;
            ptr->parent = parent;
            ptr->left = nullptr;
            ptr->right = nullptr;
        }
        catch(...){
            node_alloc::deallocate(ptr);
        }

        return ptr;
    }

    template<typename ... types>
    link_type create_insert_node(link_type par, types&&... args){
        link_type tar = node_alloc::allocate();
    
        try{
            node_alloc::construct(tar, std::forward<types>(args)..., red, par, nil, nil); 
            // data_alloc::construct(std::addressof(tar->value), std::forward<types>(args)...);
            // tar->color = red;
            // tar->left = nil;
            // tar->right = nil;
            // tar->parent = par;
        }
        catch(...){
            node_alloc::deallocate(tar);
            throw;
        }
        
        return tar;
    }

    void destroy_and_free_node(link_type node){
        data_alloc::destroy(std::addressof(node->value));
        node_alloc::deallocate(node);
    }

    link_type get_root() const { return root; }

    link_type get_nil() const { return nil; }

    link_type create_nil() const{
        link_type ptr = node_alloc::allocate();
        node_alloc::construct(ptr, black, nullptr, ptr, ptr); // 试一下这种构造方式，看能不能自动对应变量
        return ptr;
    }

    void destroy_nil() const {
        node_alloc::deallocate(nil);
    }
    
    static link_type minimum(link_type rt) { return node_type::minimum(rt); }
    static link_type maximum(link_type rt) { return node_type::maximum(rt); }


    /*------------------------ 类外定义，这里声明 --------------------------------*/
    void rb_left_rotate(link_type x); // 左旋

    void rb_right_rotate(link_type x); // 右旋

    void rb_destroy(link_type x); // 销毁树

    void rb_delete(link_type tar); 

    void rb_delete_fixup(link_type tar); // 删除节点

    iterator rb_insert_fixup(link_type tar); // 插入节点

    const_iterator lower_bound_imple(const key_type& key) const;

    const_iterator upper_bound_imple(const key_type& key) const;

    const_iterator find_imple(const key_type& key) const;

    /*------------------------------------------------- 插入、分配 -----------------------------------------------------*/

    link_type copy_assign(link_type par, const link_type cur, const link_type nl){
        link_type my = create_node(cur->color, par); //初始化颜色 和 父节点
        construct_value(my, cur->value);
        my->left = cur->left != nl ? copy_assign(my, cur->left, nl) : nil; // 递归复制左右孩子
        my->right = cur->right != nl ? copy_assign(my, cur->right, nl) : nil;

        return my;
    }

    void rb_transplant(link_type lhs, link_type rhs){ // rhs 替换 lhs
        if(lhs->parent == nil)
            root = rhs;
        else if(lhs == lhs->parent->left)
            lhs->parent->left = rhs;
        else
            lhs->parent->right = rhs;
        rhs->parent = lhs->parent;
    }
    
    template<typename value_type>
    iterator insert_equal_imple(link_type cur, value_type&& value){
        link_type par = nil;

        while (cur != nil) // 寻找插入点
        {
            par = cur;
            cur = comp(get_key(value), get_key(par->value)) ? par->left : par->right;
        }

        link_type tar = create_insert_node(par, std::forward<value_type>(value));
        return insert_imple(par, tar);
    }

    template<typename ... types>
    iterator insert_equal_imple(link_type cur, types&& ... args){
        link_type par = nil;
        link_type tar = create_insert_node(par, std::forward<types>(args)...);

        const value_type& val = tar->value;

        while (cur != nil)
        {
            par = cur;
            cur = comp(get_key(val), get_key(par->value)) ? par->left : par->right;
        }

        tar->parent = par;
        
        return insert_imple(par, tar);
    }

    template<typename value_type>
    pair_ib insert_unique_imple(link_type cur, value_type&& value){
        link_type par = nil;

        while (cur != nil)
        {
            par = cur;
            if(comp(get_key(value), get_key(cur->value)))
                cur = cur->left;
            else if(comp(get_key(cur->value), get_key(value)))
                cur = cur->right;
            else
                return pair_ib(iterator(cur), false);
        }

        link_type tar = create_insert_node(par, std::forward<value_type>(value));
        return {insert_imple(par, tar), true};
    }

    template<typename ... types>
    pair_ib insert_unique_imple(link_type cur, types && ... args){
        link_type par = nil;
        link_type tar = create_insert_node(par, std::forward<types>(args)...);

        const value_type& value = tar->value;

        while (cur != nil)
        {
            par = cur;
            if(comp(get_key(value), get_key(cur->value)))
                cur = cur->left;
            else if(comp(get_key(cur->value), get_key(value)))
                cur = cur->right;
            else{
                destroy_and_free_node(tar);
                return pair_ib(iterator(cur), false);
            }
        }
        
        tar->parent = par;
        return {insert_imple(par, tar), true};
    }

    // 插入节点
    iterator insert_imple(link_type par, link_type tar){
        if(par == nil)
            root = tar;
        else if(comp(get_key(tar->value), get_key(par->value)))
            par->left = tar;
        else
            par->right = tar;
        update_extremum(tar); // 调整 nil 节点的左右指针
        return rb_insert_fixup(tar); // 红黑树调整 + 变色
    }

    // 调整 nil 节点的左右指针
    void update_extremum(link_type tar){
        if(nil->left != nil){
            if(comp(get_key(tar->value), get_key(nil->right->value)))
                nil->right = tar;
            else if(!comp(get_key(tar->value), get_key(nil->left->value)))
                nil->left = tar;
        }
        else
            nil->left = nil->right = tar;
    }

    reference front() const { return nil->right->value; }
    reference back() const { return nil->left->value; }

    iterator make_iter(const_iterator citer) const {
        return iterator(citer.get_node());
    }
};

template<typename traits>
void rb_tree<traits>::rb_destroy(link_type x){
    if(x->left != nil)
        rb_destroy(x->left);
    if(x->right != nil)
        rb_destroy(x->right);
    destroy_and_free_node(x);
}

template<typename traits>
typename rb_tree<traits>::const_iterator 
rb_tree<traits>::lower_bound_imple(const key_type& key) const {
    auto cur = get_root();
    auto result = cur;

    while (cur != nil)
    {
        result = cur;
        if(comp(get_key(cur->value), key))
            cur = cur->right;
        else
            cur = cur->left;
    }

    if(result == nil)
        return cend();
    else if(comp(get_key(result->value), key))
        return (++const_iterator(result));
    else
        return const_iterator(result);
}

template<typename traits>
typename rb_tree<traits>::const_iterator
rb_tree<traits>::upper_bound_imple(const key_type& key) const {
    link_type cur = get_root();
    link_type result = cur;

    while (cur != nil)
    {
        result = cur;
        if(comp(key, get_key(cur->value)))
            cur = cur->left;
        else 
            cur = cur->right;
    }

    if(result == nil)
        return cend();
    else if(comp(key, get_key(result->value)))
        return const_iterator(result);
    else
        return ++const_iterator(result); 
}

template<typename traits>
typename rb_tree<traits>::const_iterator
rb_tree<traits>::find_imple(const key_type& key) const {
    link_type cur = get_root();

    while (cur != nil)
    {
        if(comp(key, get_key(cur->value)))
            cur = cur->left;
        else if(comp(get_key(cur->value), key))
            cur = cur->right;
        else
            return const_iterator(cur);
    }
    
    return cend();
}

// 参考我写的博客 https://blog.csdn.net/qq_35227495/article/details/125958281?spm=1001.2014.3001.5502
template<typename traits>
typename rb_tree<traits>::iterator 
rb_tree<traits>::rb_insert_fixup(link_type tar){
    iterator iter = iterator(tar);

    // 默认插入节点是红色
    // 1、树为空，直接插入
    // 2、父节点是黑色，直接插入
    // 3、父节点是红色，即下面的循环

    while (!tar->parent->color) // 父节点是红色, 父节点是黑色 就退出循环
    {
        auto grandpar = tar->parent->parent;

        // 第一种大情况 ：父节点是左孩子
        if(tar->parent == grandpar->left){ 
            // 情况 3 ：叔节点是红色（祖父节点一定是黑色）
            if(!grandpar->right->color){
                // 父、叔、祖父节点全部变色
                grandpar->left->color = black;
                grandpar->right->color = black;
                grandpar->color = red;
                tar = grandpar; // 变成情况 1 和 2，向上循环，直到祖父节点的父节点是黑色
            }
            else{
                // 情况 2 ：叔节点是黑色，插入点是右孩子
                if(tar == tar->parent->right){
                    // 父节点左旋，
                    rb_left_rotate(tar->parent);
                    tar = tar->left; // 变成 情况 1
                }

                // 情况 1 ：叔节点是黑色，插入点是左孩子
                // 父节点、祖父节点变色
                // 祖父节点右旋
                grandpar->color = red;
                grandpar->left->color = black;
                rb_right_rotate(grandpar);
            }
        }
        // 第二种大情况 ：父节点是右孩子
        else{
            // 情况 3 ：叔节点是红色（祖父节点一定是黑色）
            if(!grandpar->left->color){
                // 父、叔、祖父节点全部变色
                grandpar->right->color = black;
                grandpar->left->color = black;
                grandpar->color = red;
                tar = grandpar; // 变成情况 1 和 2，向上循环，直到祖父节点的父节点是黑色
            }
            else{
                // 情况 2 ：叔节点是黑色，插入点是左孩子
                if(tar == tar->parent->left){
                    // 父节点右旋
                    rb_right_rotate(tar->parent);
                    tar = tar->right; // 变成 情况 1
                }

                // 情况 1 ：叔节点是黑色，插入点是右孩子
                // 父节点、祖父节点变色
                // 祖父节点左旋
                grandpar->color = red;
                grandpar->right->color = black;
                rb_left_rotate(grandpar);
            }
        }
    }
    
    root->color = black; // 根节点设为黑色
    ++node_count;
    return iter;    
}

template<typename traits>
void rb_tree<traits>::rb_delete(link_type tar){
    // 首先要符合二叉搜索树的删除规则
    // 红节点直接删除
    // 黑节点删除会影响黑高，需要调整

    if(tar == nil) return;

    link_type y = tar;
    rb_tree_color_type y_original_color = y->color;

    link_type x;

    // 二叉搜索树的删除规则
    if(tar->left == nil){ // 左子树为空，右子树代替删除点
        x = tar->right;
        rb_transplant(tar, x);
    }
    else if(tar->right == nil){ // 右子树为空，左子树代替删除点
        x = tar->left;
        rb_transplant(tar, x);
    }
    else{ // 左右子树存在，用右子树的最小值代替删除点
        y = minimum(tar->right);
        y_original_color = y->color;
        x = y->right;

        if(y->parent == tar){ //  == 符号，之前写成了 = 号，这个 bug 让我找了一天的错误，引以为戒
            x->parent = y;
        }
        else{
            rb_transplant(y, x);
            y->right = tar->right;
            y->right->parent = y;
        }

        rb_transplant(tar, y);
        y->left = tar->left;
        y->left->parent = y;
        y->color = tar->color;
    }

    --node_count;
    // 红节点直接删除，黑节点就需要调整
    if(y_original_color){
        rb_delete_fixup(x); // x 代替了被删除的节点，所以要对 x 进行调整
    } 
        
    nil->parent = nullptr;
    destroy_and_free_node(tar);
}

template<typename traits>
void rb_tree<traits>::rb_delete_fixup(link_type x){
    // 调整分为四种情况
    // 首先 x 是黑节点
    while (x != root && x->color)
    { 
        // x 是左孩子
        if(x == x->parent->left){ 
            // 叔叔节点
            link_type w = x->parent->right; 

            // 叔叔节点是红色， X 父节点就一定是黑色
            if(!w->color){
                // 1、  叔叔节点和 X 父节点颜色互换
                // 2、  X 父节点左旋
                // 3、  w = x->parent->right
                // 4、  变成下面的三种情况
                x->parent->color = red;
                w->color = black;
                rb_left_rotate(x->parent);
                w = x->parent->right;
            }
            
            // 叔叔节点 W 是黑色， W 左右孩子都是黑色
            // 1、   W 染红， X = X -> parent
            // 2、   向上调整，直到 X 是根节点 或者 X 是红色就退出循环
            // 3、   退出循环后，X 变为黑色
            if(w->right->color && w->left->color){
                w->color = red;
                x = x->parent;
            }
            // 叔叔节点 W 是黑色， W 孩子左红右黑
            // 1、   W 和 W 左孩子颜色互换
            // 2、   W 右旋
            // 3、   W 等于其之前的左孩子
            // 4、   变成了上面的情况
            else if(w->right->color){
                w->color = red;
                w->left->color = black;
                rb_right_rotate(w);
                w = x->parent->right;
            }

            // W是黑色， W 的右孩子是红色
            // 1、  W 变为 X 父节点的颜色
            // 2、  X 父节点和 W 右孩子变黑
            // 3、  X 父节点左旋
            if(!w->right->color){
                w->color = x->parent->color;
                x->parent->color = black;
                w->right->color = black;
                rb_left_rotate(x->parent);
                x = root;
            }
        }
        else{// 这个和上面的情况反过来，就不写过程了
            link_type w = x->parent->left;
            
            if(!w->color){
                x->parent->color = red;
                w->color = black;
                rb_right_rotate(x->parent);
                w = x->parent->left;
            }

            if(w->right->color && w->left->color){
                w->color = red;
                x = x->parent;
            }
            else if(w->left->color){
                w->color = red;
                w->right->color = black;
                rb_left_rotate(w);
                w = x->parent->left;
            }

            if(!w->left->color){
                w->color = x->parent->color;
                x->parent->color = black;
                w->left->color = black;
                rb_right_rotate(x->parent);
                x = root;
            }
        }
    }

    x->color = black;
}

template<typename traits>
void rb_tree<traits>::rb_left_rotate(link_type x){
    auto x_R = x->right;
    x_R->parent = x->parent;

    if(x_R->left != nil)
        x_R->left->parent = x;
    
    x->right = x_R->left;

    if(x->parent == nil)
        root = x_R;
    else if(x->parent->left == x)
        x->parent->left = x_R;
    else
        x->parent->right = x_R;

    x_R->left = x;
    x->parent = x_R;
}

template<typename traits>
void rb_tree<traits>::rb_right_rotate(link_type x){
    auto x_L = x->left;
    x_L->parent = x->parent;

    if(x_L->right != nil)
        x_L->right->parent = x;
    x->left = x_L->right;

    if(x->parent == nil)
        root = x_L;
    else if(x->parent->left == x)
        x->parent->left = x_L;
    else
        x->parent->right = x_L;
    
    x_L->right = x;
    x->parent = x_L;
}

// template<bool multi = rb_tree<traits>::is_multi, typename ... types> 
// 上面是错的，没必要写出来默认值
// 默认值在类的声明处给出就行， 编译器会根据类名匹配自动在类里寻找对应的方法
template<typename traits>
template<bool multi, typename ... types>
GHYSTL::enable_if_t<!multi, typename rb_tree<traits>::iterator> 
rb_tree<traits>::emplace_hint(const_iterator pos, types&& ...args){
    link_type tar = create_insert_node(nil, std::forward<types>(args)...);
    const value_type& value = tar->value;
    link_type cur = pos.get_node();

    if(cur == nil){ // 插入点是空节点
        if(comp(get_key(back()), get_key(value))){ // 和最右边（值最大）的节点比较
            tar->parent = nil->left;
            return insert_imple(tar->parent, tar);
        }
    }
    else if(comp(get_key(value), get_key(*pos))){  // 插入值的键 比插入点小
        if(cur == nil->right || comp(get_key(*--pos), get_key(value))){ // 插入点是最最左边的节点，或者插入点左子树的最大值比插入值得键小
            if(cur->left == nil)
                tar->parent = cur;
            else
                tar->parent = cur = pos.get_node();
            return insert_imple(cur, tar);
        }
    }

    cur = get_root();
    link_type par = nil;
    while (cur != nil) // 树不是空得
    {
        par = cur;
        if(comp(get_key(value), get_key(cur->value))) // 寻找插入得位置
            cur = cur->left;
        else if(comp(get_key(cur->value), get_key(value))) // 寻找插入得位置
            cur = cur->right;
        else{ // key 值重复，不插入
            destroy_and_free_node(tar);
            return iterator(cur);
        }
    }
    
    tar->parent = par;
    return insert_imple(par, tar);
}   


// value 类型不能起名成 value_type，不然会转成 rb_tree<traits>::value_type,这样就匹配不上模板了
// C++ 函数命名规则 函数名_参数名， 只要这些和类里面的方法匹配上就行了
// 模板只是一个类型的别名，实际上传进来是什么，就替换成什么
template<typename traits>
template<bool multi, typename Args> 
GHYSTL::enable_if_t<!multi, typename rb_tree<traits>::iterator> 
rb_tree<traits>::emplace_hint(const_iterator pos, Args&& value){ 
    link_type cur = pos.get_node();

    if(cur == nil){
        if(comp(get_key(back()), get_key(value))){
            link_type tar = create_insert_node(nil->left, std::forward<Args>(value));
            return insert_imple(tar->parent, tar);
        }
    }
    else if(comp(get_key(value), get_key(*pos))){
        if(cur == nil->right || comp(get_key(*--pos), get_key(value))){
            if(cur->left != nil)
                cur = pos.get_node();
            link_type tar = create_insert_node(cur, std::forward<value_type>(value));
            return insert_imple(cur, tar);
        }
    }

    cur = get_root();
    link_type par = nil;

    while (cur != nil)
    {
        par = cur;
        if(comp(get_key(value), get_key(cur->value)))
            cur = cur->left;
        else if(comp(get_key(cur->value), get_key(value)))
            cur = cur->right;
        else
            return iterator(cur);
    }

    link_type tar = create_insert_node(par, std::forward<value_type>(value));
    return insert_imple(par, tar);
}

// 因为 iterator 编译器不知道是什么，（程序员也不一定知道最终是什么）
// 所以要用 typename 声明，这是一个类型
template<typename traits>
template<bool multi, typename ... types>
GHYSTL::enable_if_t<multi, typename rb_tree<traits>::iterator>
rb_tree<traits>::emplace_hint(const_iterator pos, types && ... args){

    link_type tar = create_insert_node(nil, std::forward<types>(args)...);
    const value_type& value = tar->value;
    link_type cur = pos.get_node();

    if(!empty()){ // 树不为空
        if(cur == nil){ // 从 nil 节点插入
            if(!comp(get_key(value), get_key(back()))){ // 比数最右边得节点大， 插入到最右边
                tar->parent = nil->left;
                tar->parent->right = tar;
                nil->left = tar;
                return rb_insert_fixup(tar);
            }
        }
        else if(!comp(get_key(cur->value), get_key(value))){ // 插入点的 key 比插入值的 key 大
            if(cur == nil->right){ // 插入点是最小值，插到最左边
                tar->parent = cur;
                tar->parent = cur;
                cur->left = tar;
                nil->right = tar;
                return rb_insert_fixup(tar);
            }
            else{ // 插入点不是最小值
                const_iterator tmp = pos;
                if(!comp(get_key(value), get_key(*--tmp))){ // 插入值的 key 比插入点上一个节点的 key 大
                    if(cur->left == nil){ // 如果插入点左子树为空， 插到左子树（插入点是父节点的右子树）
                        tar->parent = cur;
                        cur->left = tar;
                        return rb_insert_fixup(tar);
                    }
                    else{ // 插到插入点的右子树
                        tar->parent = tmp.get_node();
                        tar->parent->right= tar;
                        return rb_insert_fixup(tar);
                    }
                }
            }
        }
        else{ // 寻找新得插入点，并插入
            const_iterator new_pos = lower_bound(get_key(value));
            destroy_and_free_node(tar);
            return emplace_hint(new_pos, std::forward<types>(args)...);
        }
    }

    cur = get_root();
    link_type par = nil;

    while (cur != nil) // 找到插入点并插入
    {
        par = cur;
        cur = comp(get_key(value), get_key(par->value)) ? par->left : par->right;
    }
    
    tar->parent = par;
    return insert_imple(par, tar);
}


/*------------------------------------------- 重写比较运算符 ------------------------------------------------*/


template <typename traits>
inline bool operator==(const rb_tree<traits> &left,
                       const rb_tree<traits> &right)
{
  return (left.size() == right.size() && GHYSTL::equal(left.begin(), left.end(), 
                                                right.begin(), right.end()));
}

template <typename traits>
inline bool operator!=(const rb_tree<traits> &left,
                       const rb_tree<traits> &right)
{
  return (!(left == right));
}

template <typename traits>
inline bool operator<(const rb_tree<traits> &left,
                      const rb_tree<traits> &right)
{
  return GHYSTL::lexicographical_compare(left.begin(), left.end(), 
                                                right.begin(), right.end());
}

template <typename traits>
inline bool operator>(const rb_tree<traits> &left,
                      const rb_tree<traits> &right)
{
  return (right < left);
}

template <typename traits>
inline bool operator<=(const rb_tree<traits> &left,
                       const rb_tree<traits> &right)
{
  return (!(right < left));
}

template <typename traits>
inline bool operator>=(const rb_tree<traits> &left,
                       const rb_tree<traits> &right)
{
  return (!(left < right));
}

} // namespace GHYSTL
#endif