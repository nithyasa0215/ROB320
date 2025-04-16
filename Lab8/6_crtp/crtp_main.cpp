#include <iostream>

#include "crtp.hpp"

int main() {
    DynamicVector<int> dynamic_no_vector;
    std::cout << dynamic_no_vector.size() << std::endl;
    StaticVector<int, 0> static_no_vector;
    std::cout << static_no_vector.size() << std::endl;
}