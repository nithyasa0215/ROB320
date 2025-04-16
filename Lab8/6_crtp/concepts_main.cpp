#include "concepts.hpp"

#include <iostream>

template<DenseVector T>
void print_size(T const& vector) {
    std::cout << "T is gauranteed to have a size() function: " << vector.size()
    << std::endl;

    // More generally, T has every function required by the DenseVector concept.
}

int main() {
    DynamicVector<int> dynamic_no_vector;
    print_size(dynamic_no_vector);
    StaticVector<int, 0> static_no_vector;
    print_size(static_no_vector);
}