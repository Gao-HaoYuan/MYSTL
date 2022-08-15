/**
 * @file map.h
 * @author ghy (ghy_mike@163.com)
 * @brief map的实现
 * 
 * @version 1.0
 * @date 2022-08-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _MAP_H_
#define _MAP_H_

#include "rb_tree.h"


namespace GHYSTL{

template<typename key_type_, typename data_type, typename Compare, typename Alloc, bool is_multi_>
struct map_traits
{
    typedef key_type_                                           key_type;
    typedef GHYSTL::pair<const key_type, data_type>             value_type;  // key 值不能修改， 这里是 const 类型
    typedef Compare                                             key_compare;
    typedef Alloc                                               allocator_type;

    enum{
        is_multi = is_multi_
    };

    struct value_compare{
        friend map_traits<key_type, data_type, key_compare, Alloc, is_multi>;

        bool operator()(const value_type& left, const value_type& right) const {
            return key_compare()(left.first, right.first);
        }
    };


    template<typename type_1, typename type_2>
    static const key_type& key_of_value(const GHYSTL::pair<type_1, type_2>& pr){
        return pr.first;
    }
};

template<typename key_type_, typename data_type_,
            typename Compare = GHYSTL::less<key_type_>,
            typename Alloc = GHYSTL::allocator<GHYSTL::pair<const key_type_, data_type_>>>
class map
    : public rb_tree<map_traits<key_type_, data_type_, Compare, Alloc, false>>
{
protected:
    typedef key_type_                                           key_type;
    typedef data_type_                                          data_type;
    typedef GHYSTL::pair<const key_type, data_type>             value_type;
    typedef Compare                                             Key_Compare;

    typedef rb_tree<map_traits<key_type, data_type, Key_Compare, Alloc, false>>             base_type; //红黑树
    typedef map<key_type, data_type, Key_Compare>                                           self;

    typedef typename base_type::pointer                     pointer;
    typedef typename base_type::const_pointer               const_pointer;
    typedef typename base_type::reference                   reference;
    typedef typename base_type::const_reference             const_reference;
    typedef typename base_type::iterator                    iterator;
    typedef typename base_type::const_iterator              const_iterator;
    typedef typename base_type::reverse_iterator            reverse_iterator;
    typedef typename base_type::const_reverse_iterator      const_reverse_iterator;
    typedef typename base_type::size_type                   size_type;
    typedef typename base_type::difference_type             difference_type;

public: 
    map() : base_type() {}

    explicit map(const Compare& cmp) : base_type(cmp) {}

    template<typename Iter>
    map(Iter first, Iter last) : base_type() {
        base_type::insert(first, last);
    }

    template <typename InputIterator>
    map(InputIterator first, InputIterator last, const Compare &comp) : base_type(comp) {
        base_type::insert(first, last);
    }

    map(const std::initializer_list<value_type> &lst) : base_type() { 
        base_type::insert(lst.begin(), lst.end()); 
    }

    map(const std::initializer_list<value_type> &lst, const Compare &comp)
        : base_type(comp) {
            base_type::insert(lst.begin(), lst.end());
    }

    map(const self &x) : base_type(x) {}

    map(self &&x) : base_type(std::move(x)) {}

    self &operator=(const self &x) {
        base_type::operator=(x);
        return (*this);
    }

    self &operator=(self &&x) {
        base_type::operator=(std::move(x));
        return (*this);
    }

    data_type &operator[](const key_type &k) {
        // inset 返回值是 pair_ib 类型
        return (*((base_type::insert({k, data_type()})).first)).second;
    }

    data_type at(const key_type &k) {
        // inset 返回值是 pair_ib 类型
        return (*((base_type::insert({k, data_type()})).first)).second;
    }

    void swap(self &x) noexcept { base_type::swap(x); }
};


template <typename key_type, typename data_type, typename Compare, typename Alloc>
inline void swap(map<key_type, data_type, Compare, Alloc> &left,
                 map<key_type, data_type, Compare, Alloc> &right) noexcept
{
  left.swap(right);
}

/*-------------------------------------------------- multi_map -------------------------------------------------------------*/

template <typename key_type_, typename data_type_,
          typename Compare = GHYSTL::less<key_type_>,
          typename Alloc = GHYSTL::allocator<GHYSTL::pair<const key_type_, data_type_>>>
class multi_map 
    : public rb_tree<map_traits<key_type_, data_type_, Compare, Alloc, true>> // 继承了 rb_tree 中的 公有方法
{
public:
    typedef key_type_                                         key_type;
    typedef data_type_                                        data_type;
    typedef GHYSTL::pair<const key_type, data_type>           value_type;
    typedef Compare                                           key_Compare;

    typedef rb_tree<map_traits<key_type, data_type, key_Compare, Alloc, true>>    base_type;
    typedef multi_map<key_type, data_type, key_Compare>                           self;

    typedef typename base_type::pointer                           pointer;
    typedef typename base_type::const_pointer                     const_pointer;
    typedef typename base_type::reference                         reference;
    typedef typename base_type::const_reference                   const_reference;
    typedef typename base_type::iterator                          iterator;
    typedef typename base_type::const_iterator                    const_iterator;
    typedef typename base_type::reverse_iterator                  reverse_iterator;
    typedef typename base_type::const_reverse_iterator            const_reverse_iterator;
    typedef typename base_type::size_type                         size_type;
    typedef typename base_type::difference_type                   difference_type;

    multi_map() : base_type() {}

    explicit multi_map(const Compare &comp) : base_type(comp) {}

    template <typename Iter>
    multi_map(Iter first, Iter last) : base_type() {
        base_type::insert(first, last);
    }

    template <typename InputIterator>
    multi_map(InputIterator first, InputIterator last, const Compare &comp) : base_type(comp) {
        base_type::insert(first, last);
    }

    multi_map(const std::initializer_list<value_type> &lst) : base_type() { 
        base_type::insert(lst.begin(), lst.end()); 
    }

    multi_map(const std::initializer_list<value_type> &lst, const Compare &comp)
        : base_type(comp) {
            base_type::insert(lst.begin(), lst.end());
    }


    multi_map(const self &x) : base_type(x) {}

    multi_map(self &&x) : base_type(std::move(x)) {}

    self &operator=(const self &x) {
        base_type::operator=(x);
        return (*this);
    }

    self &operator=(self &&x) {
    base_type::operator=(std::move(x));
        return (*this);
    }

    data_type &operator[](const key_type &k) {
        // 返回值是 iterator
        return (*(insert({k, data_type()}))).second;
    }

    data_type at(const key_type &k) {
        // 返回值是 iterator
        return (*(insert({k, data_type()}))).second;
    }

    void swap(self &x) noexcept { base_type::swap(x); }
};

template <typename key_type, typename data_type, typename Compare, typename Alloc>
inline void swap(
    multi_map<key_type, data_type, Compare, Alloc> &left,
    multi_map<key_type, data_type, Compare, Alloc> &right) noexcept
{
  left.swap(right);
}

} // namespace GHYSTL

#endif