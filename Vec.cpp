#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm> //std::min

namespace Constants{
	static const int DEFAULT_DIM = 1;
	static const int DEFAULT_DIM_LENGTH = 1;
}

/* Classe responsable des longueurs d'un conteneur */
class ContainerLengths{
	std::size_t _dim;
	std::size_t* _lengths;

public:
	ContainerLengths(std::size_t dimension = Constants::DEFAULT_DIM, std::initializer_list<std::size_t> lengths = {}) : _dim(dimension), _lengths(new std::size_t[dimension]){
		if(dimension<1) throw std::logic_error("Dimension cannot be lesser than 1");
		if(lengths.size()==0){
			for(std::size_t i=0;i<_dim;++i) _lengths[i]=Constants::DEFAULT_DIM_LENGTH;
		}
		else{
			std::initializer_list<std::size_t>::iterator it = lengths.begin();
			std::size_t i = 0;
			while(it!=lengths.end()){
				_lengths[i]=(*it);
				++it; ++i;
			}
		}
	}
	ContainerLengths(std::size_t dimension = Constants::DEFAULT_DIM, std::size_t* lengths = NULL) : _dim(dimension), _lengths(new std::size_t[dimension]){
		if(dimension<1) throw std::logic_error("Dimension cannot be lesser than 1");
		if(lengths==NULL){
			for(std::size_t i=0;i<_dim;++i) _lengths[i]=Constants::DEFAULT_DIM_LENGTH;
		}
		else
			for(std::size_t i=0;i<_dim;++i) _lengths[i]=lengths[i];
	}
	ContainerLengths(const ContainerLengths& other) : _dim(other._dim), _lengths(new std::size_t[other._dim]) {
		for(std::size_t i=0;i<_dim;++i) _lengths[i]=other._lengths[i];
	}

	ContainerLengths& operator=(const ContainerLengths& other){
		if(this!=&other){
			_dim = other._dim; 
			delete[] _lengths; _lengths = new std::size_t[other._dim];
			for(std::size_t i=0;i<_dim;++i) _lengths[i]=other._lengths[i];
		}
		return *this;
	}
 	
 	std::size_t getDimension() const { return _dim; }
 	/* Renvoie un sous-vecteur de longueurs correspondant à la dimension donnée */
 	ContainerLengths getLengths(std::size_t dim) const { return ContainerLengths(dim, _lengths+_dim-dim); }
	std::size_t& operator[](std::size_t index){
		if(index>_dim) throw std::out_of_range("Index out of range");
 		return _lengths[index];
	}
	const std::size_t& operator[](std::size_t index) const{
		if(index>_dim) throw std::out_of_range("Index out of range");
 		return _lengths[index];
	}

	~ContainerLengths(){ delete[] _lengths; }
};


/* Classe abstraite représentant un conteneur d'élements et ayant plusieurs dimensions */
template<typename Elem, std::size_t DIM>
class DimensionalContainer{
protected:
	Elem* _val;
	ContainerLengths _lengths;

	DimensionalContainer(ContainerLengths& lengths = ContainerLengths(DIM), Elem* val = NULL) : _val(val), _lengths(lengths) {}
public:
	/* Renvoie les longueurs des dimensions de la dimension donnée */
	ContainerLengths getDimensionLengths(std::size_t dim=DIM) const{
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _lengths.getLengths(dim);
	}
	/* Modifie les longueurs des dimensions de la dimension donnée */
	void setDimensionLengths(ContainerLengths& lengths, std::size_t dim){
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		for(std::size_t i=DIM-dim;i<DIM;++i) _lengths[i] = lengths[i];
	}
	
	/* Calcule la taille d'un potentiel conteneur en utilisant 2 ContainerLengths, 
	celui de this jusqu'à dim, et l'autre entièrement */
	std::size_t computeSize(ContainerLengths& lengths, std::size_t dim) const{
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		std::size_t size = 1;
		for(std::size_t i=0;i<DIM-dim;++i) size *= _lengths[i];
		for(std::size_t i=0;i<dim;++i) size *= lengths[i];
		return size;
	}

public:
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
	bool hasSameLengths(const DimensionalContainer<Elem, DIM>& other) {
		if(DIM!=other.getDimension()) return false; 
		for(std::size_t i =0;i<DIM;++i){
			if(_lengths[i]!=other._lengths[i]) return false;
		} 
		return true;
	}
	virtual ~DimensionalContainer(){}
};

