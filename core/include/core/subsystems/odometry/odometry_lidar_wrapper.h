#pragma once

#include "core/subsystems/odometry/odometry_base.h"
#include "core/utils/math_util.h"
#include "subsystems/Lidar.h"

/**
 * OdometryNWheel
 *
 * This class handles the code for an N-pod odometry setup, where there are N <WHEELS> free spinning omni wheels
 * (dead wheels) placed in any known configuration on the robot.
 *
 * Example of a possible wheel configuration:
 *
 *  +Y   ---------------
 *  ^    |    ===      |
 *  |    |             |
 *  |    | ||   O      |
 *  |    | ||          |
 *  |    |      ===    |
 *  |    ---------------
 *  |
 *  +-------------------> + X
 *
 * Where O is the center of rotation. The robot will monitor the changes in rotation of these wheels, use this to
 * calculate a pose delta, then integrate the deltas over time to determine the robot's position.
 *
 * This is a "set and forget" class, meaning once the object is created, the robot will immediately begin
 * tracking it's movement in the background.
 *
 * https://rit.enterprise.slack.com/files/U04112Y5RB6/F080M01KPA5/predictperpindiculars2.pdf
 * 2024-2025 Notebook: Entries/Software Entries/Localization/N-Pod Odometry
 *
 * @author Jack Cammarata, Richie Sommers
 * @date Nov 14 2024
 *
 */

/**
 * tracking_wheel_cfg_t holds all the specifications for a single tracking wheel
 * The units for x, y, and radius will determine the units of the position estimate
 */

class OdometryLidarWrapper : public OdometryBase {
public:
  /**
   * Construct a new Odometry N Wheel object
   *
   * @param encoders std::array containing each CustomEncoder
   * @param wheel_configs std::array containing each tracking_wheel_cfg
   * @param imu when passed in, only uses imu for rotation measurement
   * @param is_async true to constantly run in the background
   */
  OdometryLidarWrapper(
    LidarReceiver *lidar
  )
      : OdometryBase(true), lidar_(lidar) {

  }

  /**
   * Update the current position of the robot once, using the current state of
   * the encoders and the previous known location
   *
   * @return the robot's updated position
   */
  Pose2d update() override {

    Pose2d updated_pos = lidar_->get_pose();

    static Pose2d last_pos = updated_pos;
    static double last_speed = 0;
    static double last_ang_speed = 0;
    static timer tmr;

    double speed_local = 0;
    double accel_local = 0;
    double ang_speed_local = 0;
    double ang_accel_local = 0;
    bool update_vel_accel = tmr.time(sec) > 0.1;

    // This loop runs too fast. Only check at LEAST every 1/10th sec
    if (update_vel_accel) {
      // Calculate robot velocity
      speed_local = updated_pos.translation().distance(last_pos.translation()) / tmr.time(sec);

      // Calculate robot acceleration
      accel_local = (speed_local - last_speed) / tmr.time(sec);

      // Calculate robot angular velocity (deg/sec)
      ang_speed_local = smallest_angle(updated_pos.rotation().degrees(), last_pos.rotation().degrees()) / tmr.time(sec);

      // Calculate robot angular acceleration (deg/sec^2)
      ang_accel_local = (ang_speed_local - last_ang_speed) / tmr.time(sec);

      tmr.reset();
      last_pos = updated_pos;
      last_speed = speed_local;
      last_ang_speed = ang_speed_local;
    }

    this->current_pos = updated_pos;
    if (update_vel_accel) {
      this->speed = speed_local;
      this->accel = accel_local;
      this->ang_speed_deg = ang_speed_local;
      this->ang_accel_deg = ang_accel_local;
    }

    return current_pos;
  }

  /**
   * Resets the position and rotational data to the input.
   */
  void set_position(const Pose2d &newpos) override {
    lidar_->set_pose(newpos);
    OdometryBase::set_position(newpos);
  }

  /**
   * Gets the current position and rotation
   * @return the position that the odometry believes the robot is at
   */
  Pose2d get_position(void) override {
    Pose2d without_wrapped_angle  = OdometryBase::get_position();
    Pose2d with_wrapped_angle(without_wrapped_angle.translation(),  without_wrapped_angle.rotation().wrapped_radians_360());
    return with_wrapped_angle;
  }

private:
  LidarReceiver *lidar_;
};
