#pragma once

#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "core/units/core/format_traits.h"
#include "core/units/types/angle.h"
#include "core/units/types/amount.h"
#include "core/units/types/control.h"
#include "core/units/types/current.h"
#include "core/units/types/electrical.h"
#include "core/units/types/geometry.h"
#include "core/units/types/length.h"
#include "core/units/types/luminous_intensity.h"
#include "core/units/types/mass.h"
#include "core/units/types/mechanics.h"
#include "core/units/types/number.h"
#include "core/units/types/photometry.h"
#include "core/units/types/temperature.h"
#include "core/units/types/time.h"

namespace units {

inline std::string humanize_identifier(const char* text) {
  std::string result(text);
  for (std::string::size_type i = 0; i < result.size(); ++i) {
    if (result[i] == '_') {
      result[i] = ' ';
    }
  }
  return result;
}

template<class UnitTag, class Quantity>
inline typename std::enable_if<is_quantity<Quantity>::value, double>::type
unit_value_for_text(const Quantity& quantity) {
  return quantity.template as<UnitTag>();
}

template<class UnitTag>
struct text_temperature_value;

template<>
struct text_temperature_value<kelvin_tag> {
  static double get(const Temperature& temperature) { return temperature.K(); }
  static double get(const TemperatureDelta& temperature) { return temperature.K(); }
};

template<>
struct text_temperature_value<celsius_tag> {
  static double get(const Temperature& temperature) { return temperature.degC(); }
  static double get(const TemperatureDelta& temperature) { return temperature.degC(); }
};

template<>
struct text_temperature_value<fahrenheit_tag> {
  static double get(const Temperature& temperature) { return temperature.degF(); }
  static double get(const TemperatureDelta& temperature) { return temperature.degF(); }
};

template<class UnitTag>
inline double unit_value_for_text(const Temperature& temperature) {
  return text_temperature_value<UnitTag>::get(temperature);
}

template<class UnitTag>
inline double unit_value_for_text(const TemperatureDelta& temperature) {
  return text_temperature_value<UnitTag>::get(temperature);
}

template<class Quantity>
inline std::string format_with_default_unit(const Quantity& quantity) {
  typedef typename default_unit_for_quantity<Quantity>::type unit_tag;
  std::ostringstream stream;
  stream << unit_value_for_text<unit_tag>(quantity) << ' '
         << unit_text<unit_tag>::symbol();
  return stream.str();
}

template<class UnitTag>
inline const char* symbol() {
  return unit_text<UnitTag>::symbol();
}

template<class UnitTag>
inline std::string display_name() {
  return humanize_identifier(unit_text<UnitTag>::name());
}

template<class UnitTag, class Quantity>
inline std::string to_string_as(const Quantity& quantity) {
  std::ostringstream stream;
  stream << unit_value_for_text<UnitTag>(quantity) << ' ' << symbol<UnitTag>();
  return stream.str();
}

inline std::string to_string(const Number& number) {
  std::ostringstream stream;
  stream << number.value();
  return stream.str();
}

template<class Quantity>
inline typename std::enable_if<is_quantity<Quantity>::value &&
                                   !std::is_same<Quantity, Number>::value,
                               std::string>::type
to_string(const Quantity& quantity) {
  return format_with_default_unit(quantity);
}

inline std::string to_string(const Temperature& quantity) {
  return format_with_default_unit(quantity);
}

inline std::string to_string(const TemperatureDelta& quantity) {
  return format_with_default_unit(quantity);
}

inline std::ostream& operator<<(std::ostream& stream, const Number& number) {
  stream << number.value();
  return stream;
}

template<class Quantity>
inline typename std::enable_if<is_quantity<Quantity>::value &&
                                   !std::is_same<Quantity, Number>::value,
                               std::ostream&>::type
operator<<(std::ostream& stream, const Quantity& quantity) {
  stream << to_string(quantity);
  return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const Temperature& quantity) {
  stream << to_string(quantity);
  return stream;
}

inline std::ostream& operator<<(std::ostream& stream,
                                const TemperatureDelta& quantity) {
  stream << to_string(quantity);
  return stream;
}

template<>
struct default_unit_for_quantity<Number> {
  typedef void type;
};

}  // namespace units
