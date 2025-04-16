#ifndef LAB8_CONCEPTS_HPP
#define LAB8_CONCEPTS_HPP

#include <array>
#include <concepts>
#include <vector>

/// Additional tag type checked for in the concept. Classes adhering to the
/// concept should inherit from this tag. Used to make sure the user meant to
/// adhere to the interface specified by the concept.
struct DenseVectorTag {};

/// Evaluates to true if T is derived from DenseVectorTag. This is checked in
/// the concept via IsDenseVector_v. Splitting this out allows it to also be
/// specialized to true for classes that can't be modified to inherit from
/// DenseVectorTag.
template<typename T>
struct IsDenseVector : public std::is_base_of<DenseVectorTag, T> {};

/// Report the value of the above template at compile time.
template<typename T>
constexpr bool IsDenseVector_v = IsDenseVector<T>::value;

/// Concept that requires all the functions wanted in the interface.
template<typename T>
concept DenseVector =
    requires (T type, std::size_t index) {
        type.size();
        type[index];
        { type.begin() } -> std::same_as<typename T::iterator>;
        { type.begin() } -> std::same_as<typename T::iterator>;
        { type.end() } -> std::same_as<typename T::iterator>;
        { type.end() } -> std::same_as<typename T::iterator>;
    } &&
    requires (T const type, std::size_t index) {
        type[index];
        { type.begin() } -> std::same_as<typename T::const_iterator>;
        { type.begin() } -> std::same_as<typename T::const_iterator>;
        { type.end() } -> std::same_as<typename T::const_iterator>;
        { type.end() } -> std::same_as<typename T::const_iterator>;
    } &&
    /// Also check the tag condition.
    IsDenseVector_v<T>;

/// This type satisfies the concept by inheriting from DenseVectorTag and
/// providing the required functions.
template<typename T>
class DynamicVector : private DenseVectorTag {
   public:
    using value_type = T;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    // Functions for initialization...

    // TODO: Comment out the size() function below, and note that the code
    //       no longer compiles.
    size_t size() const { return values_.size(); }
    value_type const& operator[](size_t index) const { return values_[index]; }
    iterator begin() { return values_.begin(); }
    const_iterator begin() const { return values_.cbegin(); }
    iterator end() { return values_.end(); }
    const_iterator end() const { return values_.cend(); }

    // Functions for actually doing math...

   private:
    std::vector<T> values_;
};

/// This type satisfies the concept by specializing IsDenseVector to true, and
/// implementing all the required functions.
template<typename T, std::size_t Size>
class StaticVector {
   public:
    using value_type = T;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    // Functions for initialization...

    // TODO: Comment out the size() function below, and note that the code
    //       no longer compiles.
    size_t size() const { return values_.size(); }
    value_type const& operator[](size_t index) const { return values_[index]; }
    iterator begin() { return values_.begin(); };
    const_iterator begin() const { return values_.cbegin(); };
    iterator end() { return values_.end(); }
    const_iterator end() const { return values_.cend(); }

    // Functions for actually doing math...

   private:
    std::array<T, Size> values_;
};

/// Specialize any StaticVector type to report true for IsDenseVector::value.
template<typename T, std::size_t Size>
struct IsDenseVector<StaticVector<T,Size>> : public std::true_type {};

#endif  // LAB8_CONCEPTS_HPP
