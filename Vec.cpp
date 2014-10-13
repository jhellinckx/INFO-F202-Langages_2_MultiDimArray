#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm> //std::min

/* TODO :
	REVIEW EXCEPTIONS
	SUBARRAY SCOPE ONLY IN ARRAY
	VEC DE DIM 1
	RESIZE
*/

namespace Constants{
	static const int DEFAULT_DIM = 1;
	static const int DEFAULT_DIM_LENGTH = 1;
}

class ArrayLengths{
	std::size_t _dim;
	std::size_t* _lengths;

public:
	ArrayLengths() : _dim(Constants.DEFAULT_DIM), _lengths(new std::size_t[Constants.DEFAULT_DIM]){
		for(std::size_t i=0;i<_dim;++i)
			_lengths[i] = Constants.DEFAULT_DIM_LENGTH;
	}
	ArrayLengths(std::size_t dimension, std::size_t* lengths = NULL) : _dim(dimension), _lengths(new std::size_t[dimension]){
		if(dimension<1) throw std::logic_error("Dimension cannot be lesser than 1");
		if(lengths==NULL){
			for(std::size_t i=0;i<_dim;++i) _lengths[i]=Constants.DEFAULT_DIM_LENGTH;
		}
		else
			for(std::size_t i=0;i<_dim;++i) _lengths[i]=lengths[i];
	}
	ArrayLengths(const ArrayLengths& other) : _dim(other._dim), _lengths(new std::size_t[other._dim]) {
		for(std::size_t i=0;i<_dim;++i) _lengths[i]=other._lengths[i];
	}

	ArrayLengths& operator=(const ArrayLengths& other){
		if(this!=&other){
			_dim = other_dim; 
			delete[] _lengths; _lengths = new std::size_t[other._dim];
			for(std::size_t i=0;i<_dim;++i) _lengths[i]=other._lengths[i];
		}
		return *this;
	}
 	
 	std::size_t getDimension() const { return _dim; }
 	std::size_t get(std::size_t index){
 		if(index>_dim) throw std::out_of_range("Index out of range");
 		return _lengths[index];
	}
	void set(std::size_t index, std::size_t length){
		 if(index>_dim) throw std::out_of_range("Index out of range");
		 _lengths[index] = length;
	}

	~ArrayLengths(){ delete[] _lengths; }
};


/* Classe abstraite représentant un objet ayant plusieurs dimensions */
template<typename Elem, std::size_t DIM>
class DimensionalContainer{
	Elem* _val;
	ArrayLengths _lengths;

protected:
	DimensionalContainer() : _val(NULL), _lengths(ArrayLengths(DIM)) {}
	DimensionalContainer(ArrayLengths& lengths) : _val(NULL), _dim(DIM), _lengths(lengths){}
	std::size_t* getLengthsArray() const { return _lengths; }
	std::size_t* getSubLengthsArray() const { 				//Longueurs des dimensions de la sous-dimension
		if(_dim==1) throw std::out_of_range("Array of dimension 1 has no subarray");
		return _lengths+1; 
	}
	void setLengths(std::size_t* copyLengths, std::size_t limit){
		if(limit>_dim) throw std::out_of_range("Index out of range");
		for(std::size_t i=0;i<limit;++i) _lengths[i] = copyLengths[i];
	}
	
