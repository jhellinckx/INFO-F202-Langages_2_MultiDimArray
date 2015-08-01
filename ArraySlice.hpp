/*
Nom : 			Hellinckx
Prénom : 		Jérôme
Matricule :		000376500
Année d'étude : 2014-2015
*/

#ifndef SLICE_HPP
#define SLICE_HPP

#define DEFAULT_PITCH 1

#include <exception>
#include <stdexcept>
#include "AbstractBoundedArray.hpp"
#include "DimensionalArray.hpp"
#include "SubArraySlice.hpp"

typedef std::initializer_list<std::initializer_list<std::ptrdiff_t>> InitSlice;

/* Cette classe représente un accès filtré à un tableau : chaque dimension a une borne
inférieure, une borne supérieure et un pas. 
Gère dynamiquement 4 vecteurs : borne inférieure, borne supérieure, pas et décalage */
template<typename Elem, std::size_t DIM>
class ArraySlice : public AbstractBoundedArray<DIM>, public SubDimensionalArray<Elem, DIM> {
	std::size_t* _pitch; 		//pas
	std::ptrdiff_t* _offset; 	//Si le slice est construit depuis un array borné (BoundedDimensionalArray)
	/* Initialise les bornes, le pas, et le décalage éventuel à partir d'une init_list */
	void setBounds(const InitSlice&, std::ptrdiff_t*);
public:
	ArraySlice(std::size_t*, Elem* const, const InitSlice&, std::ptrdiff_t* offset= nullptr);
	ArraySlice(const ArraySlice&);
	ArraySlice(ArraySlice&&);
	ArraySlice& operator=(const ArraySlice&);
	ArraySlice& operator=(ArraySlice&& other);
	inline SubArraySlice<Elem, DIM-1> operator[](std::ptrdiff_t) const;
	inline SubArraySlice<Elem, DIM-1> operator[](std::ptrdiff_t);
	virtual std::ptrdiff_t offset(std::size_t dim=DIM) const;
	virtual std::size_t pitch(std::size_t dim=DIM) const;
	virtual bool inBounds(std::ptrdiff_t, std::size_t dim=DIM) const;
	virtual void print(std::ostream& out) const{
		AbstractBoundedArray<DIM>::print(out);
		out << "-> ";
		for(size_t dim=DIM;dim>0;--dim) out << this->upper(dim) - this->lower(dim) + 1 << " ";
		out << ": ";
		out << "[";
		for(std::ptrdiff_t i=0;std::size_t(i)<this->length();++i){
			i+=this->offset();
  			if(this->inBounds(i)) {
  				(*this)[i].print(out);
  				if(i!=this->upper()) out << ",";
  			}
  			i-=this->offset();
  		}
  		out << "]";
	}
	virtual ~ArraySlice() {delete[] this->_lower; delete[] this->_upper; delete[] this->_pitch; delete[] this->_offset; }
};

/* Spécialisation pour DIM=1 */
template<typename Elem>
class ArraySlice<Elem, 1>: public AbstractBoundedArray<1>, public SubDimensionalArray<Elem, 1>{
	std::size_t _pitch;
	std::ptrdiff_t _offset;
	void setBounds(const std::initializer_list<std::ptrdiff_t>&);
public:
	ArraySlice(std::size_t* const, Elem* const, const std::initializer_list<std::ptrdiff_t>&, std::ptrdiff_t offset = 0);
	ArraySlice(const ArraySlice& other);
	ArraySlice(ArraySlice&& other);
	/* Les opérateurs = ne copient pas les Elem filtrés du Slice donné, mais effectuent une copie de ce Slice */
	ArraySlice& operator=(const ArraySlice&);
	ArraySlice& operator=(ArraySlice&&);
	virtual std::ptrdiff_t offset() const { return _offset; }
	virtual std::size_t pitch() const { return _pitch; }
	virtual bool inBounds(std::ptrdiff_t i) const{
		if(AbstractBoundedArray<1>::inBounds(i))
			return (std::size_t(i - this->lower()) % this->pitch()) == 0;
		return false;
	}
	inline const Elem& operator[] (std::ptrdiff_t) const;
	inline Elem& operator[] (std::ptrdiff_t);
	virtual void print(std::ostream& out) const{
		AbstractBoundedArray<1>::print(out);
		out << this->upper() - this->lower() + 1 << " ";
		out << ": ";
		out << "[";
		for(std::ptrdiff_t i=0;std::size_t(i)<this->length();++i){
			i+=this->offset();
  			if(this->inBounds(i)) {
  				out << (*this)[i];
  				if(i!=this->upper()) out << ",";
  			}
  			i-=this->offset();
  		}
  		out << "]";
	}
	virtual ~ArraySlice(){}
};

