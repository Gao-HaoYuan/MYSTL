
#include <iostream>

using namespace std;

template <typename val_type, val_type val>
struct xx
{
	static constexpr val_type value = val;

	typedef val_type value_type;
	// typedef integral_constant<value_type, val> type;

	constexpr operator value_type() const noexcept { return (value); }

	constexpr value_type operator()() const noexcept { return (value); }
};

template<typename T, typename Pointer=T*>
struct perfect_1
{
	typedef T value_type;
	typedef Pointer pointer;

	pointer t;
};



int main(){
    // xx<bool,true> t;
    // cout<< t <<endl;

	perfect_1<int> x;
	
	int a=10;
	x.t = &a;
	cout<< *x.t <<endl;

    return 0;
}