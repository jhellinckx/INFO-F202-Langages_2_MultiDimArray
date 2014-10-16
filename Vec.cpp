#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm> //std::min
#include "ContainerLengths.cpp"

template<typename Elem, std::size_t DIM>
class SubDimensionalArray;

/* Classe abstraite représentant un conteneur d'élements et ayant plusieurs dimensions */
template<std::size_t DIM> 
class DimensionalContainer{
	ContainerLengths _lengths;

protected:
	/* Classe abstraite : constructeur en protected */
	DimensionalContainer(const ContainerLengths& lengths = ContainerLengths(DIM)) : _lengths(lengths) {}

	/* Modifie les longueurs des dimensions de la dimension donnée */
	void setDimensionLengths(const ContainerLengths& lengths, std::size_t dim=DIM){
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		for(std::size_t i=DIM-dim;i<DIM;++i) _lengths[i] = lengths[i];
	}

public:
	/* Renvoie les longueurs des dimensions de la dimension donnée */
	ContainerLengths getDimensionLengths(std::size_t dim=DIM) const{
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _lengths.getLengths(dim);
	}
	/* Calcule la taille d'un potentiel conteneur en utilisant 2 ContainerLengths, 
	celui de this pour les dimensions > dim, et l'autre entièrement */
	std::size_t computeSize(ContainerLengths& lengths, std::size_t dim) const{
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		std::size_t size = 1;
		for(std::size_t i=0;i<DIM-dim;++i) size *= this->_lengths[i];
		for(std::size_t i=0;i<dim;++i) size *= lengths[i];
		return size;
	}
	std::size_t getDimension() const { return _lengths.getDimension(); }
	/* Renvoie la longueur de la dimension donnée */
	std::size_t getLength(std::size_t dim=DIM) const {		
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _lengths[DIM-dim]; 
	}
	/* Renvoie le nombre d'éléments compris dans la dimension donnée */
	std::size_t getSize(std::size_t dim=DIM) const { 		
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		std::size_t size = 1; 
		for(std::size_t i=DIM-dim;i<DIM;++i) size*=_lengths[i]; 
		return size;
	}
	/* Renvoie vrai si, respectivement, les dimensions de l'objet donné 
	en paramètre est égale aux dimensions de this */
	bool hasSameLengths(const DimensionalContainer<DIM>& other) {
		if(DIM!=other.getDimension()) return false; 
		for(std::size_t i =0;i<DIM;++i){
			if(_lengths[i]!=other._lengths[i]) return false;
		} 
		return true;
	}


	virtual ~DimensionalContainer(){}
};

//------------------------------------------------------------

/* Spécialisation complète de DimensionalContainer pour DIM=1 */
template<> 
class DimensionalContainer<1>{
private:
	std::size_t _length;

protected:
	DimensionalContainer(std::size_t length=Constants::DEFAULT_DIM_LENGTH) : _length(length){}
	
	void setLength(std::size_t length) { _length = length; }

public:
	std::size_t getDimension() const { return 1; }
	std::size_t getLength() const { return _length; }
	std::size_t getSize() const { return _length; }
	bool hasSameLength(const DimensionalContainer<1>& other) { return _length == other._length; } 
	virtual ~DimensionalContainer(){}
};

//------------------------------------------------------------


/* Classe représentant un tableau de dimension potentiellement infine */
template<typename Elem, std::size_t DIM>
class MultiDimensionalArray: public DimensionalContainer<DIM> {
	Elem* _val;

protected:
	void copyAt(Elem* pelem, std::size_t elemSize){
		for(std::size_t i=0;i<std::min(elemSize,this->getSize());++i) pelem[i] = _val[i];
	}

public:
	void resize(const std::initializer_list<std::size_t>& lengthsList){
		ContainerLengths newLengths(lengthsList);
		if(newLengths.getDimension() != DIM) throw std::logic_error("Initializer_list must have n=dimension elements when resizing");
		std::size_t newSize = this->computeSize(newLengths,DIM);
		Elem* tmp = new Elem[newSize];
		this->copyAt(tmp, newSize);
		delete[] _val; _val = tmp;
		this->setDimensionLengths(newLengths);
	}
	MultiDimensionalArray(const ContainerLengths& lengths): 
		DimensionalContainer<DIM>(lengths), _val(new Elem[this->getSize()]) {}

	MultiDimensionalArray(const std::initializer_list<std::size_t>& lengthsList): 
		MultiDimensionalArray<Elem, DIM>(ContainerLengths(lengthsList)){}
	
