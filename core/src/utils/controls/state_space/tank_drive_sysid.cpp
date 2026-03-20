#include "core/utils/controls/state_space/tank_drive_sysid.h"

#include <cmath>
#include <utility>

#include "core/utils/math_util.h"
#include "core/utils/moving_average.h"

namespace {
constexpr double kEps = 1e-9;

double clamp_voltage(double volts, double max_voltage) {
    if (volts > max_voltage) {
        return max_voltage;
    }
    if (volts < -max_voltage) {
        return -max_voltage;
    }
    return volts;
}

double linear_velocity(TankDrive &drive) {
    return 0.5 * (drive.raw_left_velocity() + drive.raw_right_velocity());
}

double angular_velocity(TankDrive &drive, const TankDriveModel &model) {
    return (drive.raw_right_velocity() - drive.raw_left_velocity()) / model.trackwidth().in();
}

void settle_drive(TankDrive &drive, double settle_time) {
    drive.stop();
    vexDelay(static_cast<int>(settle_time * 1000.0));
}

TankDriveSysIdSample make_sample(
  double t,
  double left_voltage,
  double right_voltage,
  double linear_vel,
  double linear_accel,
  double angular_vel,
  double angular_accel) {
    TankDriveSysIdSample sample;
    sample.t = t;
    sample.left_voltage = left_voltage;
    sample.right_voltage = right_voltage;
    sample.linear_velocity = linear_vel;
    sample.linear_accel = linear_accel;
    sample.angular_velocity = angular_vel;
    sample.angular_accel = angular_accel;
    return sample;
}

std::vector<TankDriveSysIdSample> run_profile(
  TankDrive &drive,
  const TankDriveModel &model,
  const TankDriveSysIdConfig &cfg,
  bool quasistatic,
  bool angular) {
    std::vector<TankDriveSysIdSample> samples;
    settle_drive(drive, cfg.settle_time);

    const double dt = cfg.sample_dt;
    const double max_voltage = model.max_voltage().V();
    const double ramp_rate = angular ? cfg.angular_quasistatic_ramp_rate : cfg.linear_quasistatic_ramp_rate;
    const double quasistatic_limit = angular ? cfg.angular_quasistatic_max_voltage : cfg.linear_quasistatic_max_voltage;
    const double dynamic_voltage = angular ? cfg.angular_dynamic_voltage : cfg.linear_dynamic_voltage;
    const double dynamic_duration = angular ? cfg.angular_dynamic_duration : cfg.linear_dynamic_duration;
    const int velocity_filter_size = std::max(cfg.velocity_filter_size, 1);
    const int accel_filter_size = std::max(cfg.accel_filter_size, 1);

    MovingAverage linear_vel_filter(velocity_filter_size);
    MovingAverage linear_accel_filter(accel_filter_size);
    MovingAverage angular_vel_filter(velocity_filter_size);
    MovingAverage angular_accel_filter(accel_filter_size);

    vex::timer timer;
    double last_linear_vel = linear_velocity(drive);
    double last_angular_vel = angular_velocity(drive, model);

    while (true) {
        const double t = timer.time(vex::sec);
        double left_voltage = 0.0;
        double right_voltage = 0.0;

        if (quasistatic) {
            double commanded = clamp_voltage(ramp_rate * t, std::min(quasistatic_limit, max_voltage));
            if (angular) {
                left_voltage = -commanded;
                right_voltage = commanded;
            } else {
                left_voltage = commanded;
                right_voltage = commanded;
            }
            if (commanded >= std::min(quasistatic_limit, max_voltage) - kEps) {
                drive.drive_tank_voltage(left_voltage, right_voltage);
                vexDelay(static_cast<int>(dt * 1000.0));
                break;
            }
        } else {
            double commanded = clamp_voltage(dynamic_voltage, max_voltage);
            if (angular) {
                left_voltage = -commanded;
                right_voltage = commanded;
            } else {
                left_voltage = commanded;
                right_voltage = commanded;
            }
            if (t >= dynamic_duration) {
                break;
            }
        }

        drive.drive_tank_voltage(left_voltage, right_voltage);
        vexDelay(static_cast<int>(dt * 1000.0));

        const double raw_linear_vel = linear_velocity(drive);
        const double raw_angular_vel = angular_velocity(drive, model);
        linear_vel_filter.add_entry(raw_linear_vel);
        angular_vel_filter.add_entry(raw_angular_vel);

        const double filtered_linear_vel = linear_vel_filter.get_value();
        const double filtered_angular_vel = angular_vel_filter.get_value();
        linear_accel_filter.add_entry((filtered_linear_vel - last_linear_vel) / dt);
        angular_accel_filter.add_entry((filtered_angular_vel - last_angular_vel) / dt);

        last_linear_vel = filtered_linear_vel;
        last_angular_vel = filtered_angular_vel;

        samples.push_back(make_sample(
          t,
          left_voltage,
          right_voltage,
          filtered_linear_vel,
          linear_accel_filter.get_value(),
          filtered_angular_vel,
          angular_accel_filter.get_value()));
    }

    drive.stop();
    vexDelay(static_cast<int>(cfg.pause_time * 1000.0));
    return samples;
}

TankDriveAxisSysIdResult characterize_axis(
  const std::vector<TankDriveSysIdSample> &quasistatic_samples,
  const std::vector<TankDriveSysIdSample> &dynamic_samples,
  double min_velocity,
  bool angular) {
    TankDriveAxisSysIdResult result;

    std::vector<std::pair<double, double>> quasistatic_points;
    for (size_t i = 0; i < quasistatic_samples.size(); ++i) {
        const TankDriveSysIdSample &sample = quasistatic_samples[i];
        const double velocity = angular ? sample.angular_velocity : sample.linear_velocity;
        if (std::abs(velocity) < min_velocity) {
            continue;
        }
        const double voltage = angular ? 0.5 * (sample.right_voltage - sample.left_voltage)
                                       : 0.5 * (sample.right_voltage + sample.left_voltage);
        quasistatic_points.push_back(std::make_pair(std::abs(velocity), std::abs(voltage)));
    }

    if (quasistatic_points.size() >= 2) {
        std::pair<double, double> fit = calculate_linear_regression(quasistatic_points);
        result.kV = fit.first;
        result.quasistatic_intercept = fit.second;
        result.quasistatic_points = static_cast<int>(quasistatic_points.size());
    }

    std::vector<std::pair<double, double>> dynamic_points;
    for (size_t i = 0; i < dynamic_samples.size(); ++i) {
        const TankDriveSysIdSample &sample = dynamic_samples[i];
        const double velocity = angular ? sample.angular_velocity : sample.linear_velocity;
        const double accel = angular ? sample.angular_accel : sample.linear_accel;
        if (std::abs(velocity) < min_velocity || std::abs(accel) < kEps) {
            continue;
        }
        const double voltage = angular ? 0.5 * (sample.right_voltage - sample.left_voltage)
                                       : 0.5 * (sample.right_voltage + sample.left_voltage);
        dynamic_points.push_back(std::make_pair(std::abs(accel), std::abs(voltage) - result.kV * std::abs(velocity)));
    }

    if (dynamic_points.size() >= 2) {
        std::pair<double, double> fit = calculate_linear_regression(dynamic_points);
        result.kA = fit.first;
        result.dynamic_intercept = fit.second;
        result.dynamic_points = static_cast<int>(dynamic_points.size());
    }

    return result;
}
} // namespace

