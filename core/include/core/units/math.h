#pragma once

#include <type_traits>

#include "gcem.hpp"

#include "core/units/core/quantity_base.h"
#include "core/units/types/angle.h"
#include "core/units/types/control.h"
#include "core/units/types/electrical.h"
#include "core/units/types/geometry.h"
#include "core/units/types/length.h"
#include "core/units/types/mechanics.h"
#include "core/units/types/number.h"
#include "core/units/types/photometry.h"

namespace units {

inline constexpr Length arc_length(const Length& radius, const Angle& angle) {
  return Length::from_canonical(radius.m() * angle.rad());
}

inline constexpr Length turn_radius(const Curvature& curvature) {
  return Angle::from<radian_tag>(1.0) / curvature;
}

inline constexpr Curvature curvature_from_radius(const Length& radius) {
  return Angle::from<radian_tag>(1.0) / radius;
}

inline constexpr bool is_left_turn(const Curvature& curvature) {
  return curvature.radpm() > 0.0;
}

inline constexpr bool is_right_turn(const Curvature& curvature) {
  return curvature.radpm() < 0.0;
}



// maybe don't use number here?/
inline constexpr Number sin(const Angle& angle) {
  return Number(gcem::sin(angle.rad()));
}

inline constexpr Number cos(const Angle& angle) {
  return Number(gcem::cos(angle.rad()));
}

inline constexpr Number tan(const Angle& angle) {
  return Number(gcem::tan(angle.rad()));
}

inline constexpr Angle asin(const Number& value) {
  return Angle(gcem::asin(value.value()));
}

inline constexpr Angle acos(const Number& value) {
  return Angle(gcem::acos(value.value()));
}

inline constexpr Angle atan(const Number& value) {
  return Angle(gcem::atan(value.value()));
}

inline constexpr Angle atan2(const Number& y, const Number& x) {
  return Angle(gcem::atan2(y.value(), x.value()));
}

// want both
inline constexpr Angle atan2(const Length& y, const Length& x) {
  return Angle(gcem::atan2(y.m(), x.m()));
}


// raise to integer power
template<int Factor, class Dim>
struct dim_scale {
  typedef dim<
      Dim::mass * Factor,
      Dim::length * Factor,
      Dim::time * Factor,
      Dim::current * Factor,
      Dim::temperature * Factor,
      Dim::amount * Factor,
      Dim::luminous_intensity * Factor,
      Dim::angle * Factor> type;
};

// no floating point powers is steamsad but uhhhhhhhhh
template<class Dim, int Divisor>
struct dim_divisible_by
    : std::integral_constant<
          bool,
          Dim::mass % Divisor == 0 && Dim::length % Divisor == 0 &&
              Dim::time % Divisor == 0 && Dim::current % Divisor == 0 &&
              Dim::temperature % Divisor == 0 && Dim::amount % Divisor == 0 &&
              Dim::luminous_intensity % Divisor == 0 &&
              Dim::angle % Divisor == 0> {};

template<class Dim, int Divisor>
struct dim_divide {
  typedef dim<
      Dim::mass / Divisor,
      Dim::length / Divisor,
      Dim::time / Divisor,
      Dim::current / Divisor,
      Dim::temperature / Divisor,
      Dim::amount / Divisor,
      Dim::luminous_intensity / Divisor,
      Dim::angle / Divisor> type;
};

template<bool Condition, class Type>
using enable_if_t = typename std::enable_if<Condition, Type>::type;

template<class Quantity, int Factor>
using scaled_dimension_t =
    typename dim_scale<Factor, typename Quantity::dimension>::type;

template<class Quantity, int Divisor>
using divided_dimension_t =
    typename dim_divide<typename Quantity::dimension, Divisor>::type;

template<class Quantity, int Factor>
using scaled_quantity_t =
    typename quantity_for_dimension<scaled_dimension_t<Quantity, Factor> >::type;

template<class Quantity, int Divisor>
using divided_quantity_t =
    typename quantity_for_dimension<divided_dimension_t<Quantity, Divisor> >::type;

template<class Quantity, int Factor, bool = is_quantity<Quantity>::value>
struct can_scale_quantity_impl : std::false_type {};

template<class Quantity, int Factor>
struct can_scale_quantity_impl<Quantity, Factor, true>
    : std::integral_constant<
          bool,
          has_quantity_for_dimension<scaled_dimension_t<Quantity, Factor> >::value> {};

template<class Quantity, int Factor, bool = is_quantity<Quantity>::value>
struct can_root_quantity_impl : std::false_type {};

template<class Quantity, int Divisor>
struct can_root_quantity_impl<Quantity, Divisor, true>
    : std::integral_constant<
          bool,
          dim_divisible_by<typename Quantity::dimension, Divisor>::value> {};

template<class Quantity, int Factor>
struct can_scale_quantity
    : can_scale_quantity_impl<Quantity, Factor, is_quantity<Quantity>::value> {};

template<class Quantity, int Divisor>
struct can_root_quantity
    : can_root_quantity_impl<Quantity, Divisor, is_quantity<Quantity>::value> {};

template<class Quantity>
using quantity_result_t = enable_if_t<is_quantity<Quantity>::value, Quantity>;

template<class Quantity>
using quantity_bool_result_t = enable_if_t<is_quantity<Quantity>::value, bool>;

template<class Quantity, int Power>
using power_result_t =
    enable_if_t<can_scale_quantity<Quantity, Power>::value, scaled_quantity_t<Quantity, Power> >;

template<class Quantity, int Divisor>
using root_result_t =
    enable_if_t<can_root_quantity<Quantity, Divisor>::value, divided_quantity_t<Quantity, Divisor> >;

// returns the same type
template<class Quantity>
constexpr quantity_result_t<Quantity>
abs(const Quantity& quantity) {
  return Quantity::from_canonical(gcem::abs(quantity.canonical_value()));
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
min(const Quantity& left, const Quantity& right) {
  return left < right ? left : right;
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
max(const Quantity& left, const Quantity& right) {
  return left < right ? right : left;
}

inline constexpr Number abs(const Number& value) {
  return Number(gcem::abs(value.value()));
}

inline constexpr Number min(const Number& left, const Number& right) {
  return left.value() < right.value() ? left : right;
}

inline constexpr Number max(const Number& left, const Number& right) {
  return left.value() < right.value() ? right : left;
}

template<class Quantity>
inline constexpr Number sgn(const Quantity& quantity) {
  return Number((quantity.canonical_value() > 0.0) - (quantity.canonical_value() < 0.0));
}

inline constexpr Number sgn(const Number& value) {
  return Number((value.value() > 0.0) - (value.value() < 0.0));
}



// NOT the same type!!
template<int Power, class Quantity>
constexpr power_result_t<Quantity, Power>
pow(const Quantity& quantity) {
  return scaled_quantity_t<Quantity, Power>::from_canonical(
      gcem::pow(quantity.canonical_value(), Power));
}

template<int Power>
inline constexpr Number pow(const Number& value) {
  return Number(gcem::pow(value.value(), Power));
}

template<typename Root, class Quantity>
constexpr root_result_t<Quantity, Root::value>
root(const Quantity& quantity) {
  return divided_quantity_t<Quantity, Root::value>::from_canonical(
      gcem::pow(quantity.canonical_value(), 1.0 / Root::value));
}

template<typename Root>
inline constexpr Number root(const Number& value) {
  return Number(gcem::pow(value.value(), 1.0 / Root::value));
}

template<class Quantity>
constexpr root_result_t<Quantity, 2>
sqrt(const Quantity& quantity) {
  return root<std::integral_constant<int, 2> >(quantity);
}

inline constexpr Number sqrt(const Number& value) {
  return root<std::integral_constant<int, 2> >(value);
}

template<class Quantity>
constexpr root_result_t<Quantity, 3>
cbrt(const Quantity& quantity) {
  return root<std::integral_constant<int, 3> >(quantity);
}

inline constexpr Number cbrt(const Number& value) {
  return root<std::integral_constant<int, 3> >(value);
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
hypot(const Quantity& left, const Quantity& right) {
  return Quantity::from_canonical(
      gcem::hypot(left.canonical_value(), right.canonical_value()));
}

inline constexpr Number hypot(const Number& left, const Number& right) {
  return Number(gcem::hypot(left.value(), right.value()));
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
mod(const Quantity& left, const Quantity& right) {
  return Quantity::from_canonical(
      gcem::fmod(left.canonical_value(), right.canonical_value()));
}

inline constexpr Number mod(const Number& left, const Number& right) {
  return Number(gcem::fmod(left.value(), right.value()));
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
remainder(const Quantity& left, const Quantity& right) {
  const double quotient = left.canonical_value() / right.canonical_value();
  return Quantity::from_canonical(
      left.canonical_value() - gcem::round(quotient) * right.canonical_value());
}

inline constexpr Number remainder(const Number& left, const Number& right) {
  const double quotient = left.value() / right.value();
  return Number(left.value() - gcem::round(quotient) * right.value());
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
copysign(const Quantity& left, const Quantity& right) {
  return Quantity::from_canonical(
      gcem::copysign(left.canonical_value(), right.canonical_value()));
}

inline constexpr Number copysign(const Number& left, const Number& right) {
  return Number(gcem::copysign(left.value(), right.value()));
}

template<class Quantity>
constexpr quantity_bool_result_t<Quantity>
signbit(const Quantity& quantity) {
  return gcem::signbit(quantity.canonical_value());
}

inline constexpr bool signbit(const Number& value) {
  return gcem::signbit(value.value());
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
clamp(const Quantity& value, const Quantity& low, const Quantity& high) {
  return max(low, min(value, high));
}

inline constexpr Number clamp(const Number& value, const Number& low, const Number& high) {
  return max(low, min(value, high));
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
ceil(const Quantity& value, const Quantity& step) {
  return Quantity::from_canonical(
      gcem::ceil(value.canonical_value() / step.canonical_value()) *
      step.canonical_value());
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
floor(const Quantity& value, const Quantity& step) {
  return Quantity::from_canonical(
      gcem::floor(value.canonical_value() / step.canonical_value()) *
      step.canonical_value());
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
trunc(const Quantity& value, const Quantity& step) {
  return Quantity::from_canonical(
      gcem::trunc(value.canonical_value() / step.canonical_value()) *
      step.canonical_value());
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
round(const Quantity& value, const Quantity& step) {
  return Quantity::from_canonical(
      gcem::round(value.canonical_value() / step.canonical_value()) *
      step.canonical_value());
}

inline constexpr Number ceil(const Number& value, const Number& step) {
  return Number(gcem::ceil(value.value() / step.value()) * step.value());
}

inline constexpr Number floor(const Number& value, const Number& step) {
  return Number(gcem::floor(value.value() / step.value()) * step.value());
}

inline constexpr Number trunc(const Number& value, const Number& step) {
  return Number(gcem::trunc(value.value() / step.value()) * step.value());
}

inline constexpr Number round(const Number& value, const Number& step) {
  return Number(gcem::round(value.value() / step.value()) * step.value());
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
fma(const Quantity& x, double y, const Quantity& z) {
  return Quantity::from_canonical(x.canonical_value() * y + z.canonical_value());
}

template<class Quantity>
constexpr quantity_result_t<Quantity>
fma(double x, const Quantity& y, const Quantity& z) {
  return Quantity::from_canonical(x * y.canonical_value() + z.canonical_value());
}

inline constexpr Number fma(const Number& x, const Number& y, const Number& z) {
  return Number(x.value() * y.value() + z.value());
}

template<class Quantity>
inline constexpr power_result_t<Quantity, 2>
square(const Quantity& quantity) {
  return pow<2>(quantity);
}

inline constexpr Number square(const Number& value) {
  return pow<2>(value);
}

template<class Quantity>
inline constexpr power_result_t<Quantity, 3>
cube(const Quantity& quantity) {
  return pow<3>(quantity);
}

inline constexpr Number cube(const Number& value) {
  return pow<3>(value);
}

}  // namespace units
