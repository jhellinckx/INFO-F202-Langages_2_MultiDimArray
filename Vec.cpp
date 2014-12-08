#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm> //std::min
#include "DimensionalArray.hpp"
#include "BoundedDimensionalArray.hpp"

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
std::ostream& operator<< (std::ostream& out, const DimensionalArray<Elem, DIM>& d) {
	out<<"[";
	for(std::size_t i=0;i<d.length();++i){
  		out << d[i];
  		if(i!=d.length()-1)
  			out<<",";
  	}
  	out<<"]";
  	return out;
}


//---------------------------------------------------------------------------

int main(){
	try{
		BoundedDimensionalArray<int, 3> array({{1,2},{3,4},{5,7}}, 5);
		array[1][3][7]=2;
		std::cout<<array<<std::endl;
	} catch(const std::exception& e){
		std::cerr << std::endl << "\033[31m" << "*** " << e.what() << " ***" << "\033[0m" << std::endl<<std::endl;;
		return 1;
	}
	return 0;
}
