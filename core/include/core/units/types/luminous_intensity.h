#pragma once

#include "core/units/semantic_macros.h"

namespace units {

typedef dim<0, 0, 0, 0, 0, 0, 1, 0> dim_luminous_intensity;

#define UNITS_LUMINOUS_INTENSITY_TABLE(X, context)                                             \
  X(context, cd, candelas, candela_tag, 1, 1)

UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(
    dim_luminous_intensity,
    UNITS_LUMINOUS_INTENSITY_TABLE)

UNITS_DEFINE_QUANTITY_TYPE(
    LuminousIntensity,
    dim_luminous_intensity,
    UNITS_LUMINOUS_INTENSITY_TABLE,
    candela_tag);

namespace literals {

UNITS_DEFINE_TABLE_LITERALS(LuminousIntensity, UNITS_LUMINOUS_INTENSITY_TABLE)

}  // namespace literals

}  // namespace units
