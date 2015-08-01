/*
Nom : 			Hellinckx
Prénom : 		Jérôme
Matricule :		000376500
Année d'étude : 2014-2015
*/

#ifndef BOUNDEDMULTIDIM_HPP
#define BOUNDEDMULTIDIM_HPP

#include <exception>
#include <stdexcept>
#include <array>
#include "DimensionalArray.hpp"
#include "AbstractBoundedArray.hpp"
#include "BoundedSubDimensionalArray.hpp"
#include "ArraySlice.hpp"

typedef std::initializer_list<std::initializer_list<std::ptrdiff_t>> InitBounds;

/* Classe définissant des bornes pour chaque dimension délimitant les valeurs prises par l'opérateur [].
A la responsabilité mémoire du vecteur des bornes inférieures. */

template<typename Elem, size_t DIM>
class BoundedDimensionalArray : public AbstractBoundedArray<DIM>, public DimensionalArray<Elem, DIM>{
	/* Initialisation des bornes pour chaque dimension à partir d'une initializer_list 
	dont chaque élément contient une borne inférieure et une borne supérieure */
	std::array<std::size_t, DIM> setBounds(const InitBounds&);
	/* Initalisation des bornes à partir des bornes d'un slice */
	std::array<std::size_t, DIM> setBounds(const ArraySlice<Elem, DIM>&);
public:
	/* upperBound à nullptr pour AbstractBoundedArray car upperBound = lowerBound+length-1 */
	explicit BoundedDimensionalArray(const InitBounds& bounds, const Elem& elem = Elem()) : 
		AbstractBoundedArray<DIM>(new std::ptrdiff_t[DIM], nullptr), DimensionalArray<Elem, DIM>(setBounds(bounds), elem) {}
	BoundedDimensionalArray(const BoundedDimensionalArray&);
	BoundedDimensionalArray(BoundedDimensionalArray&&);
	BoundedDimensionalArray& operator=(const BoundedDimensionalArray&);
	BoundedDimensionalArray& operator=(BoundedDimensionalArray&&);
	inline BoundedSubDimensionalArray<Elem, DIM-1> operator[] (std::ptrdiff_t) const;
	inline BoundedSubDimensionalArray<Elem, DIM-1> operator[] (std::ptrdiff_t); 
	/* Overriding de AbstractBoundedArray::upper() car cette classe n'utilise pas de borne supérieure */
	virtual std::ptrdiff_t upper(std::size_t dim=DIM) const;
	
	/* Construction à partir d'un slice. On utilise l'opérateur [] de cette classe et de ArraySlice pour 
	appeller operator=(SubArraySlice&) de BoundedSubArray récursivement jusque DIM=1 où les Elem sont copiés. */
	BoundedDimensionalArray(const ArraySlice<Elem, DIM>&); 
	/* Assignation à partir d'un slice */
	BoundedDimensionalArray& operator=(const ArraySlice<Elem, DIM>&);
	/* Ce tableau étant borné pour chaque dimension, les valeurs données dans les listes d'initialisation
	doivent être comprises entre les bornes (sauf le pas) */
	virtual ArraySlice<Elem, DIM> slice(const InitBounds&) const;
	virtual void print(std::ostream& out) const{
		AbstractBoundedArray<DIM>::print(out);
		out << "-> ";
		DimensionalArray<Elem, DIM>::print(out);
	}
	virtual ~BoundedDimensionalArray() { delete[] this->_lower; }
};

/* Spécialisation pour DIM=1 */
template<typename Elem>
class BoundedDimensionalArray<Elem, 1> : public AbstractBoundedArray<1>, public DimensionalArray<Elem, 1>{
	std::size_t setBounds(const std::initializer_list<std::ptrdiff_t>&);
public:
	explicit BoundedDimensionalArray(const std::initializer_list<std::ptrdiff_t>& bounds, const Elem& elem = Elem()) :
		AbstractBoundedArray<1>(0, 0), DimensionalArray<Elem,1>(setBounds(bounds), elem){}
	BoundedDimensionalArray(const BoundedDimensionalArray&);
	BoundedDimensionalArray(BoundedDimensionalArray&&);
	BoundedDimensionalArray& operator=(const BoundedDimensionalArray&);
	BoundedDimensionalArray& operator=(BoundedDimensionalArray&&);
	virtual std::ptrdiff_t upper() const; 
	inline const Elem& operator[] (std::ptrdiff_t) const;
	inline Elem& operator[] (std::ptrdiff_t);
	/* Construction à partir d'un slice */
	BoundedDimensionalArray(const ArraySlice<Elem, 1>&);
	/* Assignation à partir d'un slice */
	BoundedDimensionalArray& operator= (const ArraySlice<Elem, 1>&);
	virtual ArraySlice<Elem, 1> slice(const std::initializer_list<std::ptrdiff_t>&);
	virtual void print(std::ostream& out) const{
		AbstractBoundedArray<1>::print(out);
		out << "-> ";
		DimensionalArray<Elem, 1>::print(out);
	}
	virtual ~BoundedDimensionalArray(){}	
};

