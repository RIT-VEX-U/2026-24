#pragma once

#include "core/units/semantic_macros.h"

namespace units {

typedef dim<1, 0, 0, 0, 0, 0, 0, 0> dim_mass;

// guess what lb ISN'T whatever, don't ask about pound feet (isn't it foot pound usually?)
#define UNITS_MASS_TABLE(X, context)                                                           \
  X(context, kg, kilograms, kilogram_tag, 1, 1)                                                \
  X(context, g, grams, gram_tag, 1, 1000)                                                      \
  X(context, oz, ounces, ounce_tag, 28349523125LL, 1000000000000LL)                            \
  X(context, lb, pounds, pound_tag, 45359237, 100000000)                                       \
  X(context, ton, tons, ton_tag, 90718474, 100000)


UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_mass, UNITS_MASS_TABLE)

UNITS_DEFINE_QUANTITY_TYPE(Mass, dim_mass, UNITS_MASS_TABLE, kilogram_tag);

namespace literals {

UNITS_DEFINE_TABLE_LITERALS(Mass, UNITS_MASS_TABLE)

}  // namespace literals

}  // namespace units
