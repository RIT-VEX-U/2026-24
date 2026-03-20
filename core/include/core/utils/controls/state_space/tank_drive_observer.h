#pragma once

#include "core/units/units.h"
#include "core/utils/controls/state_space/tank_drive_model.h"
#include "core/utils/math/estimator/kalman_filter.h"
#include "vex.h"
#include <functional>

class TankDriveObserver {
  public:
    typedef TankDriveModel::ObserverPlant Plant;
    typedef Plant::VectorX StateVector;
    typedef Plant::VectorU InputVector;
    typedef Plant::VectorY OutputVector;
    typedef std::function<void(units::Length &, units::Velocity &, units::Length &, units::Velocity &)> MeasurementProvider;

    TankDriveObserver(
      const TankDriveModel &model,
      units::Time dt,
      const StateVector &state_stddevs = default_state_stddevs(),
        const OutputVector &measurement_stddevs = default_measurement_stddevs())
        : model_(model), dt_(dt), plant_(model.wheel_position_plant()), filter_(plant_, state_stddevs, measurement_stddevs) {}
    TankDriveObserver(
      const TankDriveModel &model,
      double dt,
      const StateVector &state_stddevs = default_state_stddevs(),
        const OutputVector &measurement_stddevs = default_measurement_stddevs())
        : TankDriveObserver(model, units::Time::from<units::second_tag>(dt), state_stddevs, measurement_stddevs) {}
    ~TankDriveObserver() {
        end_async();
        vexDelay(static_cast<int>(dt_.s() * 2000.0));
        delete handle_;
        handle_ = NULL;
    }

    TankDriveObserver(const TankDriveObserver &) = delete;
    TankDriveObserver &operator=(const TankDriveObserver &) = delete;

    void reset(
      units::Length left_pos,
      units::Length right_pos,
      units::Velocity left_vel = units::Velocity(),
      units::Velocity right_vel = units::Velocity()) {
        StateVector xhat{
          left_pos.in(),
          left_vel.inps(),
          right_pos.in(),
          right_vel.inps(),
        };
        filter_.reset();
        filter_.set_xhat(xhat);
        last_u_.setZero();
        initialized_ = true;
    }
    void reset(double left_pos, double right_pos, double left_vel = 0.0, double right_vel = 0.0) {
        reset(
          units::Length::from<units::inch_tag>(left_pos),
          units::Length::from<units::inch_tag>(right_pos),
          units::Velocity::from<units::inches_per_second_tag>(left_vel),
          units::Velocity::from<units::inches_per_second_tag>(right_vel));
    }

    bool initialized() const { return initialized_; }

    void set_measurement_provider(MeasurementProvider provider) { measurement_provider_ = provider; }

    bool has_measurement_provider() const { return static_cast<bool>(measurement_provider_); }

    void start_async() {
        if (handle_ != NULL) {
            return;
        }
        end_task_ = false;
        handle_ = new vex::task(background_task, (void *)this);
    }

    void end_async() { end_task_ = true; }

    void update_from_provider() {
        if (!measurement_provider_) {
            return;
        }
        units::Length left_pos;
        units::Velocity left_vel;
        units::Length right_pos;
        units::Velocity right_vel;
        measurement_provider_(left_pos, left_vel, right_pos, right_vel);
        update(left_pos, left_vel, right_pos, right_vel);
    }

    void set_input_voltages(units::Voltage left_volts, units::Voltage right_volts) {
        last_u_ = InputVector{
          clamp_voltage(left_volts).V(),
          clamp_voltage(right_volts).V(),
        };
    }
    void set_input_voltages(double left_volts, double right_volts) {
        set_input_voltages(
          units::Voltage::from<units::volt_tag>(left_volts),
          units::Voltage::from<units::volt_tag>(right_volts));
    }

    void predict(units::Voltage left_volts, units::Voltage right_volts) {
        set_input_voltages(left_volts, right_volts);
        filter_.predict(last_u_, dt_.s());
    }
    void predict(double left_volts, double right_volts) {
        predict(
          units::Voltage::from<units::volt_tag>(left_volts),
          units::Voltage::from<units::volt_tag>(right_volts));
    }

