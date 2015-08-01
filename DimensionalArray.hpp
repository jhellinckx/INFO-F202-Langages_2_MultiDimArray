/*
Nom : 			Hellinckx
Prénom : 		Jérôme
Matricule :		000376500
Année d'étude : 2014-2015
*/

#ifndef MULTIDIM_HPP
#define MULTIDIM_HPP

#include <exception>
#include <stdexcept>
#include <array>
#include "AbstractDimensionalArray.hpp"
#include "SubDimensionalArray.hpp" 
#include "ArraySlice.hpp"

/* Classe représentant un tableau de dimension DIM. 
Alloue dynamiquement un vecteur comprenant les longueurs, et un vecteur d'Elem. */
template<typename Elem, std::size_t DIM>
class DimensionalArray: public AbstractDimensionalArray<DIM>{
	/* Alloue dynamiquement un vecteur contenant les longeurs de chaque dimension */
	std::size_t* storeLengths(const std::initializer_list<std::size_t>&);
	std::size_t* storeLengths(const std::array<std::size_t, DIM>&);
protected:
	Elem* _val;
public:
	explicit DimensionalArray(const std::array<std::size_t, DIM>&, const Elem& elem = Elem());
	explicit DimensionalArray(const std::initializer_list<std::size_t>&, const Elem& elem = Elem());
	DimensionalArray(const DimensionalArray&);
	DimensionalArray(DimensionalArray&&);
	DimensionalArray& operator=(const DimensionalArray&);
	DimensionalArray& operator=(DimensionalArray&&);
	/* L'opérateur [] s'appelle récursivement sur SubDimensionalArray et renvoie un SubDimensionalArray
	de DIM-1 tant que DIM!=1. 
	Pour créer un SubDimensionalArray de DIM-1, il suffit d'effectuer un décalage d'une unité sur _lengths,
	et de i*j unités sur _val, où j est la taille d'un vecteur de DIM-1, et i la position de
	ce vecteur. */
	inline SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t);
	inline SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t) const;
	virtual void resize(const std::array<std::size_t, DIM>&, bool cpyValues = true);
	virtual void resize(const std::initializer_list<std::size_t>&, bool cpyValues = true);
	/* Pour construire un slice, l'init_list doit contenir DIM*init_list, qui elles-mêmes doivent contenir entre 
	2 et 3 valeurs {inf, sup, pas=1} */
	virtual ArraySlice<Elem, DIM> slice(const std::initializer_list<std::initializer_list<std::ptrdiff_t>>&) const;
	virtual void print(std::ostream& out) const{
		AbstractDimensionalArray<DIM>::print(out);
		out << ": ";
		out << "[";
		for(std::ptrdiff_t i=0;std::size_t(i)<this->length();++i){
  			(*this)[i].print(out);
  			if(std::size_t(i)!=this->length()-1)
  				out << ",";
  		}
  		out << "]";
	}
	virtual ~DimensionalArray(){delete[] this->_val; delete[] this->_lengths;}
};

/* Spécialisation pour DIM=1 */
template<typename Elem>
class DimensionalArray<Elem, 1>: public AbstractDimensionalArray<1>{
protected:
	Elem* _val;
public:
	explicit DimensionalArray(std::size_t, const Elem& elem = Elem());
	DimensionalArray(const DimensionalArray&);
	DimensionalArray(DimensionalArray&& other);
	DimensionalArray& operator=(const DimensionalArray&);
	DimensionalArray& operator=(DimensionalArray&&);
	const Elem& operator[](std::ptrdiff_t) const;
	Elem& operator[](std::ptrdiff_t);
	virtual ArraySlice<Elem, 1> slice(const std::initializer_list<std::ptrdiff_t>&);
	void resize(std::size_t, bool cpyValues = true);
	virtual void print(std::ostream& out) const {
		AbstractDimensionalArray<1>::print(out);
		out << "[";
		for(std::ptrdiff_t i=0;std::size_t(i)<this->length();++i){
  			out << (*this)[i];
  			if(std::size_t(i)!=this->length()-1)
  				out << ",";
  		}
  		out << "]";
	}
	virtual ~DimensionalArray(){ delete[] this->_val; }
};

