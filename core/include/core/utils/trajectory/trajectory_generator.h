#pragma once

#include <functional>
#include <utility>
#include <vector>

#include "core/units/units.h"
#include "core/utils/math/spline/hermite_point.h"
#include "core/utils/trajectory/trajectory.h"
#include "core/utils/trajectory/trajectory_config.h"

/**
 * Helper class used to generate trajectories with constraints.
 */
class TrajectoryGenerator {
 public:
  using PoseWithCurvature = std::pair<Pose2d, Curvature>;

  /**
   * Generates a trajectory from Hermite waypoints and config.
   */
  static Trajectory generate_trajectory(
      const std::vector<HermitePoint>& waypoints,
      const TrajectoryConfig& config);

  /**
   * Set error reporting function. By default, it is output to stderr.
   */
  static void set_error_handler(std::function<void(const char*)> func);

 private:
  static void report_error(const char* error);

  static const Trajectory kDoNothingTrajectory;
  static std::function<void(const char*)> s_errorFunc;
};
