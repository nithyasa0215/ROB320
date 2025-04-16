#include <iostream>

#include "decorator.hpp"

int main() {
    // Make an item that is a Taxed CppBook.
    // Note that due to the decorators, the price should be 19 * 1.07 = 20.33.
    std::unique_ptr<Item> item1(
        std::make_unique<Taxed>(0.07,
            std::make_unique<CppBook>("Effective C++", 19.0)));

    // Make an item that is a Taxed Discounted ConferenceTicket.
    // Note that due to the decorators, the price should be (999*0.8)*0.19 = 951.048.
    std::unique_ptr<Item> item2(
        std::make_unique<Taxed>(0.19,
        std::make_unique<Discounted>(0.2,
            std::make_unique<ConferenceTicket>("CppCon", 999.0))));

    // TODO: Make a Discounted, Taxed, Free, CppBook.

    // Check that actual prices.
    std::cout << "Price of Taxed CppBook: " << item1->price() << std::endl;
    std::cout << "Price of Taxed Discounted ConferenceTicket: " << item2->price() << std::endl;

    // TODO: Output the price of the new book followed by a newline.
}