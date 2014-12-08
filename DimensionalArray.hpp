#ifndef MULTIDIM_HPP
#define MULTIDIM_HPP

#include <exception>
#include <stdexcept>
#include "AbstractDimensionalArray.hpp"
#include "SubDimensionalArray.hpp" 

/* Classe représentant un tableau de dimension potentiellement infine. 
Gère en mémoire _lengths[] et _val[] */
template<typename Elem, std::size_t DIM>
class DimensionalArray: public AbstractDimensionalArray<DIM>{
protected:
	Elem* _val;
	
	std::size_t* storeLengths(const std::initializer_list<std::size_t>& lengthsList){
		if(lengthsList.size() != DIM) throw std::logic_error("Dimensions not matching");
		std::size_t* lengths = new std::size_t[DIM];
		std::copy(std::begin(lengthsList), std::end(lengthsList), lengths);
		return lengths;
	}
	std::size_t* storeLengths(const std::array<std::size_t, DIM>& lengthsArray){
		if(lengthsArray.size() != DIM) throw std::logic_error("Dimensions not matching");
		std::size_t* lengths = new std::size_t[DIM];
		std::copy(std::begin(lengthsArray), std::end(lengthsArray), lengths);
		return lengths;
	}
public:
	DimensionalArray(const std::initializer_list<std::size_t>& lengthsList, const Elem& elem = Elem()): 
		AbstractDimensionalArray<DIM>(storeLengths(lengthsList)), _val(new Elem[this->size()]){
			for(std::size_t i=0;i<this->size();++i) _val[i] = elem;
	}

	DimensionalArray(const std::array<std::size_t, DIM>& lengthsList, const Elem& elem = Elem()):
		AbstractDimensionalArray<DIM>(storeLengths(lengthsList)), _val(new Elem[this->size()]){
			for(std::size_t i=0;i<this->size();++i) _val[i] = elem;
	}
	
	/* L'opérateur [] s'appellera récursivement sur SubDimensionalArray tant que DIM != 1.
	Pour créer un array de DIM-1, il suffit d'effectuer un décalage d'une unité sur _lengths,
	et de i*j unités sur _val, où j est la taille d'un vecteur de DIM-1, et i la position de
	ce vecteur */
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i){
		if(i>=this->length(DIM)) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(this->_lengths+1, _val+i*this->size(DIM-1));
	}
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i) const{
		if(i>=this->length(DIM)) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(this->_lengths+1, _val+i*this->size(DIM-1));
	}

	DimensionalArray(const DimensionalArray<Elem, DIM>& other): 
		AbstractDimensionalArray<DIM>(new std::size_t[DIM]), _val(new Elem[other.size()]){
			for(std::size_t i=0;i<DIM;++i) this->_lengths[i] = other._lengths[i];
			for(std::size_t i=0;i<this->size();++i) this->_val[i] = other._val[i];
	} 
	DimensionalArray(DimensionalArray<Elem,DIM>&&);
	DimensionalArray<Elem, DIM>& operator=(const DimensionalArray<Elem, DIM>& other){
		if(this!=&other){
			if(this->sameLengths(other)){
				for(std::size_t i=0;i<this->size();++i) _val[i] = other._val[i];
				return *this;
			}
			delete[] _val; _val = new Elem[other.size()];
			for(std::size_t i=0;i<DIM;++i) this->_lengths[i] = other._lengths[i];
			for(std::size_t i=0;i<this->size();++i) _val[i] = other._val[i];
		}
		return *this;
	}
	void resize(const std::initializer_list<std::size_t>& lengthsList){
		if(lengthsList.size()!=DIM) throw std::logic_error("Dimensions not matching");
		std::size_t oldSize = this->size();
		std::copy(lengthsList.begin(), lengthsList.end(), this->_lengths);
		std::size_t newSize = this->size();
		Elem* tmp = new Elem[newSize];
		//Initialisation du nouveau vecteur, les anciennes valeurs sont conservées au possible
		for(std::size_t i=0;i<newSize;++i) (i<oldSize) ? tmp[i] = _val[i] : tmp[i] = Elem();
		delete[] _val; _val = tmp;
	}

	DimensionalArray<Elem, DIM>& operator=(DimensionalArray<Elem, DIM>&&);
	virtual ~DimensionalArray(){delete[] this->_val; delete[] this->_lengths;}
};

//------------------------------------------------------------

template<typename Elem>
class DimensionalArray<Elem, 1>: public AbstractDimensionalArray<1>{
	Elem* _val;

public:
	DimensionalArray(std::size_t length, const Elem& elem = Elem()): 
		AbstractDimensionalArray<1>(length), _val(new Elem[_length]) {
			for(std::size_t i=0;i<_length;++i) _val[i] = elem;
		}
	
	DimensionalArray(const DimensionalArray<Elem, 1>& other):
		AbstractDimensionalArray<1>(other._length), _val(new Elem[_length]){
			for(std::size_t i=0;i<_length;++i) _val[i] = other._val[i];
	}
	const Elem& operator[](std::ptrdiff_t i) const{
		if(i>_length) throw std::out_of_range("Index out of range");
		return _val[i];
	}

	Elem& operator[](std::ptrdiff_t i){
		if(i>_length) throw std::out_of_range("Index out of range");
		return _val[i];
	}
	virtual ~DimensionalArray(){delete[] this->_val;}

	DimensionalArray<Elem, 1>& operator=(const DimensionalArray<Elem, 1>& other){
		if(this!=&other){
			if(_length == other._length){
				for(std::size_t i=0;i<_length;++i) _val[i] = other._val[i];  
				return *this;
			}
			delete[] _val; _val = new Elem[other._length];
			_length = other._length;
			for(std::size_t i=0;i<this->size();++i) _val[i] = other._val[i];
		}
		return *this;
	}

	void resize(std::size_t newLength){
		std::size_t oldLength = _length;
		_length = newLength;
		Elem* tmp = new Elem[newLength];
		for(std::size_t i=0;i<newLength;++i) (i<oldLength) ? tmp[i] = _val[i] : tmp[i] = Elem();
		delete[] _val; _val = tmp;
	}
};

#endif