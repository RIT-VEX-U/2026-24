#pragma once

#include <array>

#include "core/utils/math/spline/cubic_hermite_spline.h"

/**
 * Quintic Hermite spline segment defined by endpoint positions, tangents, and
 * second derivatives.
 */
class QuinticHermiteSpline : public SplineBase {
  public:
    QuinticHermiteSpline() = default;

    QuinticHermiteSpline(const HermitePoint &start, const HermitePoint &end, double du = 0.01)
        : QuinticHermiteSpline(
            start.point,
            end.point,
            start.tangent,
            end.tangent,
            start.second_derivative,
            end.second_derivative,
            du) {}

    QuinticHermiteSpline(
      const Translation2d &p0,
      const Translation2d &p1,
      const Translation2d &t0,
      const Translation2d &t1,
      const Translation2d &a0,
      const Translation2d &a1,
      double du = 0.01) {
        x_ = quintic_coeffs(p0.x(), p1.x(), t0.x(), t1.x(), a0.x(), a1.x());
        y_ = quintic_coeffs(p0.y(), p1.y(), t0.y(), t1.y(), a0.y(), a1.y());
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
    static std::array<double, 6> quintic_coeffs(double p0, double p1, double v0, double v1, double a0, double a1) {
        return {
          p0,
          v0,
          a0 / 2.0,
          (-20.0 * p0 + 20.0 * p1 - 12.0 * v0 - 8.0 * v1 - 3.0 * a0 + a1) / 2.0,
          (30.0 * p0 - 30.0 * p1 + 16.0 * v0 + 14.0 * v1 + 3.0 * a0 - 2.0 * a1) / 2.0,
          (-12.0 * p0 + 12.0 * p1 - 6.0 * v0 - 6.0 * v1 - a0 + a1) / 2.0,
        };
    }

    std::array<double, 6> x_{};
    std::array<double, 6> y_{};
};
