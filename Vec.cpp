	#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm> //std::min
#include "MultiDimensionalArray.hpp"
#include "BoundedMultiDimensionalArray.hpp"

template<typename Elem, std::size_t DIM>
std::ostream& operator<< (std::ostream& out, const SubDimensionalArray<Elem, DIM>& d) {
	out<<"[";
	for(std::size_t i=0;i<d.getLength();++i){
  		out << d[i];
  		if(i!=d.getLength()-1)
  			out<<",";
  	}
  	out<<"]";
  	return out;
}

template<typename Elem, std::size_t DIM>
std::ostream& operator<< (std::ostream& out, const MultiDimensionalArray<Elem, DIM>& d) {
	out<<"[";
	for(std::size_t i=0;i<d.getLength();++i){
  		out << d[i];
  		if(i!=d.getLength()-1)
  			out<<",";
  	}
  	out<<"]";
  	return out;
}

std::ostream& operator<<(std::ostream& out, const ContainerLengths& c){
	out<<"[";
	for(std::size_t i=0;i<c.getDimension();i++){
		out << c[i];
		if(i!=c.getDimension()-1)
			out<<",";
	}
	out<<"]";
	return out;
}

//---------------------------------------------------------------------------

int main(){
	try{
		BoundedMultiDimensionalArray<int, 1> array({1,10},5,10);
		std::cout<<array<<std::endl;
		array.resize(10);
		std::cout<<array<<std::endl;
	} catch(const std::exception& e){
		std::cerr << std::endl << "\033[31m" << "*** " << e.what() << " ***" << "\033[0m" << std::endl<<std::endl;;
		return 1;
	}
	return 0;
}
