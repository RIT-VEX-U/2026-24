#pragma once

#include "core/subsystems/odometry/odometry_base.h"
#include "core/utils/geometry.h"
#include "core/utils/moving_average.h"
#include "core/robot_specs.h"

/**
 * OdometryTank defines an odometry system for a tank drivetrain
 * This requires encoders in the same orientation as the drive wheels
 * Odometry is a "start and forget" subsystem, which means once it's created and configured,
 * it will constantly run in the background and track the robot's X, Y and rotation coordinates.
 */
class OdometryOneWheel : public OdometryBase {
  public:
    OdometryOneWheel(
      vex::rotation *odompod, robot_specs_t &config, Translation2d offset, vex::inertial *imu = NULL,
      bool is_async = true
    );


    /**
     * Update the current position on the field based on the sensors
     * @return the position that odometry has calculated itself to be at
     */
    Pose2d update() override;

    /**
     * set_position tells the odometry to place itself at a position
     * @param newpos the position the odometry will take
     */
    void set_position(const Pose2d &newpos = zero_pos) override;

  private:
    /**
     * Get information from the input hardware and an existing position, and calculate a new current position
     */
    static Pose2d calculate_new_pos(robot_specs_t &config, Pose2d &old_pos, Translation2d offset, double odompod_revs, double angle_deg);

    ExponentialMovingAverage ema = ExponentialMovingAverage(3);
    Translation2d offset;

    vex::rotation *odompod;
    vex::inertial *imu;
    robot_specs_t &config;

    double rotation_offset = 0;
    double last_wheel_pos = 0;
    double last_angle = 0;
};
