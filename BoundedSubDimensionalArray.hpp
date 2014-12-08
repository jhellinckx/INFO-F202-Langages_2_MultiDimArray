#ifndef BOUNDEDSUBDIM_HPP
#define BOUNDEDSUBDIM_HPP

#include "SubDimensionalArray.hpp"
#include "AbstractBoundedArray.hpp"
#include <exception>
#include <stdexcept>

template<typename Elem, std::size_t DIM>
class BoundedDimensionalArray;

template<typename Elem, std::size_t DIM>
class BoundedSubDimensionalArray: public AbstractBoundedArray<DIM>, public SubDimensionalArray<Elem, DIM>{
protected:
	BoundedSubDimensionalArray(std::size_t* const lengths, Elem* const val, std::ptrdiff_t* const lowerBounds):
		AbstractBoundedArray<DIM>(lowerBounds, NULL), SubDimensionalArray<Elem, DIM>(lengths, val) {}
	BoundedSubDimensionalArray(const BoundedSubDimensionalArray<Elem, DIM>& other): 
		AbstractBoundedArray<DIM>(other), SubDimensionalArray<Elem, DIM>(other) {}

public:
	std::ptrdiff_t upper(std::size_t dim=DIM) const {
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return this->lower(dim)+this->length()-1;
	}

	BoundedSubDimensionalArray<Elem, DIM-1> operator[] (std::ptrdiff_t i) const{
		if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
		return BoundedSubDimensionalArray<Elem, DIM-1>(this->_lengths+1,this->_val+(i-this->_lower[0])*this->size(DIM-1),this->_lower+1);
	}
	BoundedSubDimensionalArray<Elem, DIM-1> operator[] (std::ptrdiff_t i){
		if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
		return BoundedSubDimensionalArray<Elem, DIM-1>(this->_lengths+1,this->_val+(i-this->_lower[0])*this->size(DIM-1),this->_lower+1);
	}

	virtual ~BoundedSubDimensionalArray(){}

	friend class BoundedDimensionalArray<Elem, DIM+1>;
	friend class BoundedSubDimensionalArray<Elem, DIM+1>;
};

template<typename Elem>
class BoundedSubDimensionalArray<Elem, 1>: public AbstractBoundedArray<1>, public SubDimensionalArray<Elem, 1>{
protected:
	BoundedSubDimensionalArray(std::size_t* const lengths, Elem* const val, std::ptrdiff_t* const lowerBounds):
		AbstractBoundedArray<1>(lowerBounds[0], 0), SubDimensionalArray<Elem, 1>(lengths, val) {}
	BoundedSubDimensionalArray(const BoundedSubDimensionalArray<Elem, 1>& other) : 
		AbstractBoundedArray<1>(other), SubDimensionalArray<Elem, 1>(other) {}
public:
	const Elem& operator[](std::ptrdiff_t i) const {
		if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
		return this->_val[i-this->_lower];
	}
	Elem& operator[](std::ptrdiff_t i) {
		if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
		return this->_val[i-this->_lower];
	}

	std::ptrdiff_t upper() const { return this->lower()+this->length()-1; }

	friend class BoundedDimensionalArray<Elem, 2>;
	friend class BoundedSubDimensionalArray<Elem, 2>;
};

#endif