/* Définitions pour DIM>1 */
template<typename Elem, std::size_t DIM>
std::size_t* DimensionalArray<Elem, DIM>::storeLengths(const std::initializer_list<std::size_t>& lengthsList){
	if(lengthsList.size() != DIM) throw std::logic_error("Dimensions not matching");
	std::size_t* lengths = new std::size_t[DIM];
	std::copy(std::begin(lengthsList), std::end(lengthsList), lengths);
	return lengths;
}

template<typename Elem, std::size_t DIM>
std::size_t* DimensionalArray<Elem, DIM>::storeLengths(const std::array<std::size_t, DIM>& lengthsArray){
	if(lengthsArray.size() != DIM) throw std::logic_error("Dimensions not matching");
	std::size_t* lengths = new std::size_t[DIM];
	std::copy(std::begin(lengthsArray), std::end(lengthsArray), lengths);
	return lengths;
}

template<typename Elem, std::size_t DIM>
DimensionalArray<Elem, DIM>::DimensionalArray(const std::array<std::size_t, DIM>& lengthsList, const Elem& elem):
	AbstractDimensionalArray<DIM>(storeLengths(lengthsList)), _val(new Elem[this->size()]){
		for(std::size_t i=0;i<this->size();++i) _val[i] = elem;
}

template<typename Elem, std::size_t DIM>
DimensionalArray<Elem, DIM>::DimensionalArray(const std::initializer_list<std::size_t>& lengthsList, const Elem& elem): 
	AbstractDimensionalArray<DIM>(storeLengths(lengthsList)), _val(new Elem[this->size()]){
		for(std::size_t i=0;i<this->size();++i) _val[i] = elem;
}

template<typename Elem, std::size_t DIM>
DimensionalArray<Elem, DIM>::DimensionalArray(const DimensionalArray& other): 
	AbstractDimensionalArray<DIM>(new std::size_t[DIM]), _val(new Elem[other.size()]){
		for(std::size_t i=0;i<DIM;++i) this->_lengths[i] = other._lengths[i];
		for(std::size_t i=0;i<this->size();++i) this->_val[i] = other._val[i];
} 

template<typename Elem, std::size_t DIM>
DimensionalArray<Elem, DIM>::DimensionalArray(DimensionalArray&& other): 
	AbstractDimensionalArray<DIM>(other._lengths), _val(other._val){
		other._lengths = nullptr; other._val = nullptr;
}

template<typename Elem, std::size_t DIM>
DimensionalArray<Elem, DIM>& DimensionalArray<Elem, DIM>::operator=(const DimensionalArray& other){
	if(this!=&other){
		if(this->sameLengths(other)){
			for(std::size_t i=0;i<this->size();++i) _val[i] = other._val[i];
			return *this;
		}
		delete[] _val; _val = new Elem[other.size()];
		AbstractDimensionalArray<DIM>::operator=(other);
		for(std::size_t i=0;i<this->size();++i) _val[i] = other._val[i];
	}
	return *this;
}

template<typename Elem, std::size_t DIM>
DimensionalArray<Elem, DIM>& DimensionalArray<Elem, DIM>::operator=(DimensionalArray&& other){
	if(this!=&other){
		delete[] _val; delete[] this->_lengths; 
		this->_lengths = other._lengths; this->_val = other._val;
		other._lengths = nullptr; other._val = nullptr; 
	}
	return *this;
}

template<typename Elem, std::size_t DIM>
SubDimensionalArray<Elem, DIM-1> DimensionalArray<Elem, DIM>::operator[](std::ptrdiff_t i){
	if(std::size_t(i)>=this->length(DIM)) throw std::out_of_range("Index for dimension out of range");
	return SubDimensionalArray<Elem, DIM-1>(this->_lengths+1, _val+std::size_t(i)*this->size(DIM-1));
}

template<typename Elem, std::size_t DIM>
SubDimensionalArray<Elem, DIM-1> DimensionalArray<Elem, DIM>::operator[](std::ptrdiff_t i) const{
	if(std::size_t(i)>=this->length(DIM)) throw std::out_of_range("Index for dimension out of range");
	return SubDimensionalArray<Elem, DIM-1>(this->_lengths+1, _val+std::size_t(i)*this->size(DIM-1));
}

