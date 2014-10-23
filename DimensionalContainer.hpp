#ifndef DIMENSIONALCONTAINER_HPP
#define DIMENSIONALCONTAINER_HPP

#include <exception>
#include <stdexcept>
#include "ContainerLengths.hpp"

/* Classe abstraite représentant un conteneur d'élements et ayant plusieurs dimensions */
template<std::size_t DIM> 
class DimensionalContainer{
	ContainerLengths _lengths;

protected:
	/* Classe abstraite : constructeur en protected */
	DimensionalContainer(const ContainerLengths& lengths = ContainerLengths(DIM)) : _lengths(lengths) {}

	/* Modifie les longueurs des dimensions de la dimension donnée */
	void setDimensionLengths(const ContainerLengths& lengths, std::size_t dim=DIM){
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		for(std::size_t i=DIM-dim;i<DIM;++i) _lengths[i] = lengths[i];
	}

public:
	/* Renvoie les longueurs des dimensions de la dimension donnée */
	ContainerLengths getDimensionLengths(std::size_t dim=DIM) const{
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _lengths.getLengths(dim);
	}
	/* Calcule la taille d'un potentiel conteneur en utilisant 2 ContainerLengths, 
	celui de this pour les dimensions > dim, et l'autre entièrement */
	std::size_t computeSize(ContainerLengths& lengths, std::size_t dim) const{
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		std::size_t size = 1;
		for(std::size_t i=0;i<DIM-dim;++i) size *= this->_lengths[i];
		for(std::size_t i=0;i<dim;++i) size *= lengths[i];
		return size;
	}
	std::size_t getDimension() const { return _lengths.getDimension(); }
	/* Renvoie la longueur de la dimension donnée */
	std::size_t getLength(std::size_t dim=DIM) const {		
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _lengths[DIM-dim]; 
	}
	/* Renvoie le nombre d'éléments compris dans la dimension donnée */
	std::size_t getSize(std::size_t dim=DIM) const { 		
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		std::size_t size = 1; 
		for(std::size_t i=DIM-dim;i<DIM;++i) size*=_lengths[ptrdiff_t(i)]; 
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