#pragma once

#include "core/units/semantic_macros.h"

namespace units {

typedef dim<0, 0, 0, 0, 0, 1, 0, 0> dim_amount;

// maybe I should add constants???!?!?!?!!!????????
#define UNITS_AMOUNT_TABLE(X, context)                                                         \
  X(context, mol, moles, mole_tag, 1, 1)

UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_amount, UNITS_AMOUNT_TABLE)

UNITS_DEFINE_QUANTITY_TYPE(Amount, dim_amount, UNITS_AMOUNT_TABLE, mole_tag);

namespace literals {

UNITS_DEFINE_TABLE_LITERALS(Amount, UNITS_AMOUNT_TABLE)

}  // namespace literals

}  // namespace units