	std::size_t computeSize(std::size_t dim, std::size_t* lengths) const{
		if(dim>_dim) throw std::out_of_range("Index out of range");
		std::size_t size = 1;
		for(std::size_t i=0;i<_dim;++i){
			if(i<_dim-dim) size *= _lengths[i];
		}
		for(std::size_t i=0;i<dim;++i) size *= lengths[i];
		return size;
	}

public:
	std::size_t getDimension() const { return _dim; } 
	std::size_t getLength(std::size_t i=0) const { 
		if(i>=_dim) throw std::out_of_range("Index out of range");
		return _lengths[i]; 
	}
	std::size_t getSubLength() const { 						//Longueur de la sous-dimension
		if(_dim==1) throw std::out_of_range("Index out of range");
		return _lengths[1]; 
	} 	
	std::size_t getSize(std::size_t dimI = 0) const { 		//Nombre d'éléments compris dans cette dimension
		if(dimI>=_dim) throw std::out_of_range("Index out of range");
		std::size_t l = 1; 
		for(;dimI<_dim;++dimI) l*=_lengths[dimI]; 
		return l;
	}
	std::size_t getSubSize() const { 
		if(_dim==1) throw std::out_of_range("Index out of range");
		return getSize(1); 
	}
	bool hasSameLengths(const DimensionalContainer& other) {
		if(_dim!=other._dim) return false; 
		for(std::size_t i =0;i<_dim;++i){
			if(_lengths[i]!=other._lengths[i]) return false;
		} 
		return true;
	}
		
	DimensionalContainer(const DimensionalContainer& other) : DimensionalContainer(other._dim, other._lengths){}
	DimensionalContainer& operator=(const DimensionalContainer& other){
		if(this != &other){
			_dim = other._dim;
			delete[] _lengths; _lengths = new std::size_t[_dim];
			setLengths(other._lengths,_dim);
		}
		return *this;
	}
	virtual ~DimensionalContainer(){ delete[] _lengths; }
};

//------------------------------------------------------------

template<typename Elem, std::size_t DIM>
class MultiDimensionalArray;

template<typename Elem>
class MultiDimensionalArray<Elem, 1>;

//------------------------------------------------------------

template<typename Elem, std::size_t DIM>
class SubDimensionArray: public DimensionalContainer{
	Elem* const _val;

public:
	SubDimensionArray(std::size_t lengths[], Elem* val): DimensionalContainer(DIM, lengths), _val(val){}
	SubDimensionArray(const SubDimensionArray<Elem, DIM>& other): DimensionalContainer(other), _val(other._val){}
	SubDimensionArray(SubDimensionArray<Elem, DIM>&&);
	SubDimensionArray<Elem, DIM>& operator=(const SubDimensionArray<Elem, DIM>& other){
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
	SubDimensionArray<Elem, DIM>& operator=(const MultiDimensionalArray<Elem, DIM>& other){
	}
	SubDimensionArray<Elem, DIM>& operator=(SubDimensionArray<Elem, DIM>&&);
	SubDimensionArray<Elem, DIM-1> operator[](std::ptrdiff_t i){
		if(std::size_t(i)>=this->getLength())
			throw std::out_of_range("Index for dimension out of range");
		return SubDimensionArray<Elem, DIM-1>(this->getSubLengthsArray(), _val+i*this->getSubSize());
	}
	SubDimensionArray<Elem, DIM-1> operator[](std::ptrdiff_t i) const{
		if(std::size_t(i)>=this->getLength())
			throw std::out_of_range("Index for dimension out of range");
		return SubDimensionArray<Elem, DIM-1>(this->getSubLengthsArray(), _val+i*this->getSubSize());
	}
	virtual ~SubDimensionArray(){}
};

//------------------------------------------------------------

template<typename Elem>
class SubDimensionArray<Elem, 1>: public DimensionalContainer{
	Elem* const _val;

public:
	SubDimensionArray(std::size_t lengths[], Elem* val): DimensionalContainer(1, lengths), _val(val) {}
	SubDimensionArray(const SubDimensionArray<Elem, 1>& other): DimensionalContainer(other), _val(other._val) {}
	SubDimensionArray(SubDimensionArray<Elem, 1>&&); 
	SubDimensionArray<Elem, 1>& operator=(const SubDimensionArray<Elem, 1>& other){
		if(this != &other){
			DimensionalContainer::operator=(other);
			_val = other._val;
		}
		return *this;
	}
	SubDimensionArray<Elem, 1>& operator=(const MultiDimensionalArray<Elem, 1>& other){

	}
	SubDimensionArray<Elem, 1>& operator=(SubDimensionArray<Elem, 1>&&);
	Elem& operator[](std::ptrdiff_t i){
		if(std::size_t(i)>=this->getLength())
			throw std::out_of_range("Index out of range");
		return *(_val+i);
	}
	const Elem& operator[](std::ptrdiff_t i) const{
		if(std::size_t(i)>=this->getLength())
			throw std::out_of_range("Index out of range");
		return *(_val+i);
	}
	virtual ~SubDimensionArray(){}
};

/* Classe représentant un tableau de dimension potentiellement infine */
template<typename Elem, std::size_t DIM>
class MultiDimensionalArray: public DimensionalContainer {
	Elem* _val;

protected:
	void copyAt(void* ptr,std::size_t limit){
		limit = std::min(limit,this->getSize());
		Elem* copyPtr = (Elem*)ptr;
		for(std::size_t i=0;i<limit;++i){
			copyPtr[i] = _val[i];
		}
	}

