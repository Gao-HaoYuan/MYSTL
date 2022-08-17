/**
 * @file hash_table.h
 * @author ghy (ghy_mike@163.com)
 * @brief 哈希表的实现
 * 
 * 
 * @version 1.0
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include "../algorithm/algorithm.h"
#include "../containers_seqence/vector.h"

namespace GHYSTL
{

/**************************************** 哈希表节点 **************************************************/
template<typename value_type>
struct hash_table_node
{
public:
    typedef hash_table_node<value_type>*    link_type;

    value_type   value;
    link_type    next;

    hash_table_node(link_type next = nullptr) : value(), next(next) {}

    hash_table_node(const value_type& value, link_type next = nullptr) : value(value), next(next) {}
};

/**************************************** 哈希表迭代器 **************************************************/
template<typename traits>
class hash_table;

template<typename traits>
class hash_table_const_iterator
{
public:
    friend class hash_table<traits>;

    typedef GHYSTL::forward_iterator_tag                iterator_category;
    typedef typename traits::value_type                 value_type;
    typedef const value_type&                           reference;
    typedef const value_type*                           pointer;
    typedef std::ptrdiff_t                              difference_type;

    typedef size_t                                      size_type;
    typedef hash_table_const_iterator<traits>           self;
    typedef const hash_table<traits>*                   hash_table_link;
    typedef hash_table_node<value_type>*                link_type;

protected:
    link_type               cur; // 哈希表节点
    hash_table_link         table; // 哈希表

public:
    hash_table_const_iterator(link_type cur = nullptr, hash_table_link table = nullptr) : cur(cur), table(table) {}

    hash_table_const_iterator(const self &x) : cur(x.cur), table(x.table) {}

    self& operator=(const self &x) {
        cur = x.cur;
        table = x.table;
        return (*this);
    }

    bool operator==(const self &it) const { return (cur == it.cur); }

    bool operator!=(const self &it) const { return (!(operator==(it))); }

    reference operator*() const { return (cur->value); }

    pointer operator->() const { return (&(operator*())); }

    self &operator++() {
        const link_type old = cur;
        cur = cur->next;

        if (!cur) {
            size_type index = table->get_bucket_num(table->get_key(old->value));
            for (size_t n = table->buckets.size(); !cur && ++index != n;)
                cur = table->buckets[index];
        }
        return (*this);
    }

    self operator++(int) {
        self tmp = *this;
        ++*this;
        return (tmp);
    }

};


template <typename traits>
class hash_table_iterator : public hash_table_const_iterator<traits>
{
public:
    typedef hash_table_const_iterator<traits>       base_type;

    typedef GHYSTL::forward_iterator_tag            iterator_category;
    typedef typename traits::value_type             value_type;
    typedef value_type&                             reference;
    typedef value_type*                             pointer;
    typedef std::ptrdiff_t                          difference_type;

    typedef size_t                                   size_type;
    typedef hash_table_iterator<traits>              self;
    typedef const hash_table<traits>*                hash_table_link;
    typedef hash_table_node<value_type>*             link_type;

    hash_table_iterator(const link_type cur = nullptr, const hash_table_link table = nullptr)
        : base_type(cur, table) {}

    hash_table_iterator(const self &x) : base_type(x.cur, x.table) {}

    self &operator=(const self &x) {
        this->cur = x.cur;
        this->table = x.table;
    }

    bool operator==(const self &it) const { return (this->cur == it.cur); }

    bool operator!=(const self &it) const { return (!(operator==(it))); }

    reference operator*() const { return (this->cur->value); }

    pointer operator->() const { return (&(operator*())); }

    self &operator++() {
        const link_type old = this->cur;
        this->cur = this->cur->next;

        if (!this->cur) {
            size_type index = this->table->get_bucket_num(this->table->get_key(old->value));
            for (size_t n = this->table->buckets.size(); !(this->cur) && ++index != n;)
                this->cur = this->table->buckets[index];
        }
        return (*this);
    }

    self operator++(int) {
        self tmp = *this;
        ++*this;
        return (tmp);
    }
};


