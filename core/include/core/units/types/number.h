#pragma once

#include <type_traits>

#include "core/units/core/quantity_base.h"

namespace units {

  // this only exists for the type system to be chill
class Number : public quantity_base<Number, dim_none> {
 public:
  typedef quantity_base<Number, dim_none> base_type;

  constexpr explicit Number(double value = 0.0) : base_type(value) {}

  static constexpr Number from_canonical(double value) { return Number(value); }

  /** Explicit scalar escape hatch. */
  constexpr double value() const { return canonical_value(); }
};

template<>
struct quantity_for_dimension<dim_none> {
  typedef Number type;
};

template<class Scalar>
using number_arithmetic_enable_t =
    typename std::enable_if<std::is_arithmetic<Scalar>::value, Number>::type;

template<class Scalar>
constexpr number_arithmetic_enable_t<Scalar>
operator+(const Number& left, Scalar right) {
  return Number::from_canonical(left.value() + static_cast<double>(right));
}

template<class Scalar>
constexpr number_arithmetic_enable_t<Scalar>
operator+(Scalar left, const Number& right) {
  return Number::from_canonical(static_cast<double>(left) + right.value());
}

template<class Scalar>
constexpr number_arithmetic_enable_t<Scalar>
operator-(const Number& left, Scalar right) {
  return Number::from_canonical(left.value() - static_cast<double>(right));
}

template<class Scalar>
constexpr number_arithmetic_enable_t<Scalar>
operator-(Scalar left, const Number& right) {
  return Number::from_canonical(static_cast<double>(left) - right.value());
}

}  // namespace units
