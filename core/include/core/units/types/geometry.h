#pragma once

#include "core/units/semantic_macros.h"

namespace units {

typedef dim<0, 2, 0, 0, 0, 0, 0, 0> dim_area;
typedef dim<0, 3, 0, 0, 0, 0, 0, 0> dim_volume;
typedef dim<0, 1, -1, 0, 0, 0, 0, 0> dim_velocity;
typedef dim<0, 1, -2, 0, 0, 0, 0, 0> dim_acceleration;
typedef dim<0, 1, -3, 0, 0, 0, 0, 0> dim_jerk;
typedef dim<0, 1, 1, 0, 0, 0, 0, 0> dim_absement;
typedef dim<0, 0, -1, 0, 0, 0, 0, 1> dim_angular_velocity;
typedef dim<0, 0, -2, 0, 0, 0, 0, 1> dim_angular_acceleration;
typedef dim<0, 0, -3, 0, 0, 0, 0, 1> dim_angular_jerk;
typedef dim<0, 0, 1, 0, 0, 0, 0, 1> dim_angular_absement;
typedef dim<0, -1, 0, 0, 0, 0, 0, 1> dim_curvature;
typedef dim<0, 0, -1, 0, 0, 0, 0, 0> dim_frequency;

// not happy with this organization really, I feel like a lot of this should be combine with mechanics probably?
#define UNITS_AREA_TABLE(X, context)                                                           \
  X(context, m2, square_meters, square_meter_tag, 1, 1)                                        \
  X(context, ft2, square_feet, square_foot_tag, 145161, 1562500)                               \
  X(context, yd2, square_yards, square_yard_tag, 10451592, 12500000)                           \
  X(context, acre, acres, acre_tag, 316160658, 78125)

#define UNITS_VOLUME_TABLE(X, context)                                                         \
  X(context, m3, cubic_meters, cubic_meter_tag, 1, 1)                                          \
  X(context, in3, cubic_inches, cubic_inch_tag, 2048383, 125000000000)                         \
  X(context, ft3, cubic_feet, cubic_foot_tag, 55306341, 1953125000)                            \
  X(context, liter, liters, liter_tag, 1, 1000)                                                    \
  X(context, gal, gallons, us_gallon_tag, 473176473, 125000000000)                             \
  X(context, qt, quarts, us_quart_tag, 473176473, 500000000000)                                \
  X(context, pt, pints, us_pint_tag, 473176473, 1000000000000)                                 \
  X(context, floz, fluid_ounces, us_fluid_ounce_tag, 473176473, 16000000000000)

#define UNITS_VELOCITY_TABLE(X, context)                                                       \
  X(context, mps, meters_per_second, meters_per_second_tag, 1, 1)                              \
  X(context, inps, inches_per_second, inches_per_second_tag, 127, 5000)                        \
  X(context, ftps, feet_per_second, feet_per_second_tag, 381, 1250)                            \
  X(context, mph, miles_per_hour, miles_per_hour_tag, 1397, 3125)

#define UNITS_ACCELERATION_TABLE(X, context)                                                   \
  X(context, mps2, meters_per_second_squared, meters_per_second_squared_tag, 1, 1)             \
  X(context, inps2, inches_per_second_squared, inches_per_second_squared_tag, 127, 5000)

#define UNITS_JERK_TABLE(X, context)                                                           \
  X(context, mps3, meters_per_second_cubed, meters_per_second_cubed_tag, 1, 1)                 \
  X(context, inps3, inches_per_second_cubed, inches_per_second_cubed_tag, 127, 5000)

#define UNITS_ABSEMENT_TABLE(X, context)                                                       \
  X(context, m_s, meter_seconds, meter_second_tag, 1, 1)                                       \
  X(context, in_s, inch_seconds, inch_second_tag, 127, 5000)

#define UNITS_ANGULAR_VELOCITY_TABLE(X, context)                                               \
  X(context, radps, radians_per_second, radians_per_second_tag, 1, 1)                          \
  X(context, degps, degrees_per_second, degrees_per_second_tag, 17453292519943295.0,           \
    1000000000000000000.0)                                                                     \
  X(context, rpm, revolutions_per_minute, revolutions_per_minute_tag, 10471975511965977.0,     \
    100000000000000000.0)

#define UNITS_ANGULAR_ACCELERATION_TABLE(X, context)                                           \
  X(context, radps2, radians_per_second_squared, radians_per_second_squared_tag, 1, 1)         \
  X(context, degps2, degrees_per_second_squared, degrees_per_second_squared_tag,                \
    17453292519943295.0, 1000000000000000000.0)

#define UNITS_ANGULAR_JERK_TABLE(X, context)                                                   \
  X(context, radps3, radians_per_second_cubed, radians_per_second_cubed_tag, 1, 1)             \
  X(context, degps3, degrees_per_second_cubed, degrees_per_second_cubed_tag,                    \
    17453292519943295.0, 1000000000000000000.0)

#define UNITS_ANGULAR_ABSEMENT_TABLE(X, context)                                               \
  X(context, rad_s, radian_seconds, radian_second_tag, 1, 1)                                   \
  X(context, deg_s, degree_seconds, degree_second_tag, 17453292519943295.0,                    \
    1000000000000000000.0)

#define UNITS_CURVATURE_TABLE(X, context)                                                      \
  X(context, radpm, radians_per_meter, radians_per_meter_tag, 1, 1)                            \
  X(context, degpm, degrees_per_meter, degrees_per_meter_tag, 17453292519943295.0,             \
    1000000000000000000.0)                                                                     \
  X(context, radpft, radians_per_foot, radians_per_foot_tag, 1250, 381)                        \
  X(context, degpft, degrees_per_foot, degrees_per_foot_tag, 21816615649929118.0,              \
    290322580645161280.0)

#define UNITS_FREQUENCY_TABLE(X, context)                                                      \
  X(context, Hz, hertz, hertz_tag, 1, 1)

UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_area, UNITS_AREA_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_volume, UNITS_VOLUME_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_velocity, UNITS_VELOCITY_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_acceleration, UNITS_ACCELERATION_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_jerk, UNITS_JERK_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_absement, UNITS_ABSEMENT_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_angular_velocity, UNITS_ANGULAR_VELOCITY_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_angular_acceleration, UNITS_ANGULAR_ACCELERATION_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_angular_jerk, UNITS_ANGULAR_JERK_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_angular_absement, UNITS_ANGULAR_ABSEMENT_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_curvature, UNITS_CURVATURE_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_frequency, UNITS_FREQUENCY_TABLE)

