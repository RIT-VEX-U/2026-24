#pragma once

#include "core/robot_specs.h"
#include "core/utils/math/estimator/unscented_kalman_filter.h"
#include "core/utils/math/eigen_interface.h"
#include "core/utils/math/geometry/rotation2d.h"
#include "core/utils/math/numerical/numerical_integration.h"
#include "vex.h"

/**
 * x = [pleft; vleft; pright; vright; log(kvl); log(kal); log(kva); log(kaa)]
 * u = [Vleft; Vright]
 *
 *
 */
class DriveParamUKF {
  public:

  DriveParamUKF(
    vex::motor_group *left_motors,
    vex::motor_group *right_motors,
    vex::inertial *imu,
    robot_specs_t *config,
    const EVec<8> &state_stddevs,
    const EVec<5> &meas_stddevs,
    const EVec<8> &initial_xhat,
    const EVec<8> &init_stddevs
  )
      : left_motors_(left_motors),
        right_motors_(right_motors),
        imu_(imu),
        config_(config),
        observer_(
          [this](const EVec<8> &x, const EVec<2> &u) { return this->f(x, u); },
          [this](const EVec<8> &x, const EVec<2> &u) { return this->h(x, u); },
          RK2_with_input<8, 2>,
          state_stddevs,
          meas_stddevs
        ) {
    observer_.set_xhat(initial_xhat);
    observer_.set_P(init_stddevs.cwiseProduct(init_stddevs).asDiagonal());
    handle_ = new vex::task(background_task, (void *)this);
  }

  ~DriveParamUKF() {
    end_async();
    vexDelay(20);
    delete handle_;
    handle_ = nullptr;
  }

  DriveParamUKF(const DriveParamUKF &) = delete;
  DriveParamUKF &operator=(const DriveParamUKF &) = delete;

  EVec<8> f(const EVec<8> &x, const EVec<2> &u) {
    EVec<8> xdot;
    const double kvl = (x(4));
    const double kal = (x(5));
    const double kva = (x(6));
    const double kaa = (x(7));

    const double A1 = -0.5 * ((kvl / kal) + (kva / kaa));
    const double A2 = -0.5 * ((kvl / kal) - (kva / kaa));
    const double B1 = 0.5 * ((1 / kal) + (1 / kaa));
    const double B2 = 0.5 * ((1 / kal) - (1 / kaa));
    // pleft
    xdot(0) = x(1);
    // vleft
    xdot(1) = A1 * x(1) + A2 * x(3) + B1 * u(0) + B2 * u(1);
    // pright
    xdot(2) = x(3);
    // vright
    xdot(3) = A2 * x(1) + A1 * x(3) + B2 * u(0) + B1 * u(1);

    xdot(4) = 0;
    xdot(5) = 0;
    xdot(6) = 0;
    xdot(7) = 0;

    return xdot;
  }

  // y = [pleft; vleft; pright; vright; gyrorate]
  EVec<5> h(const EVec<8> &x, const EVec<2> &u) {
    EVec<5> y;

    y(0) = x(0);
    y(1) = x(1);
    y(2) = x(2);
    y(3) = x(3);

    y(4) = (x(3) - x(1)) / config_->dist_between_wheels;

    return y;
  }

  EVec<5> measurements() {
    double left_rpm = left_motors_->velocity(vex::velocityUnits::rpm);
    double left_revs = left_motors_->position(vex::rotationUnits::rev);
    double right_rpm = right_motors_->velocity(vex::velocityUnits::rpm);
    double right_revs = right_motors_->position(vex::rotationUnits::rev);
    
    double wheel_circumference = M_PI * config_->odom_wheel_diam;

    double left_in = (left_revs) * wheel_circumference / config_->odom_gear_ratio;
    double left_vel_inps = (left_rpm / 60.0) * wheel_circumference / config_->odom_gear_ratio;
    double right_in = (right_revs) * wheel_circumference / config_->odom_gear_ratio;
    double right_vel_inps = (right_rpm / 60.0) * wheel_circumference / config_->odom_gear_ratio;

    double omega = deg2rad(imu_->gyroRate(vex::axisType::yaxis, vex::dps));
    return EVec<5>{left_in, left_vel_inps, right_in, right_vel_inps, omega};
  }

  void set_input_voltages(double left_volts, double right_volts) {
    last_u_ = {left_volts, right_volts};
  }

  void update_filter() {
    observer_.predict(last_u_, 0.01);
    observer_.correct(last_u_, measurements());
  }

  void end_async() { end_task_ = true; }

  void print_state() {
    printf(
      "pleft=%0.03f, vleft=%0.03f, pright=%0.03f, vright=%0.03f, kvl=%0.03f, kal=%0.03f, kva=%0.03f, kaa=%0.03f\n",
      observer_.xhat(0),
      observer_.xhat(1),
      observer_.xhat(2),
      observer_.xhat(3),
      (observer_.xhat(4)),
      (observer_.xhat(5)),
      (observer_.xhat(6)),
      (observer_.xhat(7))
    );
  }


  static int background_task(void *ptr) {
    DriveParamUKF &obj = *((DriveParamUKF *)ptr);
    vexDelay(10);
    while (!obj.end_task_) {
      obj.update_filter();
      vexDelay(10);
    }
    return 0;
  }

  private:
    vex::motor_group *left_motors_;
    vex::motor_group *right_motors_;
    vex::inertial *imu_;
    vex::task *handle_ = nullptr;
    EVec<2> last_u_ = EVec<2>::Zero();
    bool end_task_ = false;
    robot_specs_t *config_;
    UnscentedKalmanFilter<8, 2, 5> observer_;
};
