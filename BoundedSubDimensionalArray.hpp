/*
Nom : 			Hellinckx
Prénom : 		Jérôme
Matricule :		000376500
Année d'étude : 2014-2015
*/

#ifndef BOUNDEDSUBDIM_HPP
#define BOUNDEDSUBDIM_HPP

#include "SubDimensionalArray.hpp"
#include "AbstractBoundedArray.hpp"
#include <exception>
#include <stdexcept>

template<typename Elem, std::size_t DIM>
class BoundedDimensionalArray;

/* Cette classe réfère à un sous-tableau de BoundedDimensionalArray.
Comme sa classe mère SubDimensionalArray, elle n'a aucune responsabilité 
mémoire pour ce tableau. */
template<typename Elem, std::size_t DIM>
class BoundedSubDimensionalArray: public AbstractBoundedArray<DIM>, public SubDimensionalArray<Elem, DIM>{
	friend class BoundedDimensionalArray<Elem, DIM+1>;
	friend class BoundedSubDimensionalArray<Elem, DIM+1>;
protected:
	BoundedSubDimensionalArray(std::size_t* const, Elem* const, std::ptrdiff_t* const);
	BoundedSubDimensionalArray(const BoundedSubDimensionalArray&);
public:
	virtual std::ptrdiff_t upper(std::size_t dim=DIM) const override;
	inline BoundedSubDimensionalArray<Elem, DIM-1> operator[] (std::ptrdiff_t) const;
	inline BoundedSubDimensionalArray<Elem, DIM-1> operator[] (std::ptrdiff_t);
	BoundedSubDimensionalArray& operator=(const SubArraySlice<Elem, DIM>&);
	virtual ~BoundedSubDimensionalArray(){}
};

/* Spécialisation pour DIM=1 */
template<typename Elem>
class BoundedSubDimensionalArray<Elem, 1>: public AbstractBoundedArray<1>, public SubDimensionalArray<Elem, 1>{
	friend class BoundedDimensionalArray<Elem, 2>;
	friend class BoundedSubDimensionalArray<Elem, 2>;
protected:
	BoundedSubDimensionalArray(std::size_t* const, Elem* const, std::ptrdiff_t* const);
	BoundedSubDimensionalArray(const BoundedSubDimensionalArray&);
public:
	virtual std::ptrdiff_t upper() const;
	inline const Elem& operator[](std::ptrdiff_t) const;
	inline Elem& operator[](std::ptrdiff_t); 
	BoundedSubDimensionalArray& operator= (const SubArraySlice<Elem, 1>&);
	virtual ~BoundedSubDimensionalArray(){}
};

/* Définitions pour DIM>1 */
template<typename Elem, std::size_t DIM>
BoundedSubDimensionalArray<Elem, DIM>::BoundedSubDimensionalArray(std::size_t* const lengths, Elem* const val, std::ptrdiff_t* const lowerBounds):
	AbstractBoundedArray<DIM>(lowerBounds, nullptr), SubDimensionalArray<Elem, DIM>(lengths, val) {}

template<typename Elem, std::size_t DIM>
BoundedSubDimensionalArray<Elem, DIM>::BoundedSubDimensionalArray(const BoundedSubDimensionalArray& other): 
	AbstractBoundedArray<DIM>(other), SubDimensionalArray<Elem, DIM>(other) {}

template<typename Elem, std::size_t DIM>
std::ptrdiff_t BoundedSubDimensionalArray<Elem, DIM>::upper(std::size_t dim) const {
	if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
	return this->lower(dim)+std::ptrdiff_t(this->length(dim))-1;
}

template<typename Elem, std::size_t DIM>
BoundedSubDimensionalArray<Elem, DIM-1> BoundedSubDimensionalArray<Elem, DIM>::operator[] (std::ptrdiff_t i) const{
	if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
	return BoundedSubDimensionalArray<Elem, DIM-1>(this->_lengths+1,this->_val+(i-this->_lower[0])*this->size(DIM-1),this->_lower+1);
}

template<typename Elem, std::size_t DIM>
BoundedSubDimensionalArray<Elem, DIM-1> BoundedSubDimensionalArray<Elem, DIM>::operator[] (std::ptrdiff_t i){
	if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
	return BoundedSubDimensionalArray<Elem, DIM-1>(this->_lengths+1,this->_val+std::size_t(i-this->_lower[0])*this->size(DIM-1),this->_lower+1);
}

template<typename Elem, std::size_t DIM>
BoundedSubDimensionalArray<Elem, DIM>& BoundedSubDimensionalArray<Elem, DIM>::operator= (const SubArraySlice<Elem, DIM>& slice){
	for(std::ptrdiff_t i = slice.lower();i <= slice.upper();++i)
		(*this)[i] = slice[i];
	return *this;
}

//------------------------------------------------------------------------------

/* Définitions pour DIM=1 */
template<typename Elem>
BoundedSubDimensionalArray<Elem, 1>::BoundedSubDimensionalArray(std::size_t* const lengths, Elem* const val, std::ptrdiff_t* const lowerBounds):
	AbstractBoundedArray<1>(lowerBounds[0], 0), SubDimensionalArray<Elem, 1>(lengths, val) {}

template<typename Elem>
BoundedSubDimensionalArray<Elem, 1>::BoundedSubDimensionalArray(const BoundedSubDimensionalArray& other) : 
	AbstractBoundedArray<1>(other), SubDimensionalArray<Elem, 1>(other) {}

template<typename Elem>
const Elem& BoundedSubDimensionalArray<Elem, 1>::operator[](std::ptrdiff_t i) const {
	if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
	return SubDimensionalArray<Elem, 1>::operator[](i-this->_lower);
}

template<typename Elem>
Elem& BoundedSubDimensionalArray<Elem, 1>::operator[](std::ptrdiff_t i) {
	if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
	return SubDimensionalArray<Elem, 1>::operator[](i-this->_lower);
}

template<typename Elem>
BoundedSubDimensionalArray<Elem, 1>& BoundedSubDimensionalArray<Elem, 1>::operator= (const SubArraySlice<Elem, 1>& slice){
	for(std::ptrdiff_t i = slice.lower();i <= slice.upper();++i)
		(*this)[i] = slice[i];
	return *this;
}

template<typename Elem>
std::ptrdiff_t BoundedSubDimensionalArray<Elem, 1>::upper() const { return this->lower()+ptrdiff_t(this->length())-1; }

#endif