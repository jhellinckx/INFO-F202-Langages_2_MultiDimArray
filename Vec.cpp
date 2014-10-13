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


/* Classe abstraite représentant un objet ayant plusieurs dimensions */
class DimensionalObject{
	std::size_t _dim;
	std::size_t* _lengths;	//Longueur de chaque dimension

protected:
	DimensionalObject(std::size_t dim, std::size_t lengths[]) : _dim(dim), _lengths(new std::size_t[dim]){
		if(dim < 1)
			throw std::out_of_range("Dimension cannot be lesser than 1");
		for(std::size_t i=0;i<dim;++i)
			_lengths[i] = lengths[i];
	}
	std::size_t* getLengthsArray() const { return _lengths; }
	std::size_t* getSubLengthsArray() const { 				//Longueurs des dimensions de la sous-dimension
		if(_dim==0)
			throw std::out_of_range("Index out of range");
		return _lengths+1; 
	}
	void setLengths(std::size_t* copyLengths, std::size_t limit){
		if(limit>_dim)
			throw std::out_of_range("Index out of range");
		for(std::size_t i=0;i<limit;++i){
			_lengths[i] = copyLengths[i];
		}
	}
	bool hasSameLengths(const DimensionalObject& other) {
		if(_dim!=other._dim)
			return false; 
		for(std::size_t i =0;i<_dim;++i){
			if(_lengths[i]!=other._lengths[i])
				return false;
		} 
		return true;
	}
	std::size_t computeSize(std::size_t dim, std::size_t* lengths) const{
		if(dim>_dim)
			throw std::out_of_range("Index out of range");
		std::size_t size = 1;
		for(std::size_t i=0;i<_dim;++i){
			if(i<_dim-dim)
				size *= _lengths[i];
		}
		for(std::size_t i=0;i<dim;++i){
			size *= lengths[i];
		}
		return size;
	}
	virtual void resize(DimensionalObject& from, DimensionalObject& to){}
public: 
	std::size_t getDimension() const { return _dim; } 
	std::size_t getLength(std::size_t i=0) const { 
		if(i>=_dim)
			throw std::out_of_range("Index out of range");
		return _lengths[i]; 
	}
	std::size_t getSubLength() const { 						//Longueur de la sous-dimension
		if(_dim==1)
			throw std::out_of_range("Index out of range");
		return _lengths[1]; 
	} 	
	std::size_t getSize(std::size_t dimI = 0) const { 		//Nombre d'éléments compris dans cette dimension
		if(dimI>=_dim)
			throw std::out_of_range("Index out of range");
		std::size_t l = 1; 
		for(;dimI<_dim;++dimI) 
			l*=_lengths[dimI]; 
		return l;
	}
	std::size_t getSubSize() const { 
		if(_dim==1)
			throw std::out_of_range("Index out of range");
		return getSize(1); 
	}
		
	DimensionalObject(const DimensionalObject& other) : DimensionalObject(other._dim, other._lengths) {}
	DimensionalObject& operator=(const DimensionalObject& other){
		if(this != &other){
			_dim = other._dim;
			delete[] _lengths; _lengths = new std::size_t[_dim];
			setLengths(other._lengths,_dim);
		}
		return *this;
	}
	virtual ~DimensionalObject(){ delete[] _lengths; }
};

//------------------------------------------------------------

template<typename Elem, std::size_t DIM>
class MultiDimensionalArray;

template<typename Elem>
class MultiDimensionalArray<Elem, 1>;

//------------------------------------------------------------

