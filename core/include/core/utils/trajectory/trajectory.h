#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include "core/units/units.h"
#include "core/utils/math/geometry/pose2d.h"
#include "core/utils/math_util.h"

class Trajectory {
 public:
  struct State {
    Time t = 0_s;
    Velocity velocity = 0_inps;
    Acceleration acceleration = 0_inps2;
    Pose2d pose{0.0, 0.0, 0.0};
    Curvature curvature = 0_radpm;

    State() = default;

    State(Time t, Velocity velocity, Acceleration acceleration, Pose2d pose, Curvature curvature)
        : t(t), velocity(velocity), acceleration(acceleration), pose(pose), curvature(curvature) {}

    bool operator==(const State &other) const {
      return t == other.t && velocity == other.velocity && acceleration == other.acceleration && pose == other.pose &&
             curvature == other.curvature;
    }

    State interpolate(State end_value, double i) const {
      const Time new_t = lerp(t, end_value.t, i);
      const Time delta_t = new_t - t;

      if (delta_t < 0_s) {
        return end_value.interpolate(*this, 1.0 - i);
      }

      const bool reversing = velocity < 0_inps || (abs(velocity) < 1E-9_inps && acceleration < 0_inps2);
      const Velocity new_v = velocity + (acceleration * delta_t);
      const Length new_s =
        (velocity * delta_t + 0.5 * acceleration * delta_t * delta_t) * (reversing ? -1.0 : 1.0);

      const Length distance = Length::from<inch_tag>(end_value.pose.translation().distance(pose.translation()));
      const double interpolation_frac = distance > 1E-9_in ? (new_s / distance).value() : 0.0;

      return {
        new_t,
        new_v,
        acceleration,
        lerp(pose, end_value.pose, interpolation_frac),
        lerp(curvature, end_value.curvature, interpolation_frac),
      };
    }
  };

  Trajectory() = default;

  explicit Trajectory(std::vector<State> states) : m_states(std::move(states)) {
    if (!m_states.empty()) {
      m_total_time = m_states.back().t;
    }
  }

  bool empty() const { return m_states.empty(); }

  Time total_time() const { return m_total_time; }

  const std::vector<State> &states() const { return m_states; }

  State sample(Time t) const {
    if (m_states.empty()) {
      return State{};
    }

    if (t <= m_states.front().t) {
      return m_states.front();
    }
    if (t >= m_total_time) {
      return m_states.back();
    }

    auto sample = std::lower_bound(
      m_states.cbegin() + 1,
      m_states.cend(),
      t,
      [](const State &a, const Time &b) { return a.t < b; });

    auto prev_sample = sample - 1;

    if (abs(sample->t - prev_sample->t) < 1E-9_s) {
      return *sample;
    }

    return prev_sample->interpolate(*sample, ((t - prev_sample->t) / (sample->t - prev_sample->t)).value());
  }

  Trajectory transform_by(const Transform2d &transform) const {
    if (m_states.empty()) {
      return *this;
    }

    auto &first_state = m_states[0];
    auto &first_pose = first_state.pose;

    auto new_first_pose = first_pose + transform;
    auto new_states = m_states;
    new_states[0].pose = new_first_pose;

    for (size_t i = 1; i < new_states.size(); ++i) {
      auto &state = new_states[i];
      state.pose = new_first_pose + (state.pose - first_pose);
    }

    return Trajectory(new_states);
  }

  Trajectory relative_to(const Pose2d &pose) const {
    auto new_states = m_states;
    for (auto &state : new_states) {
      state.pose = state.pose.relative_to(pose);
    }
    return Trajectory(new_states);
  }

  Trajectory operator+(const Trajectory &other) const {
    if (m_states.empty()) {
      return other;
    }

    auto states = m_states;
    auto other_states = other.states();
    for (auto &other_state : other_states) {
      other_state.t += m_total_time;
    }

    states.insert(states.end(), other_states.begin() + 1, other_states.end());
    return Trajectory(states);
  }

  Pose2d initial_pose() const { return sample(0_s).pose; }

  bool operator==(const Trajectory &other) const {
    return m_total_time == other.m_total_time && m_states == other.m_states;
  }

 private:
  std::vector<State> m_states;
  Time m_total_time = 0_s;
};
