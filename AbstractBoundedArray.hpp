/*
Nom : 			Hellinckx
Prénom : 		Jérôme
Matricule :		000376500
Année d'étude : 2014-2015
*/

#ifndef ABSTRACTBOUNDEDARRAY_HPP
#define ABSTRACTBOUNDEDARRAY_HPP

#include <exception>
#include <stdexcept>

/* Modèle de classe définissiant des bornes inférieures et supérieures pour chaque 
dimension comprise entre 0 et DIM. N'a pas la responsabilité en mémoire des bornes si DIM>1. */
template<std::size_t DIM>
class AbstractBoundedArray{
protected: 
	std::ptrdiff_t* _lower;
	std::ptrdiff_t* _upper;
	AbstractBoundedArray(std::ptrdiff_t* lowerBounds, std::ptrdiff_t* upperBounds):
		_lower(lowerBounds), _upper(upperBounds) {}
	AbstractBoundedArray(const AbstractBoundedArray<DIM>& other): _lower(other._lower), _upper(other._upper){}
	AbstractBoundedArray& operator=(const AbstractBoundedArray<DIM>& other){
		if(this!=&other){
			this->_lower = other._lower; this->_upper = other._upper;
		}
		return *this;
	}
public:
	virtual bool inBounds(std::ptrdiff_t i, std::size_t dimension=DIM) const{
		return (i <= this->upper(dimension)) && (i >= this->lower(dimension));
	}
	virtual std::ptrdiff_t lower(std::size_t dim=DIM) const { 
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _lower[DIM-dim]; 
	}
	virtual std::ptrdiff_t upper(std::size_t dim=DIM) const {
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _upper[DIM-dim];
	}
	virtual void print(std::ostream& out) const{
		for(std::size_t dim=DIM;dim>0;--dim) out << "(" << this->lower(dim) << "," << this->upper(dim) << ") ";
	}
	virtual ~AbstractBoundedArray(){}
};

template<>
class AbstractBoundedArray<1>{
protected:
	std::ptrdiff_t _lower;
	std::ptrdiff_t _upper;
	AbstractBoundedArray(std::ptrdiff_t lowerBound, std::ptrdiff_t upperBound):
		_lower(lowerBound), _upper(upperBound){}
	AbstractBoundedArray(const AbstractBoundedArray<1>& other) : _lower(other._lower), _upper(other._upper){}
public:
	virtual bool inBounds(std::ptrdiff_t i) const { return (i>=this->lower()) && (i<=this->upper()); }
	virtual std::ptrdiff_t lower() const { return _lower; }
	virtual std::ptrdiff_t upper() const { return _upper; }
	virtual void print(std::ostream& out) const { out << "(" << this->lower() << "," << this->upper() << ") "; }
	virtual ~AbstractBoundedArray(){}
};

#endif