#ifndef CONTAINERLENGTHS_HPP
#define CONTAINERLENGTHS_HPP

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <stdexcept>

namespace Constants{
	static const int DEFAULT_DIM = 1;
	static const int DEFAULT_DIM_LENGTH = 1;
}

/* Classe responsable des longueurs d'un conteneur */
template<std::size_t DIM>
class ContainerLengths{
	std::size_t _lengths[DIM];

public:
	/* Reçoit en paramètre une liste d'initialisation contenant la longueur de chaque 
	dimension (par ordre décroissant) et en fait une copie vers un vecteur */
	ContainerLengths(const std::initializer_list<std::size_t>& lengths){
		if(DIM!=lengths.size()) throw std::logic_error("List size not matching the container dimension");
		std::size_t lengths[DIM]; std::size_t i=0;
		/* On ordonne les longueurs par dimension croissante : 
		_lengths[0] == longueur de la dimension 1 */
		for(std::initializer_list<std::size_t>::iterator it = lengths.begin();it!=lengths.end();++it)
			_lengths[DIM-i++-1] = *it;
	}
	
	ContainerLengths(std::size_t size, std::size_t lengths[]){
		if(DIM!=size throw std::logic_error("Vector size not matching the container dimension");
		for(std::size_t i=0;i<DIM;++i) _lengths[DIM-i-1]= lengths[i];
	}

	ContainerLengths(const ContainerLengths<DIM>& other) {
		for(std::size_t i=0;i<DIM;++i) _lengths[i]=other._lengths[i];
	}

	ContainerLengths& operator=(const ContainerLengths& other){
		if(this!=&other){
			for(std::size_t i=0;i<DIM;++i) _lengths[i]=other._lengths[i];
		}
		return *this;
	}
 	
 	std::size_t getDimension() const { return DIM; }

 	bool operator==(const ContainerLengths& other){
 		if(DIM != other.getDimension()) return false;
 		for(std::size_t i=0;i<DIM;++i) if(_lengths[i]!=other._lengths[i]) return false;
 		return true;
 	}

 	/* Le paramètre correspond à une dimension */
 	const std::size_t& operator[](std::ptrdiff_t dim) const{
		if(dim>DIM) throw std::out_of_range("Index out of range");
 		return _lengths[--dim];
	}
	std::size_t& operator[](std::ptrdiff_t dim){	
		if(dim>DIM) throw std::out_of_range("Index out of range");
 		return _lengths[--dim];
	}

	/* Renvoie le nombre d'éléments compris dans la dimension donnée */
	std::size_t getSize(std::size_t dim=DIM) const { 		
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		std::size_t size = 1; 
		for(std::size_t dimI=1;dimI<=DIM;++dimI) size*=_lengths[dimI]; 
		return size;
	}

	virtual ~ContainerLengths(){}
};

#endif