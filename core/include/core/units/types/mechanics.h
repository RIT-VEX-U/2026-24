#pragma once

#include "core/units/semantic_macros.h"

namespace units {

typedef dim<1, 1, -1, 0, 0, 0, 0, 0> dim_momentum;
typedef dim<1, 1, -2, 0, 0, 0, 0, 0> dim_force;
typedef dim<1, -1, -2, 0, 0, 0, 0, 0> dim_pressure;
typedef dim<1, 0, -2, 0, 0, 0, 0, 0> dim_stiffness;
typedef dim<-1, 0, 2, 0, 0, 0, 0, 0> dim_compliance;
typedef dim<1, -3, 0, 0, 0, 0, 0, 0> dim_mass_density;
typedef dim<1, -2, 0, 0, 0, 0, 0, 0> dim_area_density;
typedef dim<0, 3, -1, 0, 0, 0, 0, 0> dim_volumetric_flow;
typedef dim<1, 0, -1, 0, 0, 0, 0, 0> dim_mass_flow;
typedef dim<1, 2, 0, 0, 0, 0, 0, 0> dim_inertia;
typedef dim<1, 2, -1, 0, 0, 0, 0, 1> dim_angular_momentum;
typedef dim<1, 2, -2, 0, 0, 0, 0, -1> dim_rotational_stiffness;
typedef dim<-1, -2, 2, 0, 0, 0, 0, 1> dim_rotational_compliance;
typedef dim<1, 2, -1, 0, 0, 0, 0, -1> dim_rotational_damping;
typedef dim<1, 2, -2, 0, 0, 0, 0, 0> dim_energy;
typedef dim<1, 2, -3, 0, 0, 0, 0, 0> dim_power;

// :(
#define UNITS_MOMENTUM_TABLE(X, context)                                                       \
  X(context, kgmps, kilogram_meters_per_second, kilogram_meter_per_second_tag, 1, 1)           \
  X(context, Ns, newton_seconds, newton_second_tag, 1, 1)

// not short cuz it bricks in vex
#define UNITS_FORCE_TABLE(X, context)                                                          \
  X(context, newton, newtons, newton_tag, 1, 1)

#define UNITS_PRESSURE_TABLE(X, context)                                                       \
  X(context, Pa, pascals, pascal_tag, 1, 1)                                                    \
  X(context, psi, pounds_per_square_inch, psi_tag, 6894757293168361.0, 1000000000000.0)        \
  X(context, bar, bars, bar_tag, 100000, 1)                                                    \
  X(context, atm, atmospheres, atmosphere_tag, 101325, 1)                                      \
  X(context, torr, torr_units, torr_tag, 101325, 760)

#define UNITS_STIFFNESS_TABLE(X, context)                                                      \
  X(context, Npm, newtons_per_meter, newtons_per_meter_tag, 1, 1)                              \
  X(context, Npmm, newtons_per_millimeter, newtons_per_millimeter_tag, 1000, 1)

#define UNITS_COMPLIANCE_TABLE(X, context)                                                     \
  X(context, mpN, meters_per_newton, meters_per_newton_tag, 1, 1)                              \
  X(context, mmpN, millimeters_per_newton, millimeters_per_newton_tag, 1, 1000)

#define UNITS_MASS_DENSITY_TABLE(X, context)                                                   \
  X(context, kgpm3, kilograms_per_cubic_meter, kilograms_per_cubic_meter_tag, 1, 1)            \
  X(context, gpcm3, grams_per_cubic_centimeter, grams_per_cubic_centimeter_tag, 1000, 1)

#define UNITS_AREA_DENSITY_TABLE(X, context)                                                   \
  X(context, kgpm2, kilograms_per_square_meter, kilograms_per_square_meter_tag, 1, 1)          \
  X(context, gpm2, grams_per_square_meter, grams_per_square_meter_tag, 1, 1000)

#define UNITS_VOLUMETRIC_FLOW_TABLE(X, context)                                                \
  X(context, m3ps, cubic_meters_per_second, cubic_meters_per_second_tag, 1, 1)                 \
  X(context, Lps, liters_per_second, liters_per_second_tag, 1, 1000)                           \
  X(context, gpm, gallons_per_minute, us_gallons_per_minute_tag, 788627455, 12500000000000)

#define UNITS_MASS_FLOW_TABLE(X, context)                                                      \
  X(context, kgps, kilograms_per_second, kilograms_per_second_tag, 1, 1)                       \
  X(context, gps, grams_per_second, grams_per_second_tag, 1, 1000)

#define UNITS_ENERGY_TABLE(X, context)                                                         \
  X(context, J, joules, joule_tag, 1, 1)

#define UNITS_TORQUE_TABLE(X, context)                                                         \
  X(context, Nm, newton_meters, newton_meter_tag, 1, 1)                                        \
  X(context, lbft, pound_feet, pound_foot_torque_tag, 13558179483314004.0,                     \
    10000000000000000.0)                                                                       \
  X(context, ozin, ounce_inches, ounce_inch_torque_tag, 706155181422604.0,                     \
    100000000000000000.0)

#define UNITS_ROTATIONAL_STIFFNESS_TABLE(X, context)                                           \
  X(context, Nmprad, newton_meters_per_radian, newton_meters_per_radian_tag, 1, 1)             \
  X(context, Nmpdeg, newton_meters_per_degree, newton_meters_per_degree_tag, 5729577951308232.0, \
    100000000000000.0)

#define UNITS_ROTATIONAL_COMPLIANCE_TABLE(X, context)                                          \
  X(context, radpNm, radians_per_newton_meter, radians_per_newton_meter_tag, 1, 1)             \
  X(context, degpNm, degrees_per_newton_meter, degrees_per_newton_meter_tag,                   \
    17453292519943295.0,                                                                       \
    1000000000000000000.0)

#define UNITS_ROTATIONAL_DAMPING_TABLE(X, context)                                             \
  X(context, Nmsprad, newton_meter_seconds_per_radian, newton_meter_seconds_per_radian_tag,    \
    1, 1)                                                                                      \
  X(context, Nmspdeg, newton_meter_seconds_per_degree, newton_meter_seconds_per_degree_tag,    \
    5729577951308232.0, 100000000000000.0)

#define UNITS_INERTIA_TABLE(X, context)                                                        \
  X(context, kgm2, kilogram_square_meters, kilogram_square_meter_tag, 1, 1)                    \
  X(context, kgcm2, kilogram_square_centimeters, kilogram_square_centimeter_tag, 1, 10000)     \
  X(context, kgin2, kilogram_square_inches, kilogram_square_inch_tag, 16129, 25000000)

#define UNITS_ANGULAR_MOMENTUM_TABLE(X, context)                                               \
  X(context, kgm2ps, kilogram_square_meters_per_second,                                        \
    kilogram_square_meter_per_second_tag, 1, 1)                                                \
  X(context, Nms, newton_meter_seconds, newton_meter_second_tag, 1, 1)

#define UNITS_POWER_TABLE(X, context)                                                          \
  X(context, W, watts, watt_tag, 1, 1)

UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_momentum, UNITS_MOMENTUM_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_force, UNITS_FORCE_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_pressure, UNITS_PRESSURE_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_stiffness, UNITS_STIFFNESS_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_compliance, UNITS_COMPLIANCE_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_mass_density, UNITS_MASS_DENSITY_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_area_density, UNITS_AREA_DENSITY_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_volumetric_flow, UNITS_VOLUMETRIC_FLOW_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_mass_flow, UNITS_MASS_FLOW_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_energy, UNITS_ENERGY_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_energy, UNITS_TORQUE_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_rotational_stiffness,
    UNITS_ROTATIONAL_STIFFNESS_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_rotational_compliance,
    UNITS_ROTATIONAL_COMPLIANCE_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_rotational_damping,
    UNITS_ROTATIONAL_DAMPING_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_inertia, UNITS_INERTIA_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_angular_momentum,
    UNITS_ANGULAR_MOMENTUM_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_power, UNITS_POWER_TABLE)

