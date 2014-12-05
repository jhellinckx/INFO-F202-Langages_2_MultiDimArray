#ifndef BOUNDEDSUBDIM_HPP
#define BOUNDEDSUBDIM_HPP

template<typename Elem, std::size_t DIM>
class BoundedSubDimensionalArray: public SubDimensionalArray<Elem, DIM>{

protected:
	BoundedSubDimensionalArray(const ContainerLengths& lengths, Elem* val):
		Dimens


	friend class BoundedMultiDimensionalArray<Elem, DIM+1>;
	friend class BoundedSubDimensionalArray<Elem, DIM+1>;
};

#endif