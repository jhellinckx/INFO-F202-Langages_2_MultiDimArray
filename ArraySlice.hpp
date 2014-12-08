#ifndef SLICE_HPP
#define SLICE_HPP

#define DEFAULT_PITCH 1

#include "BoundedSubDimensionalArray.hpp"
#include "DimensionalArray.hpp"
#include "SubArraySlice.hpp"

typedef std::initializer_list<std::initializer_list<std::ptrdiff_t>> InitSlice;

template<typename Elem, std::size_t DIM>
class ArraySlice final : public BoundedSubDimensionalArray<Elem, DIM> {
	std::ptrdiff_t* const _lower;
	std::ptrdiff_t* const _upper;
	std::ptrdiff_t* const _pitch; //pas

	void setBounds(const InitSlice& initSlice, const std::ptrdiff_t* offset){
		if(initSlice.size()!=DIM) throw std::logic_error("Dimensions not matching");
		std::size_t dim=0;
		for(InitSlice::iterator dimensionIt = InitSlice.begin(); dimensionIt != InitSlice.end(); dimensionIt++){
			if(dimensionIt->size()!=2 && dimensionIt->size()!=3) 
				throw std::logic_error("Slices need at least 2 and at most 3 parameters for each dimension (pitch has default value)");
			std::initializer_list<std::size_t>>::iterator boundsIt = dimensionIt->begin();
			std::ptrdiff_t& lowerBound = *boundsIt; std::ptrdiff_t& upperBound = *(++boundsIt);
			std::ptrdiff_t offsetBound = 0; 
			//Borne inférieure d'une dimension d'un BoundedMultiDimensionalArray
			if(offset!=NULL) offsetBound = offset[dim];
			if((lowerBound<offsetBound || lowerBound>=this->length(dim)+offsetBound) || (upperBound<offsetBound || upperBound >= this->length(dim)+offsetBound))
				throw std::logic_error("Slice bounds out of range");
			else if(upperBound-lowerBound+1<0)
				throw std::logic_error("Lower bound cannot be greater than upper bound");
			_lower[dim] = lowerBound; _upper[dim] = upperBound; 
			++boundsIt != dimensionIt->end() ? _pitch[dim] = *boundsIt : _pitch[dim] = DEFAULT_PITCH;
			++dim;
		}
	}

public:
	ArraySlice(std::size_t* const lengths, Elem* const val, const InitSlice& initSlice, const std::ptrdiff_t* offset= NULL):
		BoundedSubDimensionalArray<Elem, DIM>(lengths, val, offset), _lower(new std::ptrdiff_t[DIM]),
		_upper(new std::ptrdiff_t[DIM]), _pitch(new std::ptrdiff_t[DIM]) {
			setBounds(initSlice, offset);
	}

	ArraySlice(ArraySlice<Elem, DIM>& other): BoundedSubDimensionalArray<Elem, DIM>(other),
		 _lower(new std::ptrdiff_t[DIM]), _upper(new std::ptrdiff_t[DIM]), _pitch(new std::ptrdiff_t[DIM]){
		 	for(std::size_t i=0;i<DIM;++i) _lower[i] = other._lower[i];
		 	for(std::size_t i=0;i<DIM;++i) _upper[i] = other._upper[i];
		 	for(std::size_t i=0;i<DIM;++i) _pitch[i] = other._pitch[i];
	}
	
	SubArraySlice<Elem, DIM-1> operator[](std::ptrdiff_t i) const{
		
	}
	SubArraySlice<Elem, DIM-1> operator[](std::ptrdiff_t i){}

	std::ptrdiff_t lower(std::size_t dim=DIM) const {
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _lower[DIM-dims];
	}
	std::ptrdiff_t upper(std::size_t dim=DIM) const {
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _upper[DIM-dim];
	}
	bool inBounds(std::ptrdiff_t i,std::size_t dim=DIM) const{ return i<=lower(dim) && i>=upper(dim); }

	~ArraySlice() {delete[] _lower; delete[] _upper; delete[] _pitch; }
};

template<typename Elem>
class ArraySlice<Elem, 1>: public BoundedSubDimensionalArray<Elem, 1>{

};

#endif