/* Définitions pour DIM>1 */
template<typename Elem, std::size_t DIM>
void ArraySlice<Elem, DIM>::setBounds(const InitSlice& initSlice, std::ptrdiff_t* offset){
	if(initSlice.size()!=DIM) throw std::logic_error("Dimensions not matching");
	std::size_t dim=DIM;
	for(InitSlice::iterator dimensionIt = initSlice.begin(); dimensionIt != initSlice.end(); dimensionIt++){
		if(dimensionIt->size()!=2 && dimensionIt->size()!=3) 
			throw std::logic_error("Slices need at least 2 and at most 3 parameters for each dimension (pitch has default value)");
		std::initializer_list<std::ptrdiff_t>::iterator boundsIt = dimensionIt->begin();
		const std::ptrdiff_t& lowerBound = *boundsIt; const std::ptrdiff_t& upperBound = *(++boundsIt); //au moins 2 paramètres
		std::ptrdiff_t offsetBound = 0;
		/* Si le tableau est borné (décalé -> BoundedDimensionalArray), copier son vecteur de bornes inférieures,
		sinon initialisé ce vecteur à 0 */
		if(offset!=nullptr) offsetBound = offset[DIM-dim]; 
		_offset[DIM-dim] = offsetBound;
		if((lowerBound<offsetBound || lowerBound>=std::ptrdiff_t(this->length(dim))+offsetBound) || (upperBound<offsetBound || upperBound >= std::ptrdiff_t(this->length(dim))+offsetBound))
			throw std::logic_error("Slice bounds out of range");
		else if(upperBound-lowerBound+1<0)
			throw std::logic_error("Lower bound cannot be greater than upper bound");
		this->_lower[DIM-dim] = lowerBound; this->_upper[DIM-dim] = upperBound; 
		// Si un pas est donné en 3ème paramètre
		std::ptrdiff_t pitch = DEFAULT_PITCH;
		if(++boundsIt != dimensionIt->end()) pitch = *boundsIt;
		if(pitch<=0 || std::size_t(pitch) >= this->length(dim)) throw std::logic_error("Pitch must be greater than 0 and lesser than length");
		_pitch[DIM-dim] = std::size_t(pitch);
		--dim;
	}
}

template<typename Elem, std::size_t DIM>
ArraySlice<Elem, DIM>::ArraySlice(std::size_t* lengths, Elem* const val, const InitSlice& initSlice, std::ptrdiff_t* offset):
	AbstractBoundedArray<DIM>(new std::ptrdiff_t[DIM], new std::ptrdiff_t[DIM]), 
	SubDimensionalArray<Elem, DIM>(lengths, val), _pitch(new std::size_t[DIM]), 
	_offset(new std::ptrdiff_t[DIM]) {
		setBounds(initSlice, offset);
}

template<typename Elem, std::size_t DIM>
ArraySlice<Elem, DIM>::ArraySlice(const ArraySlice& other): 
	AbstractBoundedArray<DIM>(new std::ptrdiff_t[DIM], new std::ptrdiff_t[DIM]), 
	SubDimensionalArray<Elem, DIM>(other), _pitch(new std::size_t[DIM]),
	_offset(new std::ptrdiff_t[DIM]) {
	 	for(std::size_t i=0;i<DIM;++i) {
	 		this->_lower[i] = other._lower[i]; this->_upper[i] = other._upper[i];
	 		this->_pitch[i] = other._pitch[i]; this->_offset[i] = other._offset[i];
	 	} 
}

template<typename Elem, std::size_t DIM>
ArraySlice<Elem, DIM>::ArraySlice(ArraySlice&& other): 
	AbstractBoundedArray<DIM>(other._lower, other._upper),
	SubDimensionalArray<Elem, DIM>(other._lengths, other._val), _pitch(other._pitch), _offset(other._offset) {
		other._lower = nullptr; other._upper = nullptr;
		other._pitch = nullptr; other._offset = nullptr;
}

template<typename Elem, std::size_t DIM>
ArraySlice<Elem, DIM>& ArraySlice<Elem, DIM>::operator=(const ArraySlice& other){
	if(this!=&other){
		this->_lengths = other._lengths; this->_val = other._val;
		for(std::size_t i=0;i<DIM;++i) {
	 		this->_lower[i] = other._lower[i]; this->_upper[i] = other._upper[i];
	 		this->_pitch[i] = other._pitch[i]; this->_offset[i] = other._offset[i];
	 	} 
	}
	return *this;
}

template<typename Elem, std::size_t DIM>
ArraySlice<Elem, DIM>& ArraySlice<Elem, DIM>::operator=(ArraySlice&& other){
	if(this!=&other){
		this->_length = other._lengths; this->_val = other._val;
		delete[] this->_lower; delete[] this->_upper;
		delete[] this->_pitch; delete[] this->_offset;
		this->_lower = other._lower; this->_upper = other._upper;
		this->_pitch = other._pitch; this->_offset = other._offset;
		other._lower = nullptr; other._upper = nullptr;
		other._pitch = nullptr; other._offset = nullptr;
		}
		return *this;
}

template<typename Elem, std::size_t DIM>
SubArraySlice<Elem, DIM-1> ArraySlice<Elem, DIM>::operator[](std::ptrdiff_t i) const{
	if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
	return SubArraySlice<Elem, DIM-1>(this->_lengths+1, this->_val+std::size_t((i-this->offset()))*this->size(DIM-1), 
		this->_lower+1, this->_upper+1, this->_pitch+1, this->_offset+1);
}

