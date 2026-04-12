#pragma once

#include <array>
#include <cmath>

#include "core/units/types/geometry.h"
#include "core/units/units.h"
#include "core/utils/controls/state_space/differential_drive_wheel_voltages.h"
#include "core/utils/math/eigen_interface.h"
#include "core/utils/interpolating_map.h"
#include "core/utils/controls/state_space/linear_quadratic_regulator.h"
#include "core/utils/math/geometry/pose2d.h"
#include "core/utils/math/systems/dare_solver.h"
#include "core/utils/math/systems/discretization.h"
#include "core/utils/math/systems/linear_system.h"

struct TankTrajectoryFollowerConfig {
    std::array<double, 5> q_tolerances{{0.15, 0.1, 0.1, 0.3, 0.3}};
    std::array<double, 2> r_tolerances{{12.0, 12.0}};
    Time dt = 10_ms;
    Velocity max_velocity = 0_inps;
    Velocity velocity_step = 0.1_inps;
    bool stop_at_end = true;

    EVec<5> q_tolerances_eigen() const {
        EVec<5> out;
        for (int i = 0; i < 5; ++i) {
            out(i) = q_tolerances[i];
        }
        return out;
    }

    EVec<2> r_tolerances_eigen() const {
        EVec<2> out;
        for (int i = 0; i < 2; ++i) {
            out(i) = r_tolerances[i];
        }
        return out;
    }
};

class LTVDifferentialDriveController {
  public:
    using GainMatrix = EMat<2, 5>;
    using ErrorVector = EVec<5>;
    using QMatrix = EMat<5, 5>;
    using RMatrix = EMat<2, 2>;

    LTVDifferentialDriveController(
      LinearSystem<2, 2, 2> &plant,
      Length trackwidth,
      const ErrorVector &q_tolerances,
      const EVec<2> &r_tolerances,
      Time dt,
      Velocity max_velocity = 0_inps,
      Velocity velocity_step = 0.1_inps
    )
        : m_tolerance(q_tolerances) {
        const auto A_vel = plant.A();
        const auto B_vel = plant.B();
        const auto Q = cost_matrix<5>(q_tolerances);
        const auto R = cost_matrix<2>(r_tolerances);

        const Velocity max_v = max_velocity > 0_inps ? max_velocity : estimate_max_velocity(A_vel, B_vel);
        const Velocity step = std::max(velocity_step, 1e-3_inps);

        for (Velocity v = -max_v; v <= max_v + 1e-9_inps; v += step) {
            const Velocity linearized_velocity =
              abs(v) < 1e-4_inps ? (v < 0_inps ? -1e-4_inps : 1e-4_inps) : v;
            auto [A_cont, B_cont] = make_linearized_error_dynamics(A_vel, B_vel, trackwidth, linearized_velocity);
            auto [A_disc, B_disc] = discretize_AB(A_cont, B_cont, dt.s());
            const auto S = DARE<5, 2>(A_disc, B_disc, Q, R);
            const GainMatrix K = (B_disc.transpose() * S * B_disc + R).llt().solve(B_disc.transpose() * S * A_disc);
            m_table.insert(v.inps(), K);
        }
    }

    DifferentialDriveWheelVoltages calculate(
      const Pose2d &current_pose,
      Velocity left_velocity,
      Velocity right_velocity,
      const Pose2d &pose_ref,
      Velocity left_velocity_ref,
      Velocity right_velocity_ref
    ) {
        Velocity linear_velocity = 0.5 * (left_velocity + right_velocity);
        if (abs(linear_velocity) < 1e-3_inps) {
            linear_velocity = 0.5 * (left_velocity_ref + right_velocity_ref);
        }
        const GainMatrix K = m_table[linear_velocity.inps()];

        const ErrorVector global_error{
          pose_ref.x() - current_pose.x(),
          pose_ref.y() - current_pose.y(),
          (pose_ref.rotation() - current_pose.rotation()).wrapped_radians_180(),
          (left_velocity_ref - left_velocity).inps(),
          (right_velocity_ref - right_velocity).inps(),
        };

        EMat<5, 5> in_robot_frame = EMat<5, 5>::Identity();
        in_robot_frame(0, 0) = current_pose.rotation().f_cos();
        in_robot_frame(0, 1) = current_pose.rotation().f_sin();
        in_robot_frame(1, 0) = -current_pose.rotation().f_sin();
        in_robot_frame(1, 1) = current_pose.rotation().f_cos();

        m_error = in_robot_frame * global_error;

        const EVec<2> u = K * m_error;
        DifferentialDriveWheelVoltages out;
        out.left = Voltage::from<volt_tag>(u(0));
        out.right = Voltage::from<volt_tag>(u(1));
        return out;
    }

    bool at_reference() const {
        return std::abs(m_error(0)) < m_tolerance(0) && std::abs(m_error(1)) < m_tolerance(1) &&
               std::abs(m_error(2)) < m_tolerance(2) && std::abs(m_error(3)) < m_tolerance(3) &&
               std::abs(m_error(4)) < m_tolerance(4);
    }

  private:
    static double wrap_angle(double angle_rad) { return std::atan2(std::sin(angle_rad), std::cos(angle_rad)); }

    static std::pair<EMat<5, 5>, EMat<5, 2>> make_linearized_error_dynamics(
      const EMat<2, 2> &A_vel,
      const EMat<2, 2> &B_vel,
      Length trackwidth,
      Velocity linear_velocity
    ) {
        EMat<5, 5> A = EMat<5, 5>::Zero();
        EMat<5, 2> B = EMat<5, 2>::Zero();

        A(0, 3) = 0.5;
        A(0, 4) = 0.5;
        A(1, 2) = linear_velocity.inps();
        A(2, 3) = -1.0 / trackwidth.in();
        A(2, 4) = 1.0 / trackwidth.in();

        A.template block<2, 2>(3, 3) = A_vel;
        B.template block<2, 2>(3, 0) = B_vel;
        return {A, B};
    }

    static Velocity estimate_max_velocity(const EMat<2, 2> &A, const EMat<2, 2> &B) {
        const EVec<2> u{12.0, 12.0};
        EVec<2> x_ss = (-A).lu().solve(B * u);
        return Velocity::from<inches_per_second_tag>(std::max(0.1, 0.5 * (x_ss(0) + x_ss(1))));
    }

    InterpolatingMap<double, GainMatrix> m_table;
    ErrorVector m_error = ErrorVector::Zero();
    ErrorVector m_tolerance = ErrorVector::Zero();
};
