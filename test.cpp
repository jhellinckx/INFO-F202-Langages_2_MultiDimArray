#include <iostream>
#include <array>

class A{
public:
	int* _a;
public:
	A(int* a) : _a(a){}
	virtual ~A(){}
};
template<std::size_t DIM>
class B: public A{
public:
	int _b[DIM];
	B(): A(_b) {
		for(int i=0;i<DIM;++i) _b[i]=i;
	} 
};
int main(){
	B<5> b=B<5>();
	std::cout<<b._a[2]<<std::endl;
}

