#ifndef ABSTRACTDIMENSIONALARRAY_HPP
#define ABSTRACTDIMENSIONALARRAY_HPP

#include <exception>
#include <stdexcept>

/* Classe abstraite définissant des opérations basiques sur la longueur des 
dimensions d'un tableau */
template<std::size_t DIM> 
class AbstractDimensionalArray{
protected:
	std::size_t* const _lengths;
	/* Classe abstraite : constructeur en protected */
	AbstractDimensionalArray(std::size_t* const lengthsArray) : _lengths(lengthsArray) {}
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
	bool sameLengths(const AbstractDimensionalArray<DIM>& other) const {
		if(DIM!=other.dimension()) return false; 
		for(std::size_t i=0;i<DIM;++i){
			if(_lengths[i]!=other._lengths[i]) return false;
		} 
		return true;
	}
	std::size_t dimension() const { return DIM; }
	std::size_t length(std::size_t dim=DIM) const { 
		if(dim==0 || dim>DIM) throw std::out_of_range("Dimension out of range");
		return _lengths[DIM-dim];
	}
	virtual ~AbstractDimensionalArray(){}
};

//------------------------------------------------------------

/* Spécialisation complète de AbstractDimensionalArray pour DIM=1 */
template<> 
class AbstractDimensionalArray<1>{
protected:
	std::size_t _length;
	AbstractDimensionalArray(std::size_t length) : _length(length){}
	void setLength(std::size_t length) { _length = length; }
public:
	std::size_t dimension() const { return 1; }
	std::size_t length() const { return _length; }
	std::size_t size() const { return _length; }
	bool sameLength(const AbstractDimensionalArray<1>& other) const { return _length == other._length; } 
	virtual ~AbstractDimensionalArray(){}
};

#endif