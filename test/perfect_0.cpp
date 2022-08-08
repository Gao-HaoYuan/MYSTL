#include <iostream>

using namespace std;


struct tmp
{
   using diff = ptrdiff_t;
};


template<typename T>
struct tmp_traits
{
    using diff = typename T::diff;
};

template<typename T>
struct tmp_traits<T*>
{
    using diff = typename T::diff;
};

template<typename T>
inline typename tmp_traits<T>::diff*
    diff(const T&){
       return static_cast<typename tmp_traits<T>::diff*>(0);
    }

void tes(ptrdiff_t &n){
    n=n+10;
}

int main(){
    tmp *test;

    using D = typename tmp_traits<tmp>::diff;
    D p=0;

    tes(p);

    cout<<p<<endl;

    return 0;
}