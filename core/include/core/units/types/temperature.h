#pragma once

#include "core/units/semantic_macros.h"

namespace units {

typedef dim<0, 0, 0, 0, 1, 0, 0, 0> dim_temperature;

// this one sucks cuz they're not multiplicable?
class TemperatureDelta : public quantity_base<TemperatureDelta, dim_temperature> {
 public:
  typedef quantity_base<TemperatureDelta, dim_temperature> base_type;

  constexpr explicit TemperatureDelta(double kelvin_delta = 0.0)
      : base_type(kelvin_delta) {}

  static constexpr TemperatureDelta from_canonical(double value) {
    return TemperatureDelta(value);
  }

  static constexpr TemperatureDelta from_K(double value) {
    return TemperatureDelta(value);
  }

  static constexpr TemperatureDelta from_degC(double value) {
    return TemperatureDelta(value);
  }

  static constexpr TemperatureDelta from_degF(double value) {
    return TemperatureDelta(value * 5.0 / 9.0);
  }

  constexpr double K() const { return canonical_value(); }
  constexpr double kelvin() const { return canonical_value(); }
  constexpr double degC() const { return canonical_value(); }
  constexpr double celsius() const { return canonical_value(); }
  constexpr double degF() const { return canonical_value() * 9.0 / 5.0; }
  constexpr double fahrenheit() const { return canonical_value() * 9.0 / 5.0; }
};

// this one's real (kelvin)
class Temperature {
 public:
  constexpr explicit Temperature(double kelvin = 0.0) : kelvin_(kelvin) {}

  static constexpr Temperature from_kelvin(double value) {
    return Temperature(value);
  }

  static constexpr Temperature from_celsius(double value) {
    return Temperature(value + 273.15);
  }

  static constexpr Temperature from_fahrenheit(double value) {
    return Temperature((value + 459.67) * 5.0 / 9.0);
  }

  static constexpr Temperature from_canonical(double value) {
    return Temperature(value);
  }

  constexpr double canonical_value() const { return kelvin_; }
  constexpr double K() const { return kelvin_; }
  constexpr double kelvin() const { return kelvin_; }
  constexpr double degC() const { return kelvin_ - 273.15; }
  constexpr double celsius() const { return kelvin_ - 273.15; }
  constexpr double degF() const { return kelvin_ * 9.0 / 5.0 - 459.67; }
  constexpr double fahrenheit() const { return kelvin_ * 9.0 / 5.0 - 459.67; }

 private:
  double kelvin_;
};

inline constexpr TemperatureDelta operator-(const Temperature& left,
                                            const Temperature& right) {
  return TemperatureDelta::from_canonical(
      left.canonical_value() - right.canonical_value());
}

inline constexpr Temperature operator+(const Temperature& temperature,
                                       const TemperatureDelta& delta) {
  return Temperature::from_canonical(
      temperature.canonical_value() + delta.canonical_value());
}

inline constexpr Temperature operator+(const TemperatureDelta& delta,
                                       const Temperature& temperature) {
  return temperature + delta;
}

inline constexpr Temperature operator-(const Temperature& temperature,
                                       const TemperatureDelta& delta) {
  return Temperature::from_canonical(
      temperature.canonical_value() - delta.canonical_value());
}

inline constexpr bool operator==(const Temperature& left, const Temperature& right) {
  return left.canonical_value() == right.canonical_value();
}

inline constexpr bool operator!=(const Temperature& left, const Temperature& right) {
  return !(left == right);
}

inline constexpr bool operator<(const Temperature& left, const Temperature& right) {
  return left.canonical_value() < right.canonical_value();
}

inline constexpr bool operator<=(const Temperature& left, const Temperature& right) {
  return left.canonical_value() <= right.canonical_value();
}

inline constexpr bool operator>(const Temperature& left, const Temperature& right) {
  return left.canonical_value() > right.canonical_value();
}

inline constexpr bool operator>=(const Temperature& left, const Temperature& right) {
  return left.canonical_value() >= right.canonical_value();
}

template<>
struct quantity_for_dimension<dim_temperature> {
  typedef TemperatureDelta type;
};

template<>
struct unit_text<kelvin_tag> {
  static const char* symbol() { return "K"; }
  static const char* name() { return "kelvin"; }
};

template<>
struct unit_text<celsius_tag> {
  static const char* symbol() { return "degC"; }
  static const char* name() { return "degrees_celsius"; }
};

template<>
struct unit_text<fahrenheit_tag> {
  static const char* symbol() { return "degF"; }
  static const char* name() { return "degrees_fahrenheit"; }
};

UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Temperature, kelvin_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(TemperatureDelta, kelvin_tag)

namespace literals {

UNITS_DEFINE_LITERAL_PAIR(_K, Temperature::from_kelvin(static_cast<double>(value)));
UNITS_DEFINE_LITERAL_PAIR(_degC, Temperature::from_celsius(static_cast<double>(value)));
UNITS_DEFINE_LITERAL_PAIR(_degF, Temperature::from_fahrenheit(static_cast<double>(value)));
UNITS_DEFINE_LITERAL_PAIR(_K_delta, TemperatureDelta::from_K(static_cast<double>(value)));
UNITS_DEFINE_LITERAL_PAIR(_degC_delta, TemperatureDelta::from_degC(static_cast<double>(value)));
UNITS_DEFINE_LITERAL_PAIR(_degF_delta, TemperatureDelta::from_degF(static_cast<double>(value)));

}  // namespace literals

}  // namespace units
