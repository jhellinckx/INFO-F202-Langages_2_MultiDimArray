#ifndef BOUNDEDMULTIDIM_HPP
#define BOUNDEDMULTIDIM_HPP

#include <exception>
#include <stdexcept>
#include "ContainerLengths.hpp"
#include "ContainerBounds.hpp"
#include "MultiDimensionalArray.hpp"
#include "BoundedSubDimensionalArray.hpp"

template<typename Elem, size_t DIM>
class BoundedMultiDimensionalArray : public MultiDimensionalArray<Elem, DIM>{
	ContainerBounds _bounds;

	ContainerLengths setBounds(const InitBounds& initBounds){
		std::size_t lengths[DIM];
		_bounds = ContainerBounds(DIM, lengths, initBounds);
		return ContainerLengths(DIM, lengths);
	}

public:
	BoundedMultiDimensionalArray(const InitBounds& bounds, const Elem& elem = Elem()) : 
		MultiDimensionalArray<Elem, DIM>(setBounds(), elem){}


	bool inBounds(std::size_t index, std::size_t dimension=DIM) const{
		return (index <= upper(dimension)) && (length >= lower(dimension));
	}

	/* Renvoie la borne inférieure de la dimension donnée */
	std::size_t lower(std::size_t dim=DIM) const { 
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _bounds.lower(dim); 
	}

	/* Renvoie la borne supérieure de la dimension donnée */
	std::size_t upper(std::size_t dim=DIM) const {
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _bounds.upper(dim)this->length(dim)+_bounds[dim]-1;
	}

	const Elem& operator[] (std::ptrdiff_t i) const{ return }

	~BoundedMultiDimensionalArray() { delete[] _bounds; }
};

template<typename Elem>
class BoundedMultiDimensionalArray<Elem,1> : public MultiDimensionalArray<Elem, 1>{
	std::ptrdiff_t _lowerBound;

	void setBounds(const std::initializer_list<std::size_t>& bounds){
		if(bounds.size()!=2) throw std::logic_error("Initializer_list needs exactly 2 bounds for each dimension");
		std::initializer_list<std::size_t>::iterator boundsIt = bounds.begin();
		setLowerBound(*(boundsIt));
		setUpperBound(*(++boundsIt));
		if(getLowerBound() > getUpperBound()) throw std::logic_error("Lower bound cannot be greater than upper bound");
	}

	void setLowerBound(std::size_t newBound){ _bounds.first = newBound; }
	void setUpperBound(std::size_t newBound){ _bounds.second = newBound; }

public:
	BoundedMultiDimensionalArray(const std::initializer_list<std::size_t>& bounds, std::size_t length = Constants::DEFAULT_DIM_LENGTH, const Elem& elem = Elem()) :
		MultiDimensionalArray<Elem,1>(length, elem), _bounds(){
			setBounds(bounds);
			if(!inBounds(this->getLength())) throw std::logic_error("Given length not in bounds");
		}

		bool inBounds(std::size_t length) const { 
			return length <= getUpperBound() && length >= getLowerBound();
 		}

		std::size_t getLowerBound() const { return _bounds.first; }
		std::size_t getUpperBound() const { return _bounds.second; }

		void resize(std::size_t newLength){
			if(!inBounds(newLength)) throw std::out_of_range("Resizing out of bounds");
			MultiDimensionalArray<Elem,1>::resize(newLength);
		}
};

#endif