#ifndef LAB8_OBSERVER_HPP
#define LAB8_OBSERVER_HPP

#include <iostream>
#include <set>
#include <string>

/// Pull observer interface forcing update() function based on observed object's
/// public interface and provided state change tag.
template <typename Subject, typename StateTag>
class Observer {
   public:
    virtual ~Observer() = default;

    /// Update() should cause some update to the Observer's state, or call an
    /// injected callback routine useful to a third party.
    virtual void update(Subject const& subject, StateTag property) = 0;
};

/// Object representing a person whose state may be changed and whose state may
/// be observed by multiple other software entities.
class Person {
   public:
    /// An enumeration of the different kinds of state changes that may occur.
    /// Note that in general these could correspond to more complicated states
    /// than just the state of the member variables.
    enum StateChange {
        forenameChanged,
        surnameChanged,
        addressChanged
    };

    using PersonObserver = Observer<Person,StateChange>;

    explicit Person (std::string forename, std::string surname)
        : forename_{std::move(forename)}, surname_{std::move(surname)} {}

    /// Allow observers to be attached.
    bool attach(PersonObserver* observer) {
        auto [pos,success] = observers_.insert(observer);
        return success;
    }

    /// Allow observers to be detached.
    bool detach(PersonObserver* observer) {
        return (observers_.erase(observer) > 0);
    }

    /// Provide a helper function to be called anywhere where an observable state
    /// is changed in the implementation of Person member functions.
    void notify( StateChange property) {
        for (auto iter = begin(observers_); iter != end(observers_); ) {
            auto const pos = iter++;
            (*pos)->update(*this, property);
        }
    }

    void forename(std::string new_forename) {
        forename_ = std::move(new_forename);

        // If you change a state, immediately call notify with the corresponding state.
        notify(forenameChanged);
    }

    void surname(std::string new_surname) {
        surname_ = std::move(new_surname);

        // If you change a state, immediately call notify with the corresponding state.
        notify(surnameChanged);
    }

    void address(std::string new_address) {
        address_ = std::move(new_address);

        // If you change a state, immediately call notify with the corresponding state.
        notify(addressChanged);
    }

    private:
     std::string forename_;
     std::string surname_;
     std::string address_;

     std::set<PersonObserver*> observers_;
};

/// An observer that updates if any change is made to the observed person's name.
class NameObserver : public Observer<Person, Person::StateChange> {
   public:
    void update(Person const& person, Person::StateChange property) override {
        if (property == Person::forenameChanged ||
            property == Person::surnameChanged) {
            std::cout << "Pretend this updates the NameObserver in a way that "
                         "would be useful to a third party." << std::endl;
        }
    }
};

// TODO: Make an observer called AddressObserver. Its update function should
//       simply check if the property is Person::addressChanged and if so output:
//       "Pretend this updates the AddressObserver in a way that would be useful
//       to a third party."

#endif  // LAB8_OBSERVER_HPP
