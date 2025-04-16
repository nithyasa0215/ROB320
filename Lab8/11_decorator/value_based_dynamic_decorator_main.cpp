#include <iostream>

#include "value_based_dynamic_decorator.hpp"

int main() {
    // Make an item that is a Taxed Discounted ConferenceTicket.
    Item item(Taxed(0.15, Discounted(0.2, ConferenceTicket{"Core C++", 499.0})));

    // TODO: Make a Taxed, Free, ConferenceTicket item.

    // The price should be 499*0.8*1.15 = 459.08.
    std::cout << item.price() << std::endl;

    // TODO: Output your new item's price followed by a new line.
}
