#pragma once

#include <type_traits>

#include "core/units/core/dim.h"
#include "core/units/core/unit_traits.h"

namespace units {

/**
 * Quantity is the type that all the units like Length etc inherit from...
 * this way they all have the same operators and such cuz they're all the "same"
 *
 * note UnitTag here is for templates
 */
template<class Dim>
class anonymous_quantity;

template<class Dim>
struct quantity_for_dimension;

template<class Dim>
struct quantity_for_dimension {
  typedef anonymous_quantity<Dim> type;
};

class quantity_root {};

template<class Quantity>
class has_dimension {
 private:
  typedef char yes_type;
  struct no_type {
    char data[2];
  };

  template<class T>
  static yes_type test(typename T::dimension*);

  template<class T>
  static no_type test(...);

 public:
  static const bool value = sizeof(test<Quantity>(0)) == sizeof(yes_type);
};

template<class Quantity>
struct is_quantity : std::integral_constant<bool, has_dimension<Quantity>::value> {};

template<class Dim, class = void>
struct has_quantity_for_dimension : std::false_type {};

template<class Dim>
struct has_quantity_for_dimension<Dim, typename std::enable_if<
                                           !std::is_same<
                                               typename quantity_for_dimension<Dim>::type,
                                               void>::value>::type> : std::true_type {};

template<class Left, class Right, bool Enabled = is_quantity<Left>::value &&
                                               is_quantity<Right>::value>
struct same_dimension;

template<bool Condition, class Type>
using enable_if_t = typename std::enable_if<Condition, Type>::type;

template<class Quantity>
using quantity_result_t = enable_if_t<is_quantity<Quantity>::value, Quantity>;

template<class Left, class Right>
using same_dimension_result_t =
    enable_if_t<
        is_quantity<Left>::value && is_quantity<Right>::value &&
            same_dimension<Left, Right>::value,
        typename quantity_for_dimension<typename Left::dimension>::type>;

// are templates steamhappy or steamsad?
template<class Left, class Right>
using dim_add_result_t =
    enable_if_t<
        is_quantity<Left>::value && is_quantity<Right>::value,
        typename quantity_for_dimension<
            typename dim_add<typename Left::dimension, typename Right::dimension>::type>::type>;

template<class Left, class Right>
using dim_sub_result_t =
    enable_if_t<
        is_quantity<Left>::value && is_quantity<Right>::value,
        typename quantity_for_dimension<
            typename dim_sub<typename Left::dimension, typename Right::dimension>::type>::type>;

template<class Quantity>
using inverse_quantity_result_t =
    enable_if_t<
        is_quantity<Quantity>::value &&
            has_quantity_for_dimension<
                typename dim_sub<dim_none, typename Quantity::dimension>::type>::value,
        typename quantity_for_dimension<
            typename dim_sub<dim_none, typename Quantity::dimension>::type>::type>;

template<class Left, class Right>
using comparison_result_t =
    enable_if_t<
        is_quantity<Left>::value && is_quantity<Right>::value &&
            same_dimension<Left, Right>::value,
        bool>;

template<class Derived, class Dim>
class quantity_base : public quantity_root {
 public:
  typedef Dim dimension;

  // internal storage
  constexpr double canonical_value() const { return canonical_value_; }

  // convert from storage to otherj
  template<class UnitTag>
  constexpr double as() const {
    static_assert(
        std::is_same<typename unit_traits<UnitTag>::dimension, Dim>::value,
        "unit dimension mismatch");
    return canonical_value_ * unit_traits<UnitTag>::scale_den /
           unit_traits<UnitTag>::scale_num;
  }

 protected:
  constexpr explicit quantity_base(double canonical_value)
      : canonical_value_(canonical_value) {}

  template<class UnitTag>
  static constexpr double to_canonical(double value) {
    static_assert(
        std::is_same<typename unit_traits<UnitTag>::dimension, Dim>::value,
        "unit dimension mismatch");
    return value * unit_traits<UnitTag>::scale_num /
           unit_traits<UnitTag>::scale_den;
  }

  void set_canonical_value(double canonical_value) {
    canonical_value_ = canonical_value;
  }

 public:
  template<class OtherQuantity>
  typename std::enable_if<
      std::is_same<Dim, typename OtherQuantity::dimension>::value,
      Derived&>::type
  operator+=(const OtherQuantity& other) {
    set_canonical_value(canonical_value() + other.canonical_value());
    return static_cast<Derived&>(*this);
  }

  template<class OtherQuantity>
  typename std::enable_if<
      std::is_same<Dim, typename OtherQuantity::dimension>::value,
      Derived&>::type
  operator-=(const OtherQuantity& other) {
    set_canonical_value(canonical_value() - other.canonical_value());
    return static_cast<Derived&>(*this);
  }

