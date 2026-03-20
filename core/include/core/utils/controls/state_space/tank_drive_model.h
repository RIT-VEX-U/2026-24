#pragma once

#include "core/units/units.h"
#include "core/utils/math/systems/linear_system.h"

struct TankChassisState {
    units::Velocity linear;
    units::AngularVelocity angular;
};

struct TankWheelState {
    units::Velocity left;
    units::Velocity right;
};

class TankDriveModel {
  public:
    typedef LinearSystem<2, 2, 2> Plant;
    typedef Plant::VectorX StateVector;
    typedef LinearSystem<4, 2, 4> ObserverPlant;
    typedef ObserverPlant::VectorX ObserverStateVector;
    typedef units::LinearVelocityFeedforward LinearKV;
    typedef units::LinearAccelerationFeedforward LinearKA;
    typedef units::AngularVelocityProportionalGain AngularKV;
    typedef units::AngularVelocityDerivativeGain AngularKA;

    TankDriveModel(
      units::Length trackwidth,
      units::Voltage max_voltage,
      LinearKV kV_linear,
      LinearKA kA_linear,
      AngularKV kV_angular,
      AngularKA kA_angular)
        : trackwidth_(trackwidth),
          max_voltage_(max_voltage),
          kV_linear_(kV_linear),
          kA_linear_(kA_linear),
          kV_angular_(kV_angular),
          kA_angular_(kA_angular) {}

    units::Length trackwidth() const { return trackwidth_; }
    units::Voltage max_voltage() const { return max_voltage_; }
    LinearKV kV_linear() const { return kV_linear_; }
    LinearKA kA_linear() const { return kA_linear_; }
    AngularKV kV_angular() const { return kV_angular_; }
    AngularKA kA_angular() const { return kA_angular_; }

    Plant chassis_plant() const {
        const double linear_kv = kV_linear_.VpInps();
        const double linear_ka = kA_linear_.VpInps2();
        const double angular_kv = kV_angular_.VspRad();
        const double angular_ka = kA_angular_.Vs2pRad();

        const Plant::MatrixA A{
          {-linear_kv / linear_ka, 0.0},
          {0.0, -angular_kv / angular_ka},
        };

        const Plant::MatrixB B{
          {0.5 / linear_ka, 0.5 / linear_ka},
          {-0.5 / angular_ka, 0.5 / angular_ka},
        };

        return Plant(A, B, Plant::MatrixC::Identity(), Plant::MatrixD::Zero());
    }

    Plant wheel_plant() const {
        Plant chassis = chassis_plant();
        const double trackwidth = trackwidth_.in();

        const EMat<2, 2> M{
          {0.5, 0.5},
          {-1.0 / trackwidth, 1.0 / trackwidth},
        };
        const EMat<2, 2> M_inv{
          {1.0, -trackwidth / 2.0},
          {1.0, trackwidth / 2.0},
        };

        Plant::MatrixA A = M_inv * chassis.A() * M;
        Plant::MatrixB B = M_inv * chassis.B();
        return Plant(A, B, Plant::MatrixC::Identity(), Plant::MatrixD::Zero());
    }

    ObserverPlant wheel_position_plant() const {
        Plant velocity_plant = wheel_plant();
        Plant::MatrixA A_vel = velocity_plant.A();
        Plant::MatrixB B_vel = velocity_plant.B();

        ObserverPlant::MatrixA A = ObserverPlant::MatrixA::Zero();
        ObserverPlant::MatrixB B = ObserverPlant::MatrixB::Zero();
        ObserverPlant::MatrixC C = ObserverPlant::MatrixC::Zero();

        A(0, 1) = 1.0;
        A(2, 3) = 1.0;
        A(1, 1) = A_vel(0, 0);
        A(1, 3) = A_vel(0, 1);
        A(3, 1) = A_vel(1, 0);
        A(3, 3) = A_vel(1, 1);

        B.template block<1, 2>(1, 0) = B_vel.template block<1, 2>(0, 0);
        B.template block<1, 2>(3, 0) = B_vel.template block<1, 2>(1, 0);

        C(0, 0) = 1.0;
        C(1, 1) = 1.0;
        C(2, 2) = 1.0;
        C(3, 3) = 1.0;

        return ObserverPlant(A, B, C, ObserverPlant::MatrixD::Zero());
    }

    StateVector chassis_to_wheels(units::Velocity linear_velocity, units::AngularVelocity angular_velocity) const {
        const units::Length half_trackwidth = trackwidth_ * 0.5;
        const units::Velocity differential = angular_velocity * half_trackwidth / units::Angle::from<units::radian_tag>(1.0);

        return StateVector{
          (linear_velocity - differential).inps(),
          (linear_velocity + differential).inps(),
        };
    }

    StateVector chassis_to_wheels(double linear_velocity, double angular_velocity) const {
        return chassis_to_wheels(
          units::Velocity::from<units::inches_per_second_tag>(linear_velocity),
          units::AngularVelocity::from<units::radians_per_second_tag>(angular_velocity));
    }

    StateVector chassis_to_wheels(const TankChassisState &chassis_state) const {
        return chassis_to_wheels(chassis_state.linear, chassis_state.angular);
    }

    StateVector chassis_to_wheels(const StateVector &chassis_state) const {
        return chassis_to_wheels(chassis_state(0), chassis_state(1));
    }

    StateVector wheels_to_chassis(units::Velocity left_velocity, units::Velocity right_velocity) const {
        const units::Velocity linear = (left_velocity + right_velocity) * 0.5;
        const units::AngularVelocity angular =
          ((right_velocity - left_velocity) / trackwidth_) * units::Angle::from<units::radian_tag>(1.0);

        return StateVector{
          linear.inps(),
          angular.radps(),
        };
    }

    StateVector wheels_to_chassis(double left_velocity, double right_velocity) const {
        return wheels_to_chassis(
          units::Velocity::from<units::inches_per_second_tag>(left_velocity),
          units::Velocity::from<units::inches_per_second_tag>(right_velocity));
    }

    StateVector wheels_to_chassis(const StateVector &wheel_state) const {
        return wheels_to_chassis(wheel_state(0), wheel_state(1));
    }

    TankWheelState chassis_to_wheel_state(const TankChassisState &chassis_state) const {
        TankWheelState out;
        const units::Length half_trackwidth = trackwidth_ * 0.5;
        const units::Velocity differential =
          chassis_state.angular * half_trackwidth / units::Angle::from<units::radian_tag>(1.0);
        out.left = chassis_state.linear - differential;
        out.right = chassis_state.linear + differential;
        return out;
    }

    TankChassisState wheel_to_chassis_state(const TankWheelState &wheel_state) const {
        TankChassisState out;
        out.linear = (wheel_state.left + wheel_state.right) * 0.5;
        out.angular = ((wheel_state.right - wheel_state.left) / trackwidth_) * units::Angle::from<units::radian_tag>(1.0);
        return out;
    }

  private:
    units::Length trackwidth_;
    units::Voltage max_voltage_;
    LinearKV kV_linear_;
    LinearKA kA_linear_;
    AngularKV kV_angular_;
    AngularKA kA_angular_;
};
