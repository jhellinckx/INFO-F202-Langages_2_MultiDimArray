/*
Nom : 			Hellinckx
Prénom : 		Jérôme
Matricule :		000376500
Année d'étude : 2014-2015
*/

#ifndef ABSTRACTDIMENSIONALARRAY_HPP
#define ABSTRACTDIMENSIONALARRAY_HPP

#include <exception>
#include <stdexcept>

/* Classe abstraite définissant des opérations basiques sur la longueur des 
dimensions d'un tableau. 
Cette classe n'a qu'un pointeur vers un vecteur dont le premier élément
est la longueur de la dimension DIM, le suivant DIM-1, etc. 
Une classe dérivée aura donc la responsabilité en mémoire de ce vecteur. 
Il y aura deux types distincts de classes dérivées : d'une part
celles avec la responsabilité mémoire du tableau et de ses caractéristiques (DimensionalArray),
d'autre part celles dont les attributs pointent vers une zone mémoire de ce
tableau et ses caractéristiques (SubDimensionalArray). On évite donc d'utiliser
de la mémoire inutilement pour ces dernières. */

template<std::size_t DIM> 
class AbstractDimensionalArray{
protected:
	std::size_t* _lengths;
	/* Classe abstraite : constructeurs en protected */
	AbstractDimensionalArray(std::size_t* lengthsArray) : _lengths(lengthsArray) {}
	AbstractDimensionalArray(const AbstractDimensionalArray& other): _lengths(other._lengths) {}
	AbstractDimensionalArray<DIM>& operator=(const AbstractDimensionalArray& other) {
		if(this!=&other){
			for(std::size_t i=0;i<DIM;++i) _lengths[i] = other._lengths[i];
		}
		return *this;
	}
public:
	/* Renvoie le nombre d'éléments compris dans la dimension donnée */
	virtual std::size_t size(std::size_t dim=DIM) const { 		
		if(dim==0 || dim>DIM) throw std::out_of_range("Dimension out of range");
		std::size_t size = 1; 
		for(std::size_t i=DIM-dim;i<DIM;++i) size*=_lengths[i]; 
		return size;
	}
	/* Renvoie vrai si, respectivement, les dimensions de l'objet donné 
	en paramètre est égale aux dimensions de this */
	virtual bool sameLengths(const AbstractDimensionalArray<DIM>& other) const {
		for(std::size_t i=0;i<DIM;++i){
			if(_lengths[i]!=other._lengths[i]) return false;
		} 
		return true;
	}
	virtual std::size_t dimension() const { return DIM; }
	virtual std::size_t length(std::size_t dim=DIM) const { 
		if(dim==0 || dim>DIM) throw std::out_of_range("Dimension out of range");
		return _lengths[DIM-dim];
	}
	virtual void print(std::ostream& out) const{
		out << this->size() << "(";
		for(std::size_t dim=DIM;dim>0;--dim) {
			out << this->length(dim);
			if(dim!=1) out << ",";
		}
		out << ")";
	}

	virtual ~AbstractDimensionalArray(){}
};

/* Spécialisation complète de AbstractDimensionalArray pour DIM=1 */
template<> 
class AbstractDimensionalArray<1>{
protected:
	std::size_t _length;
	AbstractDimensionalArray(std::size_t length) : _length(length) {}
	AbstractDimensionalArray(const AbstractDimensionalArray<1>& other) : _length(other._length) {}
	void setLength(std::size_t length) { _length = length; }
public:
	virtual std::size_t dimension() const { return 1; }
	virtual std::size_t length() const { return _length; }
	virtual std::size_t size() const { return _length; }
	virtual bool sameLength(const AbstractDimensionalArray<1>& other) const { return _length == other._length; } 
	virtual void print(std::ostream& out) const { out << this->length(); }
	virtual ~AbstractDimensionalArray(){}
};

/* Opérateur d'output ne fait qu'appeller print(), méthode virtuelle overridée par les classes dérivées */
template<std::size_t DIM>
std::ostream& operator<< (std::ostream& out, const AbstractDimensionalArray<DIM>& vec) {
	vec.print(out);
  	return out;
}

#endif