template<typename Elem, std::size_t DIM>
void DimensionalArray<Elem, DIM>::resize(const std::array<std::size_t, DIM>& lengthsArray, bool cpyValues){
	std::size_t oldSize = this->size();
	std::copy(lengthsArray.begin(), lengthsArray.end(), this->_lengths);
	std::size_t newSize = this->size();
	Elem* tmp = new Elem[newSize];
	/* Initialisation du nouveau vecteur, les anciennes valeurs sont conservées au possible 
	si cpyValues est true */
	if(cpyValues){
		for(std::size_t i=0;i<newSize;++i) (i<oldSize) ? tmp[i] = _val[i] : tmp[i] = Elem();
	}
	delete[] _val; _val = tmp; tmp = nullptr;
}

template<typename Elem, std::size_t DIM>
void DimensionalArray<Elem, DIM>::resize(const std::initializer_list<std::size_t>& lengthsList, bool cpyValues){
	if(lengthsList.size()!=DIM) throw std::logic_error("Dimensions not matching");
	std::array<std::size_t, DIM> lengthsArray;
	std::copy(lengthsList.begin(),lengthsList.end(),lengthsArray.begin());
	resize(lengthsArray, cpyValues);
}

template<typename Elem, std::size_t DIM>
ArraySlice<Elem, DIM> DimensionalArray<Elem, DIM>::slice(const std::initializer_list<std::initializer_list<std::ptrdiff_t>>& initSlice) const {
	return ArraySlice<Elem, DIM>(this->_lengths, this->_val, initSlice);
}

//------------------------------------------------------------

/* Définitions pour DIM=1 */
template<typename Elem>
DimensionalArray<Elem, 1>::DimensionalArray(std::size_t length, const Elem& elem): 
	AbstractDimensionalArray<1>(length), _val(new Elem[_length]) {
		for(std::size_t i=0;i<_length;++i) _val[i] = elem;
}

template<typename Elem>
DimensionalArray<Elem, 1>::DimensionalArray(const DimensionalArray& other):
	AbstractDimensionalArray<1>(other._length), _val(new Elem[_length]){
		for(std::size_t i=0;i<_length;++i) _val[i] = other._val[i];
}

template<typename Elem>
DimensionalArray<Elem, 1>::DimensionalArray(DimensionalArray&& other): 
	AbstractDimensionalArray<1>(other._length), _val(other._val){
		other._length = 0; other._val = nullptr;
}

template<typename Elem>
DimensionalArray<Elem, 1>& DimensionalArray<Elem, 1>::operator=(const DimensionalArray& other){
	if(this!=&other){
		if(_length == other._length){
			for(std::size_t i=0;i<this->length();++i) _val[i] = other._val[i];  
			return *this;
		}
		delete[] _val; _val = new Elem[other._length]; _length = other._length;
		for(std::size_t i=0;i<this->size();++i) _val[i] = other._val[i];
	}
	return *this;
}

template<typename Elem>
DimensionalArray<Elem, 1>& DimensionalArray<Elem, 1>::operator=(DimensionalArray&& other){
	if(this!=&other){
		delete[] _val; this->_length = other._length; _val = other._val;
		other._length = 0; other._val = nullptr;
	}
	return *this;
}

template<typename Elem>
const Elem& DimensionalArray<Elem, 1>::operator[](std::ptrdiff_t i) const{
	if(std::size_t(i)>_length) throw std::out_of_range("Index out of range");
	return _val[i];
}

template<typename Elem>
Elem& DimensionalArray<Elem, 1>::operator[](std::ptrdiff_t i){
	if(std::size_t(i)>_length) throw std::out_of_range("Index out of range");
	return _val[i];
}

template<typename Elem>
ArraySlice<Elem, 1> DimensionalArray<Elem, 1>::slice(const std::initializer_list<std::ptrdiff_t>& initSlice) {
	return ArraySlice<Elem, 1>(&(this->_length), this->_val, initSlice);
}

template<typename Elem>
void DimensionalArray<Elem, 1>::resize(std::size_t newLength, bool cpyValues){
	std::size_t oldLength = _length;
	_length = newLength;
	Elem* tmp = new Elem[newLength];
	if(cpyValues){
		for(std::size_t i=0;i<newLength;++i) (i<oldLength) ? tmp[i] = _val[i] : tmp[i] = Elem();
	}
	delete[] _val; _val = tmp; tmp = nullptr;
}

#endif