template<typename Elem, std::size_t DIM>
SubArraySlice<Elem, DIM-1> ArraySlice<Elem, DIM>::operator[](std::ptrdiff_t i){
	if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
	return SubArraySlice<Elem, DIM-1>(this->_lengths+1, this->_val+std::size_t((i-this->offset()))*this->size(DIM-1), 
		this->_lower+1, this->_upper+1, this->_pitch+1, this->_offset+1);
}

template<typename Elem, std::size_t DIM>
std::ptrdiff_t ArraySlice<Elem, DIM>::offset(std::size_t dim) const{
	if(dim==0 || dim>DIM) throw std::out_of_range("Dimension out of range");
	return _offset[DIM-dim];
}

template<typename Elem, std::size_t DIM>
std::size_t ArraySlice<Elem, DIM>::pitch(std::size_t dim) const{
	if(dim==0 || dim>DIM) throw std::out_of_range("Dimension out of range");
	return _pitch[DIM-dim];
}

template<typename Elem, std::size_t DIM>
bool ArraySlice<Elem, DIM>::inBounds(std::ptrdiff_t i, std::size_t dim) const{
	if(AbstractBoundedArray<DIM>::inBounds(i,dim))
		return (std::size_t(i - this->lower(dim)) % this->pitch(dim)) == 0;
	return false;
}

//--------------------------------------------------------------------------------------

/* Définitions pour DIM=1 */
template<typename Elem>
void ArraySlice<Elem, 1>::setBounds(const std::initializer_list<std::ptrdiff_t>& initSlice){
	if(initSlice.size()!=2 && initSlice.size()!=3) 
		throw std::logic_error("A slice need at least 2 and at most 3 parameters (pitch has default value)");
	std::initializer_list<std::ptrdiff_t>::iterator boundsIt = initSlice.begin();
	const std::ptrdiff_t& lowerBound = *boundsIt; const std::ptrdiff_t& upperBound = *(++boundsIt);
	if((lowerBound<_offset || std::size_t(lowerBound)>=this->length()+std::size_t(_offset)) || (upperBound<_offset || std::size_t(upperBound) >= this->length()+std::size_t(_offset)))
		throw std::logic_error("Slice bounds out of range");
	else if(upperBound-lowerBound+1<0)
		throw std::logic_error("Lower bound cannot be greater than upper bound");
	this->_lower = lowerBound; this->_upper = upperBound;
	std::ptrdiff_t pitch = DEFAULT_PITCH;
	if(++boundsIt != initSlice.end()) pitch = *boundsIt;
	if(pitch<=0 || std::size_t(pitch) >= this->length()) throw std::logic_error("Pitch must be greater than 0 and lesser than length");
	_pitch = std::size_t(pitch);
}

template<typename Elem>
ArraySlice<Elem, 1>::ArraySlice(std::size_t* const lengths, Elem* const val, const std::initializer_list<std::ptrdiff_t>& initSlice, std::ptrdiff_t offset):
	AbstractBoundedArray<1>(0, 0), SubDimensionalArray<Elem, 1>(lengths, val), 
	_pitch(0), _offset(offset){
		setBounds(initSlice);
}

template<typename Elem>
ArraySlice<Elem, 1>::ArraySlice(const ArraySlice& other): AbstractBoundedArray<1>(other), SubDimensionalArray<Elem, 1>(other),
	_pitch(other._pitch), _offset(other._offset) {}

template<typename Elem>
ArraySlice<Elem, 1>::ArraySlice(ArraySlice&& other): AbstractBoundedArray<1>(other), SubDimensionalArray<Elem, 1>(other),
	_pitch(other._pitch), _offset(other._offset) {
		other._lower = 0; other._upper = 0; 
		other._pitch = 0; other._offset = 0;
} 

template<typename Elem>
ArraySlice<Elem, 1>& ArraySlice<Elem, 1>::operator=(const ArraySlice& other){
	if(this!=&other){
		this->_val = other._val; this->_lengths = other._lengths;
		this->_lower = other._lower; this->_upper = other._upper;
		this->_pitch = other._pitch; this->_offset = other._offset;
	}
	return *this;
}

template<typename Elem>
ArraySlice<Elem, 1>& ArraySlice<Elem, 1>::operator=(ArraySlice&& other){
	if(this!=&other){
		this->_val = other._val; this->_length = other._length;
		this->_lower = other._lower; this->_upper = other._upper;
		this->_pitch = other._pitch; this->_offset = other._offset;
		other._lower = 0; other._upper = 0;
		other._pitch = 0; other._offset = 0;
	}
	return *this;
}

template<typename Elem>
const Elem& ArraySlice<Elem, 1>::operator[] (std::ptrdiff_t i) const{
	if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
	return SubDimensionalArray<Elem, 1>::operator[](i-this->_offset);
}

template<typename Elem>
Elem& ArraySlice<Elem, 1>::operator[] (std::ptrdiff_t i) {
	if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
	return SubDimensionalArray<Elem, 1>::operator[](i-this->_offset);
}

#endif