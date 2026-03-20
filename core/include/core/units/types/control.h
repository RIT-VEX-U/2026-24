#pragma once

#include "core/units/semantic_macros.h"

namespace units {

typedef dim<1, 1, -2, -1, 0, 0, 0, 0> dim_linear_velocity_feedforward;
typedef dim<1, 1, -1, -1, 0, 0, 0, 0> dim_linear_acceleration_feedforward;
typedef dim<1, 1, -3, -1, 0, 0, 0, 0> dim_linear_proportional_gain;
typedef dim<1, 1, -4, -1, 0, 0, 0, 0> dim_linear_integral_gain;
typedef dim<1, 1, -2, -1, 0, 0, 0, 0> dim_linear_derivative_gain;
typedef dim<1, 1, -1, -1, 0, 0, 0, 0> dim_linear_second_derivative_gain;
typedef dim<-1, -2, 2, 1, 0, 0, 0, 1> dim_angular_velocity_feedforward;
typedef dim<-1, -2, 1, 1, 0, 0, 0, 1> dim_angular_acceleration_feedforward;
typedef dim<1, 2, -3, -1, 0, 0, 0, -1> dim_angular_proportional_gain;
typedef dim<1, 2, -4, -1, 0, 0, 0, -1> dim_angular_integral_gain;
typedef dim<1, 2, -2, -1, 0, 0, 0, -1> dim_angular_derivative_gain;
typedef dim<1, 2, -1, -1, 0, 0, 0, -1> dim_angular_second_derivative_gain;


#define UNITS_LINEAR_VELOCITY_FEEDFORWARD_TABLE(X, context)                                    \
  X(context, VpMps, volts_per_meter_per_second, volts_per_meter_per_second_tag, 1, 1)         \
  X(context, VpInps, volts_per_inch_per_second, volts_per_inch_per_second_tag, 5000, 127)

#define UNITS_LINEAR_ACCELERATION_FEEDFORWARD_TABLE(X, context)                                \
  X(context, VpMps2, volts_per_meter_per_second_squared,                                       \
    volts_per_meter_per_second_squared_tag, 1, 1)                                              \
  X(context, VpInps2, volts_per_inch_per_second_squared,                                       \
    volts_per_inch_per_second_squared_tag, 5000, 127)

#define UNITS_LINEAR_PROPORTIONAL_GAIN_TABLE(X, context)                                       \
  X(context, VpM, volts_per_meter, volts_per_meter_tag, 1, 1)                                  \
  X(context, VpIn, volts_per_inch, volts_per_inch_tag, 5000, 127)

#define UNITS_LINEAR_INTEGRAL_GAIN_TABLE(X, context)                                           \
  X(context, VpMS, volts_per_meter_second, volts_per_meter_second_tag, 1, 1)                   \
  X(context, VpInS, volts_per_inch_second, volts_per_inch_second_tag, 5000, 127)

#define UNITS_LINEAR_DERIVATIVE_GAIN_TABLE(X, context)                                         \
  X(context, VspM, volt_seconds_per_meter, volt_seconds_per_meter_tag, 1, 1)                   \
  X(context, VspIn, volt_seconds_per_inch, volt_seconds_per_inch_tag, 5000, 127)

#define UNITS_LINEAR_SECOND_DERIVATIVE_GAIN_TABLE(X, context)                                  \
  X(context, Vs2pM, volt_seconds_squared_per_meter,                                            \
    volt_seconds_squared_per_meter_tag, 1, 1)                                                  \
  X(context, Vs2pIn, volt_seconds_squared_per_inch,                                            \
    volt_seconds_squared_per_inch_tag, 5000, 127)

#define UNITS_ANGULAR_VELOCITY_FEEDFORWARD_TABLE(X, context)                                   \
  X(context, radpspV, radians_per_second_per_volt, radians_per_second_per_volt_tag, 1, 1)      \
  X(context, degpspV, degrees_per_second_per_volt, degrees_per_second_per_volt_tag,            \
    17453292519943295.0, 1000000000000000000.0)

#define UNITS_ANGULAR_ACCELERATION_FEEDFORWARD_TABLE(X, context)                               \
  X(context, radps2pV, radians_per_second_squared_per_volt,                                    \
    radians_per_second_squared_per_volt_tag,                                                   \
    1, 1)                                                                                      \
  X(context, degps2pV, degrees_per_second_squared_per_volt,                                    \
    degrees_per_second_squared_per_volt_tag,                                                   \
    17453292519943295.0, 1000000000000000000.0)

#define UNITS_ANGULAR_PROPORTIONAL_GAIN_TABLE(X, context)                                      \
  X(context, VpRad, volts_per_radian, volts_per_radian_tag, 1, 1)                              \
  X(context, VpDeg, volts_per_degree, volts_per_degree_tag, 5729577951308232.0,               \
    100000000000000.0)

#define UNITS_ANGULAR_INTEGRAL_GAIN_TABLE(X, context)                                          \
  X(context, VpRadS, volts_per_radian_second, volts_per_radian_second_tag, 1, 1)               \
  X(context, VpDegS, volts_per_degree_second, volts_per_degree_second_tag, 5729577951308232.0, \
    100000000000000.0)

#define UNITS_ANGULAR_DERIVATIVE_GAIN_TABLE(X, context)                                        \
  X(context, VspRad, volt_seconds_per_radian, volt_seconds_per_radian_tag, 1, 1)               \
  X(context, VspDeg, volt_seconds_per_degree, volt_seconds_per_degree_tag, 5729577951308232.0, \
    100000000000000.0)

#define UNITS_ANGULAR_SECOND_DERIVATIVE_GAIN_TABLE(X, context)                                 \
  X(context, Vs2pRad, volt_seconds_squared_per_radian,                                         \
    volt_seconds_squared_per_radian_tag, 1, 1)                                                 \
  X(context, Vs2pDeg, volt_seconds_squared_per_degree, volt_seconds_squared_per_degree_tag,     \
    5729577951308232.0, 100000000000000.0)

UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_linear_velocity_feedforward,
    UNITS_LINEAR_VELOCITY_FEEDFORWARD_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_linear_acceleration_feedforward,
    UNITS_LINEAR_ACCELERATION_FEEDFORWARD_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_linear_proportional_gain,
    UNITS_LINEAR_PROPORTIONAL_GAIN_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_linear_integral_gain,
    UNITS_LINEAR_INTEGRAL_GAIN_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_linear_derivative_gain,
    UNITS_LINEAR_DERIVATIVE_GAIN_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_linear_second_derivative_gain,
    UNITS_LINEAR_SECOND_DERIVATIVE_GAIN_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_angular_velocity_feedforward,
    UNITS_ANGULAR_VELOCITY_FEEDFORWARD_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_angular_acceleration_feedforward,
    UNITS_ANGULAR_ACCELERATION_FEEDFORWARD_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_angular_proportional_gain,
    UNITS_ANGULAR_PROPORTIONAL_GAIN_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_angular_integral_gain,
    UNITS_ANGULAR_INTEGRAL_GAIN_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_angular_derivative_gain,
    UNITS_ANGULAR_DERIVATIVE_GAIN_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_angular_second_derivative_gain,
    UNITS_ANGULAR_SECOND_DERIVATIVE_GAIN_TABLE)

