#ifndef BOUNDEDMULTIDIM_HPP
#define BOUNDEDMULTIDIM_HPP

#include <exception>
#include <stdexcept>
#include <array>
#include "DimensionalArray.hpp"
#include "BoundedSubDimensionalArray.hpp"

typedef std::initializer_list<std::initializer_list<std::ptrdiff_t>> InitBounds;

template<typename Elem, size_t DIM>
class BoundedDimensionalArray : public DimensionalArray<Elem, DIM>{
	std::ptrdiff_t* _offset;

	std::array<std::size_t, DIM> setBounds(const InitBounds& initBounds){
		if(initBounds.size()!=DIM) throw std::logic_error("Initializer_list needs bounds for each dimension");
			std::array<std::size_t, DIM> lengthsArray;
			_offset = new std::ptrdiff_t[DIM];
			std::size_t dim=0;
			for(InitBounds::iterator dimensionIt = initBounds.begin();dimensionIt!=initBounds.end();++dimensionIt){
				if(dimensionIt->size()!=2) throw std::logic_error("Initializer_list needs exactly 2 bounds for each dimension");
				std::initializer_list<std::ptrdiff_t>::iterator boundsIt = dimensionIt->begin();
				std::ptrdiff_t lowerBound& = *(boundsIt); std::ptrdiff_t& upperBound = *(++boundsIt);
				upperBound-lowerBound+1 < 0 ? throw std::logic_error("Lower bound cannot be greater than upper bound") : lengthsArray[dim] = upperBound-lowerBound+1;
				_offset[dim++] = lowerBound;
			}
		return lengthsArray;
	}

public:
	BoundedDimensionalArray(const InitBounds& bounds, const Elem& elem = Elem()) : 
		DimensionalArray<Elem, DIM>(setBounds(bounds), elem){}
	BoundedDimensionalArray(const BoundedDimensionalArray<Elem, DIM>& other): 
		DimensionalArray<Elem, DIM>(other), _offset(new std::ptrdiff_t[DIM]){
			for(std::size_t i=0;i<DIM;++i) this->_offset[i] = other._offset[i];
		}

	bool inBounds(std::size_t index, std::size_t dimension=DIM) const{
		return (index <= this->upper(dimension)) && (index >= this->lower(dimension));
	}
	/* Renvoie la borne inférieure de la dimension donnée */
	std::ptrdiff_t lower(std::size_t dim=DIM) const { 
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _offset[DIM-dim]; 
	}
	/* Renvoie la borne supérieure de la dimension donnée */
	std::ptrdiff_t upper(std::size_t dim=DIM) const {
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return lower(dim)+this->length()-1;
	}
	BoundedSubDimensionalArray<Elem, DIM-1> operator[] (std::ptrdiff_t i) const{ 
		if(!inBounds(i)) throw std::out_of_range("Index out of bounds");
		return BoundedSubDimensionalArray<Elem, DIM-1>(this->_lengths+1,this->_val+(i-_offset[0])*this->size(DIM-1),this->_offset+1); 
	}
	BoundedSubDimensionalArray<Elem, DIM-1> operator[] (std::ptrdiff_t i) {
		if(!inBounds(i)) throw std::out_of_range("Index out of bounds");
		return BoundedSubDimensionalArray<Elem, DIM-1>(this->_lengths+1,this->_val+(i-_offset[0])*this->size(DIM-1),this->_offset+1); 
	}

	~BoundedDimensionalArray() { delete[] _offset; }
};

template<typename Elem>
class BoundedDimensionalArray<Elem,1> : public DimensionalArray<Elem, 1>{
	std::ptrdiff_t _offset;

	std::size_t setBounds(const std::initializer_list<std::ptrdiff_t>& bounds){
		if(bounds.size()!=2) throw std::logic_error("Initializer_list needs exactly 2 bounds for each dimension");
		std::initializer_list<std::ptrdiff_t>::iterator boundsIt = bounds.begin();
		std::size_t length;
		std::ptrdiff_t lowerBound = *boundsIt; std::ptrdiff_t upperBound = *(++boundsIt);
		upperBound-lowerBound+1 < 0 ? throw std::logic_error("Lower bound cannot be greater than upper bound") : length = upperBound-lowerBound+1;
		_offset = lowerBound; 
		return length;
	}

public:
	BoundedDimensionalArray(const std::initializer_list<std::size_t>& bounds, const Elem& elem = Elem()) :
		DimensionalArray<Elem,1>(setBounds(bounds), elem){}

	bool inBounds(std::size_t index) const{ return (index <= upper()) && (index >= lower()); }
	std::ptrdiff_t lower() const { return _offset; }
	std::ptrdiff_t upper() const { return lower()+this->length()-1; }
};

#endif