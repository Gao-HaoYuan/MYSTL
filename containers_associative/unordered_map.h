/**
 * @file unordered_map.h
 * @author ghy (ghy_mike@163.com)
 * @brief unordered_set + unordered_multiset 的实现
 * 
 * @version 1.0
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _UNORDERED_MAP_H_
#define _UNORDERED_MAP_H_

#include "hash_table.h"

namespace GHYSTL
{

/*------------------------------------------------ unordered_map_traits -------------------------------------------------------*/
template <typename key_type_, typename data_type_, 
            typename Hash_Function_,
                typename Equal_Key_, typename Alloc, bool is_multi_>
struct unordered_map_traits
{
    typedef key_type_                                       key_type;
    typedef data_type_                                      data_type;
    typedef GHYSTL::pair<const key_type, data_type>         value_type;
    typedef Hash_Function_                                  Hash_Function;
    typedef Equal_Key_                                      EquaL_Key;
    typedef Alloc                                           allocator_type;

    enum
    {
        is_multi = is_multi_
    };

    template <typename type1, typename type2>
    inline static const key_type &ExtractKey(const GHYSTL::pair<type1, type2> &pr) {
        return (pr.first);
    }

    template <typename type1, typename type2>
    inline static const data_type &ExtractData(const std::pair<type1, type2> &pr) {
        return (pr.second);
    }
};

/*------------------------------------------------ unordered_map -------------------------------------------------------*/

template <typename key_type_, typename data_type_,
          typename Hash_Function = GHYSTL::hash<key_type_>, // 仿函数中定义的方法
          typename Equal_Key = GHYSTL::equal_to<key_type_>,
          typename Alloc = GHYSTL::allocator<GHYSTL::pair<const key_type_, data_type_>>>
class unordered_map
    : public hash_table<unordered_map_traits<key_type_, data_type_, Hash_Function,
                                            Equal_Key, Alloc, false>>
{
public:
    typedef hash_table<unordered_map_traits<key_type_, data_type_, Hash_Function,
                                            Equal_Key, Alloc, false>>                      base_type;

    typedef data_type_                            data_type;
    typedef data_type                             mapped_type;
    typedef typename base_type::key_type          key_type;
    typedef typename base_type::value_type        value_type;
    typedef typename base_type::hasher            hasher;
    typedef typename base_type::equal_key         equal_key;
    typedef typename base_type::size_type         size_type;

    typedef unordered_map<key_type, data_type, Hash_Function, Equal_Key, Alloc>     self;

    typedef typename base_type::difference_type           difference_type;
    typedef typename base_type::pointer                   pointer;
    typedef typename base_type::const_pointer             const_pointer;
    typedef typename base_type::reference                 reference;
    typedef typename base_type::const_reference           const_reference;
    typedef typename base_type::iterator                  iterator;
    typedef typename base_type::const_iterator            const_iterator;
    typedef typename base_type::const_local_iterator      const_local_iterator;
    typedef typename base_type::local_iterator            local_iterator;

    unordered_map() : base_type() { }

    explicit unordered_map(size_type n) : base_type(n) {}

    unordered_map(size_type n, const hasher &hf) : base_type(n, hf) {}

    unordered_map(const size_type n, const hasher &hf, const equal_key &eql)
        : base_type(n, hf, eql) {}

    unordered_map(const std::initializer_list<value_type>& lst) : base_type()
    {
        base_type::insert(lst.begin(), lst.end());
    }

    template <typename IIter>
    unordered_map(IIter first, IIter last) : base_type()
    {
        base_type::insert(first, last);
    }

    template <typename IIter>
    unordered_map(IIter first, IIter last, size_type n) : base_type(n)
    {
        base_type::insert(first, last);
    }

    template <typename IIter>
    unordered_map(IIter first, IIter last, size_type n, const hasher &hf)
        : base_type(n, hf)
    {
        base_type::insert(first, last);
    }

    template <typename IIter>
    unordered_map(IIter first, IIter last, const size_type n, const hasher &hf,
                    const equal_key &eql)
        : base_type(n, hf, eql)
    {
        base_type::insert(first, last);
    }

    unordered_map(const self &x) : base_type(x) {}

    self &operator=(const self &x)
    {
        base_type::operator=(x);
        return (*this);
    }

    self &operator=(self &&x)
    {
        base_type::operator=(std::move(x));
        return (*this);
    }

    self &operator=(const std::initializer_list<value_type> &lst)
    {
        base_type::clear();
        base_type::insert(lst.begin(), lst.end());
        return (*this);
    }

    data_type &operator[](const key_type &k)
    {
        // 返回值是 Pair_IB  
        return (*((base_type::insert({k, data_type()})).first)).second;
    }

    data_type &at(const key_type &k)
    {
    // 返回值是 Pair_IB  
    return (*((base_type::insert({k, data_type()})).first)).second;
    }


  void swap(self &x) noexcept { base_type::swap(x); }
};

