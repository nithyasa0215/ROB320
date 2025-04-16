#ifndef LAB8_DECORATOR_HPP
#define LAB8_DECORATOR_HPP

#include <memory>
#include <cmath>

using Money = double;

/// Item interface that all items should support. Only operations in this
/// interface can be modified via decoration.
class Item {
   public:
    virtual ~Item() = default;
    virtual Money price() const = 0;
};

/// Base class for all decorated items, which enforces decorated items storing a
/// pointer to an Item.
class DecoratedItem : public Item {
   public:
    explicit DecoratedItem( std::unique_ptr<Item> item)
        : item_(std::move(item))
    {
        if(!item_) {
            throw std::invalid_argument("Invalid item");
        }
    }

   protected:
    Item& item() { return *item_; }
    Item const& item() const { return *item_; }

   private:
    std::unique_ptr<Item> item_;
};

/// A specific kind of item, adding in a title() function.
class CppBook : public Item {
   public:
    CppBook( std::string title, Money price)
        : title_{std::move(title)}, price_{price} {}

    std::string const& title() const { return title_; }
    Money price() const override { return price_; }

   private:
    std::string title_{};
    Money price_{};
};

/// Another specific kind of item, adding in a name() function.
class ConferenceTicket : public Item {
   public:
    ConferenceTicket(std::string name, Money price)
        : name_{std::move(name)}, price_{price} {}

    std::string const& name() const { return name_; }
    Money price() const override {return price_; }

   private:
    std::string name_{};
    Money price_{};
};

/// A decorated item that wraps the item's price() functionality with a discount.
class Discounted : public DecoratedItem {
   public:
    Discounted(double discount, std::unique_ptr<Item> item)
        : DecoratedItem(std::move(item)), factor_(1.0 - discount)
    {
        if(!std::isfinite(discount) || discount < 0.0 || discount > 1.0) {
            throw std::invalid_argument("Invalid discount");
        }
    }

    Money price() const override {
        return item().price() * factor_;
    }

   private:
    double factor_;
};

/// A decorated item that wraps the item's price() functionality with a tax.
class Taxed : public DecoratedItem {
   public:
    Taxed(double tax_rate, std::unique_ptr<Item> item)
        : DecoratedItem( std::move(item)), factor_(1.0 + tax_rate)
    {
        if(!std::isfinite(tax_rate) || tax_rate < 0.0) {
            throw std::invalid_argument("Invalid tax");
        }
    }

    Money price() const override {
        return item().price() * factor_;
    }

   private:
     double factor_;
};

// TODO: Write a decorator that makes the item free, called Free.

#endif  // LAB8_DECORATOR_HPP
