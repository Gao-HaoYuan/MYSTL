/*
* Vector的实现
*/

#ifndef GHYSTL_VECTOR_H_
#define GHYSTL_VECTOR_H_

#include "allocator.h"
#include "algo_base.h"
#include "uninitialized.h"
#include "excetdef.h"
#include "type_traits.h"
#include "iterator.h"

namespace GHYSTL{

    // --------------- vector的 const_iterator ---------------------------------

    template<typename value_type_>
    class vector_const_iterator : public GHYSTL::iterator<random_access_iterator_tag, value_type_>
    {
    public:
        typedef GHYSTL::random_access_iterator_tag  iterator_category;
        typedef value_type_                         value_type;
        typedef const value_type                    *pointer;
        typedef const value_type                    &reference;
        typedef ptrdiff_t                           difference_type;

        typedef GHYSTL::true_type                   memory_copy_tag;
        typedef vector_const_iterator<value_type>   self;

        vector_const_iterator(value_type *cur = nullptr) : cur(cur){ }

        vector_const_iterator(const self &x) : cur(x.cur){ }// 拷贝构造函数

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

        self operator++(int){
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
        typedef GHYSTL::random_access_iterator_tag  iterator_category;
        typedef value_type_                         value_type;
        typedef const value_type                    *pointer;
        typedef const value_type                    &reference;
        typedef ptrdiff_t                           difference_type;
        
        typedef GHYSTL::true_type                   memory_copy_tag;
        typedef vector_iterator<value_type>         self;

        typedef vector_const_iterator<value_type>   base_type;

        // 通过 using 调用父类成员变量
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

        self operator++(int){
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

    template<class T, class Alloc = alloc<T>>
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

        typedef vector<value_type, Alloc> self;

    protected:
        typedef simple_alloc<value_type, Alloc> data_allocator; // 定义空间配置器
        iterator start;     //目前使用空间头
        iterator finish;    //目前使用空间尾
        iterator end_of_storage; //目前可用空间的尾

        
        // --------------- 指定位置插入元素 ---------------

        void insert_aux(iterator position, const T& value);
         

        // --------------- 释放内存和析构元素 ---------------
        void deallocate(){
            if(start){
                data_allocator::deallocate(start, end_of_storage - start);
            }
        }

        // --------------- 分配内存和构造元素 ---------------
        void fill_initailize(size_type n, const T& value){
            start = allocate_and_fill(n,value);
            finish = start + n;
            end_of_storage = finish;
        }

        iterator allocate_and_fill(size_type n, const T& x){
            //获取空间
            iterator result = data_allocator::allocate(n);
            //在获取到的空间上创建对象
            GHYSTL::uninitialized_fill_n(result, n, x);
            return result;
        }

    public:
        // --------------- vector的操作函数 ---------------
        iterator begin() const { return start;}
        iterator end() const { return finish;}
        size_type size() const { return static_cast<size_type>(finish - start);}
        size_type capacity() const { return static_cast<size_type>(end_of_storage - start);}
        bool empty() const { return start == finish;}
        reference operator[](difference_type n) { 
            GHTSTL_DEBUG(n < size());
            return *(start + n);
        }
        
        // 构造函数
        vector():start(0), finish(0), end_of_storage(0){};

        explicit vector(const size_type n) { fill_initailize(n, T());}

        vector(size_type n, const T& value) { fill_initialize(n, value);}
        vector(int       n, const T& value) { fill_initialize(n, value);} 
        vector(long      n, const T& value) { fill_initialize(n, value);}

        vector(const std::initializer_list<value_type> &v){
            auto start_v = v.begin();
            auto end_v = v.end();
            size_type n = v.size();
            fill_initailize(n, T());
            finish = GHYSTL::copy(start_v, end_v, start);
            end_of_storage = finish;
        }

        vector(const self &x){ // 拷贝构造函数
            start = data_allocator::allocate(x.size());
            finish = GHYSTL::uninitialized_copy(x.start, x.finish, start);
            end_of_storage = start + x.size();
        }

        vector(self &&x) noexcept : start(x.start), 
                                    finish(x.finish), 
                                    end_of_storage(x.end_of_storage){ // 移动构造函数
            x.start = x.finish = x.end_of_storage = 0;
        }

        ~vector(){
            GHYSTL::destroy(start, finish);
            deallocate();
        }

        self &operator=(self x){ //赋值  和 移动运算符
            swap(x);
            return (*this)
        }

        // 可以修改
        self &operator=(const std::initializer_list<value_type> &v){
            auto start_v = v.begin();
            auto end_v = v.end();
            size_type n = v.size();
            fill_initailize(n, T());
            finish = GHYSTL::copy(start_v, end_v, start);
            end_of_storage = finish;
            return *this;
        }

        void shrink_to_fit(){
            if(end_of_storage != finish && !empty()){
                size_t len = this->size();
                pointer ptr = data_allocator::allocate(len);
                GHYSTL::uninitialized_copy(start, fin);
            }
        }




    };

    template<class T, class Alloc>
    void vector<T, Alloc>::insert_aux(iterator position, const T& x){
        if(finish != end_of_storage){
            // 还有备用空间
            // 在备用空间开始处创建一个对象，并以vector的最后一个对象为初始值
            GHYSTL::construct(finish, *(finish-1));
            ++finish;
            T x_copy = x;
            // [position, finish-2] 的元素 整体 向后移动一位
            GHYSTL::copy_backward(position, finish-2, finish-1);
            *position = x_copy;
        }else{ 
            // 没有备用空间
            
            const size_type old_size = size();
            // 内存不足则申请原来两倍的新内存
            const size_type new_size = old_size != 0 ? 2 * old_size : 1;

            iterator new_start = data_allocator::allocate(new_size);
            iterator new_finish = new_start;
            try{
                new_finish = GHYSTL::uninitialized_copy(start, position, new_start);
                GHYSTL::construct(new_finish, x);
                new_finish++;
                new_finish = GHYSTL::uninitialized_copy(position, finish, new_finish);
            }catch(...){
                GHYSTL::destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, new_size);
                throw;
            }
            
            GHYSTL::destroy(begin(), end());//销毁原来的vector内存空间
            deallocate();

            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + new_size;
        }
    }
    
    
}
#endif