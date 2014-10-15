#include <cstdlib>
#include <exception>
#include <stdexcept>

namespace Constants{
	static const int DEFAULT_DIM = 1;
	static const int DEFAULT_DIM_LENGTH = 1;
}

/* Classe responsable des longueurs d'un conteneur */
class ContainerLengths{
	std::size_t _dim;
	std::size_t* _lengths;

public:
	ContainerLengths(const std::initializer_list<std::size_t>& lengths = {Constants::DEFAULT_DIM_LENGTH}) : _dim(lengths.size()), _lengths(new std::size_t[_dim]){
		if(_dim<1) throw std::logic_error("Dimension cannot be lesser than 1");
		std::initializer_list<std::size_t>::iterator it = lengths.begin();
		std::size_t i = 0;
		while(it!=lengths.end()){
			_lengths[i]=(*it);
			++it; ++i;
		}
	}
	ContainerLengths(std::size_t dimension, std::size_t* lengths) : _dim(dimension), _lengths(new std::size_t[dimension]){
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

 	const std::size_t& operator[](std::size_t index) const{
		if(index>_dim) throw std::out_of_range("Index out of range");
 		return _lengths[index];
	}
	std::size_t& operator[](std::size_t index){
		if(index>_dim) throw std::out_of_range("Index out of range");
 		return _lengths[index];
	}

	~ContainerLengths(){ delete[] _lengths; }
};