UNITS_DEFINE_SEMANTIC_ALIAS(
    LinearVelocityFeedforward,
    dim_linear_velocity_feedforward,
    UNITS_LINEAR_VELOCITY_FEEDFORWARD_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    LinearProportionalGain,
    dim_linear_proportional_gain,
    UNITS_LINEAR_PROPORTIONAL_GAIN_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    LinearIntegralGain,
    dim_linear_integral_gain,
    UNITS_LINEAR_INTEGRAL_GAIN_TABLE);
UNITS_DEFINE_SEMANTIC_ALIAS(
    LinearAccelerationFeedforward,
    dim_linear_acceleration_feedforward,
    UNITS_LINEAR_ACCELERATION_FEEDFORWARD_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    LinearDerivativeGain,
    dim_linear_derivative_gain,
    UNITS_LINEAR_DERIVATIVE_GAIN_TABLE);
// unfortunately this is the same thing 
UNITS_DEFINE_SEMANTIC_ALIAS(
    LinearVelocityProportionalGain,
    dim_linear_derivative_gain,
    UNITS_LINEAR_DERIVATIVE_GAIN_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    LinearVelocityDerivativeGain,
    dim_linear_second_derivative_gain,
    UNITS_LINEAR_SECOND_DERIVATIVE_GAIN_TABLE);
