all: Vec

make++: Vec++

Vec++: Vec.cpp
			clang++ -std=c++11 -Wpedantic -Wall -Wextra -Winit-self -Winline -Wconversion -Weffc++ -Wctor-dtor-privacy -Woverloaded-virtual -Wconversion -Wsign-promo Vec.cpp -o Vec

Vec: Vec.cpp
		g++-mp-4.8 -std=c++11 -Wpedantic -Wall -Wextra -Winit-self -Winline -Wconversion -Weffc++ -Wstrict-null-sentinel -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wconversion -Wsign-promo -Wzero-as-null-pointer-constant Vec.cpp -o Vec