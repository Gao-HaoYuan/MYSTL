#include <iostream>

using namespace std;

template<typename ... type>
void my_cout(type ... arg){
    cout<<"yes";
}

template<typename ... types>
void print(types && ... args){ // 模板可变参数就是把多个参数传递到一个函数里执行
    my_cout(std::forward<types>(args)...);
}


// 参数模板递归调用传进来的的参数的时候，需要提供一个空函数
// 因为下面 Print 的执行流程是 Print(0, "color", 103.1),  Print("color", 103.1)  Print(103.1)  Print()
// 最终需要一个空的函数来接受
// tuple 的实现就是基于上述思想 （递归）

void Print() {}

template<typename T, typename ... Types>
void Print (const T& firstArg, const Types&... args)
{
    std::cout << "arg: " << firstArg << std::endl;
    Print(args...);
}


int main(){
    Print(0, "color", 103.1);
    return 0;
}