#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "core/units/units.h"
#include "core/utils/trajectory/constraints/trajectory_constraint.h"
#include "core/utils/trajectory/trajectory.h"

class TrajectoryParameterizer {
 public:
  using PoseWithCurvature = std::pair<Pose2d, Curvature>;

  static Trajectory time_parameterize_trajectory(
      const std::vector<PoseWithCurvature>& points,
      const std::vector<std::unique_ptr<TrajectoryConstraint>>& constraints,
      Velocity start_velocity,
      Velocity end_velocity,
      Velocity max_velocity,
      Acceleration max_acceleration,
      bool reversed);

 private:
  constexpr static double kEpsilon = 1E-6;

  struct ConstrainedState {
    PoseWithCurvature pose = {Pose2d{}, 0_radpm};
    Length distance = 0_in;
    Velocity maxVelocity = 0_inps;
    Acceleration minAcceleration = 0_inps2;
    Acceleration maxAcceleration = 0_inps2;
  };

  static bool enforce_acceleration_limits(
      bool reverse,
      const std::vector<std::unique_ptr<TrajectoryConstraint>>& constraints,
      ConstrainedState* state);
};