/* Définitions pour DIM > 1 */ 
template<typename Elem, std::size_t DIM>
std::array<std::size_t, DIM> BoundedDimensionalArray<Elem, DIM>::setBounds(const InitBounds& initBounds){
	if(initBounds.size()!=DIM) throw std::logic_error("Initializer_list needs bounds for each dimension");
		std::array<std::size_t, DIM> lengthsArray;
		std::size_t dim=DIM;
		for(InitBounds::iterator dimensionIt = initBounds.begin();dimensionIt!=initBounds.end();++dimensionIt){
			if(dimensionIt->size()!=2) throw std::logic_error("Initializer_list needs exactly 2 bounds for each dimension");
			std::initializer_list<std::ptrdiff_t>::iterator boundsIt = dimensionIt->begin();
			const std::ptrdiff_t& lowerBound = *(boundsIt); const std::ptrdiff_t& upperBound = *(++boundsIt);
			upperBound-lowerBound+1 < 0 ? throw std::logic_error("Lower bound cannot be greater than upper bound") : lengthsArray[DIM-dim] = std::size_t(upperBound-lowerBound+1);
			this->_lower[DIM-dim--] = lowerBound;
		}
	return lengthsArray;
}

template<typename Elem, std::size_t DIM>
std::array<std::size_t, DIM> BoundedDimensionalArray<Elem, DIM>::setBounds(const ArraySlice<Elem, DIM>& slice){
	std::array<std::size_t, DIM> lengthsArray;
	for(std::size_t dim=DIM;dim>0;--dim){
		if(slice.pitch(dim) != 1)
			throw std::logic_error("Slice pitch must be 1 to convert it into a bounded array");
		lengthsArray[DIM-dim] = slice.upper(dim) - slice.lower(dim) + 1;
		this->_lower[DIM-dim] = slice.lower(dim);
	}
	return lengthsArray;
}

template<typename Elem, std::size_t DIM>
BoundedDimensionalArray<Elem, DIM>::BoundedDimensionalArray(const BoundedDimensionalArray& other): 
	AbstractBoundedArray<DIM>(new std::ptrdiff_t[DIM], nullptr), DimensionalArray<Elem, DIM>(other){
		for(std::size_t i=0;i<DIM;++i) this->_lower[i] = other._lower[i];
}

template<typename Elem, std::size_t DIM>
BoundedDimensionalArray<Elem, DIM>::BoundedDimensionalArray(BoundedDimensionalArray&& other): 
	AbstractBoundedArray<DIM>(other._lower, other._upper), DimensionalArray<Elem, DIM>(std::move(other)) {
		other._lower = nullptr; other._upper = nullptr;
}

template<typename Elem, std::size_t DIM>
BoundedDimensionalArray<Elem, DIM>& BoundedDimensionalArray<Elem, DIM>::operator=(const BoundedDimensionalArray& other){
	if(this!=&other){
		DimensionalArray<Elem, DIM>::operator=(other);
		for(std::size_t i=0;i<DIM;++i) this->_lower[i] = other._lower[i];
	}
	return *this;
}

template<typename Elem, std::size_t DIM>
BoundedDimensionalArray<Elem, DIM>& BoundedDimensionalArray<Elem, DIM>::operator=(BoundedDimensionalArray&& other){
	if(this!=&other){
		DimensionalArray<Elem, DIM>::operator=(std::move(other));
		delete[] this->_lower;
		this->_lower = other._lower; this->_upper = other._upper;
		other._lower = nullptr; other._upper = nullptr;
	}
	return *this;
}

template<typename Elem, std::size_t DIM>
BoundedSubDimensionalArray<Elem, DIM-1> BoundedDimensionalArray<Elem, DIM>::operator[] (std::ptrdiff_t i) const{ 
	if(!this->inBounds(i)) throw std::out_of_range("Index out of bounds");
	return BoundedSubDimensionalArray<Elem, DIM-1>(this->_lengths+1,this->_val+std::size_t(i-this->_lower[0])*this->size(DIM-1),this->_lower+1); 
}

template<typename Elem, std::size_t DIM>
BoundedSubDimensionalArray<Elem, DIM-1> BoundedDimensionalArray<Elem, DIM>::operator[] (std::ptrdiff_t i) {
	if(!this->inBounds(i)) throw std::out_of_range("Index out of bounds");
	return BoundedSubDimensionalArray<Elem, DIM-1>(this->_lengths+1,this->_val+std::size_t(i-this->_lower[0])*this->size(DIM-1),this->_lower+1); 
}

template<typename Elem, std::size_t DIM>
std::ptrdiff_t BoundedDimensionalArray<Elem, DIM>::upper(std::size_t dim) const {
	if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
	return this->lower(dim)+std::ptrdiff_t(this->length(dim))-1;
}

