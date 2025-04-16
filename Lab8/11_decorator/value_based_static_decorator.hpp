#ifndef LAB8_VALUE_BASED_STATIC_DECORATOR_HPP
#define LAB8_VALUE_BASED_STATIC_DECORATOR_HPP

#include <memory>
#include <utility>

using Money = double;

/// A regular class with a price() function.
class ConferenceTicket {
   public : ConferenceTicket(std::string name, Money price)
        : name_{std::move(name)}, price_{price} {}

    std::string const& name() const { return name_; }
    Money price() const { return price_; }

   private:
    std::string name_;
    Money price_;
};

/// A concept forcing the templated type to have a price function that returns
/// Money.
template <typename T>
concept PricedItem =
    requires (T item) {
        { item.price() } -> std::same_as<Money>;
    };

/// Decorator storing the decorated item via a member variable.
template <int discount, PricedItem Item>
class Discounted {
   public:
    /// Templated constructor to initialize the member variable as any PricedItem.
    template <typename... Args>
    explicit Discounted(Args&&... args)
        : item_{std::forward<Args>(args)... } {}

    /// Price() function wrapping stored item's price() function.
    Money price() const {
        // Can't template with a double in C++, so must use int, then cast.
        return item_.price() * (1.0 - static_cast<double>(discount) / 100.0);
    }

   private:
    Item item_;
};

/// Decorator storing the decorated item as a base class.
template <int tax_rate, PricedItem Item>
class Taxed : private Item {
   public:
    /// Templated constructor to initialize the base class as any PricedItem.
    template <typename... Args>
    explicit Taxed(Args&&... args)
        : Item{std::forward<Args>(args)... } {}

    /// Price() function wrapping base class's price() function.
    Money price() const {
        // Can't template with a double in C++, so must use int, then cast.
        return Item::price() * (1.0 + static_cast<double>(tax_rate) / 100.0);
    }
};

// TODO: Make a decorator called Free to make an item's price zero. Feel free to
//       use either pattern.

#endif  // LAB8_VALUE_BASED_STATIC_DECORATOR_HPP
