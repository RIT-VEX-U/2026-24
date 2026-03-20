#pragma once

#include <algorithm>
#include <cmath>
#include <limits>

#include "core/units/units.h"
#include "core/utils/math_util.h"
#include "core/utils/trajectory/constraints/trajectory_constraint.h"

class TankVoltageConstraint : public TrajectoryConstraint {
 public:
  TankVoltageConstraint(LinearVelocityFeedforward Kv, LinearAccelerationFeedforward Ka, Voltage maxVoltage, Length trackWidth)
      : m_Kv(Kv),
        m_Ka(Ka),
        m_maxVoltage(maxVoltage),
        m_trackWidth(trackWidth) {}

  Velocity max_velocity(
      const Pose2d& pose, Curvature curvature,
      Velocity velocity) const override {
    return Velocity(std::numeric_limits<double>::max());
  }

  MinMax min_max_acceleration(
      const Pose2d& pose, Curvature curvature,
      Velocity speed) const override {
    Velocity leftVelocity = (speed - (m_trackWidth / 2 * (speed * curvature / 1_rad)));
    Velocity rightVelocity = (speed + (m_trackWidth / 2 * (speed * curvature / 1_rad)));

    Velocity maxWheelSpeed = units::max(leftVelocity, rightVelocity);
    Velocity minWheelSpeed = units::min(leftVelocity, rightVelocity);

    Acceleration maxWheelAcceleration = (m_maxVoltage - (m_Kv * maxWheelSpeed)) / m_Ka;
    Acceleration minWheelAcceleration = (-m_maxVoltage - m_Kv * minWheelSpeed) / m_Ka;

    Acceleration maxChassisAcceleration;
    Acceleration minChassisAcceleration;

    if (abs(speed).inps() <= 1e-9) {
      maxChassisAcceleration = maxWheelAcceleration / (1 + m_trackWidth * abs(curvature / 1_rad) / 2);
      minChassisAcceleration = minWheelAcceleration / (1 + m_trackWidth * abs(curvature / 1_rad) / 2);
    } else {
      maxChassisAcceleration = maxWheelAcceleration / (1 + m_trackWidth * abs(curvature / 1_rad) * sgn(speed) / 2);
      minChassisAcceleration = minWheelAcceleration / (1 - m_trackWidth * abs(curvature / 1_rad) * sgn(speed) / 2);
    }

    if (abs(curvature) > 1E-9_radpm && (m_trackWidth / 2) > 1_rad / abs(curvature)) {
      if (speed > 0_mps) {
        minChassisAcceleration = -minChassisAcceleration;
      } else if (speed < 0_mps) {
        maxChassisAcceleration = -maxChassisAcceleration;
      }
    }

    return {minChassisAcceleration, maxChassisAcceleration};
  }

 private:
  LinearVelocityFeedforward m_Kv;
  LinearAccelerationFeedforward m_Ka;
  Voltage m_maxVoltage;
  Length m_trackWidth;
};