template <typename traits>
class hash_table_const_local_iterator
{
public:
    friend class hash_table<traits>;

    typedef GHYSTL::forward_iterator_tag                  iterator_category;
    typedef typename traits::value_type                   value_type;
    typedef const value_type&                             reference;
    typedef const value_type*                             pointer;
    typedef std::ptrdiff_t                                difference_type;

    typedef size_t                                            size_type;
    typedef hash_table_const_local_iterator<traits>           self;
    typedef hash_table_node<value_type>*                      link_type;

protected:
    link_type cur;

public:

    hash_table_const_local_iterator(link_type cur = nullptr) : cur(cur) {}

    hash_table_const_local_iterator(const self &x) : cur(x.cur) {}

    bool operator==(const self &it) const { return (cur == it.cur); }

    bool operator!=(const self &it) const { return (!(operator==(it))); }

    reference operator*() const { return (cur->value); }

    pointer operator->() const { return (&(operator*())); }

    self &operator++() {
    cur = cur->next;
    return (*this);
    }

    self operator++(int) {
    self tmp = *this;
    ++*this;
    return (tmp);
    }
};


template <typename traits>
class hash_table_local_iterator : public hash_table_const_local_iterator<traits>
{
public:
    friend class hash_table<traits>;

    typedef GHYSTL::forward_iterator_tag            iterator_category;
    typedef typename traits::value_type             value_type;
    typedef value_type&                             reference;
    typedef value_type*                             pointer;
    typedef std::ptrdiff_t                          difference_type;

    typedef size_t                                       size_type;
    typedef hash_table_const_local_iterator<traits>      base_type;
    typedef hash_table_local_iterator<traits>            self;
    typedef hash_table_node<value_type>*                 link_type;

    hash_table_local_iterator(link_type cur = nullptr) : base_type(cur) {}

    hash_table_local_iterator(const self &x) : base_type(x) {}

    bool operator==(const self &it) const { return (this->cur == it.cur); }

    bool operator!=(const self &it) const { return (!(operator==(it))); }

    reference operator*() const { return (this->cur->value); }

    pointer operator->() const { return (&(operator*())); }

    self &operator++() {
        this->cur = this->cur->next;
        return (*this);
    }

    self operator++(int) {
        self tmp = *this;
        ++*this;
        return (tmp);
    }
};


/****************************************   哈希表  **************************************************/
static const int num_primes = 31;  // 质数
static const size_t prime_list[num_primes] = {
    7, 19, 37, 53, 97, 193,
    389, 769, 1543, 3079, 6151, 12289,
    24593, 49157, 98317, 196613, 393241, 786433,
    1572869, 3145739, 6291469, 12582917, 25165843, 50331653,
    100663319, 201326611, 402653189, 805306457, 1610612741, 3221225437ul, 4294967291ul
};

inline size_t next_prime(size_t n){
    const size_t* first = prime_list;
    const size_t* last = first + num_primes;
    const size_t* pos = GHYSTL::lower_bound(first, last, n);
    return pos == last ? *(last - 1) : *pos;
}

template<typename traits>
class hash_table{
public:
    friend class hash_table_const_iterator<traits>;
    friend class hash_table_iterator<traits>;

    typedef typename traits::allocator_type         allocator_type;
    typedef typename traits::Hash_Function          hasher;
    typedef typename traits::EquaL_Key              equal_key;
    typedef typename traits::key_type               key_type;
    typedef typename traits::value_type             value_type;

    typedef value_type*             pointer;
    typedef const value_type*       const_pointer;
    typedef value_type&             reference;
    typedef const value_type&       const_reference;
    typedef size_t                  size_type;
    typedef std::ptrdiff_t          difference_type;

    typedef typename allocator_type::template rebind<hash_table_node<value_type>>::other node_alloc; // 哈希表节点的空间配置
    