// again, this blows
UNITS_DEFINE_SEMANTIC_ALIAS(
    LinearVelocityIntegralGain,
    dim_linear_proportional_gain,
    UNITS_LINEAR_PROPORTIONAL_GAIN_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    AngularVelocityFeedforward,
    dim_angular_velocity_feedforward,
    UNITS_ANGULAR_VELOCITY_FEEDFORWARD_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    AngularProportionalGain,
    dim_angular_proportional_gain,
    UNITS_ANGULAR_PROPORTIONAL_GAIN_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    AngularIntegralGain,
    dim_angular_integral_gain,
    UNITS_ANGULAR_INTEGRAL_GAIN_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    AngularAccelerationFeedforward,
    dim_angular_acceleration_feedforward,
    UNITS_ANGULAR_ACCELERATION_FEEDFORWARD_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    AngularDerivativeGain,
    dim_angular_derivative_gain,
    UNITS_ANGULAR_DERIVATIVE_GAIN_TABLE);
// please
UNITS_DEFINE_SEMANTIC_ALIAS(
    AngularVelocityProportionalGain,
    dim_angular_derivative_gain,
    UNITS_ANGULAR_DERIVATIVE_GAIN_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    AngularVelocityDerivativeGain,
    dim_angular_second_derivative_gain,
    UNITS_ANGULAR_SECOND_DERIVATIVE_GAIN_TABLE);
// I beg you
UNITS_DEFINE_SEMANTIC_ALIAS(
    AngularVelocityIntegralGain,
    dim_angular_proportional_gain,
    UNITS_ANGULAR_PROPORTIONAL_GAIN_TABLE);

UNITS_LINEAR_VELOCITY_FEEDFORWARD_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_LINEAR_PROPORTIONAL_GAIN_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_LINEAR_INTEGRAL_GAIN_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_LINEAR_ACCELERATION_FEEDFORWARD_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_LINEAR_DERIVATIVE_GAIN_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_LINEAR_SECOND_DERIVATIVE_GAIN_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ANGULAR_VELOCITY_FEEDFORWARD_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ANGULAR_PROPORTIONAL_GAIN_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ANGULAR_INTEGRAL_GAIN_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ANGULAR_ACCELERATION_FEEDFORWARD_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ANGULAR_DERIVATIVE_GAIN_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ANGULAR_SECOND_DERIVATIVE_GAIN_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)