	void* resize(DimensionalContainer& from, DimensionalContainer& to){
		std::size_t newSize = this->computeSize(from.getDimension(), to.getLengthsArray());
		Elem* tmp = new Elem[newSize];
		copyAt(tmp,newSize);
		
		this->setLengths(to.getLengthsArray(),from.getDimension());

	}
	
public:
	SubDimensionArray<Elem, DIM-1> operator[](std::ptrdiff_t i){
		if(std::size_t(i)>=this->getLength())
			throw std::out_of_range("Index for dimension out of range");
		return SubDimensionArray<Elem, DIM-1>(this->getSubLengthsArray(), _val+i*this->getSubSize());
	}
	SubDimensionArray<Elem, DIM-1> operator[](std::ptrdiff_t i) const{
		if(std::size_t(i)>=this->getLength())
			throw std::out_of_range("Index for dimension out of range");
		return SubDimensionArray<Elem, DIM-1>(this->getSubLengthsArray(), _val+i*this->getSubSize());
	}
	explicit MultiDimensionalArray(std::size_t lengths[]):
		DimensionalContainer(DIM, lengths), _val(new Elem[this->getSize()]){}
	MultiDimensionalArray(std::size_t lengths[], const Elem& elem) : MultiDimensionalArray(lengths){
		for(std::size_t i=0;i<this->getSize();++i)
			_val[i] = elem;
	}
	MultiDimensionalArray(Elem const elem[], std::size_t lengths[]): MultiDimensionalArray(lengths){
		for(std::size_t i=0;i<this->getSize();++i){
			_val[i] = elem[i];
		}
	}
	MultiDimensionalArray(const MultiDimensionalArray<Elem, DIM>&);
	MultiDimensionalArray(MultiDimensionalArray<Elem,DIM>&&);
	MultiDimensionalArray<Elem, DIM>& operator=(const MultiDimensionalArray<Elem, DIM>&);
	MultiDimensionalArray<Elem, DIM>& operator=(MultiDimensionalArray<Elem, DIM>&&);
	virtual ~MultiDimensionalArray(){delete[] _val;}
};


//------------------------------------------------------------
template<typename Elem, std::size_t DIM>
std::ostream& operator<< (std::ostream& out, const MultiDimensionalArray<Elem, DIM>& d) {
	out<<"MultiDimensionalArray : dimension = " << d.getDimension() << " size = " << d.getSize() << std::endl;
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
std::ostream& operator<< (std::ostream& out, const SubDimensionArray<Elem, DIM>& d) {
	out<<"[";
	for(std::size_t i=0;i<d.getLength();++i){
  		out << d[i];
  		if(i!=d.getLength()-1)
  			out<<",";
  	}
  	out<<"]";
  	return out;
}

int main(){
	try{
		std::size_t lengths[] = {2,2,2,4};
		MultiDimensionalArray<int, 4> array(lengths);
		//array[1][1] = MultiDimensionalArray<int, 2>(&lengths[2]);
		std::cout<<array[1][1][1]<<std::endl;
	} catch(const std::exception& e){
		std::cerr << std::endl << "*** " << e.what() << " ***" << std::endl;
		return 1;
	}
	return 0;
}
