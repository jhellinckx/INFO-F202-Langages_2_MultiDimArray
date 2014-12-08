#ifndef BOUNDEDMULTIDIM_HPP
#define BOUNDEDMULTIDIM_HPP

#include <exception>
#include <stdexcept>
#include <array>
#include "DimensionalArray.hpp"
#include "AbstractBoundedArray.hpp"
#include "BoundedSubDimensionalArray.hpp"

typedef std::initializer_list<std::initializer_list<std::ptrdiff_t>> InitBounds;

template<typename Elem, size_t DIM>
class BoundedDimensionalArray : public AbstractBoundedArray<DIM>, public DimensionalArray<Elem, DIM>{
	/* Initialisation des bornes pour chaque dimension à partir de initializer_list */
	std::array<std::size_t, DIM> setBounds(const InitBounds& initBounds){
		if(initBounds.size()!=DIM) throw std::logic_error("Initializer_list needs bounds for each dimension");
			std::array<std::size_t, DIM> lengthsArray;
			std::size_t dim=0;
			for(InitBounds::iterator dimensionIt = initBounds.begin();dimensionIt!=initBounds.end();++dimensionIt){
				if(dimensionIt->size()!=2) throw std::logic_error("Initializer_list needs exactly 2 bounds for each dimension");
				std::initializer_list<std::ptrdiff_t>::iterator boundsIt = dimensionIt->begin();
				const std::ptrdiff_t& lowerBound = *(boundsIt); const std::ptrdiff_t& upperBound = *(++boundsIt);
				upperBound-lowerBound+1 < 0 ? throw std::logic_error("Lower bound cannot be greater than upper bound") : lengthsArray[dim] = upperBound-lowerBound+1;
				this->_lower[dim++] = lowerBound;
			}
		return lengthsArray;
	}

public:
	/* upperBound à NULL pour AbstractBoundedArray car une borne suffit */
	BoundedDimensionalArray(const InitBounds& bounds, const Elem& elem = Elem()) : 
		AbstractBoundedArray<DIM>(new std::ptrdiff_t[DIM], NULL), DimensionalArray<Elem, DIM>(setBounds(bounds), elem){}
	BoundedDimensionalArray(const BoundedDimensionalArray<Elem, DIM>& other): 
		AbstractBoundedArray<DIM>(new std::ptrdiff_t[DIM], NULL), DimensionalArray<Elem, DIM>(other){
			for(std::size_t i=0;i<DIM;++i) this->_lower[i] = other._lower[i];
	}

	BoundedSubDimensionalArray<Elem, DIM-1> operator[] (std::ptrdiff_t i) const{ 
		if(!this->inBounds(i)) throw std::out_of_range("Index out of bounds");
		return BoundedSubDimensionalArray<Elem, DIM-1>(this->_lengths+1,this->_val+(i-this->_lower[0])*this->size(DIM-1),this->_lower+1); 
	}
	BoundedSubDimensionalArray<Elem, DIM-1> operator[] (std::ptrdiff_t i) {
		if(!this->inBounds(i)) throw std::out_of_range("Index out of bounds");
		return BoundedSubDimensionalArray<Elem, DIM-1>(this->_lengths+1,this->_val+(i-this->_lower[0])*this->size(DIM-1),this->_lower+1); 
	}

	std::ptrdiff_t upper(std::size_t dim=DIM) const {
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return this->lower(dim)+this->length()-1;
	}

	virtual ~BoundedDimensionalArray() { delete[] this->_lower; }
};

template<typename Elem>
class BoundedDimensionalArray<Elem,1> : public AbstractBoundedArray<1>, public DimensionalArray<Elem, 1>{

	std::size_t setBounds(const std::initializer_list<std::ptrdiff_t>& bounds){
		if(bounds.size()!=2) throw std::logic_error("Initializer_list needs exactly 2 bounds for each dimension");
		std::initializer_list<std::ptrdiff_t>::iterator boundsIt = bounds.begin();
		std::size_t length;
		std::ptrdiff_t lowerBound = *boundsIt; std::ptrdiff_t upperBound = *(++boundsIt);
		upperBound-lowerBound+1 < 0 ? throw std::logic_error("Lower bound cannot be greater than upper bound") : length = upperBound-lowerBound+1;
		this->_lower = lowerBound; 
		return length;
	}

public:
	BoundedDimensionalArray(const std::initializer_list<std::size_t>& bounds, const Elem& elem = Elem()) :
		AbstractBoundedArray<1>(0, 0), DimensionalArray<Elem,1>(setBounds(bounds), elem){}

	std::ptrdiff_t upper() const { return lower()+this->length()-1; }
};

#endif