    void correct(units::Length left_pos, units::Velocity left_vel, units::Length right_pos, units::Velocity right_vel) {
        OutputVector y{
          left_pos.in(),
          left_vel.inps(),
          right_pos.in(),
          right_vel.inps(),
        };

        filter_.correct(y, last_u_);
    }
    void correct(double left_pos, double left_vel, double right_pos, double right_vel) {
        correct(
          units::Length::from<units::inch_tag>(left_pos),
          units::Velocity::from<units::inches_per_second_tag>(left_vel),
          units::Length::from<units::inch_tag>(right_pos),
          units::Velocity::from<units::inches_per_second_tag>(right_vel));
    }

    void update(
      units::Voltage left_volts,
      units::Voltage right_volts,
      units::Length left_pos,
      units::Velocity left_vel,
      units::Length right_pos,
      units::Velocity right_vel
    ) {
        if (!initialized_) {
            reset(left_pos, right_pos, left_vel, right_vel);
            return;
        }
        predict(left_volts, right_volts);
        correct(left_pos, left_vel, right_pos, right_vel);
    }
    void update(
      double left_volts, double right_volts, double left_pos, double left_vel, double right_pos, double right_vel
    ) {
        update(
          units::Voltage::from<units::volt_tag>(left_volts),
          units::Voltage::from<units::volt_tag>(right_volts),
          units::Length::from<units::inch_tag>(left_pos),
          units::Velocity::from<units::inches_per_second_tag>(left_vel),
          units::Length::from<units::inch_tag>(right_pos),
          units::Velocity::from<units::inches_per_second_tag>(right_vel));
    }

    void update(units::Length left_pos, units::Velocity left_vel, units::Length right_pos, units::Velocity right_vel) {
        if (!initialized_) {
            reset(left_pos, right_pos, left_vel, right_vel);
            return;
        }
        filter_.predict(last_u_, dt_.s());
        correct(left_pos, left_vel, right_pos, right_vel);
    }
    void update(double left_pos, double left_vel, double right_pos, double right_vel) {
        update(
          units::Length::from<units::inch_tag>(left_pos),
          units::Velocity::from<units::inches_per_second_tag>(left_vel),
          units::Length::from<units::inch_tag>(right_pos),
          units::Velocity::from<units::inches_per_second_tag>(right_vel));
    }

    units::Length left_position() const { return units::Length::from<units::inch_tag>(filter_.xhat(0)); }
    units::Velocity left_velocity() const { return units::Velocity::from<units::inches_per_second_tag>(filter_.xhat(1)); }
    units::Length right_position() const { return units::Length::from<units::inch_tag>(filter_.xhat(2)); }
    units::Velocity right_velocity() const { return units::Velocity::from<units::inches_per_second_tag>(filter_.xhat(3)); }

    TankWheelState wheel_state() const {
        TankWheelState out;
        out.left = left_velocity();
        out.right = right_velocity();
        return out;
    }
    TankChassisState chassis_state() const { return model_.wheel_to_chassis_state(wheel_state()); }

    const StateVector &xhat() const { return filter_.xhat(); }

    static StateVector default_state_stddevs() {
        StateVector q{0.1, 2.0, 0.1, 2.0};
        return q;
    }

    static OutputVector default_measurement_stddevs() {
        OutputVector r{0.02, 2.0, 0.02, 2.0};
        return r;
    }

    static int background_task(void *ptr) {
        TankDriveObserver &obj = *((TankDriveObserver *)ptr);
        vexDelay(10);
        while (!obj.end_task_) {
            obj.update_from_provider();
            vexDelay(static_cast<int>(obj.dt_.s() * 1000.0));
        }
        return 0;
    }

  private:
    units::Voltage clamp_voltage(units::Voltage volts) const {
        const units::Voltage max_voltage = model_.max_voltage();
        if (volts > max_voltage) {
            return max_voltage;
        }
        if (volts < -max_voltage) {
            return -max_voltage;
        }
        return volts;
    }

    const TankDriveModel &model_;
    units::Time dt_ = units::Time::from<units::second_tag>(0.01);
    Plant plant_;
    KalmanFilter<4, 2, 4> filter_;
    InputVector last_u_ = InputVector::Zero();
    bool initialized_ = false;
    MeasurementProvider measurement_provider_;
    vex::task *handle_ = NULL;
    bool end_task_ = false;
};
