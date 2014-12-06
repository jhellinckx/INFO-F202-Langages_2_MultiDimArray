#ifndef MULTIDIM_HPP
#define MULTIDIM_HPP

#include <exception>
#include <stdexcept>
#include "ContainerLengths.hpp"
#include "SubDimensionalArray.hpp"

/* Classe représentant un tableau de dimension potentiellement infine */
template<typename Elem, std::size_t DIM>
class MultiDimensionalArray{
	ContainerLengths _lengths;
	Elem* _val;
	
public:
	MultiDimensionalArray(const ContainerLengths& lengths, const Elem& elem = Elem()): _lengths(lengths), 
		_val(new Elem[_lengths.size()]) {
			if(lengths.dimension()!=DIM) throw std::logic_error("Dimensions not matching");
			for(std::size_t i=0;i<_lengths.size();++i) _val[i] = elem;
	}

	MultiDimensionalArray(const std::initializer_list<std::size_t>& lengthsList, const Elem& elem = Elem()): 
		_lengths(ContainerLengths(DIM, lengthsList)), _val(new Elem[_lengths.size()]){
			if(lengthsList.size()!=DIM) throw std::logic_error("Dimensions not matching");
			for(std::size_t i=0;i<_lengths.size();++i) _val[i] = elem;
	}
	
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i){
		if(i>=_lengths[DIM]) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(_lengths, _val+i*_lengths.size(DIM-1));
	}
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i) const{
		if(i>=_lengths[DIM]) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(_lengths, _val+i*_lengths.size(DIM-1));
	}

	MultiDimensionalArray(const MultiDimensionalArray<Elem, DIM>&);
	MultiDimensionalArray(MultiDimensionalArray<Elem,DIM>&&);
	MultiDimensionalArray<Elem, DIM>& operator=(const MultiDimensionalArray<Elem, DIM>& other){
		if(this!=&other){
			if(_lengths == other._lengths){
				for(std::size_t i=0;i<_lengths.size();++i) _val[i] = other._val[i];
				return *this;
			}
			delete[] _val; _val = new Elem[other._lengths.size()];
			_lengths = other._lengths;
			for(std::size_t i=0;i<other._lengths.size();++i) _val[i] = other._val[i];
		}
		return *this;
	}
	void resize(const std::initializer_list<std::size_t>& lengthsList){
		if(lengthsList.size()!=DIM) throw std::logic_error("Initializer list size not matching the array dimension");
		std::size_t oldSize = _lengths.size();
		_lengths = ContainerLengths(DIM,lengthsList);
		std::size_t newSize = _lengths.size();
		Elem* tmp = new Elem[newSize];
		//Initialisation du nouveau vecteur, les anciennes valeurs sont conservées au possible
		for(std::size_t i=0;i<newSize;++i) (i<oldSize) ? tmp[i] = _val[i] : tmp[i] = Elem();
		delete[] _val; _val = tmp;
	}

	MultiDimensionalArray<Elem, DIM>& operator=(MultiDimensionalArray<Elem, DIM>&&);
	std::size_t dimension() const { return DIM; }
	std::size_t length(std::size_t dim=DIM) const { return _lengths[dim]; }
	virtual ~MultiDimensionalArray(){delete[] this->_val;}
};

//------------------------------------------------------------

template<typename Elem>
class MultiDimensionalArray<Elem, 1>{
	Elem* _val;
	std::size_t _length;

public:
	MultiDimensionalArray(std::size_t length, const Elem& elem = Elem()): 
		_length(length), _val(new Elem[_length]) {
			for(std::size_t i=0;i<_length;++i) _val[i] = elem;
		}
	
	MultiDimensionalArray(const MultiDimensionalArray<Elem, 1>& other):
		_length(other._length), _val(new Elem[_length]){
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
	virtual ~MultiDimensionalArray(){delete[] this->_val;}

	MultiDimensionalArray<Elem, 1>& operator=(const MultiDimensionalArray<Elem, 1>& other){
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

	std::size_t length() const { return _length; }
	std::size_t dimension() const {return 1;}

	void resize(std::size_t newLength){
		std::size_t oldLength = _length;
		_length = newLength;
		Elem* tmp = new Elem[newLength];
		for(std::size_t i=0;i<newLength;++i) (i<oldLength) ? tmp[i] = _val[i] : tmp[i] = Elem();
		delete[] _val; _val = tmp;
	}
};

#endif