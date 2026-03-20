#pragma once

#include "core/units/semantic_macros.h"

namespace units {

typedef dim<0, 0, 0, 0, 0, 0, 0, 2> dim_solid_angle;
typedef dim<0, 0, 0, 0, 0, 0, 1, 2> dim_luminous_flux;
typedef dim<0, -2, 0, 0, 0, 0, 1, 2> dim_illuminance;

#define UNITS_SOLID_ANGLE_TABLE(X, context)                                                    \
  X(context, sr, steradians, steradian_tag, 1, 1)

#define UNITS_LUMINOUS_FLUX_TABLE(X, context)                                                  \
  X(context, lm, lumens, lumen_tag, 1, 1)

#define UNITS_ILLUMINANCE_TABLE(X, context)                                                    \
  X(context, lx, lux, lux_tag, 1, 1)

UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_solid_angle, UNITS_SOLID_ANGLE_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_luminous_flux, UNITS_LUMINOUS_FLUX_TABLE)
UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_illuminance, UNITS_ILLUMINANCE_TABLE)

UNITS_DEFINE_SEMANTIC_QUANTITY(
    SolidAngle,
    dim_solid_angle,
    UNITS_SOLID_ANGLE_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    LuminousFlux,
    dim_luminous_flux,
    UNITS_LUMINOUS_FLUX_TABLE);
UNITS_DEFINE_SEMANTIC_QUANTITY(
    Illuminance,
    dim_illuminance,
    UNITS_ILLUMINANCE_TABLE);

UNITS_SOLID_ANGLE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_LUMINOUS_FLUX_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)
UNITS_ILLUMINANCE_TABLE(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)

UNITS_DEFINE_DEFAULT_UNIT_TRAIT(SolidAngle, steradian_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(LuminousFlux, lumen_tag)
UNITS_DEFINE_DEFAULT_UNIT_TRAIT(Illuminance, lux_tag)

namespace literals {

UNITS_DEFINE_TABLE_LITERALS(SolidAngle, UNITS_SOLID_ANGLE_TABLE)
UNITS_DEFINE_TABLE_LITERALS(LuminousFlux, UNITS_LUMINOUS_FLUX_TABLE)
UNITS_DEFINE_TABLE_LITERALS(Illuminance, UNITS_ILLUMINANCE_TABLE)

}  // namespace literals

}  // namespace units
