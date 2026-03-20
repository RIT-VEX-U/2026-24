#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "core/units/units.h"
#include "core/utils/trajectory/constraints/tank_kinematics_constraint.h"
#include "core/utils/trajectory/constraints/trajectory_constraint.h"


class TrajectoryConfig {
 public:
  TrajectoryConfig(Velocity max_velocity, Acceleration max_acceleration)
      : m_max_velocity(max_velocity), m_max_acceleration(max_acceleration) {}

  TrajectoryConfig(Velocity max_velocity, Acceleration max_acceleration, bool reversed)
      : TrajectoryConfig(max_velocity, max_acceleration) {
    set_reversed(reversed);
  }

  TrajectoryConfig(Velocity max_velocity, Acceleration max_acceleration, Length track_width)
      : TrajectoryConfig(max_velocity, max_acceleration) {
    set_track_width(track_width);
  }

  TrajectoryConfig(
      Velocity max_velocity,
      Acceleration max_acceleration,
      Velocity start_velocity,
      Velocity end_velocity,
      bool reversed)
      : TrajectoryConfig(max_velocity, max_acceleration, reversed) {
    set_start_velocity(start_velocity);
    set_end_velocity(end_velocity);
  }

  TrajectoryConfig(
      Velocity max_velocity,
      Acceleration max_acceleration,
      Velocity start_velocity,
      Velocity end_velocity,
      bool reversed,
      Length track_width)
      : TrajectoryConfig(max_velocity, max_acceleration, start_velocity, end_velocity, reversed) {
    set_track_width(track_width);
  }

  TrajectoryConfig(const TrajectoryConfig &) = delete;
  TrajectoryConfig &operator=(const TrajectoryConfig &) = delete;

  TrajectoryConfig(TrajectoryConfig &&) = default;
  TrajectoryConfig &operator=(TrajectoryConfig &&) = default;

  void set_start_velocity(Velocity start_velocity) { m_start_velocity = start_velocity; }

  void set_end_velocity(Velocity end_velocity) { m_end_velocity = end_velocity; }

  void set_reversed(bool reversed) { m_reversed = reversed; }

  template <typename Constraint>
  typename std::enable_if<std::is_base_of<TrajectoryConstraint, typename std::decay<Constraint>::type>::value, void>::type
  add_constraint(Constraint &&constraint) {
    typedef typename std::decay<Constraint>::type C;
    m_constraints.emplace_back(std::unique_ptr<TrajectoryConstraint>(new C(std::forward<Constraint>(constraint))));
  }

  void set_track_width(Length track_width) { add_constraint(TankKinematicsConstraint(track_width, m_max_velocity)); }

  Velocity start_velocity() const { return m_start_velocity; }

  Velocity end_velocity() const { return m_end_velocity; }

  Velocity max_velocity() const { return m_max_velocity; }

  Acceleration max_acceleration() const { return m_max_acceleration; }

  const std::vector<std::unique_ptr<TrajectoryConstraint>> &constraints() const { return m_constraints; }

  bool is_reversed() const { return m_reversed; }

 private:
  Velocity m_start_velocity = 0_inps;
  Velocity m_end_velocity = 0_inps;
  Velocity m_max_velocity;
  Acceleration m_max_acceleration;
  std::vector<std::unique_ptr<TrajectoryConstraint>> m_constraints;
  bool m_reversed = false;
};
