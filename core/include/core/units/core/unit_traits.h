#pragma once

#include "core/units/core/dim.h"

namespace units {

template<class UnitTag>
struct unit_traits;

#define UNITS_DEFINE_MULTIPLICATIVE_TRAITS(UnitTag, DimType, ScaleNum, ScaleDen)               \
  template<>                                                                                   \
  struct unit_traits<UnitTag> {                                                                \
    typedef DimType dimension;                                                                 \
    static constexpr double scale_num = static_cast<double>(ScaleNum);                         \
    static constexpr double scale_den = static_cast<double>(ScaleDen);                         \
    static constexpr bool is_affine = false;                                                   \
  };

}  // namespace units
