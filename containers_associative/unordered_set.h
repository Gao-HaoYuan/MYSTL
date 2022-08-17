/**
 * @file unordered_set.h
 * @author ghy (ghy_mike@163.com)
 * @brief unordered_map + unordered_multimap  的实现
 * 
 * @version 1.0
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _UNORDERED_SET_H_
#define _UNORDERED_SET_H_

#include "hash_table.h"

namespace GHYSTL{

/*------------------------------------------------ unordered_set_traits -------------------------------------------------------*/

template<typename key_type_, typename Hash_Function_, typename Equal_Key_,
            typename Alloc, bool is_multi_>
struct unordered_set_traits
{
    typedef key_type_           key_type;
    typedef key_type            value_type;
    typedef Hash_Function_      Hash_Function;
    typedef Equal_Key_           EquaL_Key;
    typedef Alloc               allocator_type;


    enum{
        is_multi = is_multi_
    };

    static const key_type& ExtractKey(const value_type& val) { return val; }
};

/*------------------------------------------------ unordered_set -------------------------------------------------------*/

template <typename key_type_, typename Hash_Function = GHYSTL::hash<key_type_>,
          typename Equal_Key = GHYSTL::equal_to<key_type_>,
          typename Alloc = GHYSTL::allocator<key_type_>>
class unordered_set
    : public hash_table<unordered_set_traits<key_type_, Hash_Function, Equal_Key, Alloc, false>>
{
public:
    typedef hash_table<unordered_set_traits<key_type_, Hash_Function, Equal_Key, Alloc, false>>          base_type;
    typedef unordered_set<key_type_, Hash_Function, Equal_Key>                                            self;

    typedef typename base_type::key_type              key_type;
    typedef typename base_type::value_type            value_type;
    typedef typename base_type::hasher                hasher;
    typedef typename base_type::equal_key             key_equal;
    typedef typename base_type::size_type             size_type;

    typedef typename base_type::difference_type       difference_type;
    typedef typename base_type::const_pointer         pointer;
    typedef typename base_type::const_pointer         const_pointer;
    typedef typename base_type::const_reference       reference;
    typedef typename base_type::const_reference       const_reference;

    typedef typename base_type::const_iterator            iterator;
    typedef typename base_type::const_iterator            const_iterator;
    typedef typename base_type::const_local_iterator      const_local_iterator;
    typedef typename base_type::local_iterator            local_iterator;

public:
    unordered_set() : base_type() {}

    explicit unordered_set(size_type n) : base_type(n) {}

    unordered_set(size_type n, const hasher &hf) : base_type(n, hf) {}

    unordered_set(const size_type n, const hasher &hf, const key_equal &eql) : base_type(n, hf, eql) {}

    template <typename IIter>
    unordered_set(IIter first, IIter last) : base_type()
    {
      base_type::insert(first, last);
    }

    unordered_set(const std::initializer_list<value_type> &lst)
        : unordered_set(lst.begin(), lst.end()) { }

    template <typename IIter>
    unordered_set(IIter first, IIter last, size_type n) : base_type(n)
    {
        base_type::insert(first, last);
    }

    template <typename IIter>
    unordered_set(IIter first, IIter last, size_type n, const hasher &hf)
        : base_type(n, hf)
    {
        base_type::insert(first, last);
    }

    template <typename IIter>
    unordered_set(IIter first, IIter last, const size_type n, const hasher &hf,
                  const key_equal &eql)
        : base_type(n, hf, eql)
    {
        base_type::insert(first, last);
    }

    unordered_set(const self &x) : base_type(x) {}

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

    void swap(self &x) noexcept { base_type::swap(x); }
};

template <typename key_type, typename Hash_Function, typename Equal_Key, typename Alloc>
void swap(
    unordered_set<key_type, Hash_Function, Equal_Key, Alloc> &left,
    unordered_set<key_type, Hash_Function, Equal_Key, Alloc> &right) noexcept
{
    left.swap(right);
}

/*------------------------------------------------ unordered_multiset -------------------------------------------------------*/

template <typename key_type_, typename Hash_Function = GHYSTL::hash<key_type_>,
          typename Equal_Key = GHYSTL::equal_to<key_type_>,
          typename Alloc = GHYSTL::allocator<key_type_>>
class unordered_multiset
    : public hash_table<unordered_set_traits<key_type_, Hash_Function, Equal_Key, Alloc, true>>
{
public:
    typedef hash_table<unordered_set_traits<key_type_, Hash_Function, Equal_Key, Alloc, true>>          base_type;
    typedef unordered_multiset<key_type_, Hash_Function, Equal_Key>                                     self;

    typedef typename base_type::key_type              key_type;
    typedef typename base_type::value_type            value_type;
    typedef typename base_type::hasher                hasher;
    typedef typename base_type::equal_key             key_equal;
    typedef typename base_type::size_type             size_type;

    typedef typename base_type::difference_type       difference_type;
    typedef typename base_type::const_pointer         pointer;
    typedef typename base_type::const_pointer         const_pointer;
    typedef typename base_type::const_reference       reference;
    typedef typename base_type::const_reference       const_reference;

    typedef typename base_type::const_iterator            iterator;
    typedef typename base_type::const_iterator            const_iterator;
    typedef typename base_type::const_local_iterator      const_local_iterator;
    typedef typename base_type::local_iterator            local_iterator;

public:
    unordered_multiset() : base_type() {}

    explicit unordered_multiset(size_type n) : base_type(n) {}

    unordered_multiset(size_type n, const hasher &hf) : base_type(n, hf) {}

    unordered_multiset(const size_type n, const hasher &hf, const key_equal &eql) : base_type(n, hf, eql) {}

    template <typename IIter>
    unordered_multiset(IIter first, IIter last) : base_type()
    {
      base_type::insert(first, last);
    }

    unordered_multiset(const std::initializer_list<value_type> &lst)
        : unordered_multiset(lst.begin(), lst.end()) { }

    template <typename IIter>
    unordered_multiset(IIter first, IIter last, size_type n) : base_type(n)
    {
        base_type::insert(first, last);
    }

    template <typename IIter>
    unordered_multiset(IIter first, IIter last, size_type n, const hasher &hf)
        : base_type(n, hf)
    {
        base_type::insert(first, last);
    }

    template <typename IIter>
    unordered_multiset(IIter first, IIter last, const size_type n, const hasher &hf,
                  const key_equal &eql)
        : base_type(n, hf, eql)
    {
        base_type::insert(first, last);
    }

    unordered_multiset(const self &x) : base_type(x) {}

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

    void swap(self &x) noexcept { base_type::swap(x); }
};

template <typename key_type, typename Hash_Function, typename Equal_Key, typename Alloc>
void swap(
    unordered_multiset<key_type, Hash_Function, Equal_Key, Alloc> &left,
    unordered_multiset<key_type, Hash_Function, Equal_Key, Alloc> &right) noexcept
{
    left.swap(right);
}

}

#endif