#pragma once

#include "core/units/semantic_macros.h"

namespace units {

typedef dim<0, 0, 0, 1, 0, 0, 0, 0> dim_current;

#define UNITS_CURRENT_TABLE(X, context)                                                        \
  X(context, A, amperes, ampere_tag, 1, 1)                                                     \
  X(context, mA, milliamperes, milliampere_tag, 1, 1000)                                       \
  X(context, uA, microamperes, microampere_tag, 1, 1000000)

UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_current, UNITS_CURRENT_TABLE)

UNITS_DEFINE_QUANTITY_TYPE(Current, dim_current, UNITS_CURRENT_TABLE, ampere_tag);

namespace literals {

UNITS_DEFINE_TABLE_LITERALS(Current, UNITS_CURRENT_TABLE)

}  // namespace literals

}  // namespace units
