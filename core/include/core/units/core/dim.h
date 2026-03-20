#pragma once

#include <type_traits>

namespace units {

// used to define everything, and deal with math... i.e. density is kg/m^3... so dim<1, -3, 0, 0, 0, 0, 0, 0>
template<int M, int L, int Ti, int I, int Th, int N, int J, int A>
struct dim {
  static constexpr int mass = M;
  static constexpr int length = L;
  static constexpr int time = Ti;
  static constexpr int current = I;
  static constexpr int temperature = Th;
  static constexpr int amount = N;
  static constexpr int luminous_intensity = J;
  static constexpr int angle = A;
};

/** Combines dimensions for multiplication operations. */
template<class LeftDim, class RightDim>
struct dim_add {
  typedef dim<
      LeftDim::mass + RightDim::mass,
      LeftDim::length + RightDim::length,
      LeftDim::time + RightDim::time,
      LeftDim::current + RightDim::current,
      LeftDim::temperature + RightDim::temperature,
      LeftDim::amount + RightDim::amount,
      LeftDim::luminous_intensity + RightDim::luminous_intensity,
      LeftDim::angle + RightDim::angle> type;
};

/** Combines dimensions for division operations. */
template<class LeftDim, class RightDim>
struct dim_sub {
  typedef dim<
      LeftDim::mass - RightDim::mass,
      LeftDim::length - RightDim::length,
      LeftDim::time - RightDim::time,
      LeftDim::current - RightDim::current,
      LeftDim::temperature - RightDim::temperature,
      LeftDim::amount - RightDim::amount,
      LeftDim::luminous_intensity - RightDim::luminous_intensity,
      LeftDim::angle - RightDim::angle> type;
};

typedef dim<0, 0, 0, 0, 0, 0, 0, 0> dim_none;

}  // namespace units
