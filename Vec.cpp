#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm> //std::min
#include "MultiDimensionalArray.hpp"

template<typename Elem, std::size_t DIM>
std::ostream& operator<< (std::ostream& out, const SubDimensionalArray<Elem, DIM>& d) {
	out<<"[";
	for(std::size_t i=0;i<d.length();++i){
  		out << d[i];
  		if(i!=d.length()-1)
  			out<<",";
  	}
  	out<<"]";
  	return out;
}

template<typename Elem, std::size_t DIM>
std::ostream& operator<< (std::ostream& out, const MultiDimensionalArray<Elem, DIM>& d) {
	out<<"[";
	for(std::size_t i=0;i<d.length();++i){
  		out << d[i];
  		if(i!=d.length()-1)
  			out<<",";
  	}
  	out<<"]";
  	return out;
}

std::ostream& operator<<(std::ostream& out, const ContainerLengths& c){
	out<<"[";
	for(std::size_t i=0;i<c.dimension();i++){
		out << c[i];
		if(i!=c.dimension()-1)
			out<<",";
	}
	out<<"]";
	return out;
}

//---------------------------------------------------------------------------

int main(){
	try{
		MultiDimensionalArray<int, 3> array({2,3,2}, 5);
		std::cout<<array<<std::endl;
		array.resize({1,1,10});
		std::cout<<array<<std::endl;
	} catch(const std::exception& e){
		std::cerr << std::endl << "\033[31m" << "*** " << e.what() << " ***" << "\033[0m" << std::endl<<std::endl;;
		return 1;
	}
	return 0;
}
