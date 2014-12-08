#ifndef SUBDIM_HPP
#define SUBDIM_HPP

#include <exception>
#include <stdexcept>
#include "AbstractDimensionalArray.hpp"

template<typename Elem, std::size_t DIM>
class DimensionalArray;

template<typename Elem, std::size_t DIM>
class SubDimensionalArray: public AbstractDimensionalArray<DIM>{
protected:
	Elem* const _val;
	SubDimensionalArray(std::size_t* const lengths, Elem* const val): 
		AbstractDimensionalArray<DIM>(lengths), _val(val) {}
	SubDimensionalArray(const SubDimensionalArray<Elem, DIM>& other) : 
		AbstractDimensionalArray<DIM>(other._lengths), _val(other._val){}

public:
	SubDimensionalArray<Elem, DIM>& operator=(const SubDimensionalArray<Elem, DIM>& other){
		if(this != &other){
			if(this->sameLengths(other)){
				for(std::size_t i=0;i<this->size(DIM);++i){
					_val[i] = other._val[i];
				}
				return *this;
			}
			throw std::logic_error("Cannot assign subarrays of different lengths");
		}
		return *this;
	}
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i){
		if(i>=this->length(DIM)) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(this->_lengths+1, _val+i*this->size(DIM-1));
	}
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i) const{
		if(i>=this->length(DIM)) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(this->_lengths+1, _val+i*this->size(DIM-1));
	}

	virtual ~SubDimensionalArray(){}

	friend class DimensionalArray<Elem, DIM+1>;
	friend class SubDimensionalArray<Elem, DIM+1>;
};

//------------------------------------------------------------

template<typename Elem>
class SubDimensionalArray<Elem, 1>: public AbstractDimensionalArray<1>{
protected:
	Elem* const _val;
public:
	SubDimensionalArray(const std::size_t* const lengths, Elem* const val): 
		AbstractDimensionalArray<1>(lengths[0]), _val(val) {}
	SubDimensionalArray(const SubDimensionalArray<Elem, 1>& other):
		AbstractDimensionalArray<1>(other._length), _val(other._val) {}

	SubDimensionalArray<Elem, 1>& operator=(const SubDimensionalArray<Elem, 1>& other){
		if(this != &other){
			if(this->_length == other._length){
				for(std::size_t i=0;i<_length;++i) _val[i] = other._val[i];
				return *this;
			}
			throw std::logic_error("Cannot assign subarrays of different lengths");
		}
		return *this;
	}
	Elem& operator[](std::ptrdiff_t i){
		if(i>=_length) throw std::out_of_range("Index out of range");
		return *(_val+i);
	}
	const Elem& operator[](std::ptrdiff_t i) const{
		if(i>=_length) throw std::out_of_range("Index out of range");
		return *(_val+i);
	}
	virtual ~SubDimensionalArray(){}

	friend class DimensionalArray<Elem, 2>;
	friend class SubDimensionalArray<Elem, 2>;
};

#endif
