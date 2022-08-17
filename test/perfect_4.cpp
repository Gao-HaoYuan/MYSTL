
#include <iostream>

using namespace std;

// 基类
struct Animal {
    void eat(int i){
        cout << "A";
    } // 函数
    // sleep() 函数
};


//派生类
struct Dog : public Animal {

    using Animal::eat;

    void eat(int i, int c) {
        cout << "V";
    }
};

struct two { char a; char b; };
template <class U> static two test(...){
    cout << "11" << endl;
    return {0,0};
}

int main(){ 
    
    // Dog D;
    // D.Animal::eat(1);
    // D.eat(1);

    test<int>(1,2,3,"ad");
    
    return 0;
}