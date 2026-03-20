#pragma once

#include "core/units/semantic_macros.h"

namespace units {

typedef dim<0, 0, 1, 1, 0, 0, 0, 0> dim_charge;
typedef dim<1, 2, -3, -1, 0, 0, 0, 0> dim_voltage;
typedef dim<1, 2, -3, -2, 0, 0, 0, 0> dim_resistance;
typedef dim<-1, -2, 3, 2, 0, 0, 0, 0> dim_conductance;
typedef dim<-1, -2, 4, 2, 0, 0, 0, 0> dim_capacitance;
typedef dim<1, 2, -2, -2, 0, 0, 0, 0> dim_inductance;
typedef dim<1, 2, -2, -1, 0, 0, 0, 0> dim_magnetic_flux;
typedef dim<1, 0, -2, -1, 0, 0, 0, 0> dim_magnetic_flux_density;

// not short cuz it bricks in vex
#define UNITS_CHARGE_TABLE(X, context)                                                         \
  X(context, coul, coulombs, coulomb_tag, 1, 1)

#define UNITS_VOLTAGE_TABLE(X, context)                                                        \
  X(context, V, volts, volt_tag, 1, 1)                                                         \
  X(context, mV, millivolts, millivolt_tag, 1, 1000)

#define UNITS_RESISTANCE_TABLE(X, context)                                                     \
  X(context, ohm, ohms, ohm_tag, 1, 1)                                                         \
  X(context, kohm, kiloohms, kiloohm_tag, 1000, 1)

// not short cuz it bricks in vex
#define UNITS_CONDUCTANCE_TABLE(X, context)                                                    \
  X(context, siem, siemens, siemens_tag, 1, 1)

#define UNITS_CAPACITANCE_TABLE(X, context)                                                    \
  X(context, F, farads, farad_tag, 1, 1)                                                       \
  X(context, mF, millifarads, millifarad_tag, 1, 1000)                                         \
  X(context, uF, microfarads, microfarad_tag, 1, 1000000)

#define UNITS_INDUCTANCE_TABLE(X, context)                                                     \
  X(context, H, henries, henry_tag, 1, 1)

#define UNITS_MAGNETIC_FLUX_TABLE(X, context)                                                  \
  X(context, Wb, webers, weber_tag, 1, 1)

#define UNITS_MAGNETIC_FLUX_DENSITY_TABLE(X, context)                                          \
  X(context, T, teslas, tesla_tag, 1, 1)

UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_charge, UNITS_CHARGE_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_voltage, UNITS_VOLTAGE_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_resistance, UNITS_RESISTANCE_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_conductance, UNITS_CONDUCTANCE_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_capacitance, UNITS_CAPACITANCE_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_inductance, UNITS_INDUCTANCE_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_magnetic_flux, UNITS_MAGNETIC_FLUX_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_magnetic_flux_density,
    UNITS_MAGNETIC_FLUX_DENSITY_TABLE)

UNITS_DEFINE_SEMANTIC_QUANTITY(Charge, dim_charge, UNITS_CHARGE_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(Voltage, dim_voltage, UNITS_VOLTAGE_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    Resistance,
    dim_resistance,
    UNITS_RESISTANCE_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    Conductance,
    dim_conductance,
    UNITS_CONDUCTANCE_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    Capacitance,
    dim_capacitance,
    UNITS_CAPACITANCE_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    Inductance,
    dim_inductance,
    UNITS_INDUCTANCE_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    MagneticFlux,
    dim_magnetic_flux,
    UNITS_MAGNETIC_FLUX_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    MagneticFluxDensity,
    dim_magnetic_flux_density,
    UNITS_MAGNETIC_FLUX_DENSITY_TABLE);

UNITS_CHARGE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_VOLTAGE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_RESISTANCE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_CONDUCTANCE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_CAPACITANCE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_INDUCTANCE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_MAGNETIC_FLUX_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_MAGNETIC_FLUX_DENSITY_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)

UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Charge, coulomb_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Voltage, volt_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Resistance, ohm_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Conductance, siemens_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Capacitance, farad_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Inductance, henry_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(MagneticFlux, weber_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(MagneticFluxDensity, tesla_tag)

namespace literals {

UNITS_DEFINE_TABLE_LITERALS(Charge, UNITS_CHARGE_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Voltage, UNITS_VOLTAGE_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Resistance, UNITS_RESISTANCE_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Conductance, UNITS_CONDUCTANCE_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Capacitance, UNITS_CAPACITANCE_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Inductance, UNITS_INDUCTANCE_TABLE)
UNITS_DEFINE_TABLE_LITERALS(MagneticFlux, UNITS_MAGNETIC_FLUX_TABLE)
UNITS_DEFINE_TABLE_LITERALS(MagneticFluxDensity, UNITS_MAGNETIC_FLUX_DENSITY_TABLE)

}  // namespace literals

}  // namespace units
