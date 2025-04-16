#include "observer.hpp"

int main() {
    // Make a NameObserver and attach it to two people.
    NameObserver name_observer;
    Person chad("Chad", "Jenkins");
    Person brody("Brody", "Riopelle");
    chad.attach(&name_observer);
    brody.attach(&name_observer);

    // Change Chad's name, inducing a response from the observer.
    chad.forename("Professor Chad");

    // TODO: Make an AddressObserver and attach it to Brody.

    // TODO: Update Brody's address to be "2505 Hayward Ave.".

    // TODO: Detach both observers wherever they are attached.
}