template <typename key_type, typename data_type, typename Hash_Function,
          typename Equal_Key, typename Alloc>
void swap(unordered_map<key_type, data_type, Hash_Function, Equal_Key, Alloc> & left,
             unordered_map<key_type, data_type, Hash_Function, Equal_Key, Alloc> & right) noexcept
{
    left.swap(right);
}


/*---------------------------------------------- unordered_multimap --------------------------------------------------------*/
template <typename key_type_, typename data_type_,
          typename Hash_Function = GHYSTL::hash<key_type_>,
          typename Equal_Key = GHYSTL::equal_to<key_type_>,
          typename Alloc = GHYSTL::allocator<std::pair<const key_type_, data_type_>>>
class unordered_multimap
    : public hash_table<unordered_map_traits<key_type_, data_type_, Hash_Function,
                                            Equal_Key, Alloc, true>>
{
public:
    typedef hash_table<unordered_map_traits<key_type_, data_type_, Hash_Function,
                                            Equal_Key, Alloc, true>>                       base_type;

    typedef data_type_                                data_type;
    typedef data_type                                 mapped_type;
    typedef typename base_type::key_type              key_type;
    typedef typename base_type::value_type            value_type;
    typedef typename base_type::hasher                hasher;
    typedef typename base_type::equal_key             key_equal;
    typedef typename base_type::size_type             size_type;

    typedef unordered_multimap<key_type, data_type, Hash_Function, Equal_Key, Alloc>      self;

    typedef typename base_type::difference_type           difference_type;
    typedef typename base_type::pointer                   pointer;
    typedef typename base_type::const_pointer             const_pointer;
    typedef typename base_type::reference                 reference;
    typedef typename base_type::const_reference           const_reference;
    typedef typename base_type::iterator                  iterator;
    typedef typename base_type::const_iterator            const_iterator;

    unordered_multimap() : base_type() {}

    explicit unordered_multimap(size_type n) : base_type(n) {}

    unordered_multimap(size_type n, const hasher &hf) : base_type(n, hf) {}

    unordered_multimap(const size_type n, const hasher &hf, const key_equal &eql)
        : base_type(n, hf, eql) {}

    unordered_multimap(const std::initializer_list<value_type>& lst) : base_type()
    {
        base_type::insert(lst.begin(), lst.end());
    }

    template <typename IIter>
    unordered_multimap(IIter first, IIter last) : base_type()
    {
        base_type::insert(first, last);
    }

    template <typename IIter>
    unordered_multimap(IIter first, IIter last, size_type n) : base_type(n)
    {
        base_type::insert(first, last);
    }

    template <typename IIter>
    unordered_multimap(IIter first, IIter last, size_type n, const hasher &hf)
        : base_type(n, hf)
    {
        base_type::insert(first, last);
    }

    template <typename IIter>
    unordered_multimap(IIter first, IIter last, size_type n, const hasher &hf,
                        const key_equal &eql)
        : base_type(n, hf, eql)
    {
        base_type::insert(first, last);
    }

    unordered_multimap(const self &x) : base_type(x) {}

    self &operator=(const self &x) {
        base_type::operator=(x);
        return (*this);
    }

    self &operator=(self &&x) {
        base_type::operator=(std::move(x));
        return (*this);
    }

    self &operator=(const std::initializer_list<value_type> &lst) {
        base_type::clear();
        base_type::insert(lst.begin(), lst.end());
        return (*this);
    }

    data_type &operator[](const key_type &k) {
        return (*((base_type::insert({k, data_type()})).first)).second;
    }

    data_type &at(const key_type &k) {
        return (*((base_type::insert({k, data_type()})).first)).second;
    }

    void swap(self &x) noexcept { base_type::swap(x); }
};

template <typename key_type, typename data_type, typename Hash_Function,
          typename Equal_Key, typename Alloc>
void swap(unordered_multimap<key_type, data_type, Hash_Function, Equal_Key, Alloc> &left,
          unordered_multimap<key_type, data_type, Hash_Function, Equal_Key, Alloc> &right) noexcept
{
    left.swap(right);
}

































    
} // namespace GHYSTL
#endif