/**
 * CRTP can also be used to support mixin behaviors, but it doesn't really
 * provide a decoupling abstraction.
 */

#include "crtp_mixin.hpp"

int main() {
    // Try out operations on Meters.
    Meter<int> distance_1_m(100);
    Meter<int> distance_2_m(200);
    Meter<int> cumulative_distance_m = distance_1_m + distance_2_m;
    swap(distance_1_m, distance_2_m);
    std::cout << cumulative_distance_m << std::endl;

    // Check that adding Meters and Kilometers doesn't compile.
    Kilometer<int> distance_km(5);
    // auto bad_news = distance_km + distance_1_m;

    // Try out operations on Names.
    Name name_1("Sai");
    Name name_2("Halina");
    swap(name_1, name_2);
    std::cout << name_1 << std::endl;
    std::cout << name_2 << std::endl;

    // Check that adding Names doesn't compile.
    // auto more_bad_news = name_1 + name_2;
}