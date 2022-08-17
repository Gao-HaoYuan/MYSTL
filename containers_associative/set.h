/**
 * @file set.h
 * @author ghy (ghy_mike@163.com)
 * @brief set + multi_set 的实现
 * 
 * @version 1.0
 * @date 2022-08-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _SET_H_
#define _SET_H_

#include "rb_tree.h"

namespace GHYSTL
{
 
template <typename key_type_, typename key_compare_, typename Alloc, bool is_multi_>
struct set_traits {
  typedef key_type_                     key_type;
  typedef key_type                      value_type;
  typedef key_compare_                  key_compare;
  typedef key_compare                   value_compare;
  typedef Alloc                         allocator_type;

  enum { is_multi = is_multi_ };

  static const key_type& key_of_value(const key_type& key) { return (key); }

};

template <typename key_type_, 
            typename Compare = GHYSTL::less<key_type_>,
            typename Alloc = GHYSTL::allocator<key_type_>>
class set : public rb_tree<set_traits<key_type_, Compare, Alloc, false>> {
public:
    typedef key_type_                             key_type;
    typedef key_type                              value_type;
    typedef Compare                               key_compare;

    typedef rb_tree<set_traits<key_type, key_compare, Alloc, false>>      base_type;

    typedef typename base_type::const_pointer                 pointer;
    typedef typename base_type::const_pointer                 const_pointer;
    typedef typename base_type::const_reference               reference;
    typedef typename base_type::const_reference               const_reference;
    typedef typename base_type::const_iterator                iterator;           // set 只提供常迭代器, 即 set::iterator 是常迭代器
    typedef typename base_type::const_iterator                const_iterator;
    typedef typename base_type::const_reverse_iterator        reverse_iterator;
    typedef typename base_type::const_reverse_iterator        const_reverse_iterator;
    typedef typename base_type::size_type                     size_type;
    typedef typename base_type::difference_type               difference_type;

    typedef set<key_type, Compare, Alloc>               self;
    typedef typename GHYSTL::pair<iterator, bool>          pair_ib;



    set() : base_type() {}

    explicit set(const Compare& comp) : base_type(comp) {}

    set(const self& x) : base_type(x) {}

    template <typename Iter>
    set(Iter first, Iter last) : base_type() {
        base_type::insert(first, last);
    }

    template <typename InputIterator>
    set(InputIterator first, InputIterator last, const Compare& comp)
        : base_type(comp) {
        base_type::insert(first, last);
    }

    set(const std::initializer_list<key_type>& lst, const Compare& comp)
        : set(lst.begin(), lst.end(), comp) {}

    set(const std::initializer_list<key_type>& lst)
        : set(lst.begin(), lst.end()) {}

    set(self&& x) : base_type(std::move(x)) {}

    self& operator=(const self& x) {
        base_type::operator=(x);
        return (*this);
    }

    self& operator=(self&& x) {
        base_type::operator=(std::move(x));
        return (*this);
    }

    void swap(self& x) noexcept { base_type::swap(x); }
};

template <typename key_type, typename Compare, typename Alloc>
inline void swap(set<key_type, Compare, Alloc>& left,
                 set<key_type, Compare, Alloc>& right) noexcept {
    left.swap(right);
}

/*----------------------------------------------- multi_set --------------------------------------------------------*/
template <typename key_type_, 
            typename Compare = GHYSTL::less<key_type_>,
            typename Alloc = GHYSTL::allocator<key_type_>>
class multi_set : public rb_tree<set_traits<key_type_, Compare, Alloc, true>> {
public:
    typedef key_type_                     key_type;
    typedef key_type                      value_type;
    typedef Compare                       key_compare;

    typedef rb_tree<set_traits<key_type, Compare, Alloc, true>>   base_type;

    typedef typename base_type::const_pointer                     pointer;
    typedef typename base_type::const_pointer                     const_pointer;
    typedef typename base_type::const_reference                   reference;
    typedef typename base_type::const_reference                   const_reference;
    typedef typename base_type::const_iterator                    iterator;
    typedef typename base_type::const_iterator                    const_iterator;
    typedef typename base_type::const_reverse_iterator            reverse_iterator;
    typedef typename base_type::const_reverse_iterator            const_reverse_iterator;
    typedef typename base_type::size_type                         size_type;
    typedef typename base_type::difference_type                   difference_type;

    typedef multi_set<key_type, key_compare, Alloc>                  self;
    typedef typename GHYSTL::pair<iterator, bool>                    pairib;

    multi_set() : base_type() {}

    explicit multi_set(const Compare& comp) : base_type(comp) {}

    multi_set(const self& x) : base_type(x) {}

    template <typename Iter>
    multi_set(Iter first, Iter last) : base_type() {
        base_type::insert(first, last);
    }

    template <typename InputIterator>
    multi_set(InputIterator first, InputIterator last, const Compare& comp)
        : base_type(comp) {
        base_type::insert(first, last);
    }

    multi_set(const std::initializer_list<key_type>& lst, const Compare& comp)
        : multi_set(lst.begin(), lst.end(), comp) {}

    multi_set(const std::initializer_list<key_type>& lst)
        : multi_set(lst.begin(), lst.end()) {}

    multi_set(self&& x) : base_type(std::move(x)) {}

    self& operator=(const self& x) {
        base_type::operator=(x);
        return (*this);
    }

    self& operator=(self&& x) {
        base_type::operator=(std::move(x));
        return (*this);
    }

    void swap(self& x) noexcept { base_type::swap(x); }
};

template <typename key_type, typename Compare, typename Alloc>
inline void swap(multi_set<key_type, Compare, Alloc>& left,
                multi_set<key_type, Compare, Alloc>& right) noexcept {
    left.swap(right);
}

} // namespace GHYSTL
#endif