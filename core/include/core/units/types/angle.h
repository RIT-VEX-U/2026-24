#pragma once

#include "gcem.hpp"

#include "core/units/semantic_macros.h"

namespace units {

typedef dim<0, 0, 0, 0, 0, 0, 0, 1> dim_angle;

// this is egregious and I don't like it, but I couldn't think of a godo way to integrate Rotation2d.......
#define UNITS_ANGLE_TABLE(X, context)                                                          \
  X(context, rad, radians, radian_tag, 1, 1)                                                   \
  X(context, deg, degrees, degree_tag, 17453292519943295.0, 1000000000000000000.0)            \
  X(context, rev, revolutions, revolution_tag, 6283185307179586.0, 1000000000000000.0)

UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_angle, UNITS_ANGLE_TABLE);



class Angle : public quantity_base<Angle, dim_angle> {
 public:
  typedef quantity_base<Angle, dim_angle> base_type;

  // M_PI exists in vex but not my server? somehow
  static constexpr double pi() { return 3.14159265358979323846; }

  // I should hagve done this with rotation2d perchance, thanks chat
  static constexpr double normalization_epsilon() { return 1e-12; }

  constexpr explicit Angle(double radians = 0.0) : base_type(radians) {}

  static constexpr Angle from_canonical(double value) { return Angle(value); }

  template<class UnitTag>
  static constexpr Angle from(double value) {
    return Angle(base_type::template to_canonical<UnitTag>(value));
  }

#define UNITS_DECLARE_ANGLE_GETTER(context, short_name, long_name, tag_name, ...)              \
  constexpr double short_name() const { return this->template as<tag_name>(); }                \
  constexpr double long_name() const { return this->template as<tag_name>(); }

  UNITS_ANGLE_TABLE(UNITS_DECLARE_ANGLE_GETTER, _);

#undef UNITS_DECLARE_ANGLE_GETTER

  constexpr Angle normalized_0_to_2pi() const {
    const double two_pi = 2.0 * pi();
    const double wrapped = gcem::fmod(rad(), two_pi);
    const double normalized = wrapped < 0.0 ? wrapped + two_pi : wrapped;
    return Angle(gcem::abs(normalized - two_pi) <= normalization_epsilon() ||
                         gcem::abs(normalized) <= normalization_epsilon()
                     ? 0.0
                     : normalized);
  }

  constexpr Angle normalized_minus_pi_to_pi() const {
    const double two_pi = 2.0 * pi();
    const double wrapped = gcem::fmod(rad() + pi(), two_pi);
    const double shifted = wrapped < 0.0 ? wrapped + two_pi : wrapped;
    const double normalized = shifted - pi();
    return Angle(gcem::abs(normalized) <= normalization_epsilon() ? 0.0
                                                                   : normalized);
  }

  constexpr Angle normalized_0_to_360() const {
    return normalized_0_to_2pi();
  }

  constexpr Angle normalized_minus_180_to_180() const {
    return normalized_minus_pi_to_pi();
  }
};

template<>
struct quantity_for_dimension<dim_angle> {
  typedef Angle type;
};

UNITS_ANGLE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Angle, radian_tag)

namespace literals {
UNITS_DEFINE_TABLE_LITERALS(Angle, UNITS_ANGLE_TABLE)

}  // namespace literals

inline constexpr Angle shortest_angular_distance(const Angle& from, const Angle& to) {
  return (to - from).normalized_minus_pi_to_pi();
}

}  // namespace units
