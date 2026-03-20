#pragma once

#include "core/utils/math/geometry/rotation2d.h"
#include "core/utils/math/geometry/translation2d.h"

/**
 * Each point stores position, first derivative, and optional second
 * derivative information.
 */
struct HermitePoint {
    Translation2d point;
    Translation2d tangent;
    Translation2d second_derivative;

    HermitePoint() = default;

    HermitePoint(
      const Translation2d &point,
      const Translation2d &tangent,
      const Translation2d &second_derivative = Translation2d())
        : point(point),
          tangent(tangent),
          second_derivative(second_derivative) {}

    HermitePoint(double x, double y, double vx, double vy)
        : point(x, y),
          tangent(vx, vy),
          second_derivative(0.0, 0.0) {}

    HermitePoint(double x, double y, double vx, double vy, double ax, double ay)
        : point(x, y),
          tangent(vx, vy),
          second_derivative(ax, ay) {}

    static HermitePoint from_derivatives(
      const Translation2d &point,
      const Translation2d &tangent,
      const Translation2d &second_derivative = Translation2d()) {
        return HermitePoint(point, tangent, second_derivative);
    }

    static HermitePoint from_heading(
      double x,
      double y,
      double heading_rad,
      double speed,
      double accel_heading_rad = 0.0,
      double accel_mag = 0.0) {
        return HermitePoint(
          Translation2d(x, y),
          Translation2d(speed, Rotation2d(heading_rad)),
          Translation2d(accel_mag, Rotation2d(accel_heading_rad)));
    }

    static HermitePoint from_heading(
      const Translation2d &point,
      double heading_rad,
      double speed,
      double accel_heading_rad = 0.0,
      double accel_mag = 0.0) {
        return HermitePoint(
          point,
          Translation2d(speed, Rotation2d(heading_rad)),
          Translation2d(accel_mag, Rotation2d(accel_heading_rad)));
    }

    Translation2d get_point() const { return point; }
    Translation2d get_tangent() const { return tangent; }
    Translation2d get_second_derivative() const { return second_derivative; }
};