template<typename Elem, std::size_t DIM>
class SubDimensionArray: public DimensionalObject{
	Elem* const _val;
	DimensionalObject* const _masterArray;

public:
	SubDimensionArray(DimensionalObject* const master, std::size_t lengths[], Elem* val): 
		DimensionalObject(DIM, lengths), _val(val), _masterArray(master) {}
	SubDimensionArray(const SubDimensionArray<Elem, DIM>& other): 
		DimensionalObject(other), _val(other._val), _masterArray(other._masterArray) {}
	SubDimensionArray(SubDimensionArray<Elem, DIM>&&);
	SubDimensionArray<Elem, DIM>& operator=(const SubDimensionArray<Elem, DIM>& other){
		if(this != &other){
			if(this->hasSameLengths(other)){
				for(std::size_t i=0;i<this->getSize();++i){
					_val[i] = other._val[i];
				}
				return *this;
			}
			_masterArray->resize(other);
			
		}
		return *this;
	}
	SubDimensionArray<Elem, DIM>& operator=(const MultiDimensionalArray<Elem, DIM>& other){
	}
	SubDimensionArray<Elem, DIM>& operator=(SubDimensionArray<Elem, DIM>&&);
	SubDimensionArray<Elem, DIM-1> operator[](std::ptrdiff_t i){
		if(std::size_t(i)>=this->getLength())
			throw std::out_of_range("Index for dimension out of range");
		return SubDimensionArray<Elem, DIM-1>(_masterArray, this->getSubLengthsArray(), _val+i*this->getSubSize());
	}
	SubDimensionArray<Elem, DIM-1> operator[](std::ptrdiff_t i) const{
		if(std::size_t(i)>=this->getLength())
			throw std::out_of_range("Index for dimension out of range");
		return SubDimensionArray<Elem, DIM-1>(_masterArray, this->getSubLengthsArray(), _val+i*this->getSubSize());
	}
	virtual ~SubDimensionArray(){}
};

//------------------------------------------------------------

template<typename Elem>
class SubDimensionArray<Elem, 1>: public DimensionalObject{
	Elem* const _val;
	DimensionalObject* const _masterArray;

public:
	SubDimensionArray(DimensionalObject* const master, std::size_t lengths[], Elem* val): 
		DimensionalObject(1, lengths), _val(val), _masterArray(master) {}
	SubDimensionArray(const SubDimensionArray<Elem, 1>& other): 
		DimensionalObject(other), _val(other._val), _masterArray(other._masterArray) {}
	SubDimensionArray(SubDimensionArray<Elem, 1>&&); 
	SubDimensionArray<Elem, 1>& operator=(const SubDimensionArray<Elem, 1>& other){
		if(this != &other){
			DimensionalObject::operator=(other);
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
class MultiDimensionalArray: public DimensionalObject {
	Elem* _val;


protected:
	void copyAt(void* ptr,std::size_t limit){
		limit = std::min(limit,this->getSize());
		Elem* copyPtr = (Elem*)ptr;
		for(std::size_t i=0;i<limit;++i){
			copyPtr[i] = _val[i];
		}
	}

	template<typename T, std::size_t D>
	void resize(SubDimensionArray<T, D>& from, SubDimensionArray<T,D>& to){
		std::size_t newSize = this->computeSize(from.getDimension(), to.getLengthsArray());
		Elem* tmp = new Elem[newSize];
		copyAt(tmp,newSize);
		
		this->setLengths(to.getLengthsArray(),from.getDimension());

	}
	
public:
	SubDimensionArray<Elem, DIM-1> operator[](std::ptrdiff_t i){
		if(std::size_t(i)>=this->getLength())
			throw std::out_of_range("Index for dimension out of range");
		return SubDimensionArray<Elem, DIM-1>(this, this->getSubLengthsArray(), _val+i*this->getSubSize());
	}
	SubDimensionArray<Elem, DIM-1> operator[](std::ptrdiff_t i) const{
		if(std::size_t(i)>=this->getLength())
			throw std::out_of_range("Index for dimension out of range");
		return SubDimensionArray<Elem, DIM-1>(const_cast<MultiDimensionalArray<Elem, DIM>*>(this), this->getSubLengthsArray(), _val+i*this->getSubSize());
	}
	explicit MultiDimensionalArray(std::size_t lengths[]):
		DimensionalObject(DIM, lengths), _val(new Elem[this->getSize()]){}
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
