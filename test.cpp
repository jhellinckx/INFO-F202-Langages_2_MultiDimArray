#include <iostream>
#include <array>

class A{
public:
	virtual char lower(){return 'a';}
	char test(){return lower();}
};

class B: public A{
public:
	virtual char lower(){return 'b';}
};


int main(){
	B b;
	std::cout<<b.test()<<std::endl;
	
}