//------------------------------------------------------------

/* Spécialisation partielle de DimensionalContainer pour DIM=1 */
template<typename Elem>
class DimensionalContainer<Elem, 1>{
protected:
	Elem* _val;
	std::size_t _length;

	DimensionalContainer() : _val(NULL), _length(Constants::DEFAULT_DIM_LENGTH) {}
	DimensionalContainer(std::size_t length) : _val(NULL), _length(length){}
	
	void setLength(std::size_t length) { _length = length; }

public:
	std::size_t getDimension() const { return 1; }
	std::size_t getLength() const { return _length; }
	std::size_t getSize() const { return _length; }
	bool hasSameLength(const DimensionalContainer<Elem, 1>& other) { return _length == other._length; } 
	virtual ~DimensionalContainer(){}
};

//------------------------------------------------------------

template<typename Elem, std::size_t DIM>
class MultiDimensionalArray;

//------------------------------------------------------------

template<typename Elem, std::size_t DIM>
class SubDimensionalArray: public DimensionalContainer<Elem, DIM>{
	Elem* const _val;

public:
	SubDimensionalArray(const ContainerLengths& lengths, Elem* const val): DimensionalContainer<Elem, DIM>(lengths), _val(val){}
	
	SubDimensionalArray<Elem, DIM>& operator=(const DimensionalContainer<Elem, DIM>& other){
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
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i){
		if(std::size_t(i)>=this->getLength())
			throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(this->getDimensionLengths(DIM-1), _val+i*this->getSize(DIM-1));
	}
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i) const{
		if(std::size_t(i)>=this->getLength())
			throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(this->getDimensionLengths(DIM-1), _val+i*this->getSize(DIM-1));
	}
	virtual ~SubDimensionalArray(){}
};

//------------------------------------------------------------

template<typename Elem>
class SubDimensionalArray<Elem, 1>: public DimensionalContainer<Elem, 1>{
	Elem* const _val;

public:
	SubDimensionalArray(const ContainerLengths& lengths, Elem* const val): DimensionalContainer<Elem, 1>(lengths[0]), _val(val) {}
	SubDimensionalArray<Elem, 1>& operator=(const DimensionalContainer<Elem, 1>& other){
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

/* Classe représentant un tableau de dimension potentiellement infine */
template<typename Elem, std::size_t DIM>
class MultiDimensionalArray: public DimensionalContainer<Elem, DIM> {
	Elem* _val;
// protected:
	// void copyAt(void* ptr,std::size_t limit){
	// 	limit = std::min(limit,this->getSize());
	// 	Elem* copyPtr = (Elem*)ptr;
	// 	for(std::size_t i=0;i<limit;++i){
	// 		copyPtr[i] = _val[i];
	// 	}
	// }

	// void* resize(DimensionalContainer& from, DimensionalContainer& to){
	// 	std::size_t newSize = this->computeSize(from.getDimension(), to.getLengthsArray());
	// 	Elem* tmp = new Elem[newSize];
	// 	copyAt(tmp,newSize);
		
	// 	this->setLengths(to.getLengthsArray(),from.getDimension());

	// }
	
public:
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i){
		if(std::size_t(i)>=this->getLength()) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(this->getDimensionLengths(DIM-1), _val+i*this->getSize(DIM-1));
	}
	SubDimensionalArray<Elem, DIM-1> operator[](std::ptrdiff_t i) const{
		if(std::size_t(i)>=this->getLength()) throw std::out_of_range("Index for dimension out of range");
		return SubDimensionalArray<Elem, DIM-1>(this->getDimensionLengths(DIM-1), _val+i*this->getSize(DIM-1));
	}
	MultiDimensionalArray(ContainerLengths& lengths):
		DimensionalContainer<Elem, DIM>(lengths), _val(new Elem[this->getSize()]) {}
	MultiDimensionalArray(ContainerLengths& lengths, const Elem& elem) : MultiDimensionalArray(lengths){
		for(std::size_t i=0;i<this->getSize();++i) _val[i] = elem;
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
	 	ContainerLengths lengths(2,{2,3});
		MultiDimensionalArray<int, 2> array(lengths);
		//array[1][1] = MultiDimensionalArray<int, 2>(&lengths[2]);
		std::cout<<array[1]<<std::endl;
	} catch(const std::exception& e){
		std::cerr << std::endl << "*** " << e.what() << " ***" << std::endl;
		return 1;
	}
	return 0;
}
