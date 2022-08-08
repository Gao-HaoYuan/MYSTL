/**
 * @file iterator_container_adapters.h
 * @author ghy (ghy_mike@163.com)
 * @brief  容器的配接器：前插入、后插入
 *          迭代器的配接器：反转迭代器（重载的 * 和 -> 不能修改成员变量）
 * 
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */


namespace GHYSTL{

     /*--------------------------------------------------------- 配接器 -------------------------------------------------------*/

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // back_insert_iterator实现部分,  只有提供了push_back()操作的容器才能使用back_insert_iterator 
    ///////////////////////////////////////////////////////////////////////////////////////////////

    template<class Container>
    class back_insert_iterator
    {
    protected:
        Container* container;
    public:
        typedef output_iterator_tag iterator_category;
        typedef void                value_type;
        typedef void                difference_type;
        typedef void                pointer;
        typedef void                reference;

        explicit back_insert_iterator(Container& x) : container(&x){}

        back_insert_iterator<Container>& operator=(const typename Container::value_type& value){
            container->push_back(value);
            return *this;
        }

        back_insert_iterator<Container>& operator*(){return *this;}

        // C++规定后缀调用需要有一个int型参数, 作为区分前缀与后缀调用的区别
        // 前缀 ++； 例如 ++i
        back_insert_iterator<Container>& operator++(){return *this;}
        // 后缀 ++； 例如 i++
        back_insert_iterator<Container>& operator++(int){return *this;}
    };

    template <class Container>
    inline back_insert_iterator<Container> back_insert(Container& x){
        return back_insert_iterator<Container>(x);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // front_insert_iterator实现部分, 只有提供了push_front()操作的容器才能使用front_insert_iterator  
    ///////////////////////////////////////////////////////////////////////////////////////////////

    template <class Container>
    class front_insert_iterator
    {
    protected:
        Container* container;
    public:
        typedef output_iterator_tag iterator_category;
        typedef void                value_type;
        typedef void                difference_type;
        typedef void                pointer;
        typedef void                reference;

        explicit front_insert_iterator(Container& x) : container(&x){}

        front_insert_iterator<Container>& operator=(const typename Container::value_type& value){
            container->push_front(value);
            return *this;
        }

        front_insert_iterator<Container>& operator*(){return *this;}
        front_insert_iterator<Container>& operator++(){return *this;}
        front_insert_iterator<Container>& operator++(int){return *this;}
    };
    
    template <class Container>
    inline output_iterator_tag iterator_category(const front_insert_iterator<Container>&)
    {
        return output_iterator_tag();
    }
    
    template <class Container>
    inline front_insert_iterator<Container> front_insert(Container& x){
        return front_insert_iterator<Container>(x);
    }

    ////////////////////////////////////////////////////////////////////////////////  
	// insert_iterator实现部分, 只有提供了insert操作的容器才能使用insert_iterator  
	////////////////////////////////////////////////////////////////////////////////

    template <class Container>
    class insert_iterator
    {
    protected:
        Container* container;
        typename Container::iterator iter;
    public:
        typedef output_iterator_tag iterator_category;
        typedef void                value_type;
        typedef void                difference_type;
        typedef void                pointer;
        typedef void                reference;

        insert_iterator(Container& x, typename Container::iterator i) : container(&x),iter(i){}

        insert_iterator<Container>& operator=(const typename Container::value_type& value){
            iter = container->insert(iter, value);
            ++iter;
            return *this;
        }

        insert_iterator<Container>& operator*(){return *this;}
        insert_iterator<Container>& operator++(){return *this;}
        insert_iterator<Container>& operator++(int){return *this;}
    };

    template <class Container>
    inline output_iterator_tag iterator_category(const insert_iterator<Container>&)
    {
        return output_iterator_tag();
    }

    template <class Container, class Iterator>
    inline insert_iterator<Container> insert(Container& x, Iterator i){
        typedef typename Container::iterator iter;
        return insert_iterator<Container>(x,iter(i));
    }
    
    ////////////////////////////////////////////////////////////////////////////////  
	// reverse_iterator实现部分  
	////////////////////////////////////////////////////////////////////////////////
    //    reverse_iterator使用的是BidirectionalIterator 
    //    所以要对operator *(), ++(), ++(int)都提供处理  
    //    同时因为是反向迭代器, 所以重载运算符的操作要特殊处理
    ////////////////////////////////////////////////////////////////////////////////

    template <class Iterator>
    class reverse_iterator
    {
    private:
        Iterator current; // 记录对应的正向迭代器
    public:
        // 反向迭代器的五种相应型别
        typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
        typedef typename iterator_traits<Iterator>::value_type        value_type;
        typedef typename iterator_traits<Iterator>::difference_type   difference_type;
        typedef typename iterator_traits<Iterator>::pointer           pointer;
        typedef typename iterator_traits<Iterator>::reference         reference;

        typedef Iterator                                              iterator_type;
        typedef reverse_iterator<Iterator>                            self;

    public:
        // 构造函数
        reverse_iterator() {}
        explicit reverse_iterator(iterator_type i) : current(i){}
        reverse_iterator(const self& rhs) : current(rhs.current){} //拷贝构造函数

        // 取出对应的正向迭代器
        iterator_type base() const {return current;}

        // 重载操作符, 实际对应正向迭代器的前一个位置
        reference operator*() const {
            auto tmp = current;
            // -- 运算符的优先级高于 解引用 *运算符的优先级
            return *--tmp;
        }

        pointer operator->() const {return &(operator*());}

        // 这个是反转类，所以 ++ 要变为 --
        self& operator++(){
            --current;
            // 返回的是对象的地址
            return *this;
        }

        self operator++(int){
            self tmp = *this;
            --current;
            return tmp;
        }

        self& operator--(){
            ++current;
            return *this;
        }

        self operator--(int){
            self tmp = *this;
            ++current;
            return tmp;
        }

        self& operator+=(difference_type n){
            current -= n;
            return *this;
        }

        self operator+(difference_type n) const
        {
            return self(current - n);
        }

        self& operator-=(difference_type n)
        {
            current += n;
            return *this;
        }

        self operator-(difference_type n) const
        {
            return self(current + n);
        }

        reference operator[](difference_type n) const
        {
            return (*(current + n));
        }
    };
    // 重载 operator-
    template<class Iterator>
    typename reverse_iterator<Iterator>::difference_type
    inline operator-(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs){
            return rhs.base() - lhs.base();
    }

    // 重载 比较操作符
    template<class Iterator>
    inline operator==(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs){
            return rhs.base() == lhs.base();
    }


    template<class Iterator>
    inline operator!=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs){
            return !(lhs == rhs);
    }

    template<class Iterator>
    inline operator<(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs){
            return lhs.base() < rhs.base();
    }
    
    template<class Iterator>
    inline operator<=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs){
            // 使用重载的 < 号
            return !(rhs < lhs);
    }

    template<class Iterator>
    operator>(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs){
            return rhs < lhs;
    }

    template<class Iterator>
    operator>=(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs){
            return !(lhs < rhs);
    }
}