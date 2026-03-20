#pragma once

#include <cmath>

#include "core/units/units.h"
#include "core/utils/trajectory/constraints/trajectory_constraint.h"

class CentripetalAccelerationConstraint : public TrajectoryConstraint {
 public:
  explicit CentripetalAccelerationConstraint(Acceleration maxCentripetalAcceleration)
      : m_maxCentripetalAcceleration(maxCentripetalAcceleration) {}

  Velocity max_velocity(
      const Pose2d& pose, Curvature curvature,
      Velocity velocity) const override {
    // m/s2 * rad/m
    // force the rad out of existence (it's not a real unit after all)
    return sqrt(m_maxCentripetalAcceleration / abs(curvature / 1_rad));
  }

  MinMax min_max_acceleration(
      const Pose2d& pose, Curvature curvature,
      Velocity speed) const override {
    return {};
  }

 private:
  Acceleration m_maxCentripetalAcceleration;
};