std::vector<TankDriveSysIdSample>
run_linear_quasistatic_sysid(TankDrive &drive, const TankDriveModel &model, const TankDriveSysIdConfig &cfg) {
    return run_profile(drive, model, cfg, true, false);
}

std::vector<TankDriveSysIdSample>
run_linear_dynamic_sysid(TankDrive &drive, const TankDriveModel &model, const TankDriveSysIdConfig &cfg) {
    return run_profile(drive, model, cfg, false, false);
}

std::vector<TankDriveSysIdSample>
run_angular_quasistatic_sysid(TankDrive &drive, const TankDriveModel &model, const TankDriveSysIdConfig &cfg) {
    return run_profile(drive, model, cfg, true, true);
}

std::vector<TankDriveSysIdSample>
run_angular_dynamic_sysid(TankDrive &drive, const TankDriveModel &model, const TankDriveSysIdConfig &cfg) {
    return run_profile(drive, model, cfg, false, true);
}

TankDriveAxisSysIdResult characterize_linear_axis(
  const std::vector<TankDriveSysIdSample> &quasistatic_samples,
  const std::vector<TankDriveSysIdSample> &dynamic_samples,
  const TankDriveSysIdConfig &cfg) {
    return characterize_axis(quasistatic_samples, dynamic_samples, cfg.min_linear_velocity, false);
}

TankDriveAxisSysIdResult characterize_angular_axis(
  const std::vector<TankDriveSysIdSample> &quasistatic_samples,
  const std::vector<TankDriveSysIdSample> &dynamic_samples,
  const TankDriveSysIdConfig &cfg) {
    return characterize_axis(quasistatic_samples, dynamic_samples, cfg.min_angular_velocity, true);
}

TankDriveSysIdResult
characterize_tank_drive(TankDrive &drive, const TankDriveModel &model, const TankDriveSysIdConfig &cfg) {
    TankDriveSysIdResult result;
    result.linear_quasistatic_samples = run_linear_quasistatic_sysid(drive, model, cfg);
    vexDelay(20000);
    result.linear_dynamic_samples = run_linear_dynamic_sysid(drive, model, cfg);
    vexDelay(20000);
    result.angular_quasistatic_samples = run_angular_quasistatic_sysid(drive, model, cfg);
    vexDelay(5000);
    result.angular_dynamic_samples = run_angular_dynamic_sysid(drive, model, cfg);

    result.linear = characterize_linear_axis(result.linear_quasistatic_samples, result.linear_dynamic_samples, cfg);
    result.angular = characterize_angular_axis(result.angular_quasistatic_samples, result.angular_dynamic_samples, cfg);
    return result;
}
