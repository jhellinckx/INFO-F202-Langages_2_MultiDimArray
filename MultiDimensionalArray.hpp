#ifndef MULTIDIM_HPP
#define MULTIDIM_HPP

#include <exception>
#include <stdexcept>
#include "ContainerLengths.hpp"
#include "DimensionalContainer.hpp"
#include "SubDimensionalArray.hpp"

/* Classe représentant un tableau de dimension potentiellement infine */
template<typename Elem, std::size_t DIM>
class MultiDimensionalArray{
	Elem* _val;
	
public:
	MultiDimensionalArray(const ContainerLengths& lengths): 
		DimensionalContainer<DIM>(lengths), _val(new Elem[this->getSize()]) {
			if(this->getDimension()!=DIM) throw std::logic_error("Dimensions not matching");
			for(std::size_t i=0;i<this->getSize();++i) _val[i] = Elem();
		}

	MultiDimensionalArray(const ContainerLengths& lengths, const Elem& elem) : 
		DimensionalContainer<DIM>(lengths), _val(new Elem[this->getSize()]) {
			if(this->getDimension()!=DIM) throw std::logic_error("Dimensions not matching");
			for(std::size_t i=0;i<this->getSize();++i) _val[i] = elem;
	}
	MultiDimensionalArray(const std::initializer_list<std::size_t>& lengthsList): 
		MultiDimensionalArray<Elem, DIM>(ContainerLengths(lengthsList)){}
	MultiDimensionalArray(const std::initializer_list<std::size_t>& lengthsList, const Elem& elem): 
		MultiDimensionalArray<Elem, DIM>(ContainerLengths(lengthsList), elem){}
	
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i){
		if(std::size_t(i)>=this->getLength()) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(this->getDimensionLengths(DIM-1), _val+i*this->getSize(DIM-1));
	}
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i) const{
		if(std::size_t(i)>=this->getLength()) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(this->getDimensionLengths(DIM-1), _val+i*this->getSize(DIM-1));
	}

	MultiDimensionalArray(const MultiDimensionalArray<Elem, DIM>&);
	MultiDimensionalArray(MultiDimensionalArray<Elem,DIM>&&);
	MultiDimensionalArray<Elem, DIM>& operator=(const MultiDimensionalArray<Elem, DIM>& other){
		if(this!=&other){
			if(this->hasSameLengths(other)){
				for(std::size_t i=0;i<this->getSize();++i) _val[i] = other._val[i];
				return *this;
			}
			delete[] _val; _val = new Elem[other.getSize()];
			this->setDimensionLengths(other.getDimensionLengths());
			for(std::size_t i=0;i<this->getSize();++i) _val[i] = other._val[i];
		}
		return *this;
	}
	virtual void resize(const std::initializer_list<std::size_t>& lengthsList){
		if(lengthsList.size()!=DIM) throw std::logic_error("Initializer list size not matching the array dimension")
		ContainerLengths newLengths(lengthsList);
		std::size_t newSize = this->computeSize(newLengths,DIM);
		std::size_t oldSize = this->getSize();
		Elem* tmp = new Elem[newSize];
		//Init the new vector
		for(std::size_t i=0;i<newSize;++i) (i<oldSize) ? tmp[i] = _val[i] : tmp[i] = Elem();
		delete[] _val; _val = tmp;
		this->setDimensionLengths(newLengths);
	}

	MultiDimensionalArray<Elem, DIM>& operator=(MultiDimensionalArray<Elem, DIM>&&);
	virtual ~MultiDimensionalArray(){delete[] this->_val;}
};

//------------------------------------------------------------

template<typename Elem>
class MultiDimensionalArray<Elem, 1>: public DimensionalContainer<1>{
	Elem* _val;

public:
	MultiDimensionalArray(std::size_t length = Constants::DEFAULT_DIM_LENGTH): 
		DimensionalContainer<1>(length), _val(new Elem[this->getLength()]) {
			for(std::size_t i=0;i<this->getLength();++i) _val[i] = Elem();
		}
	MultiDimensionalArray(std::size_t length, const Elem& elem): 
		DimensionalContainer<1>(length), _val(new Elem[this->getLength()]) {
			for(std::size_t i=0;i<this->getLength();++i) _val[i] = elem;
		}
	MultiDimensionalArray(const MultiDimensionalArray<Elem, 1>& other) : MultiDimensionalArray<Elem, 1>(other.getLength()){
		for(std::size_t i=0;i<this->getLength();++i) _val[i] = other._val[i];
	}
	const Elem& operator[](std::ptrdiff_t i) const{
		if(std::size_t(i)>this->getLength()) throw std::out_of_range("Index out of range");
		return this->_val[i];
	}

	Elem& operator[](std::ptrdiff_t i){
		if(i>this->getLength()) throw std::out_of_range("Index out of range");
		return this->_val[i];
	}
	virtual ~MultiDimensionalArray(){delete[] this->_val;}

	MultiDimensionalArray<Elem, 1>& operator=(const MultiDimensionalArray<Elem, 1>& other){
		if(this!=&other){
			if(this->hasSameLength(other)){
				for(std::size_t i=0;i<this->getLength();++i) _val[i] = other._val[i];
				return *this;
			}
			delete[] _val; _val = new Elem[other.getLength()];
			this->setLength(other.getLength());
			for(std::size_t i=0;i<this->getSize();++i) _val[i] = other._val[i];
		}
		return *this;
	}

	virtual void resize(std::size_t newLength){
		std::size_t newSize = newLength;
		std::size_t oldSize = this->getLength();
		Elem* tmp = new Elem[newSize];
		//Init the new vector
		for(std::size_t i=0;i<newSize;++i) (i<oldSize) ? tmp[i] = _val[i] : tmp[i] = Elem();
		delete[] _val; _val = tmp;
		this->setLength(newLength);
	}
};

#endif