#ifndef CONTAINERBOUNDS_HPP
#define CONTAINERBOUNDS_HPP

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include "ContainerLengths.hpp"

/* global scope typedef */
typedef std::initializer_list<std::initializer_list<std::ptrdiff_t>> InitBounds;

class ContainerBounds{
	std::ptrdiff_t* _lower;

public:
	/* le constructeur rempli lengths par la longueur de chaque dimension tel que 
	lengths[0] == longueur de la plus grande dimension  */
	ContainerBounds(std::size_t dimension, std::size_t* lengths, const InitBounds& bounds) : 
		_lower(new std::ptrdiff_t[dimension]){
			if(initBounds.size()!=dimension) throw std::logic_error("Initializer_list needs bounds for each dimension");
			std::size_t dim=0;
			for(InitBounds::iterator dimensionIt = initBounds.begin();dimensionIt!=initBounds.end();++dimensionIt){
				if(dimensionIt->size()!=2) throw std::logic_error("Initializer_list needs exactly 2 bounds for each dimension");
				std::initializer_list<std::ptrdiff_t>::iterator boundsIt = dimensionIt->begin();
				std::ptrdiff_t lowerBound = *(boundsIt); std::ptrdiff_t upperBound = *(++boundsIt);
				upperBound-lowerBound+1 < 0 ? throw std::logic_error("Lower bound cannot be greater than upper bound") : lengths[dim] = upperBound-lowerBound+1;
				_lower[dim++] = lowerBound;
			}
	}
	~ContainerBounds(delete[] _lower;)

};

#endif 