	MultiDimensionalArray(const ContainerLengths& lengths, const Elem& elem) : 
		MultiDimensionalArray<Elem, DIM>(lengths){
			for(std::size_t i=0;i<this->getSize();++i) _val[i] = elem;
	}
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

template<typename Elem, std::size_t DIM>
class SubDimensionalArray: public DimensionalContainer<DIM>{
	Elem* const _val;

public:
	SubDimensionalArray(const ContainerLengths& lengths, Elem* val): 
		DimensionalContainer<DIM>(lengths), _val(val) {}
	SubDimensionalArray(const SubDimensionalArray<Elem, DIM>& other) : 
		DimensionalContainer<DIM>(other), _val(other._val){}

	SubDimensionalArray<Elem, DIM>& operator=(const SubDimensionalArray<Elem, DIM>& other){
		if(this != &other){
			if(this->hasSameLengths(other)){
				for(std::size_t i=0;i<this->getSize();++i){
					_val[i] = other._val[i];
				}
				return *this;
			}
			throw std::logic_error("Cannot assign subarrays of different lengths");
		}
		return *this;
	}
	SubDimensionalArray<Elem, DIM>& operator=(const MultiDimensionalArray<Elem, DIM>& other){
		if(this->hasSameLengths(other)){
			for(std::size_t i=0;i<this->getSize();++i){
				_val[i] = other._val[i];
			}
			return *this;
		}
		throw std::logic_error("Cannot assign a MultiDimensionalArray to a subarray of different lengths");
	}

	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i){
		if(std::size_t(i)>=this->getLength()) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(this->getDimensionLengths(DIM-1), _val+i*this->getSize(DIM-1));
	}
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i) const{
		if(std::size_t(i)>=this->getLength()) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(this->getDimensionLengths(DIM-1), _val+i*this->getSize(DIM-1));
	}

	virtual ~SubDimensionalArray(){}
};

//------------------------------------------------------------

template<typename Elem>
class SubDimensionalArray<Elem, 1>: public DimensionalContainer<1>{
	Elem* const _val;

public:
	SubDimensionalArray(const ContainerLengths& length, Elem* val): 
		DimensionalContainer<1>(length[0]), _val(val) {}
	SubDimensionalArray(const SubDimensionalArray<Elem, 1>& other):
		DimensionalContainer<1>(other.getLength()), _val(other._val) {}

	SubDimensionalArray<Elem, 1>& operator=(const SubDimensionalArray<Elem, 1>& other){
		if(this != &other){
			if(this->hasSameLength(other)){
				for(std::size_t i=0;i<this->getSize();++i) _val[i] = other._val[i];
				return *this;
			}
			throw std::logic_error("Cannot assign subarrays of different lengths");
		}
		return *this;
	}
	Elem& operator[](std::ptrdiff_t i){
		if(std::size_t(i)>=this->getLength()) throw std::out_of_range("Index out of range");
		return *(_val+i);
	}
	const Elem& operator[](std::ptrdiff_t i) const{
		if(std::size_t(i)>=this->getLength()) throw std::out_of_range("Index out of range");
		return *(_val+i);
	}
	virtual ~SubDimensionalArray(){}
};


//------------------------------------------------------------
template<typename Elem, std::size_t DIM>
std::ostream& operator<< (std::ostream& out, const SubDimensionalArray<Elem, DIM>& d) {
	out<<"[";
	for(std::size_t i=0;i<d.getLength();++i){
  		out << d[i];
  		if(i!=d.getLength()-1)
  			out<<",";
  	}
  	out<<"]";
  	return out;
}

template<typename Elem, std::size_t DIM>
std::ostream& operator<< (std::ostream& out, const MultiDimensionalArray<Elem, DIM>& d) {
	out<<"[";
	for(std::size_t i=0;i<d.getLength();++i){
  		out << d[i];
  		if(i!=d.getLength()-1)
  			out<<",";
  	}
  	out<<"]";
  	return out;
}

std::ostream& operator<<(std::ostream& out, const ContainerLengths& c){
	out<<"[";
	for(std::size_t i=0;i<c.getDimension();i++){
		out << c[i];
		if(i!=c.getDimension()-1)
			out<<",";
	}
	out<<"]";
	return out;
}

int main(){
	try{
		MultiDimensionalArray<int, 3> array({2,3,2});
		std::cout<<array<<std::endl;
		array.resize({1,10,2});
		std::cout<<array<<std::endl;
		
	} catch(const std::exception& e){
		std::cerr << std::endl << "\033[31m" << "*** " << e.what() << " ***" << "\033[0m" << std::endl<<std::endl;;
		return 1;
	}
	return 0;
}