  Derived& operator*=(double scalar) {
    set_canonical_value(canonical_value() * scalar);
    return static_cast<Derived&>(*this);
  }

  Derived& operator/=(double scalar) {
    set_canonical_value(canonical_value() / scalar);
    return static_cast<Derived&>(*this);
  }

  Derived& operator++() {
    set_canonical_value(canonical_value() + 1.0);
    return static_cast<Derived&>(*this);
  }

  Derived operator++(int) {
    Derived copy = static_cast<Derived&>(*this);
    ++(*this);
    return copy;
  }

  Derived& operator--() {
    set_canonical_value(canonical_value() - 1.0);
    return static_cast<Derived&>(*this);
  }

  Derived operator--(int) {
    Derived copy = static_cast<Derived&>(*this);
    --(*this);
    return copy;
  }

 private:
  double canonical_value_;
};

template<class Dim>
class anonymous_quantity : public quantity_base<anonymous_quantity<Dim>, Dim> {
 public:
  typedef quantity_base<anonymous_quantity<Dim>, Dim> base_type;
  typedef Dim dimension;

  constexpr explicit anonymous_quantity(double canonical_value = 0.0)
      : base_type(canonical_value) {}

  template<class OtherQuantity>
  constexpr anonymous_quantity(const OtherQuantity& other,
                               typename std::enable_if<
                                   std::is_same<Dim, typename OtherQuantity::dimension>::value,
                                   int>::type = 0)
      : base_type(other.canonical_value()) {}

  static constexpr anonymous_quantity from_canonical(double value) {
    return anonymous_quantity(value);
  }
};

template<class Left, class Right, bool Enabled>
struct same_dimension : std::false_type {};

template<class Left, class Right>
struct same_dimension<Left, Right, true>
    : std::is_same<typename Left::dimension, typename Right::dimension> {};

template<class Left, class Right>
constexpr same_dimension_result_t<Left, Right>
operator+(const Left& left, const Right& right) {
  return same_dimension_result_t<Left, Right>::from_canonical(
      left.canonical_value() + right.canonical_value());
}

template<class Left, class Right>
constexpr same_dimension_result_t<Left, Right>
operator-(const Left& left, const Right& right) {
  return same_dimension_result_t<Left, Right>::from_canonical(
      left.canonical_value() - right.canonical_value());
}

template<class Left, class Right>
constexpr dim_add_result_t<Left, Right>
operator*(const Left& left, const Right& right) {
  return dim_add_result_t<Left, Right>::from_canonical(
      left.canonical_value() * right.canonical_value());
}

template<class Left, class Right>
constexpr dim_sub_result_t<Left, Right>
operator/(const Left& left, const Right& right) {
  return dim_sub_result_t<Left, Right>::from_canonical(
      left.canonical_value() / right.canonical_value());
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
operator*(const Quantity& quantity, double scalar) {
  return Quantity::from_canonical(quantity.canonical_value() * scalar);
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
operator*(double scalar, const Quantity& quantity) {
  return Quantity::from_canonical(quantity.canonical_value() * scalar);
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
operator/(const Quantity& quantity, double scalar) {
  return Quantity::from_canonical(quantity.canonical_value() / scalar);
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
operator-(const Quantity& quantity) {
  return Quantity::from_canonical(-quantity.canonical_value());
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
operator+(const Quantity& quantity) {
  return quantity;
}

template<class Quantity>
constexpr inverse_quantity_result_t<Quantity>
operator/(double scalar, const Quantity& quantity) {
  return inverse_quantity_result_t<Quantity>::from_canonical(
      scalar / quantity.canonical_value());
}

template<class Left, class Right>
constexpr comparison_result_t<Left, Right>
operator==(const Left& left, const Right& right) {
  return left.canonical_value() == right.canonical_value();
}

template<class Left, class Right>
constexpr comparison_result_t<Left, Right>
operator!=(const Left& left, const Right& right) {
  return !(left == right);
}

template<class Left, class Right>
constexpr comparison_result_t<Left, Right>
operator<(const Left& left, const Right& right) {
  return left.canonical_value() < right.canonical_value();
}

template<class Left, class Right>
constexpr comparison_result_t<Left, Right>
operator<=(const Left& left, const Right& right) {
  return left.canonical_value() <= right.canonical_value();
}

template<class Left, class Right>
constexpr comparison_result_t<Left, Right>
operator>(const Left& left, const Right& right) {
  return left.canonical_value() > right.canonical_value();
}

template<class Left, class Right>
constexpr comparison_result_t<Left, Right>
operator>=(const Left& left, const Right& right) {
  return left.canonical_value() >= right.canonical_value();
}

}  // namespace units