UNITS_DEFINE_DEFAULT_UNIT_TRAIT(LinearVelocityFeedforward, volts_per_meter_per_second_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(LinearProportionalGain, volts_per_meter_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(LinearIntegralGain, volts_per_meter_second_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(
    LinearAccelerationFeedforward,
    volts_per_meter_per_second_squared_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(LinearDerivativeGain, volt_seconds_per_meter_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(LinearVelocityProportionalGain, volt_seconds_per_meter_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(
    LinearVelocityDerivativeGain,
    volt_seconds_squared_per_meter_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(LinearVelocityIntegralGain, volts_per_meter_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(
    AngularVelocityFeedforward,
    radians_per_second_per_volt_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(AngularProportionalGain, volts_per_radian_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(AngularIntegralGain, volts_per_radian_second_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(
    AngularAccelerationFeedforward,
    radians_per_second_squared_per_volt_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(AngularDerivativeGain, volt_seconds_per_radian_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(AngularVelocityProportionalGain, volt_seconds_per_radian_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(
    AngularVelocityDerivativeGain,
    volt_seconds_squared_per_radian_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(AngularVelocityIntegralGain, volts_per_radian_tag)

namespace literals {

UNITS_DEFINE_TABLE_LITERALS(LinearVelocityFeedforward, UNITS_LINEAR_VELOCITY_FEEDFORWARD_TABLE)
UNITS_DEFINE_TABLE_LITERALS(LinearProportionalGain, UNITS_LINEAR_PROPORTIONAL_GAIN_TABLE)
UNITS_DEFINE_TABLE_LITERALS(LinearIntegralGain, UNITS_LINEAR_INTEGRAL_GAIN_TABLE)
UNITS_DEFINE_TABLE_LITERALS(
    LinearAccelerationFeedforward,
    UNITS_LINEAR_ACCELERATION_FEEDFORWARD_TABLE)
UNITS_DEFINE_TABLE_LITERALS(LinearDerivativeGain, UNITS_LINEAR_DERIVATIVE_GAIN_TABLE)
UNITS_DEFINE_TABLE_LITERALS(
    LinearVelocityDerivativeGain,
    UNITS_LINEAR_SECOND_DERIVATIVE_GAIN_TABLE)
UNITS_DEFINE_TABLE_LITERALS(
    AngularVelocityFeedforward,
    UNITS_ANGULAR_VELOCITY_FEEDFORWARD_TABLE)
UNITS_DEFINE_TABLE_LITERALS(AngularProportionalGain, UNITS_ANGULAR_PROPORTIONAL_GAIN_TABLE)
UNITS_DEFINE_TABLE_LITERALS(AngularIntegralGain, UNITS_ANGULAR_INTEGRAL_GAIN_TABLE)
UNITS_DEFINE_TABLE_LITERALS(
    AngularAccelerationFeedforward,
    UNITS_ANGULAR_ACCELERATION_FEEDFORWARD_TABLE)
UNITS_DEFINE_TABLE_LITERALS(AngularDerivativeGain, UNITS_ANGULAR_DERIVATIVE_GAIN_TABLE)
UNITS_DEFINE_TABLE_LITERALS(
    AngularVelocityDerivativeGain,
    UNITS_ANGULAR_SECOND_DERIVATIVE_GAIN_TABLE)

UNITS_DEFINE_LITERAL_PAIR(_VpMPs, LinearDerivativeGain::from<volt_seconds_per_meter_tag>(static_cast<double>(value)));
UNITS_DEFINE_LITERAL_PAIR(_VpInPs, LinearDerivativeGain::from<volt_seconds_per_inch_tag>(static_cast<double>(value)));
UNITS_DEFINE_LITERAL_PAIR(_VpRadPs, AngularDerivativeGain::from<volt_seconds_per_radian_tag>(static_cast<double>(value)));
UNITS_DEFINE_LITERAL_PAIR(_VpDegPs, AngularDerivativeGain::from<volt_seconds_per_degree_tag>(static_cast<double>(value)));
UNITS_DEFINE_LITERAL_PAIR(_VpMPs2, LinearVelocityDerivativeGain::from<volt_seconds_squared_per_meter_tag>(static_cast<double>(value)));
UNITS_DEFINE_LITERAL_PAIR(_VpInPs2, LinearVelocityDerivativeGain::from<volt_seconds_squared_per_inch_tag>(static_cast<double>(value)));
UNITS_DEFINE_LITERAL_PAIR(_VpRadPs2, AngularVelocityDerivativeGain::from<volt_seconds_squared_per_radian_tag>(static_cast<double>(value)));
UNITS_DEFINE_LITERAL_PAIR(_VpDegPs2, AngularVelocityDerivativeGain::from<volt_seconds_squared_per_degree_tag>(static_cast<double>(value)));

}  // namespace literals

}  // namespace units
