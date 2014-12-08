#ifndef ABSTRACTBOUNDEDARRAY_HPP
#define ABSTRACTBOUNDEDARRAY_HPP

/* Modèle de classe définissiant des bornes inférieures et supérieures pour chaque 
valeur entre 0 et DIM. N'a pas la responsabilité en mémoire des bornes. */
template<std::size_t DIM>
class AbstractBoundedArray{
protected: 
	std::ptrdiff_t* const _lower;
	std::ptrdiff_t* const _upper;
	AbstractBoundedArray(std::ptrdiff_t* const lowerBounds, std::ptrdiff_t* const upperBounds):
		_lower(lowerBounds), _upper(upperBounds) {}
	//TODO
	AbstractBoundedArray(const AbstractBoundedArray<DIM>& other): _lower(other._lower), _upper(other._upper){}
	AbstractBoundedArray& operator=(const AbstractBoundedArray<DIM>& other){
		if(this!=&other){
			this->_lower = other._lower; this->_upper = other._upper;
		}
		return *this;
	}

public:
	virtual bool inBounds(std::size_t index, std::size_t dimension=DIM) const{
		return (index <= this->upper(dimension)) && (index >= this->lower(dimension));
	}
	/* Renvoie la borne inférieure de la dimension donnée */
	virtual std::ptrdiff_t lower(std::size_t dim=DIM) const { 
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _lower[DIM-dim]; 
	}
	/* Renvoie la borne supérieure de la dimension donnée */
	virtual std::ptrdiff_t upper(std::size_t dim=DIM) const {
		if(dim==0 || dim>DIM) throw std::out_of_range("Index out of range");
		return _upper[DIM-dim];
	}
	virtual ~AbstractBoundedArray(){}
};

template<>
class AbstractBoundedArray<1>{
protected:
	std::ptrdiff_t _lower;
	std::ptrdiff_t _upper;
	AbstractBoundedArray(std::ptrdiff_t lowerBound, std::ptrdiff_t upperBound):
		_lower(lowerBound), _upper(upperBound){}
	AbstractBoundedArray(const AbstractBoundedArray<1>& other) : _lower(other._lower), _upper(other._upper){}
public:
	virtual bool inBounds(std::size_t index) const { return (index>=this->lower()) && (index<=this->upper()); }
	virtual std::ptrdiff_t lower() const { return _lower; }
	virtual std::ptrdiff_t upper() const { return _upper; }
	virtual ~AbstractBoundedArray(){}
};

#endif