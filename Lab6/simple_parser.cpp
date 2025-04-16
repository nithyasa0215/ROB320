#include "simple_parser.hpp"

bool is_approx(float a, float b) {
    return std::abs(a - b) < 0.0001;
}

int main () {
    Example example;
    parse_example("example.json", example);

    assert(example.my_number == 123);
    assert(example.my_string == "ROB320!");
    assert(example.my_array.size() == 3);
    assert(example.my_array[0] == 4);
    assert(example.my_array[1] == 5);
    assert(example.my_array[2] == 6);
    assert(example.my_object.my_bool == true);
    assert(is_approx(example.my_object.my_float, 3.14f));
}