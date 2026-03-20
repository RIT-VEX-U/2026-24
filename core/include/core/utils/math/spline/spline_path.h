#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "core/utils/math/spline/cubic_hermite_spline.h"
#include "core/utils/math/spline/quintic_hermite_spline.h"

class SplinePath {
  public:
    enum class Order {
        Cubic = 3,
        Quintic = 5,
    };

    SplinePath() = default;

    /**
     * Constructs a path by joining Hermite spline segments between each pair of
     * waypoints.
     *
     * @param points Hermite waypoints defining the path.
     * @param order Spline order used for each segment.
     * @param du spacing used when building segment arc table.
     */
    static SplinePath from_hermite(const std::vector<HermitePoint> &points, Order order = Order::Quintic, double du = 0.01) {
        SplinePath path;
        if (points.size() < 2) {
            return path;
        }

        path.segments_.reserve(points.size() - 1);
        path.segment_starts_.reserve(points.size() - 1);

        double accum = 0.0;
        for (size_t i = 0; i + 1 < points.size(); ++i) {
            std::shared_ptr<SplineBase> segment;
            if (order == Order::Cubic) {
                segment = std::shared_ptr<SplineBase>(new CubicHermiteSpline(points[i], points[i + 1], du));
            } else {
                segment = std::shared_ptr<SplineBase>(new QuinticHermiteSpline(points[i], points[i + 1], du));
            }

            path.segment_starts_.push_back(accum);
            accum += segment->length();
            path.segments_.push_back(segment);
        }
        path.total_length_ = accum;
        return path;
    }

    bool empty() const { return segments_.empty(); }
    size_t segment_count() const { return segments_.size(); }
    double length() const { return total_length_; }

    SplineSample sample_by_s(double s) const {
        if (segments_.empty()) {
            return {};
        }
        if (s <= 0.0) {
            SplineSample out = segments_.front()->sample_by_s(0.0);
            out.s = 0.0;
            return out;
        }
        if (s >= total_length_) {
            SplineSample out = segments_.back()->sample_by_s(segments_.back()->length());
            out.s = total_length_;
            return out;
        }

        const size_t idx = segment_index_for_s(s);
        const double local_s = s - segment_starts_[idx];
        SplineSample out = segments_[idx]->sample_by_s(local_s);
        out.s = s;
        return out;
    }

    std::vector<Translation2d> sample_by_spacing(double ds) const {
        std::vector<Translation2d> points;
        if (segments_.empty() || ds <= 0.0) {
            return points;
        }
        for (double s = 0.0; s < total_length_; s += ds) {
            points.push_back(sample_by_s(s).position);
        }
        points.push_back(sample_by_s(total_length_).position);
        return points;
    }

    std::vector<Translation2d> sample_by_segment_steps(double steps_per_segment) const {
        std::vector<Translation2d> points;
        if (segments_.empty() || steps_per_segment <= 0.0) {
            return points;
        }

        const int sample_count = static_cast<int>(steps_per_segment);
        if (sample_count <= 0) {
            return points;
        }

        for (size_t i = 0; i < segments_.size(); ++i) {
            for (int step = 0; step < sample_count; ++step) {
                const double u = static_cast<double>(step) / static_cast<double>(sample_count);
                points.push_back(segments_[i]->position(u));
            }
        }
        points.push_back(segments_.back()->position(1.0));
        return points;
    }

  private:
    size_t segment_index_for_s(double s) const {
        auto upper = std::upper_bound(segment_starts_.begin(), segment_starts_.end(), s);
        if (upper == segment_starts_.begin()) {
            return 0;
        }
        size_t idx = static_cast<size_t>((upper - segment_starts_.begin()) - 1);
        if (idx >= segments_.size()) {
            idx = segments_.size() - 1;
        }
        return idx;
    }

    std::vector<std::shared_ptr<SplineBase>> segments_;
    std::vector<double> segment_starts_;
    double total_length_ = 0.0;
};
