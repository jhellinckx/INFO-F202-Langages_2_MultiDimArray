#ifndef SUBDIM_HPP
#define SUBDIM_HPP

#include <exception>
#include <stdexcept>
#include "ContainerLengths.hpp"
#include "DimensionalContainer.hpp"

template<typename Elem, std::size_t DIM>
class MultiDimensionalArray;

template<typename Elem, std::size_t DIM>
class SubDimensionalArray: public DimensionalContainer<DIM>{
	Elem* const _val;

	SubDimensionalArray(const ContainerLengths& lengths, Elem* val): 
		DimensionalContainer<DIM>(lengths), _val(val) {}
	SubDimensionalArray(const SubDimensionalArray<Elem, DIM>& other) : 
		DimensionalContainer<DIM>(other), _val(other._val){}

public:
	SubDimensionalArray<Elem, DIM>& operator=(const SubDimensionalArray<Elem, DIM>& other){
		if(this != &other){
			if(this->hasSameLengths(other)){
				for(std::size_t i=0;i<this->getSize();++i){
					_val[i] = other._val[i];
				}
				return *this;
			}
			throw std::logic_error("Cannot assign subarrays of different lengths");
		}
		return *this;
	}
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i){
		if(std::size_t(i)>=this->getLength()) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(this->getDimensionLengths(DIM-1), _val+i*this->getSize(DIM-1));
	}
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i) const{
		if(std::size_t(i)>=this->getLength()) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(this->getDimensionLengths(DIM-1), _val+i*this->getSize(DIM-1));
	}

	virtual ~SubDimensionalArray(){}

	friend class MultiDimensionalArray<Elem, DIM+1>;
	friend class SubDimensionalArray<Elem, DIM+1>;
};

//------------------------------------------------------------

template<typename Elem>
class SubDimensionalArray<Elem, 1>: public DimensionalContainer<1>{
	Elem* const _val;

public:
	SubDimensionalArray(const ContainerLengths& length, Elem* val): 
		DimensionalContainer<1>(length[0]), _val(val) {}
	SubDimensionalArray(const SubDimensionalArray<Elem, 1>& other):
		DimensionalContainer<1>(other.getLength()), _val(other._val) {}

	SubDimensionalArray<Elem, 1>& operator=(const SubDimensionalArray<Elem, 1>& other){
		if(this != &other){
			if(this->hasSameLength(other)){
				for(std::size_t i=0;i<this->getSize();++i) _val[i] = other._val[i];
				return *this;
			}
			throw std::logic_error("Cannot assign subarrays of different lengths");
		}
		return *this;
	}
	Elem& operator[](std::ptrdiff_t i){
		if(std::size_t(i)>=this->getLength()) throw std::out_of_range("Index out of range");
		return *(_val+i);
	}
	const Elem& operator[](std::ptrdiff_t i) const{
		if(std::size_t(i)>=this->getLength()) throw std::out_of_range("Index out of range");
		return *(_val+i);
	}
	virtual ~SubDimensionalArray(){}

	friend class MultiDimensionalArray<Elem, 2>;
	friend class SubDimensionalArray<Elem, 2>;
};

#endif
