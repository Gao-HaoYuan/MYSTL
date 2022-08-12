#include "string.h"
#include "stdio.h"

class String
{
public:
    String(char* x = nullptr){
        data = new char[100];
        strcpy(data, x);
    }   

    String(const String& x){
        data = new char[100];
        strcpy(data, x.data);
    }

    void print(){
        printf("%s", data);
    }

private:
    char* data;
};

// 类内可以直接使用，类外不可以
// 类声明时。类中包含本类得对象，该对象可以直接调用这个私有变量

int main(){
    char* str = new char[2];
    str[0] = '1';
    str[1] = '2';

    String s(str);

    String b(s);


    b.print();
    
    return 0;
}