#pragma once

#include "core/units/semantic_macros.h"

namespace units {

typedef dim<0, 0, 1, 0, 0, 0, 0, 0> dim_time;

#define UNITS_TIME_TABLE(X, context)                                                           \
  X(context, s, seconds, second_tag, 1, 1)                                                     \
  X(context, ms, milliseconds, millisecond_tag, 1, 1000)                                       \
  X(context, us, microseconds, microsecond_tag, 1, 1000000)                                    \
  X(context, min, minutes, minute_tag, 60, 1)                                                  \
  X(context, hr, hours, hour_tag, 3600, 1)                                                     \
  X(context, day, days, day_tag, 86400, 1)

UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(dim_time, UNITS_TIME_TABLE)

UNITS_DEFINE_QUANTITY_TYPE(Time, dim_time, UNITS_TIME_TABLE, second_tag);

namespace literals {

UNITS_DEFINE_TABLE_LITERALS(Time, UNITS_TIME_TABLE)

}  // namespace literals

}  // namespace units
