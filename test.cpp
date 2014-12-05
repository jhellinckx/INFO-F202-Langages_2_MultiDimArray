#include <iostream>
#include <array>

void foo(int* a){
	int b =1;
	a = &b;
}
int main(){
	int* a;
	foo(a);
	std::cout<<*a<<std::endl;
}

