#ifndef LAB8_VALUE_BASED_DYNAMIC_DECORATOR_HPP
#define LAB8_VALUE_BASED_DYNAMIC_DECORATOR_HPP

#include <memory>

using Money = double;

/// Item class which internally stores any object with a price() function.
class Item {
   public:
    /// Templated constructor allows any object to be passed in and stored via
    /// a pimpl_ pointer.
    template <typename T>
    Item(T item)
       : pimpl_(std::make_unique<Model<T>>(std::move(item))) {}

    /// Allow copy operations via prototype.
    Item(Item const& item) : pimpl_(item.pimpl_->clone()) {}

    /// Allow copy operations via prototype.
    Item& operator=(Item const& item) {
        pimpl_ = item.pimpl_->clone();
        return *this;
    }

    /// Default destructor and move constructors.
    ~Item() = default;
    Item(Item&&) = default;
    Item& operator=(Item&& item) = default;

    /// The function to be decorated. Calls into the stored object.
    Money price() const { return pimpl_->price(); }

   private:
    /// Uniform interface for stored object.
    struct Concept {
        virtual ~Concept() = default;
        virtual Money price() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };

    /// Stored object template where template type must have a price() function.
    template <typename T>
    struct Model : public Concept {
        explicit Model(T const& item) : item_(item) {}
        explicit Model(T&& item) : item_(std::move(item)) {}

        Money price() const override { return item_.price(); }

        std::unique_ptr<Concept> clone() const override {
            return std::make_unique<Model<T>>(*this);
        }

        /// The stored type, which can be any type with a price() function.
        T item_;
    };

    /// The object passed to the constructor, now accessible only via the Concept
    /// interface.
    std::unique_ptr<Concept> pimpl_;
};

/// A regular class that stores an Item and wraps its price() function.
class Discounted {
   public:
    Discounted(double discount, Item item)
        : item_(std::move(item)), factor_(1.0 - discount) {}

    Money price() const {
        return item_.price() * factor_;
    }

   private:
    Item item_;
    double factor_;
};

/// Another regular class that stores an Item and wraps its price() function.
class Taxed {
   public:
    Taxed(double tax_rate, Item item)
        : item_(std::move(item)), factor_(1.0 + tax_rate) {}

    Money price() const {
        return item_.price() * factor_;
    }

   private:
    Item item_;
    double factor_;
};

/// A terminal class with just a price() function and no stored Item.
class ConferenceTicket {
   public : ConferenceTicket(std::string name, Money price)
        : name_{std::move(name)}, price_{price} {}

    std::string const& name() const { return name_; }
    Money price() const { return price_; }

   private:
    std::string name_;
    Money price_;
};

// TODO: Add another decorator class called "Free" which makes the stored item
//       free.

#endif  // LAB8_VALUE_BASED_DYNAMIC_DECORATOR_HPP
