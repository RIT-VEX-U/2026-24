#pragma once

#include <cmath>
#include <vector>

#include "core/units/units.h"
#include "core/utils/math/geometry/rotation2d.h"
#include "core/utils/math/geometry/translation2d.h"

struct SplineSample {
    double u = 0.0;
    double s = 0.0;
    Translation2d position;
    Translation2d velocity;
    Translation2d acceleration;
    Rotation2d heading;
    Curvature curvature = 0_radpm;
};

class SplineBase {
  public:
    virtual ~SplineBase() = default;

    virtual Translation2d position(double u) const = 0;

    virtual Translation2d velocity(double u) const = 0;

    virtual Translation2d acceleration(double u) const = 0;

    Rotation2d heading(double u) const {
        const Translation2d vel = velocity(clamp_u(u));
        return vel.norm() > 1e-9 ? vel.theta() : Rotation2d();
    }

    Curvature curvature(double u) const {
        const Translation2d vel = velocity(clamp_u(u));
        const Translation2d acc = acceleration(clamp_u(u));
        const double denom = std::pow((vel.x() * vel.x()) + (vel.y() * vel.y()), 1.5);
        if (denom < 1e-9) {
            return 0_radpm;
        }

        // nonsense
        constexpr double kInchesPerMeter = Length::from<meter_tag>(1.0).in();
        const double curvature_rad_per_in = ((vel.x() * acc.y()) - (vel.y() * acc.x())) / denom;
        return Curvature::from<radians_per_meter_tag>(curvature_rad_per_in * kInchesPerMeter);
    }

    double length() const { return arc_lengths_.empty() ? 0.0 : arc_lengths_.back(); }

    double u_from_s(double s) const {
        if (arc_lengths_.empty() || us_.empty()) {
            return 0.0;
        }
        if (s <= 0.0) {
            return 0.0;
        }
        if (s >= length()) {
            return 1.0;
        }

        auto upper = std::lower_bound(arc_lengths_.begin(), arc_lengths_.end(), s);
        const size_t idx = static_cast<size_t>(upper - arc_lengths_.begin());
        if (idx == 0) {
            return us_.front();
        }

        const double s0 = arc_lengths_[idx - 1];
        const double s1 = arc_lengths_[idx];
        const double u0 = us_[idx - 1];
        const double u1 = us_[idx];
        const double alpha = (s - s0) / std::max(s1 - s0, 1e-9);
        return u0 + ((u1 - u0) * alpha);
    }

    SplineSample sample(double u) const {
        const double clamped_u = clamp_u(u);
        SplineSample out;
        out.u = clamped_u;
        out.s = s_from_u(clamped_u);
        out.position = position(clamped_u);
        out.velocity = velocity(clamped_u);
        out.acceleration = acceleration(clamped_u);
        out.heading = heading(clamped_u);
        out.curvature = curvature(clamped_u);
        return out;
    }

    SplineSample sample_by_s(double s) const {
        SplineSample out = sample(u_from_s(s));
        out.s = clamp_value(s, 0.0, length());
        return out;
    }

    std::vector<Translation2d> sample_by_count(int count) const {
        std::vector<Translation2d> points;
        if (count <= 0) {
            return points;
        }
        if (count == 1) {
            points.push_back(position(0.0));
            return points;
        }

        points.reserve(static_cast<size_t>(count));
        for (int i = 0; i < count; ++i) {
            const double u = static_cast<double>(i) / static_cast<double>(count - 1);
            points.push_back(position(u));
        }
        return points;
    }

    std::vector<Translation2d> sample_by_spacing(double ds) const {
        std::vector<Translation2d> points;
        if (ds <= 0.0 || length() <= 0.0) {
            return points;
        }

        for (double s = 0.0; s < length(); s += ds) {
            points.push_back(sample_by_s(s).position);
        }
        points.push_back(position(1.0));
        return points;
    }

  protected:
    template <size_t N> static double eval_poly(const std::array<double, N> &coeffs, double u) {
        double out = 0.0;
        for (size_t i = N; i-- > 0;) {
            out = (out * u) + coeffs[i];
        }
        return out;
    }

    template <size_t N> static double eval_poly_derivative(const std::array<double, N> &coeffs, double u) {
        double out = 0.0;
        for (size_t i = N - 1; i-- > 0;) {
            out = (out * u) + (coeffs[i + 1] * static_cast<double>(i + 1));
        }
        return out;
    }

    template <size_t N> static double eval_poly_second_derivative(const std::array<double, N> &coeffs, double u) {
        double out = 0.0;
        for (size_t i = N - 2; i-- > 0;) {
            out = (out * u) + (coeffs[i + 2] * static_cast<double>((i + 1) * (i + 2)));
        }
        return out;
    }

    void build_arc_table(double du = 0.01) {
        us_.clear();
        arc_lengths_.clear();

        const double step = std::max(du, 1e-4);
        Translation2d prev = position(0.0);
        double accum = 0.0;

        us_.push_back(0.0);
        arc_lengths_.push_back(0.0);

        for (double u = step; u < 1.0; u += step) {
            Translation2d cur = position(u);
            accum += cur.distance(prev);
            us_.push_back(u);
            arc_lengths_.push_back(accum);
            prev = cur;
        }

        Translation2d end = position(1.0);
        accum += end.distance(prev);
        us_.push_back(1.0);
        arc_lengths_.push_back(accum);
    }

    static double clamp_u(double u) { return clamp_value(u, 0.0, 1.0); }

    static double clamp_value(double value, double lo, double hi) {
        if (value < lo) {
            return lo;
        }
        if (value > hi) {
            return hi;
        }
        return value;
    }

  private:
    double s_from_u(double u) const {
        if (us_.empty() || arc_lengths_.empty()) {
            return 0.0;
        }
        if (u <= 0.0) {
            return 0.0;
        }
        if (u >= 1.0) {
            return length();
        }

        auto upper = std::lower_bound(us_.begin(), us_.end(), u);
        const size_t idx = static_cast<size_t>(upper - us_.begin());
        if (idx == 0) {
            return arc_lengths_.front();
        }

        const double u0 = us_[idx - 1];
        const double u1 = us_[idx];
        const double s0 = arc_lengths_[idx - 1];
        const double s1 = arc_lengths_[idx];
        const double alpha = (u - u0) / std::max(u1 - u0, 1e-9);
        return s0 + ((s1 - s0) * alpha);
    }

    std::vector<double> us_;
    std::vector<double> arc_lengths_;
};
