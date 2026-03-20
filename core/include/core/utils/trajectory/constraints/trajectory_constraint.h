#pragma once

#include <limits>

#include "core/units/units.h"
#include "core/utils/math/geometry/pose2d.h"

/**
 * An interface for defining user-defined trajectory constraints.
 */
class TrajectoryConstraint {
 public:
  TrajectoryConstraint() = default;

  TrajectoryConstraint(const TrajectoryConstraint&) = default;
  TrajectoryConstraint& operator=(const TrajectoryConstraint&) = default;

  TrajectoryConstraint(TrajectoryConstraint&&) = default;
  TrajectoryConstraint& operator=(TrajectoryConstraint&&) = default;

  virtual ~TrajectoryConstraint() = default;

  struct MinMax {
    MinMax(Acceleration minAcceleration, Acceleration maxAcceleration)
        : minAcceleration(minAcceleration), maxAcceleration(maxAcceleration) {}
    MinMax()
        : minAcceleration(-std::numeric_limits<double>::max()),
          maxAcceleration(std::numeric_limits<double>::max()) {}

    Acceleration minAcceleration{-std::numeric_limits<double>::max()};
    Acceleration maxAcceleration{std::numeric_limits<double>::max()};
  };

  virtual Velocity max_velocity(
      const Pose2d& pose, Curvature curvature,
      Velocity velocity) const = 0;

  virtual MinMax min_max_acceleration(
      const Pose2d& pose, Curvature curvature,
      Velocity speed) const = 0;
};