UNITS_DEFINE_SEMANTIC_QUANTITY(Area, dim_area, UNITS_AREA_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(Volume, dim_volume, UNITS_VOLUME_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(Velocity, dim_velocity, UNITS_VELOCITY_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(Absement, dim_absement, UNITS_ABSEMENT_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    Acceleration,
    dim_acceleration,
    UNITS_ACCELERATION_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(Jerk, dim_jerk, UNITS_JERK_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    AngularVelocity,
    dim_angular_velocity,
    UNITS_ANGULAR_VELOCITY_TABLE);
// I don't think this has an official name?
UNITS_DEFINE_SEMANTIC_QUANTITY(
    AngularAbsement,
    dim_angular_absement,
    UNITS_ANGULAR_ABSEMENT_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    AngularAcceleration,
    dim_angular_acceleration,
    UNITS_ANGULAR_ACCELERATION_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    AngularJerk,
    dim_angular_jerk,
    UNITS_ANGULAR_JERK_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(Curvature, dim_curvature, UNITS_CURVATURE_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(Frequency, dim_frequency, UNITS_FREQUENCY_TABLE);

UNITS_AREA_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_VOLUME_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_VELOCITY_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ABSEMENT_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ACCELERATION_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_JERK_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ANGULAR_VELOCITY_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ANGULAR_ABSEMENT_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ANGULAR_ACCELERATION_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ANGULAR_JERK_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_CURVATURE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_FREQUENCY_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)

UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Area, square_meter_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Volume, cubic_meter_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Velocity, meters_per_second_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Absement, meter_second_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Acceleration, meters_per_second_squared_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Jerk, meters_per_second_cubed_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(AngularVelocity, radians_per_second_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(AngularAbsement, radian_second_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(AngularAcceleration, radians_per_second_squared_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(AngularJerk, radians_per_second_cubed_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Curvature, radians_per_meter_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Frequency, hertz_tag)

namespace literals {
UNITS_DEFINE_TABLE_LITERALS(Area, UNITS_AREA_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Volume, UNITS_VOLUME_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Velocity, UNITS_VELOCITY_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Absement, UNITS_ABSEMENT_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Acceleration, UNITS_ACCELERATION_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Jerk, UNITS_JERK_TABLE)
UNITS_DEFINE_TABLE_LITERALS(AngularVelocity, UNITS_ANGULAR_VELOCITY_TABLE)
UNITS_DEFINE_TABLE_LITERALS(AngularAbsement, UNITS_ANGULAR_ABSEMENT_TABLE)
UNITS_DEFINE_TABLE_LITERALS(AngularAcceleration, UNITS_ANGULAR_ACCELERATION_TABLE)
UNITS_DEFINE_TABLE_LITERALS(AngularJerk, UNITS_ANGULAR_JERK_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Curvature, UNITS_CURVATURE_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Frequency, UNITS_FREQUENCY_TABLE)

}  // namespace literals

}  // namespace units
