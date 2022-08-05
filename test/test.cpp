#include "../allocator/allocator.h"

#include <iostream>
template<typename ... Args>
void Print(Args ... rest) {
    std::cout << sizeof...(rest) << std::endl;
}

void fun1(){
    Print();
    Print(1);
    Print(1, "ab");
    Print(1.0, "23", 4);
}

int main() {
   

   return 0;
}