UNITS_DEFINE_SEMANTIC_QUANTITY(Momentum, dim_momentum, UNITS_MOMENTUM_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(Force, dim_force, UNITS_FORCE_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(Pressure, dim_pressure, UNITS_PRESSURE_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    Stiffness,
    dim_stiffness,
    UNITS_STIFFNESS_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    Compliance,
    dim_compliance,
    UNITS_COMPLIANCE_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    MassDensity,
    dim_mass_density,
    UNITS_MASS_DENSITY_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    AreaDensity,
    dim_area_density,
    UNITS_AREA_DENSITY_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    VolumetricFlow,
    dim_volumetric_flow,
    UNITS_VOLUMETRIC_FLOW_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    MassFlow,
    dim_mass_flow,
    UNITS_MASS_FLOW_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(Inertia, dim_inertia, UNITS_INERTIA_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    AngularMomentum,
    dim_angular_momentum,
    UNITS_ANGULAR_MOMENTUM_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(Energy, dim_energy, UNITS_ENERGY_TABLE);
// it's the same why is it the same 
UNITS_DEFINE_SEMANTIC_ALIAS(Torque, dim_energy, UNITS_TORQUE_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    RotationalStiffness,
    dim_rotational_stiffness,
    UNITS_ROTATIONAL_STIFFNESS_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    RotationalCompliance,
    dim_rotational_compliance,
    UNITS_ROTATIONAL_COMPLIANCE_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    RotationalDamping,
    dim_rotational_damping,
    UNITS_ROTATIONAL_DAMPING_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(Power, dim_power, UNITS_POWER_TABLE);

// please
typedef Momentum Impulse;

UNITS_MOMENTUM_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_FORCE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_PRESSURE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_STIFFNESS_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_COMPLIANCE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_MASS_DENSITY_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_AREA_DENSITY_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_VOLUMETRIC_FLOW_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_MASS_FLOW_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ENERGY_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_TORQUE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ROTATIONAL_STIFFNESS_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ROTATIONAL_COMPLIANCE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ROTATIONAL_DAMPING_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_INERTIA_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ANGULAR_MOMENTUM_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_POWER_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)

UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Momentum, kilogram_meter_per_second_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Force, newton_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Pressure, pascal_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Stiffness, newtons_per_meter_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Compliance, meters_per_newton_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(MassDensity, kilograms_per_cubic_meter_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(AreaDensity, kilograms_per_square_meter_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(VolumetricFlow, cubic_meters_per_second_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(MassFlow, kilograms_per_second_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Energy, joule_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Torque, newton_meter_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(
    RotationalStiffness,
    newton_meters_per_radian_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(
    RotationalCompliance,
    radians_per_newton_meter_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(
    RotationalDamping,
    newton_meter_seconds_per_radian_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Inertia, kilogram_square_meter_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(
    AngularMomentum,
    kilogram_square_meter_per_second_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Power, watt_tag)

namespace literals {

UNITS_DEFINE_TABLE_LITERALS(Momentum, UNITS_MOMENTUM_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Force, UNITS_FORCE_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Pressure, UNITS_PRESSURE_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Stiffness, UNITS_STIFFNESS_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Compliance, UNITS_COMPLIANCE_TABLE)
UNITS_DEFINE_TABLE_LITERALS(MassDensity, UNITS_MASS_DENSITY_TABLE)
UNITS_DEFINE_TABLE_LITERALS(AreaDensity, UNITS_AREA_DENSITY_TABLE)
UNITS_DEFINE_TABLE_LITERALS(VolumetricFlow, UNITS_VOLUMETRIC_FLOW_TABLE)
UNITS_DEFINE_TABLE_LITERALS(MassFlow, UNITS_MASS_FLOW_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Inertia, UNITS_INERTIA_TABLE)
UNITS_DEFINE_TABLE_LITERALS(AngularMomentum, UNITS_ANGULAR_MOMENTUM_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Energy, UNITS_ENERGY_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Torque, UNITS_TORQUE_TABLE)
UNITS_DEFINE_TABLE_LITERALS(RotationalStiffness, UNITS_ROTATIONAL_STIFFNESS_TABLE)
UNITS_DEFINE_TABLE_LITERALS(RotationalCompliance, UNITS_ROTATIONAL_COMPLIANCE_TABLE)
UNITS_DEFINE_TABLE_LITERALS(RotationalDamping, UNITS_ROTATIONAL_DAMPING_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Power, UNITS_POWER_TABLE)

}  // namespace literals

}  // namespace units
