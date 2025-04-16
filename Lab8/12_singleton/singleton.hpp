#ifndef LAB8_SINGLETON_HPP
#define LAB8_SINGLETON_HPP

#include <iostream>

/// An interface for the Singleton, to provide dependency inversion for classes
/// using the Singleton in their implementations.
class PersistenceInterface {
   public:
    virtual ~PersistenceInterface() = default;

    /// Public interface to be used by any dependent classes. Calls private interface.
    bool read() const {
        return do_read();
    }
    bool write() {
        return do_write();
    }

   private:
    /// Private interface to be defined by implementations of the Singleton.
    virtual bool do_read() const = 0;
    virtual bool do_write() = 0;
};

/// Free function for getting the interface, providing a default instantiation.
PersistenceInterface* get_persistence_interface();

/// Free function for setting the interface, provided a new instantiation.
void set_persistence_interface(PersistenceInterface* persistence);

/// Declaration of a global pointer to the Singleton's interface, not to be
/// accessed directly.
extern PersistenceInterface* instance;

/// Default implementation of the Singleton (a database in this case).
class Database : public PersistenceInterface {
   public:
    Database() {}
    Database(Database const&) = delete;
    Database& operator=(Database const&) = delete;
    Database(Database&&) = delete;
    Database& operator=(Database&&) = delete;

   private:
    bool do_read() const override {
        std::cout << "Pretend this performs a read operation" << std::endl;
        return true;
    }
    bool do_write() override {
        std::cout << "Pretend this performs a write operation" << std::endl;
        return true;
    }

    // Actual data base member variables . . .
};

/// An implementation of the Singleton with fixed returns that could be set
/// while testing any dependant classes.
class FakeDataBase : public PersistenceInterface {
public:
    FakeDataBase() {}
    FakeDataBase(FakeDataBase const&) = delete;
    FakeDataBase& operator=(FakeDataBase const&) = delete;
    FakeDataBase(FakeDataBase&&) = delete;
    FakeDataBase& operator=(FakeDataBase&&) = delete;

private:
    bool do_read() const override {
        std::cout << "Pretend this has a fixed read behavior, used for testing" << std::endl;
        return true;
    }
    bool do_write() override {
        std::cout << "Pretend this has a fixed write behavior, used for testing" << std::endl;
        return true;
    }

    // Data member variables if necessary.
};

#endif  // LAB8_SINGLETON_HPP
