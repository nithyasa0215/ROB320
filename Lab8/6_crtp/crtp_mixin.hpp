/**
 * CRTP can also be used to support mixin behaviors, but it doesn't really
 * provide a decoupling abstraction.
 */

#ifndef LAB8_CRTP_MIXIN_HPP
#define LAB8_CRTP_MIXIN_HPP

#include <iostream>

template <typename T, typename Tag, template<typename> class... Skills>
class StrongType : private Skills<StrongType<T, Tag, Skills... >>... {
   public:
    using value_type = T;

    explicit StrongType( T const& value ) : value_(value) {}

    T& get() {
        return value_;
    }

    T const& get() const {
        return value_;
    }

   private:
    T value_;
};

template <typename Derived>
class Addable {
    friend Derived& operator+=(Derived& lhs, Derived& rhs) {
        return lhs.get() += rhs.get();
    }

    friend Derived operator+(Derived const& lhs, Derived const& rhs) {
        return Derived{lhs.get() + rhs.get()};
    }
};

template <typename Derived>
class Printable {
    friend std::ostream& operator<<(std::ostream& os, const Derived& d) {
        os << d.get();
        return os;
    }
};

template <typename Derived>
class Swappable {
    friend void swap(Derived& lhs, Derived& rhs) {
        using std::swap;
        swap(lhs.get(), rhs.get());
    }
};

template <typename T>
using Meter = StrongType<T, struct MeterTag, Addable, Printable, Swappable>;

template <typename T>
using Kilometer = StrongType<T, struct KilometerTag, Addable, Printable, Swappable>;

using Name = StrongType<std::string, struct NameTag, Printable, Swappable>;

#endif  // LAB8_CRTP_MIXIN_HPP
