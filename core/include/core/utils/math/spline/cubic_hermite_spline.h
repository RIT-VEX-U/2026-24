#pragma once

#include <array>

#include "core/utils/math/spline/hermite_point.h"
#include "core/utils/math/spline/spline_base.h"

/**
 * Cubic Hermite spline segment defined by endpoint positions and tangents.
 */
class CubicHermiteSpline : public SplineBase {
  public:
    CubicHermiteSpline() = default;

    CubicHermiteSpline(const HermitePoint &start, const HermitePoint &end, double du = 0.01)
        : CubicHermiteSpline(start.point, end.point, start.tangent, end.tangent, du) {}

    CubicHermiteSpline(
      const Translation2d &p0,
      const Translation2d &p1,
      const Translation2d &t0,
      const Translation2d &t1,
      double du = 0.01) {
        x_ = cubic_coeffs(p0.x(), p1.x(), t0.x(), t1.x());
        y_ = cubic_coeffs(p0.y(), p1.y(), t0.y(), t1.y());
        build_arc_table(du);
    }

    Translation2d position(double u) const override {
        const double clamped_u = clamp_u(u);
        return Translation2d(eval_poly(x_, clamped_u), eval_poly(y_, clamped_u));
    }

    Translation2d velocity(double u) const override {
        const double clamped_u = clamp_u(u);
        return Translation2d(eval_poly_derivative(x_, clamped_u), eval_poly_derivative(y_, clamped_u));
    }

    Translation2d acceleration(double u) const override {
        const double clamped_u = clamp_u(u);
        return Translation2d(eval_poly_second_derivative(x_, clamped_u), eval_poly_second_derivative(y_, clamped_u));
    }

  private:
    static std::array<double, 4> cubic_coeffs(double p0, double p1, double v0, double v1) {
        return {
          p0,
          v0,
          -3.0 * p0 + 3.0 * p1 - 2.0 * v0 - v1,
          2.0 * p0 - 2.0 * p1 + v0 + v1,
        };
    }

    std::array<double, 4> x_{};
    std::array<double, 4> y_{};
};
