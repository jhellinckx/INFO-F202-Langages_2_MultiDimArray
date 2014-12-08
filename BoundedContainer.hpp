#ifndef BOUNDEDCONTAINER_HPP
#define BOUNDEDCONTAINER_HPP

#include <exception>
#include <stdexcept>

template<std::size_t DIM>
class BoundedContainer: virtual public DimensionalContainer<DIM>{
protected:
	std::ptrdiff_t* const _lower;
	BoundedContainer(std::size_t* lengths, std::ptrdiff_t* const lowerBounds): 
	DimensionalContainer<DIM>(lowerBounds){}
public:
	bool inBounds(std::size_t index, std::size_t dimension=DIM) const{
		return (index <= upper(dimension)) && (index >= lower(dimension));
	}
	/* Renvoie la borne inférieure de la dimension donnée */
	std::ptrdiff_t lower(std::size_t dim=DIM) const { 
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _lower[DIM-dim]; 
	}
	/* Renvoie la borne supérieure de la dimension donnée */
	std::ptrdiff_t upper(std::size_t dim=DIM) const {
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return lower(dim)+this->length()-1;
	}
};

template<>
class BoundedContainer<1>{
protected: 
	std::ptrdiff_t _lower;
	BoundedContainer(std::ptrdiff_t lowerBound): _lower(lowerBound){}
public:
	std::ptrdiff_t lower() const {return _lower;}
	std::ptrdiff_t upper() const {}

};


#endif