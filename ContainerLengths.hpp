#ifndef CONTAINERLENGTHS_HPP
#define CONTAINERLENGTHS_HPP

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <stdexcept>

/* Classe responsable des longueurs d'un conteneur. 
La valeur en i correspond à la longueur de la ième+1 dimension. */
class ContainerLengths{
	std::size_t _dim;
	std::size_t* _lengths;

public:
	/* Reçoit en paramètre une liste d'initialisation contenant la longueur de chaque 
	dimension (par ordre décroissant) et en fait une copie vers un vecteur */
	ContainerLengths(std::size_t dimension, const std::initializer_list<std::size_t>& lengths) :
		_dim(dimension), _lengths(new std::size_t[dimension]){
			if(dimension!=lengths.size()) throw std::logic_error("List size does not match the given dimension");
			std::size_t i=0;
			/* On ordonne les longueurs par dimension croissante : 
			_lengths[i] == longueur de la dimension i+1 */
			for(std::initializer_list<std::size_t>::iterator it = lengths.begin();it!=lengths.end();++it)
				_lengths[dimension-i++-1] = *it;
	}
	
	ContainerLengths(std::size_t dimension, std::size_t* lengths) :
		_dim(dimension), _lengths(new std::size_t[dimension]){
			for(std::size_t i=0;i<dimension;++i) _lengths[dimension-i-1]= lengths[i];
	}

	ContainerLengths(const ContainerLengths& other) : 
		_dim(other._dim), _lengths(new std::size_t[other._dim]){
			for(std::size_t i=0;i<_dim;++i) _lengths[i]=other._lengths[i];
	}

	ContainerLengths& operator=(const ContainerLengths& other){
		if(this!=&other){
			if(other._dim == this->_dim)
				for(std::size_t i=0;i<_dim;++i) _lengths[i]=other._lengths[i];
			else throw std::logic_error("Cannot assign ContainerLengths of different dimensions");
		}
		return *this;
	}
 	
 	std::size_t dimension() const { return _dim; }

 	bool operator==(const ContainerLengths& other){
 		if(this->_dim != other._dim) return false;
 		for(std::size_t i=0;i<this->_dim;++i) if(_lengths[i]!=other._lengths[i]) return false;
 		return true;
 	}

 	/* Le paramètre correspond à une dimension */
 	const std::size_t& operator[](std::ptrdiff_t dim) const{
		if(dim==0 || std::size_t(dim)>this->_dim) throw std::out_of_range("Index out of range");
 		return _lengths[dim-1];
	}
	std::size_t& operator[](std::ptrdiff_t dim){	
		if(dim==0 || std::size_t(dim)>this->_dim) throw std::out_of_range("Index out of range");
 		return _lengths[dim-1];
	}

	/* Renvoie le nombre d'éléments compris dans la dimension donnée */
	std::size_t size(std::size_t dim) const { 		
		if(dim==0 || dim>this->_dim) throw std::out_of_range("Index out of range");
		std::size_t size = 1; 
		for(std::size_t dimI=1;dimI<=dim;++dimI) size*=_lengths[dimI-1]; 
		return size;
	}

	std::size_t size() const { return size(_dim); }

	virtual ~ContainerLengths(){delete[] _lengths;}
};

#endif