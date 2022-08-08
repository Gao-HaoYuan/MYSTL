/**
 * @file function_adapters.h
 * @author ghy (ghy_mike@163.com)
 * @brief 仿函数的配接器
 * 
 * @version 0.1
 * @date 2022-08-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _FUNCTION_ADAPTERS_H_
#define _FUNCTION_ADAPTERS_H_

#include "../functional/functional.h"

namespace GHYSTL{

    /*------------------------------------------ 对一元函数的运算结果 逻辑非 运算 ---------------------------------*/
    template<class Unary_Fun>
    class  unary_negate : 
            public GHYSTL::unary_function<typename Unary_Fun::argumet_type, bool> 
    {
    protected:
        Unary_Fun pred;
    public:
        // 显示声明，其他的构造器不能隐式调用这个构造器
        // exlpicit 只能用于 修饰只有一个参数的构造函数
        explicit unary_negate(const Unary_Fun& x) : pred(x) {} 

        bool operator()(const typename Unary_Fun::argumet_type& x) const {
            return !pred(x);
        }
    };
    
    // 返回一个一元函数 逻辑非 运算对象
    template<class Unary_Fun>
    inline unary_negate<Unary_Fun> not_unary(const Unary_Fun& pred){
        return GHYSTL::unary_negate<Unary_Fun>(pred);
    }



    /*------------------------------------------ 对二元函数的运算结果 逻辑非 运算 ---------------------------------*/
    template<class Binary_Fun>
    class  binary_negate :
                public GHYSTL::binary_function<typename Binary_Fun::first_argument_type, 
                                                typename Binary_Fun::second_argument_type, bool>
    {
    protected:
        Binary_Fun pred;
    public:
        explicit binary_negate(const Binary_Fun& x) : pred(x) {}

        bool operator()(const typename Binary_Fun::first_argument_type& x, 
                        const typename Binary_Fun::second_argument_type& y) const {
            return !pred(x, y);
        }
    };
    
    template<class Binary_Fun>
    inline binary_negate<Binary_Fun> not_binary(const Binary_Fun& pred){
        return GHYSTL::binary_negate<Binary_Fun>(pred);
    }


    /*------------------------------------------ 二元函数绑定第一个参数的值，传入另一个随机变量  ---------------------------------*/
    template<class Binary_Fun>
    class bind_first : public GHYSTL::unary_function<typename Binary_Fun::first_argument_type, 
                                                        typename Binary_Fun::result_type>
    {
    protected:
        Binary_Fun op;
        typename Binary_Fun::first_argument_type value;
    public:
        bind_first(const Binary_Fun& x, 
                    const typename Binary_Fun::first_argument_type& y) : op(x), value(y){}

        typename Binary_Fun::result_type operator()(const typename Binary_Fun::second_argument_type& x) const {
            return op(value, x);
        }
    };

    template<class Binary_Fun, class value_type>
    inline bind_first<Binary_Fun> binder_first(const Binary_Fun& pred, const value_type& first){
        typedef typename Binary_Fun::first_argument_type arg1_type;
        return GHYSTL::bind_first<Binary_Fun>(pred, static_cast<arg1_type>(first));
    } 
    

    /*------------------------------------------ 二元函数绑定第二个参数的值，传入另一个随机变量  ---------------------------------*/
    template<class Binary_Fun>
    class bind_second : public GHYSTL::unary_function<typename Binary_Fun::second_argument_type, 
                                                        typename Binary_Fun::result_type>
    {
    protected:
        Binary_Fun op;
        typename Binary_Fun::second_argument_type value;
    public:
        bind_second(const Binary_Fun& x, 
                        const typename Binary_Fun::second_argument_type& y) : op(x), value(y){}

        typename Binary_Fun::result_type operator()(const typename Binary_Fun::first_argument_type& x) const {
            return op(x, value);
        }
    };

    template<class Binary_Fun, class value_type>
    inline bind_second<Binary_Fun> binder_second(const Binary_Fun& pred, const value_type& second){
        typedef typename Binary_Fun::second_argument_type arg2_type;
        return GHYSTL::bind_second<Binary_Fun>(pred, static_cast<arg2_type>(second));
    }

    /*------------------------------------------ 组合两个一元函数  ---------------------------------*/
    template<class Unary1, class Unary2>
    class unary_compose : public GHYSTL::binary_function<typename Unary1::argument_type, 
                                                        typename Unary2::argument_type, 
                                                        typename Unary1::result_type>
    {
    protected:
        Unary1 op1;
        Unary2 op2;
    public:
        unary_compose(const Unary1& x, const Unary2& y) : op1(x), op2(y){}

        typename Unary1::result_type operator()(const typename Unary2::argument_type& x) const {
            return op1(op2(x)); // 合成两个操作
        }
    };

    template<class Unary1, class Unary2>
    inline unary_compose<Unary1, Unary2> compose_unary(const Unary1& x, const Unary2& y){
        return GHYSTL::unary_compose<Unary1, Unary2>(x, y);
    }

    /*------------------------------------------ f(x)、g(y), h(x, y) 组合成 h(f(x), g(y))  ---------------------------------*/
    template<class Unary1, class Unary2, class Binary>
    class binary_compose : public binary_function<typename Unary1::argument_type,
                                                typename Unary2::argument_type, typename Unary1::result_type>
    {
    protected:
        Unary1 op1;
        Unary2 op2;
        Binary op3;
    public:
        binary_compose(const Unary1& x, const Unary2& y, const Binary& z) : op1(x), op2(y), op3(z){}

        typename Unary1::result_type operator()(const typename Unary1::argument_type& x,
                                            const typename Unary2::argument_type& y) const {
            return op3(op1(x), op2(y));
        }
    };

    template<class Unary1, class Unary2, class Binary>
    inline GHYSTL::binary_compose<Unary1, Unary2, Binary> compose_binary(const Unary1& x, const Unary2& y, const Binary& z){
        return GHYSTL::binary_compose<Unary1, Unary2, Binary>(x, y, z);
    }

    /*------------------------------------------ 把关于 某个类型的函数 通过一元函数 转换成另一个类型  ---------------------------------*/

    // 可以实现 类型 转换
    template<typename value_type, typename result_type>
    class pointer_unary : public GHYSTL::unary_function<value_type, result_type>
    {
    protected:
        result_type (*ptr)(value_type); // 函数指针， ptr 指向参数为（value_type）的函数
    public:
        explicit pointer_unary(result_type (*x)(value_type)) : ptr(x){} // x 是一个函数指针，指向参数为 value_type 类型的函数
        pointer_unary() = default;

        result_type operator()(value_type x) const {
            return ptr(x); // ptr现在是函数， x是传进来的参数，计算后返回 result_type 类型
        }
    };

    template<typename value_type, typename result_type>
    inline GHYSTL::pointer_unary<value_type, result_type> get_pointer_unary(result_type (*x)(value_type)){
        return pointer_unary<value_type, result_type>(x);
    }

    /*------------------------------------------ 把关于 某两个类型的函数 通过二元函数 转换成另一个类型  ---------------------------------*/
    template<typename value_type1, typename value_type2, typename result_type>
    class pointer_binary : public GHYSTL::binary_function<value_type1, value_type2, result_type>
    {
    protected:
        result_type (*ptr)(value_type1, value_type2);
    public:
        explicit pointer_binary(result_type (*x)(value_type1, value_type2)) : ptr(x){}
        pointer_binary() = default;

        result_type operator()(const value_type1& x, const value_type2& y) const{
            return ptr(x, y);
        }
    };

    template<typename value_type1, typename value_type2, typename result_type>
    inline GHYSTL::pointer_binary<value_type1, value_type2, result_type> 
    get_pointer_binary(result_type (*x)(value_type1, value_type2)){
        return GHYSTL::pointer_binary<value_type1, value_type2, result_type>(x);
    }
}   
#endif