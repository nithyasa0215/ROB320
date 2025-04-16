#include "singleton.hpp"

int main() {
    // TODO: Make a fake database and set the Singleton interface to use it.

    // Get the Singleton interface and use it in downstream code.
    PersistenceInterface* database = get_persistence_interface();
    database->read();
    database->write();
}