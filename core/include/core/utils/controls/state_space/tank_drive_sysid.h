#pragma once

#include <vector>

#include "core/subsystems/tank_drive.h"
#include "core/utils/controls/state_space/tank_drive_model.h"

struct TankDriveSysIdConfig {
    double sample_dt = 0.01;
    double settle_time = 0.25;
    double pause_time = 0.5;

    double linear_quasistatic_ramp_rate = 0.5; // volts / second
    double linear_quasistatic_max_voltage = 8.0;
    double linear_dynamic_voltage = 8.0;
    double linear_dynamic_duration = 1.5;

    double angular_quasistatic_ramp_rate = 0.5; // volts / second
    double angular_quasistatic_max_voltage = 8.0;
    double angular_dynamic_voltage = 8.0;
    double angular_dynamic_duration = 1.5;

    double min_linear_velocity = 2.0;   // in / s
    double min_angular_velocity = 0.25; // rad / s
    int velocity_filter_size = 5;
    int accel_filter_size = 5;
};

struct TankDriveSysIdSample {
    double t = 0.0;
    double left_voltage = 0.0;
    double right_voltage = 0.0;
    double linear_velocity = 0.0;
    double linear_accel = 0.0;
    double angular_velocity = 0.0;
    double angular_accel = 0.0;
};

struct TankDriveAxisSysIdResult {
    double kV = 0.0;
    double kA = 0.0;
    double quasistatic_intercept = 0.0;
    double dynamic_intercept = 0.0;
    int quasistatic_points = 0;
    int dynamic_points = 0;
};

struct TankDriveSysIdResult {
    TankDriveAxisSysIdResult linear;
    TankDriveAxisSysIdResult angular;

    std::vector<TankDriveSysIdSample> linear_quasistatic_samples;
    std::vector<TankDriveSysIdSample> linear_dynamic_samples;
    std::vector<TankDriveSysIdSample> angular_quasistatic_samples;
    std::vector<TankDriveSysIdSample> angular_dynamic_samples;
};

std::vector<TankDriveSysIdSample>
run_linear_quasistatic_sysid(TankDrive &drive, const TankDriveModel &model, const TankDriveSysIdConfig &cfg = TankDriveSysIdConfig());

std::vector<TankDriveSysIdSample>
run_linear_dynamic_sysid(TankDrive &drive, const TankDriveModel &model, const TankDriveSysIdConfig &cfg = TankDriveSysIdConfig());

std::vector<TankDriveSysIdSample>
run_angular_quasistatic_sysid(TankDrive &drive, const TankDriveModel &model, const TankDriveSysIdConfig &cfg = TankDriveSysIdConfig());

std::vector<TankDriveSysIdSample>
run_angular_dynamic_sysid(TankDrive &drive, const TankDriveModel &model, const TankDriveSysIdConfig &cfg = TankDriveSysIdConfig());

TankDriveAxisSysIdResult characterize_linear_axis(
  const std::vector<TankDriveSysIdSample> &quasistatic_samples,
  const std::vector<TankDriveSysIdSample> &dynamic_samples,
  const TankDriveSysIdConfig &cfg = TankDriveSysIdConfig());

TankDriveAxisSysIdResult characterize_angular_axis(
  const std::vector<TankDriveSysIdSample> &quasistatic_samples,
  const std::vector<TankDriveSysIdSample> &dynamic_samples,
  const TankDriveSysIdConfig &cfg = TankDriveSysIdConfig());

TankDriveSysIdResult
characterize_tank_drive(TankDrive &drive, const TankDriveModel &model, const TankDriveSysIdConfig &cfg = TankDriveSysIdConfig());