    typedef hash_table_const_iterator<traits>                                           const_iterator; // 哈希表节点 和 哈希表
    typedef typename If<is_same<key_type, value_type>::value, const_iterator,
                        hash_table_iterator<traits>>::type                              iterator;
    typedef hash_table_const_local_iterator<traits>                                     const_local_iterator; // 只有哈希表节点,代表某个桶
    typedef typename If<is_same<key_type, value_type>::value, const_local_iterator,
                    hash_table_local_iterator<traits>>::type                            local_iterator;

    enum{
        is_multi = traits::is_multi
    };

    typedef hash_table_node<value_type>*        link_type;
    typedef hash_table<traits>                  self;
    typedef GHYSTL::vector<link_type>           container;

    typedef GHYSTL::pair<iterator, bool>                                Pair_IB;
    typedef GHYSTL::pair<local_iterator, local_iterator>                Pair_II;
    typedef GHYSTL::pair<const_local_iterator, const_local_iterator>    Pair_CC;
    typedef GHYSTL::pair<link_type, link_type>                          Pair_LL;

private:
    hasher          hash;       // 哈希算法
    equal_key       equals;     // 键值比较
    container       buckets;    // 哈希表
    size_type       num_elements; // 元素个数

public:
/*-----------------------------------------------构造 析构函数-------------------------------------------------*/
    hash_table() : hash(), equals(), num_elements(0) { init_buckets(0); }

    explicit hash_table(size_t n) : hash(), equals(), num_elements(0) {
        init_buckets(n);
    }

    hash_table(size_t n, const hasher &hash) : hash(hash), equals(), num_elements(0) {
        init_buckets(n);
    }

    hash_table(const hasher &hash, const equal_key &equals) : hash(hash), equals(equals), num_elements(0) {
        init_buckets(0);
    }

    hash_table(const size_t n, const hasher &hash, const equal_key &equals) : hash(hash), equals(equals), num_elements(0) {
        init_buckets(n);
    }

    hash_table(const self &x) : buckets(x.buckets.size(), nullptr), hash(x.hash),
                                    equals(x.equals), num_elements(x.num_elements)
    {
        for (size_t i = 0; i != buckets.size(); ++i)
            for (link_type cur = x.buckets[i]; cur; cur = cur->next)
                buckets[i] = create_node(cur->value, buckets[i]); // 头插法
    }

    hash_table(self &&x) : num_elements(x.num_elements), hash(std::move(x.hash)),
                            equals(std::move(x.equals)), buckets(std::move(x.buckets)) { }

