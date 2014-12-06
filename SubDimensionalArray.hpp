#ifndef SUBDIM_HPP
#define SUBDIM_HPP

#include <exception>
#include <stdexcept>
#include "ContainerLengths.hpp"

template<typename Elem, std::size_t DIM>
class MultiDimensionalArray;

template<typename Elem, std::size_t DIM>
class SubDimensionalArray{
	const ContainerLengths& _lengths;
	Elem* const _val;
protected:
	SubDimensionalArray(const ContainerLengths& lengths, Elem* val): 
		_lengths(lengths), _val(val) {}
	SubDimensionalArray(const SubDimensionalArray<Elem, DIM>& other) : 
		_lengths(other._lengths), _val(other._val){}

public:
	SubDimensionalArray<Elem, DIM>& operator=(const SubDimensionalArray<Elem, DIM>& other){
		if(this != &other){
			if(_lengths == other._lengths){
				for(std::size_t i=0;i<_lengths.size(DIM);++i){
					_val[i] = other._val[i];
				}
				return *this;
			}
			throw std::logic_error("Cannot assign subarrays of different lengths");
		}
		return *this;
	}
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i){
		if(i>=_lengths[DIM]) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(_lengths, _val+i*_lengths.size(DIM-1));
	}
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i) const{
		if(i>=_lengths[DIM]) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(_lengths, _val+i*_lengths.size(DIM-1));
	}

	std::size_t length() const { return _lengths[DIM]; }
	std::size_t dimension() const { return DIM; }

	virtual ~SubDimensionalArray(){}

	friend class MultiDimensionalArray<Elem, DIM+1>;
	friend class SubDimensionalArray<Elem, DIM+1>;
};

//------------------------------------------------------------

template<typename Elem>
class SubDimensionalArray<Elem, 1>{
	std::size_t _length;
	Elem* const _val;

public:
	/* length[1] == longueur de la dimension 1 */
	SubDimensionalArray(const ContainerLengths& length, Elem* val): 
		_length(length[1]), _val(val) {}
	SubDimensionalArray(const SubDimensionalArray<Elem, 1>& other):
		_length(other._length), _val(other._val) {}

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

	std::size_t dimension() const { return 1; }
	std::size_t length() const { return _length; }

	friend class MultiDimensionalArray<Elem, 2>;
	friend class SubDimensionalArray<Elem, 2>;
};

#endif
