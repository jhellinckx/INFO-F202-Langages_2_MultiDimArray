#ifndef MULTIDIM_HPP
#define MULTIDIM_HPP

#include <exception>
#include <stdexcept>
#include "ContainerLengths.hpp"
#include "DimensionalContainer.hpp"
#include "SubDimensionalArray.hpp"

/* Classe représentant un tableau de dimension potentiellement infine */
template<typename Elem, std::size_t DIM>
class MultiDimensionalArray: public DimensionalContainer<DIM> {
	Elem* _val;

protected:
	void copyAt(Elem* pelem, std::size_t elemSize){
		for(std::size_t i=0;i<std::min(elemSize,this->getSize());++i) pelem[i] = _val[i];
	}
	void resize(const std::initializer_list<std::size_t>& lengthsList){
		ContainerLengths newLengths(lengthsList);
		if(newLengths.getDimension() != DIM) throw std::logic_error("Initializer_list must have n=dimension elements when resizing");
		std::size_t newSize = this->computeSize(newLengths,DIM);
		Elem* tmp = new Elem[newSize];
		this->copyAt(tmp, newSize);
		delete[] _val; _val = tmp;
		this->setDimensionLengths(newLengths);
	}
public:
	MultiDimensionalArray(const ContainerLengths& lengths): 
		DimensionalContainer<DIM>(lengths), _val(new Elem[this->getSize()]) {
			for(std::size_t i=0;i<this->getSize();++i) _val[i] = Elem();
		}

	MultiDimensionalArray(const ContainerLengths& lengths, const Elem& elem) : 
		MultiDimensionalArray<Elem, DIM>(lengths){
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

	const Elem& get(std::ptrdiff_t i) const {return *(_val+i); }

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

	MultiDimensionalArray<Elem, DIM>& operator=(MultiDimensionalArray<Elem, DIM>&&);
	virtual ~MultiDimensionalArray(){delete[] this->_val;}
};

//------------------------------------------------------------

template<typename Elem>
class MultiDimensionalArray<Elem, 1>: public DimensionalContainer<1>{
	Elem* _val;

public:
	MultiDimensionalArray(std::size_t length): 
		DimensionalContainer<1>(length), _val(new Elem[this->getLength()]) {}
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
};

#endif