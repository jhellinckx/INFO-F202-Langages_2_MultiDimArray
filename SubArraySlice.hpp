/*
Nom : 			Hellinckx
Prénom : 		Jérôme
Matricule :		000376500
Année d'étude : BA2 - 2014-2015
*/

#ifndef SUBARRAYSLICE_HPP
#define SUBARRAYSLICE_HPP

#include <exception>
#include <stdexcept>
#include "SubDimensionalArray.hpp"
#include "AbstractBoundedArray.hpp"

template<typename Elem, std::size_t DIM>
class ArraySlice;

template<typename Elem, std::size_t DIM>
class SubArraySlice : public AbstractBoundedArray<DIM>, public SubDimensionalArray<Elem, DIM>{
	friend class ArraySlice<Elem, DIM+1>;
	friend class SubArraySlice<Elem, DIM+1>;
	std::size_t* const _pitch;
	std::ptrdiff_t* const _offset;
protected:
	SubArraySlice(std::size_t* const, Elem* const, std::ptrdiff_t* const,
		std::ptrdiff_t* const, std::size_t* const, std::ptrdiff_t* const); 
public:
	virtual std::ptrdiff_t offset(std::size_t dim=DIM) const;
	virtual std::size_t pitch(std::size_t dim=DIM) const;
	virtual bool inBounds(std::ptrdiff_t, std::size_t dim=DIM) const;
	inline SubArraySlice<Elem, DIM-1> operator[](std::ptrdiff_t) const;
	inline SubArraySlice<Elem, DIM-1> operator[](std::ptrdiff_t);
	virtual void print(std::ostream& out) const{
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
	virtual ~SubArraySlice() {}
};

/* Spécialisation pour DIM=1 */
template<typename Elem>
class SubArraySlice<Elem, 1>: public AbstractBoundedArray<1>, public SubDimensionalArray<Elem, 1>{
	friend class ArraySlice<Elem, 2>;
	friend class SubArraySlice<Elem, 2>;
	std::size_t _pitch;
	std::ptrdiff_t _offset;
protected:
	SubArraySlice(std::size_t*, Elem* const, std::ptrdiff_t* const, 
		std::ptrdiff_t* const, std::size_t* const, std::ptrdiff_t* const);
public: 
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
	virtual ~SubArraySlice() {}	
};

/* Définitions pour DIM>1 */
template<typename Elem, std::size_t DIM>
SubArraySlice<Elem, DIM>::SubArraySlice(std::size_t* const lengths, Elem* const val, std::ptrdiff_t* const lower,
	std::ptrdiff_t* const upper, std::size_t* const pitch, std::ptrdiff_t* const offset) :
	AbstractBoundedArray<DIM>(lower, upper), SubDimensionalArray<Elem, DIM>(lengths, val),
	_pitch(pitch), _offset(offset) {}

template<typename Elem, std::size_t DIM>
std::ptrdiff_t SubArraySlice<Elem, DIM>::offset(std::size_t dim) const{
	if(dim==0 || dim>DIM) throw std::out_of_range("Dimension out of range");
	return _offset[DIM-dim];
}

template<typename Elem, std::size_t DIM>
std::size_t SubArraySlice<Elem, DIM>::pitch(std::size_t dim) const{
	if(dim==0 || dim>DIM) throw std::out_of_range("Dimension out of range");
	return _pitch[DIM-dim];
}

template<typename Elem, std::size_t DIM>
bool SubArraySlice<Elem, DIM>::inBounds(std::ptrdiff_t i, std::size_t dim) const{
	if(AbstractBoundedArray<DIM>::inBounds(i, dim))
		return (std::size_t(i - this->lower(dim)) % this->pitch(dim)) == 0;
	return false;
}

template<typename Elem, std::size_t DIM>
SubArraySlice<Elem, DIM-1> SubArraySlice<Elem, DIM>::operator[](std::ptrdiff_t i) const{
	if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
	return SubArraySlice<Elem, DIM-1>(this->_lengths+1, this->_val+std::size_t((i-this->offset()))*this->size(DIM-1), 
		this->_lower+1, this->_upper+1, this->_pitch+1, this->_offset+1);
}

template<typename Elem, std::size_t DIM>
SubArraySlice<Elem, DIM-1> SubArraySlice<Elem, DIM>::operator[](std::ptrdiff_t i){
	if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
	return SubArraySlice<Elem, DIM-1>(this->_lengths+1, this->_val+std::size_t((i-this->offset()))*this->size(DIM-1), 
		this->_lower+1, this->_upper+1, this->_pitch+1, this->_offset+1);
}

//---------------------------------------------------------------------

/* Définitions pour DIM=1 */
template<typename Elem>
SubArraySlice<Elem, 1>::SubArraySlice(std::size_t* length, Elem* const val, std::ptrdiff_t* const lower, 
	std::ptrdiff_t* const upper, std::size_t* const pitch, std::ptrdiff_t* const offset):
		AbstractBoundedArray<1>(lower[0], upper[0]), SubDimensionalArray<Elem, 1>(length, val), 
		_pitch(pitch[0]), _offset(offset[0]){}

template<typename Elem>
const Elem& SubArraySlice<Elem, 1>::operator[] (std::ptrdiff_t i) const{
	if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
	return SubDimensionalArray<Elem, 1>::operator[](i-this->_offset);
}

template<typename Elem>
Elem& SubArraySlice<Elem, 1>::operator[] (std::ptrdiff_t i) {
	if(!this->inBounds(i)) throw std::out_of_range("Index not in bounds");
	return SubDimensionalArray<Elem, 1>::operator[](i-this->_offset);
}

#endif