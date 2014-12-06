#ifndef DIMENSIONALCONTAINER_HPP
#define DIMENSIONALCONTAINER_HPP

#include <exception>
#include <stdexcept>
#include "ContainerLengths.hpp"

/* Classe abstraite définissant des opérations basiques sur la longueur des 
dimensions d'un conteneur */
template<std::size_t DIM> 
class DimensionalContainer{
	const std::size_t* _lengths;

protected:
	/* Classe abstraite : constructeur en protected */
	DimensionalContainer(const std::size_t* l) : _lengths(l) {}

public:
	
	/* Renvoie le nombre d'éléments compris dans la dimension donnée */
	std::size_t size(std::size_t dim=DIM) const { 		
		if(dim==0 || dim>DIM) throw std::out_of_range("Dimension out of range");
		std::size_t size = 1; 
		for(std::size_t i=DIM-dim;i<DIM;++i) size*=_lengths[i]; 
		return size;
	}

	/* Renvoie vrai si, respectivement, les dimensions de l'objet donné 
	en paramètre est égale aux dimensions de this */
	bool hasSameLengths(const DimensionalContainer<DIM>& other) {
		if(DIM!=other.getDimension()) return false; 
		for(std::size_t i =0;i<DIM;++i){
			if(_lengths[i]!=other._lengths[i]) return false;
		} 
		return true;
	}
	std::size_t dimension() const { return DIM; }
	std::size_t length(std::size_t dim=DIM){ 
		if(dim==0 || dim>DIM) throw std::out_of_range("Dimension out of range")
		return _lengths[DIM-dim]
	}

	virtual ~DimensionalContainer(){}
};

//------------------------------------------------------------

/* Spécialisation complète de DimensionalContainer pour DIM=1 */
template<> 
class DimensionalContainer<1>{
private:
	std::size_t _length;

protected:
	DimensionalContainer(std::size_t length=Constants::DEFAULT_DIM_LENGTH) : _length(length){}
	void setLength(std::size_t length) { _length = length; }

public:
	std::size_t getDimension() const { return 1; }
	std::size_t getLength() const { return _length; }
	std::size_t getSize() const { return _length; }
	bool hasSameLength(const DimensionalContainer<1>& other) { return _length == other._length; } 
	virtual ~DimensionalContainer(){}
};

#endif