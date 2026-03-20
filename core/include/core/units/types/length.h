#pragma once

#include "core/units/semantic_macros.h"

namespace units {

typedef dim<0, 1, 0, 0, 0, 0, 0, 0> dim_length;

// tile? sorry will :skull:
#define UNITS_LENGTH_TABLE(X, context)                                                         \
  X(context, m, meters, meter_tag, 1, 1)                                                       \
  X(context, mm, millimeters, millimeter_tag, 1, 1000)                                         \
  X(context, cm, centimeters, centimeter_tag, 1, 100)                                          \
  X(context, km, kilometers, kilometer_tag, 1000, 1)                                           \
  X(context, in, inches, inch_tag, 127, 5000)                                                  \
  X(context, tile, tiles, tile_tag, 2413, 4000)                                                \
  X(context, ft, feet, foot_tag, 381, 1250)                                                    \
  X(context, yd, yards, yard_tag, 1143, 1250)                                                  \
  X(context, mi, miles, mile_tag, 201168, 125)

UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_length, UNITS_LENGTH_TABLE)

UNITS_DEFINE_QUANTITY_TYPE(Length, dim_length, UNITS_LENGTH_TABLE, meter_tag);

namespace literals {

UNITS_DEFINE_TABLE_LITERALS(Length, UNITS_LENGTH_TABLE)

}  // namespace literals

}  // namespace units