template<typename Elem, std::size_t DIM>
BoundedDimensionalArray<Elem, DIM>::BoundedDimensionalArray(const ArraySlice<Elem, DIM>& slice) : 
	AbstractBoundedArray<DIM>(new std::ptrdiff_t[DIM], nullptr), DimensionalArray<Elem, DIM>(setBounds(slice)){
		for(std::ptrdiff_t i = this->lower();i <= this->upper();++i)
			(*this)[i] = slice[i];
} 

template<typename Elem, std::size_t DIM>
BoundedDimensionalArray<Elem, DIM>& BoundedDimensionalArray<Elem, DIM>::operator=(const ArraySlice<Elem, DIM>& slice){
 	this->resize(setBounds(slice), false);
 	for(std::ptrdiff_t i = this->lower();i <= this->upper();++i)
			(*this)[i] = slice[i];
	return *this;
}

template<typename Elem, std::size_t DIM>
ArraySlice<Elem, DIM> BoundedDimensionalArray<Elem, DIM>::slice(const InitBounds& initSlice) const {
	return ArraySlice<Elem, DIM>(this->_lengths, this->_val, initSlice, this->_lower);
}

//--------------------------------------------------------------------

/* Définitions pour DIM=1 */
template<typename Elem>
std::size_t BoundedDimensionalArray<Elem, 1>::setBounds(const std::initializer_list<std::ptrdiff_t>& bounds){
	if(bounds.size()!=2) throw std::logic_error("Initializer_list needs exactly 2 bounds for each dimension");
	std::initializer_list<std::ptrdiff_t>::iterator boundsIt = bounds.begin();
	std::size_t length;
	std::ptrdiff_t lowerBound = *boundsIt; std::ptrdiff_t upperBound = *(++boundsIt);
	upperBound-lowerBound+1 < 0 ? throw std::logic_error("Lower bound cannot be greater than upper bound") : length = std::size_t(upperBound-lowerBound+1);
	this->_lower = lowerBound; 
	return length;
}

template<typename Elem>
BoundedDimensionalArray<Elem, 1>::BoundedDimensionalArray(const BoundedDimensionalArray& other):
	AbstractBoundedArray<1>(other), DimensionalArray<Elem, 1>(other) {}
	
template<typename Elem>
BoundedDimensionalArray<Elem, 1>::BoundedDimensionalArray(BoundedDimensionalArray&& other): 
	AbstractBoundedArray<1>(other._lower, other._upper), DimensionalArray<Elem, 1>(std::move(other)){
		other._lower = 0; other._upper = 0;
}

template<typename Elem>
BoundedDimensionalArray<Elem, 1>& BoundedDimensionalArray<Elem, 1>::operator=(const BoundedDimensionalArray& other){
	if(this!=&other){
		DimensionalArray<Elem, 1>::operator=(other);
		this->_lower = other._lower; this->_upper = other._upper;
	}
	return *this;
}

template<typename Elem>
BoundedDimensionalArray<Elem, 1>& BoundedDimensionalArray<Elem, 1>::operator=(BoundedDimensionalArray&& other){
	if(this!=&other){
		DimensionalArray<Elem, 1>::operator=(std::move(other));
		this->_lower = other._lower; this->_upper = other._upper;
		other._lower = 0; other._upper = 0;
	}
	return *this;
}

template<typename Elem>
std::ptrdiff_t BoundedDimensionalArray<Elem, 1>::upper() const { return this->lower()+std::ptrdiff_t(this->length())-1; }

template<typename Elem>
const Elem& BoundedDimensionalArray<Elem, 1>::operator[] (std::ptrdiff_t i) const{
	if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
	return DimensionalArray<Elem, 1>::operator[](i-this->_lower);
}

template<typename Elem>
Elem& BoundedDimensionalArray<Elem, 1>::operator[] (std::ptrdiff_t i) {
	if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
	return DimensionalArray<Elem, 1>::operator[](i-this->_lower);
}

template<typename Elem>
BoundedDimensionalArray<Elem, 1>::BoundedDimensionalArray(const ArraySlice<Elem, 1>& slice): 
	AbstractBoundedArray<1>(slice.lower(), 0),
	DimensionalArray<Elem, 1>(std::size_t(slice.upper()-slice.lower()+1)){
		for(std::ptrdiff_t i = this->lower(); i<=this->upper();++i)
		(*this)[i] = slice[i];
}

template<typename Elem>
BoundedDimensionalArray<Elem, 1>& BoundedDimensionalArray<Elem, 1>::operator= (const ArraySlice<Elem, 1>& slice){
	if(slice.pitch() != 1) throw std::logic_error("Slice pitch must be 1 to convert it into a bounded array");
	this->_lower = slice.lower();
	this->resize(slice.upper()-slice.lower()+1, false);
	for(std::ptrdiff_t i = this->lower();i <= this->upper();++i)
		(*this)[i] = slice[i];
	return *this;
}

template<typename Elem>
ArraySlice<Elem, 1> BoundedDimensionalArray<Elem, 1>::slice(const std::initializer_list<std::ptrdiff_t>& initSlice){
	return ArraySlice<Elem, 1>(&(this->_length), this->_val, initSlice, this->_lower);
}

#endif