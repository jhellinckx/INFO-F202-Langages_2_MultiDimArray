/*
Nom : 			Hellinckx
Prénom : 		Jérôme
Matricule :		000376500
Année d'étude : BA2 - 2014-2015
*/

#ifndef SUBDIM_HPP
#define SUBDIM_HPP

#include <exception>
#include <stdexcept>
#include "AbstractDimensionalArray.hpp"

template<typename Elem, std::size_t DIM>
class DimensionalArray;

/* Cette classe réfère à un sous-tableau de DimensionalArray. 
Elle n'a pas la responsabilité en mémoire des Elem de ce tableau ni de ses caractéristiques. */
template<typename Elem, std::size_t DIM>
class SubDimensionalArray: public AbstractDimensionalArray<DIM>{
	friend class DimensionalArray<Elem, DIM+1>;
	friend class SubDimensionalArray<Elem, DIM+1>;
protected:
	Elem* const _val;
	/* Classe utilitaire pour DimensionalArray de DIM+1 et SubArray de DIM+1, constructeurs protected */
	SubDimensionalArray(std::size_t* const, Elem* const);
	SubDimensionalArray(const SubDimensionalArray&);
public:
	SubDimensionalArray& operator=(const SubDimensionalArray&);
	SubDimensionalArray& operator=(const DimensionalArray<Elem, DIM>&);
	/* Appel récursif tant que DIM>1 */
	inline SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t);
	inline SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t) const;
	virtual void print(std::ostream& out) const {
		out << "[";
		for(std::ptrdiff_t i=0;std::size_t(i)<this->length();++i){
  			(*this)[i].print(out);
  			if(std::size_t(i)!=this->length()-1)
  				out << ",";
  		}
  		out << "]";
	}
	virtual ~SubDimensionalArray(){}
};

/* Spécialisation pour DIM=1 */
template<typename Elem>
class SubDimensionalArray<Elem, 1>: public AbstractDimensionalArray<1>{
	friend class DimensionalArray<Elem, 2>;
	friend class SubDimensionalArray<Elem, 2>;
protected:
	Elem* const _val;
	SubDimensionalArray(std::size_t*, Elem* const);
	SubDimensionalArray(const SubDimensionalArray&);
public:
	SubDimensionalArray& operator=(const SubDimensionalArray&);
	SubDimensionalArray& operator=(const DimensionalArray<Elem, 1>&);
	inline Elem& operator[](std::ptrdiff_t);
	inline const Elem& operator[](std::ptrdiff_t) const;
	virtual void print(std::ostream& out) const{
		out << "[";
		for(std::ptrdiff_t i=0;std::size_t(i)<this->length();++i){
  			out << (*this)[i];
  			if(std::size_t(i)!=this->length()-1)
  				out << ",";
  		}
  		out << "]";
	}
	virtual ~SubDimensionalArray(){}
};

/* Définitions pour DIM>1 */
template<typename Elem, std::size_t DIM>
SubDimensionalArray<Elem, DIM>::SubDimensionalArray(std::size_t* const lengths, Elem* const val): 
		AbstractDimensionalArray<DIM>(lengths), _val(val) {}

template<typename Elem, std::size_t DIM>
SubDimensionalArray<Elem, DIM>::SubDimensionalArray(const SubDimensionalArray& other): 
	AbstractDimensionalArray<DIM>(other), _val(other._val) {}

template<typename Elem, std::size_t DIM>
SubDimensionalArray<Elem, DIM>& SubDimensionalArray<Elem, DIM>::operator=(const SubDimensionalArray& other){
	if(this != &other){
		if(this->sameLengths(other)){
			for(std::size_t i=0;i<this->size();++i){
				_val[i] = other._val[i];
			}
			return *this;
		}
		throw std::logic_error("Cannot assign subarrays of different lengths");
	}
	return *this;
}

template<typename Elem, std::size_t DIM>
SubDimensionalArray<Elem, DIM>& SubDimensionalArray<Elem, DIM>::operator=(const DimensionalArray<Elem, DIM>& other){
	if(this->sameLengths(other)){
		for(std::ptrdiff_t i=0;i<ptrdiff_t(this->length());++i){
			(*this)[i] = other[i];
		}
	}
	else throw std::logic_error("Cannot assign an array and a subarray of different lengths");
	return *this;
}

template<typename Elem, std::size_t DIM>
SubDimensionalArray<Elem, DIM-1> SubDimensionalArray<Elem, DIM>::operator[](std::ptrdiff_t i){
	if(std::size_t(i)>=this->length(DIM)) throw std::out_of_range("Index for dimension out of range");
	return SubDimensionalArray<Elem, DIM-1>(this->_lengths+1, _val+std::size_t(i)*this->size(DIM-1));
}

template<typename Elem, std::size_t DIM>
SubDimensionalArray<Elem, DIM-1> SubDimensionalArray<Elem, DIM>::operator[](std::ptrdiff_t i) const{
	if(std::size_t(i)>=this->length(DIM)) throw std::out_of_range("Index for dimension out of range");
	return SubDimensionalArray<Elem, DIM-1>(this->_lengths+1, _val+std::size_t(i)*this->size(DIM-1));
}

//------------------------------------------------------------

/* Définitions pour DIM=1 */
template<typename Elem>
SubDimensionalArray<Elem, 1>::SubDimensionalArray(std::size_t* lengths, Elem* const val): 
	AbstractDimensionalArray<1>(lengths[0]), _val(val) {}

template<typename Elem>
SubDimensionalArray<Elem, 1>::SubDimensionalArray(const SubDimensionalArray& other): 
	AbstractDimensionalArray<1>(other), _val(other._val) {}

template<typename Elem>
SubDimensionalArray<Elem, 1>& SubDimensionalArray<Elem, 1>::operator=(const SubDimensionalArray& other){
	if(this != &other){
		if(this->sameLength(other)){
			for(std::size_t i=0;i<this->size();++i) _val[i] = other._val[i];
			return *this;
		}
		throw std::logic_error("Cannot assign subarrays of different lengths");
	}
	return *this;
}

template<typename Elem>
SubDimensionalArray<Elem, 1>& SubDimensionalArray<Elem, 1>::operator=(const DimensionalArray<Elem, 1>& other){
	if(this->sameLength(other)){
		for(std::ptrdiff_t i=0;i<std::ptrdiff_t(this->length());++i) (*this)[i] = other[i];
	}
	else throw std::logic_error("Cannot assign an array and a subarray of different lengths");
	return *this;
}

template<typename Elem>
Elem& SubDimensionalArray<Elem, 1>::operator[](std::ptrdiff_t i){
	if(std::size_t(i)>=_length) throw std::out_of_range("Index out of range");
	return *(_val+i);
}

template<typename Elem>
const Elem& SubDimensionalArray<Elem, 1>::operator[](std::ptrdiff_t i) const{
	if(std::size_t(i)>=_length) throw std::out_of_range("Index out of range");
	return *(_val+i);
}

#endif
