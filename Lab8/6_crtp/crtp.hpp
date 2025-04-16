#ifndef LAB8_CRTP_HPP
#define LAB8_CRTP_HPP

#include <array>
#include <vector>

/// Template with Derived class so you can cast to it and call its functions.
template <typename Derived>
class DenseVector {
   public:
    /// Convenience functions for casting to template type.
    Derived& derived() { return static_cast<Derived&>(*this); }
    Derived const& derived() const { return static_cast<Derived const&>(*this); }

    /// Implement functions you want in the interface. For each, simply cast to
    /// the derived type and call its version of the function. The return type
    /// needs to be deduced at compile time via decltype(auto).
    size_t size() const { return derived().size_impl(); }
    decltype(auto) operator[](size_t index) { return derived()[index]; }
    decltype(auto) operator[](size_t index) const { return derived()[index]; }
    decltype(auto) begin() { return derived().begin_impl(); }
    decltype(auto) begin() const { return derived().begin_impl(); }
    decltype(auto) end() { return derived().end_impl(); }
    decltype(auto) end() const { return derived().end_impl(); }

    // Functions for actually doing math...

   protected:
    ~DenseVector() = default;
};

/// Inherit from the base type, templating it with this derived type.
/// Creating an instance of this class will cause the compiler to instantiate
/// code for the base class, which will in turn check that all the Derived type
/// member functions being called in the base class actually exist.
template <typename T>
class DynamicVector : public DenseVector<DynamicVector<T>> {
   public:
    using value_type = T;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    // Functions for initialization...

    // TODO: Comment out the size_impl() function below, and note that the code
    //       no longer compiles.
    size_t size_impl() const { return values_.size(); }
    value_type& operator[](size_t index) { return values_[index]; }
    value_type const& operator[](size_t index) const { return values_[index]; }
    iterator begin_impl() { return values_.begin(); }
    const_iterator begin_impl() const { return values_.cbegin(); }
    iterator end_impl() { return values_.end(); }
    const_iterator end_impl() const { return values_.cend(); }

    // Functions for actually doing math...

   private:
    std::vector<T> values_;
};

/// Note that the base classes never get used concretely (no pointers!) and the
/// two derived classes actually each instantiate their own base class. It is
/// always one base class per derived class.
template <typename T, size_t Size>
class StaticVector : public DenseVector<StaticVector<T, Size>> {
   public:
    using value_type = T;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    // Functions for initialization...

    // TODO: Comment out the size_impl() function below, and note that the code
    //       no longer compiles.
    size_t size_impl() const { return values_.size(); }
    value_type& operator[](size_t index) { return values_[index]; }
    value_type const& operator[](size_t index) const { return values_[index]; }
    iterator begin_impl() { return values_.begin(); };
    const_iterator begin_impl() const { return values_.cbegin(); };
    iterator end_impl() { return values_.end(); }
    const_iterator end_impl() const { return values_.cend(); }

    // Functions for actually doing math...

   private:
    std::array<T, Size> values_;
};

#endif  // LAB8_CRTP_HPP
