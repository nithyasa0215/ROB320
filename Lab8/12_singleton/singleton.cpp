#include "singleton.hpp"

/// Definition of the global pointer to the Singleton interface.
PersistenceInterface* instance = nullptr;

/// Free function for getting the interface, providing a default instantiation.
PersistenceInterface* get_persistence_interface() {
    // Static immediately evaluated lambda allows you to declare Database that
    // will not destruct after get_persistence_interface() returns.
    static bool init = [](){
        if(!instance) {
            static Database db;
            instance = &db;
        }
        return true;
    }();
    return instance;
}

/// Free function for setting the interface, provided a new instantiation.
void set_persistence_interface(PersistenceInterface* persistence) {
    instance = persistence;
}
