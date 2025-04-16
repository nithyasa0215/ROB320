#include <iostream>

#include "value_based_static_decorator.hpp"

int main() {
    // Make a Taxed Discounted ConferenceTicket priced item.
    Taxed<15, Discounted<20, ConferenceTicket>> item{"Core C++", 499.0};

    // TODO: Make a Free Discounted ConferenceTicket item.

    // The price should be 499*0.8*1.15 = 459.08.
    std::cout << item.price() << std::endl;

    // TODO: Output the price of the new item followed by a newline.
}