    hash_table& operator=(const hash_table& rhs){
        if(this != &rhs){
            hash_table tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    hash_table& operator=(hash_table&& rhs) noexcept {
        hash_table tmp(std::move(rhs));
        swap(tmp);
        return *this;
    }

    ~hash_table() { clear(); }
/*------------------------------------------------- 常规函数 ---------------------------------------------------*/

    size_type bucket(const key_type &k) const { return (get_bucket_num(k)); }

    size_type count(const key_type& k) const {
        size_type counter = 0;
        size_t n = get_bucket_num(k);

        for (link_type cur = buckets[n]; cur; cur = cur->next)
            if (equals(get_key(cur->value), k))
                ++counter;
        
        return (counter);
    }

    void swap(self& x) noexcept {
        GHYSTL::swap(hash, x.hash);
        GHYSTL::swap(equals, x.equals);
        GHYSTL::swap(num_elements, x.num_elements);
        buckets.swap(x.buckets);
    }

    const_iterator find(const key_type &k) const {
        return (const_iterator(find_imple(k), this));
    }

    iterator find(const key_type &k) { return (iterator(find_imple(k), this)); }

    size_type bucket_count() const { return (buckets.size()); }

    size_type bucket_size(const size_type n) const {
        return (elements_in_bucket(n));
    }

    size_type max_size() const { return (prime_list[num_primes - 1]); }

    void rehash(const size_type new_n) { resize(new_n); }

    void reserve(const size_type new_n) { resize(new_n); }

    Pair_II equal_range(const key_type &key) {
        Pair_LL ret = equal_range_imple(key);
        return (Pair_II(local_iterator(ret.first), local_iterator(ret.second)));
    }

    Pair_CC equal_range(const key_type &key) const {
        Pair_LL ret = equal_range_imple(key);
        return (Pair_CC(const_local_iterator(ret.first),
                            const_local_iterator(ret.second)));
    }

    allocator_type get_allocator() const { return (allocator_type()); }

    hasher hash_function() const { return (hash); }

    equal_key key_eq() const { return (equals); }

/*-----------------------------------------------迭代器-------------------------------------------------*/
    iterator begin() {
        if(empty()) return end();

        size_t i = 0;
        for(const size_t len = buckets.size(); i != len && !buckets[i]; ) ++i;
        return iterator(buckets[i], this);
    }

    iterator end() {
        return iterator(nullptr, this);
    }

    local_iterator begin(size_type index) {
        return local_iterator(buckets[index]);
    }

    local_iterator end(size_type index) { return iterator(nullptr); }

    const_iterator cbegin() const {
        if (empty()) return (cend());
        size_t i = 0;

        for (const size_t len = buckets.size(); i != len && (!buckets[i]);) ++i;
        return (const_iterator(buckets[i], this));
    }

    const_local_iterator cbegin(size_type index) const  {
        return (const_local_iterator(buckets[index]));
    }

    const_local_iterator cend(size_type index) const {
        return (const_local_iterator(nullptr));
    }

    const_iterator cend() const { return (const_iterator(nullptr, this)); }

    const_iterator begin() const { return (cbegin()); }
    const_iterator end() const { return (cend()); }
    const_local_iterator begin(size_type index) const { return (cbegin(index)); }
    const_local_iterator end(size_type index) const { return (cend(index)); }

/*----------------------------------------------- 容量 -------------------------------------------------*/

    size_type size() const { return (num_elements); }

    bool empty() const { return (!size()); }

    // 装入因子
    float load_factor() const noexcept{ // 每个桶平均有几个元素
        return ((float)size() / (float)bucket_count());
    }

/*----------------------------------------------- erase -------------------------------------------------*/

    size_type erase(const key_type& k){
        size_t n = get_bucket_num(k);
        link_type cur = buckets[n];
        size_type count = 0;

        for(link_type tmp = cur; cur && equals(get_key(cur->value), k); tmp = cur){
            --num_elements;
            cur = cur->next;
            ++count;
            destroy_and_free_node(tmp);
        }

        buckets[n] = cur;

        if(cur){
            for(link_type next = cur->next; next; next = cur->next){
                if(equals(get_key(next->value), k)){
                    ++count;
                    --num_elements;
                    cur->next = next->next;
                    destroy_and_free_node(next);
                }
                else
                    cur = next;
            }
        }

        return count;
    }

    void erase(const_iterator x){
        size_t n = get_bucket_num(get_key(*x));
        link_type cur = buckets[n];
        link_type tar = x.cur;

        if(cur == tar)
            buckets[n] = x.cur->next;
        else{
            for(; cur->next != tar; )
                cur = cur->next;
            cur->next = tar->next; // 和 x 节点断链
        }
        --num_elements;
        destroy_and_free_node(tar);
    }

    void erase(const_iterator first, const_iterator last){
        for(; first != last; )
            erase(first++);
    }

    void clear(){
        const size_t len = buckets.size();
        for (size_t i = 0; i != len; ++i) {
            for (link_type cur = buckets[i], next; cur; cur = next) {
                next = cur->next;
                destroy_and_free_node(cur);
            }
        }

        memset(&buckets[0], 0, len);
        num_elements = 0;
    }

    /*------------------------------------------- 插入 emplace -----------------------------------------------*/

    template <bool multi = is_multi>
    enable_if_t<!multi, Pair_IB> emplace(value_type &&val) {
        return (emplace_imple(std::forward<value_type>(val)));
    }

    template <bool multi = is_multi, typename... types>
    enable_if_t<!multi, Pair_IB> emplace(types &&... args) {
        return (emplace_imple(std::forward<types>(args)...));
    }

    template <bool multi = is_multi>
    enable_if_t<multi, iterator> emplace(value_type &&val) {
        return (emplace_imple(std::forward<value_type>(val)));
    }

    template <bool multi = is_multi, typename... types>
    enable_if_t<multi, iterator> emplace(types &&... args) {
        return (emplace_imple(std::forward<types>(args)...));
    }


    // emplace_hint 在指定位置插入元素, 因为 hash_table 要根据 key 插入, 所以这里不提供指定的位置
    template <bool multi = is_multi>
    enable_if_t<!multi, Pair_IB> emplace_hint(const_iterator, value_type &&val)  {
        return (emplace_imple(std::forward<value_type>(val)));
    }

    template <bool multi = is_multi, typename... types>
    enable_if_t<!multi, Pair_IB> emplace_hint(const_iterator, types &&... args) {
        return (emplace_imple(std::forward<types>(args)...));
    }

    template <bool multi = is_multi>
    enable_if_t<multi, iterator> emplace_hint(const_iterator, value_type &&val) {
        return (emplace_imple(std::forward<value_type>(val)));
    }

    template <bool multi = is_multi, typename... types>
    enable_if_t<multi, iterator> emplace_hint(const_iterator, types &&... args) {
        return (emplace_imple(std::forward<types>(args)...));
    }

/*------------------------------------------- 插入 insert -----------------------------------------------*/

    template <bool multi = is_multi, enable_if_t<!multi, int> = 0>
    Pair_IB insert(const value_type &val) {
        return (emplace(val));
    }

    template <bool multi = is_multi, enable_if_t<!multi, int> = 0>
    Pair_IB insert(value_type &&val) {
        return (emplace(std::forward<value_type>(val)));
    }

    template <bool multi = is_multi, enable_if_t<multi, int> = 0>
    iterator insert(const value_type &val) {
        return (emplace(val));
    }

    template <bool multi = is_multi, enable_if_t<multi, int> = 0>
    iterator insert(value_type &&val) {
        return (emplace(std::forward<value_type>(val)));
    }

    // 在指定位置插入元素, 同 emplace_hint
    template <bool multi = is_multi, enable_if_t<multi, int> = 0>
    iterator insert(const_iterator, value_type &&val) {
        return (emplace(std::forward<value_type>(val)));
    }

    template <bool multi = is_multi, enable_if_t<!multi, int> = 0>
    iterator insert(const_iterator, value_type &&val) {
        // 不是重复元素的时候 返回的是 Pair_IB
        return (emplace(std::forward<value_type>(val)).first);
    }

    template <bool multi = is_multi, enable_if_t<multi, int> = 0>
    iterator insert(const_iterator, const value_type &val) {
        return (emplace(val));
    }

    template <bool multi = is_multi, enable_if_t<!multi, int> = 0>
    iterator insert(const_iterator, const value_type &val) {
        // 不是重复元素的时候 返回的是 Pair_IB
        return (emplace(val).first);
    }

    template <typename IIter>
    void insert(IIter first, IIter last) {
        
        for (; first != last; ++first){
            insert(*first);
        }     
    }

    void insert(const std::initializer_list<value_type> &lst) {
        insert(lst.begin(), lst.end());
    }

private:
    /*-------------------------------------------------- 底层操作 ----------------------------------------------------*/
    // 把 first2 接到 first1 后面
    void splice_after(link_type first1, link_type first2) const {
        link_type tar = first2->next;
        first2->next = tar->next;
        tar->next = first1->next;
        first1->next = tar;
    }

    Pair_LL equal_range_imple(const key_type& key) const {
        link_type first = find_imple(key);
        if(!first) return Pair_LL(nullptr, nullptr);

        link_type cur = first;
        link_type next = cur->next;

        for(; next && equals(key, get_key(next->value)); next = cur->next)
            cur = next;

        if(next){
            link_type before = next;
            for(next = before->next; next; next = before->next){
                if(equals(key, get_key(next->value))){
                    splice_after(cur, before);
                    cur = cur->next;
                }
                else
                    before = next;
            }
        }

        return Pair_LL(first, cur);
    }

    template<typename ... types>
    link_type create_node(types&& ... args){
        link_type node = node_alloc::allocate();
        node_alloc::construct(node, std::forward<types>(args)...);
        return node;
    }

    void destroy_and_free_node(link_type node) {
        node_alloc::destroy(node);
        node_alloc::deallocate(node);
    }

    void init_buckets(const size_type n) {
        const size_type n_buckets = next_prime(n);
        buckets.resize(n_buckets, nullptr);
    }

    size_t get_bucket_num(const key_type &key, const size_t n) const {
        return hash(key) % n;
    }

    size_type get_bucket_num(const key_type &key) const {
        return get_bucket_num(key, buckets.size());
    }

    size_type elements_in_bucket(const size_type n) const {
        size_type counter = 0;
        for (link_type cur = buckets[n]; cur; cur = cur->next)
            ++counter;
        return (counter);
    }

    link_type find_imple(const key_type& key) const {
        size_t n = get_bucket_num(key);
        link_type cur = buckets[n];

        for(; cur && !equals(key, get_key(cur->value)); )
            cur = cur->next;
        return cur;
    }

    const key_type& get_key(const value_type& val) const {
        return traits::ExtractKey(val);
    }

    iterator make_iter(const_iterator& citer) const {
        return iterator(citer.cur, citer.table);
    }

    // 不是 multi
    template<typename ... types>
    Pair_IB insert_unique_noresize(types && ... args ){
        link_type node = create_node(std::forward<types>(args)...);
        const value_type& val = node->value;

        size_type n = get_bucket_num(get_key(val));
        link_type first = buckets[n];

        for(link_type cur = first; cur; cur = cur->next){
            if(equals(get_key(cur->value), get_key(val))){
                destroy_and_free_node(node);
                return (Pair_IB(iterator(cur, this), false));
            }
        }

        node->next = first;
        buckets[n] = node;
        ++num_elements;
        return Pair_IB(iterator(node, this), true);
    }

    template <typename value_type>
    Pair_IB insert_unique_noresize(value_type &&val) {
        size_type n = get_bucket_num(get_key(val));
        link_type first = buckets[n];

        for (link_type cur = first; cur; cur = cur->next)
            if (equals(get_key(cur->value), get_key(val)))
                return (Pair_IB(iterator(cur, this), false));

        link_type node = create_node(std::forward<value_type>(val), first);
        buckets[n] = node;
        ++num_elements;
        return (Pair_IB(iterator(node, this), true));
    }

    template <typename... types>
    iterator insert_equal_noresize(types &&... args) {
        link_type node = create_node(std::forward<types>(args)...);
        const value_type &val = node->value;
        const size_type n = get_bucket_num(get_key(val));

        node->next = buckets[n];
        buckets[n] = node;
        ++num_elements;
        return iterator(node, this);
    }

    template <typename value_type>
    iterator insert_equal_noresize(value_type &&val) {
        const size_type n = get_bucket_num(get_key(val));
        link_type tmp = create_node(std::forward<value_type>(val), buckets[n]);

        buckets[n] = tmp;
        ++num_elements;
        return iterator(tmp, this);
    }

    template <bool multi = is_multi, typename... types>
    enable_if_t<!multi, Pair_IB> emplace_imple(types &&... args) {
        resize(num_elements + 1);
        return (insert_unique_noresize(std::forward<types>(args)...));
    }

    template <bool multi = is_multi>
    enable_if_t<!multi, Pair_IB> emplace_imple(value_type &&val) {
        resize(num_elements + 1);
        return (insert_unique_noresize(std::forward<value_type>(val)));
    }

    template <bool multi = is_multi, typename... types>
    enable_if_t<multi, iterator> emplace_imple(types &&... args)  {
        resize(num_elements + 1);
        return (insert_equal_noresize(std::forward<types>(args)...));
    }

    template <bool multi = is_multi>
    enable_if_t<multi, iterator> emplace_imple(value_type &&val) {
        resize(num_elements + 1);
        return (insert_equal_noresize(std::forward<value_type>(val)));
    }

    void resize(const size_type new_n){
        const size_type old_n = buckets.size();

        if(new_n > old_n){
            const size_type n = next_prime(new_n);
            
            if(n > old_n){
                container tmp(n, nullptr);
                size_type new_bucket_num;
                for(size_type i = 0; i != old_n; ++i){
                    for(link_type first = buckets[i]; first; first = buckets[i]){
                        new_bucket_num = get_bucket_num(get_key(first->value), n); // 重新 hash
                        buckets[i] = first->next; // 从原来的 hash_table 断链
                        first->next = tmp[new_bucket_num]; // 头插法
                        tmp[new_bucket_num] = first;
                    }
                }

                buckets.swap(tmp);
            }
        }
    }

}; // end of hash_table


template <typename traits>
inline void swap(hash_table<traits> &left, hash_table<traits> &right) noexcept {
    left.swap(right);
}

template <typename traits> // 比较两个 hash_table 元素是否一样
inline bool _hash_element_equal(const hash_table<traits> &left,
                                const hash_table<traits> &right, true_type)
{
    typedef typename hash_table<traits>::const_iterator Iter;
    typedef typename hash_table<traits>::const_local_iterator LIter;
    typedef GHYSTL::pair<LIter, LIter> Pair_LII;
    Pair_LII Lrange;
    Pair_LII Rrange;

    for (Iter first = left.cbegin(), last = right.cend(); first != last; ++first)
    {
        Lrange = left.equal_range(traits::ExtractKey(*first));
        Rrange = right.equal_range(traits::ExtractKey(*first));

        // is_permutation 元素值是否是另一个容器元素值组合, 元素可以是无序的
        if (!GHYSTL::is_permutation(Lrange.first, Lrange.second, Rrange.first, Rrange.second))
            return (false);
    }

    return (true);
}


template <typename traits>
inline bool _hash_element_equal_not_multi(const hash_table<traits> &left,
                                          const hash_table<traits> &right, false_type)
{
    // 这个方法 key_type 和 value_type 不是一种类型
    typedef typename hash_table<traits>::const_iterator Iter;
    Iter ret;
    Iter last2 = right.cend();

    // 在哈希表 1 中的元素的 value 是否存在在于哈希表 2 中
    // 因为 key_type 和 value_type 不是同一种类型, 所以要单独比较一次 key
    for (Iter cur = left.cbegin(), last1 = left.cend(); cur != last1; ++cur)
    {
        ret = right.find(traits::ExtractKey(*cur));
        if (ret == last2 || (!(traits::ExtractData(*ret) == traits::ExtractData(*cur))))
            return (false);
    }

    return (true);
}

template <typename traits>
inline bool _hash_element_equal_not_multi(const hash_table<traits> &left,
                                          const hash_table<traits> &right, true_type)
{
    // 这个方法保证了 key_type 和 value_type 是一种类型
    typedef typename hash_table<traits>::const_iterator Iter;
    Iter last2 = right.cend();

    // 在哈希表 1 中的元素的 value 是否存在在于哈希表 2 中
    // 因为 key_type 和 value_type 是一种类型, 所以 hash 出来的 key 也一样
    for (Iter cur = left.cbegin(), last1 = left.cend(); cur != last1; ++cur)
        if (right.find(traits::ExtractKey(*cur)) == last2)
            return (false);

    return (true);
}


template <typename traits>
inline bool _hash_element_equal(const hash_table<traits> &left,
                                const hash_table<traits> &right, false_type)
{
  return (GHYSTL::_hash_element_equal_not_multi( left, right,
                        GHYSTL::is_same<typename traits::key_type, typename traits::value_type>()));
}

template <typename traits>
inline bool operator==(const hash_table<traits> &left,
                       const hash_table<traits> &right)
{
  return (left.size() == right.size() &&
          GHYSTL::_hash_element_equal(
              left, right, GHYSTL::integral_constant<bool, traits::is_multi>()));
}

template <typename traits>
inline bool operator!=(const hash_table<traits> &left,
                       const hash_table<traits> &right)
{
  return (!(left == right));
}

} // namespace GHYSTL
#endif