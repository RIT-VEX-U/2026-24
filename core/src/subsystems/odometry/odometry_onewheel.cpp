#include "core/subsystems/odometry/odometry_onewheel.h"

OdometryOneWheel::OdometryOneWheel(
  vex::rotation *odompod, robot_specs_t &config, Translation2d offset, vex::inertial *imu, bool is_async
)
    : OdometryBase(is_async), offset(offset), odompod(odompod), imu(imu), config(config) {}

/**
 * Resets the position and rotational data to the input.
 *
 */
void OdometryOneWheel::set_position(const Pose2d &newpos) {
    mut.lock();
    rotation_offset = newpos.rotation().degrees() - (current_pos.rotation().degrees() - rotation_offset);
    mut.unlock();

    OdometryBase::set_position(newpos);
}

/**
 * Update, store and return the current position of the robot. Only use if not initializing
 * with a separate thread.
 */
Pose2d OdometryOneWheel::update() {
    double odompod_revs = odompod->position(vex::rotationUnits::rev);
    double angle = -imu->rotation(vex::rotationUnits::deg);
  

    // Offset the angle, if we've done a set_position
    // angle += rotation_offset;

    // Limit the angle betwen 0 and 360.
    // fmod (floating-point modulo) gets it between -359 and +359, so tack on another 360 if it's negative.
    angle = fmod(angle, 360.0);
    if (angle < 0) {
        angle += 360;
    }
  // printf("angle: %.2f\n", angle);

  double delta_angle = from_degrees(angle - last_angle).wrapped_degrees_180();

    // printf("imu rotation: %.2f\n", angle);
    double dist_driven = (odompod_revs) * PI * config.odom_wheel_diam;

    double dist_driven_delta = dist_driven - last_wheel_pos;

    current_pos = calculate_new_pos(config, current_pos, offset, dist_driven_delta, delta_angle);

  // std::cout << current_pos << std::endl;

    last_wheel_pos = dist_driven;

    static Pose2d old_pos = current_pos;
    static double last_speed = 0;
    static double last_ang_speed = 0;
    static timer tmr;
    bool update_vel_accel = tmr.time(sec) > 0.02;

    // This loop runs too fast. Only check at LEAST every 1/10th sec
    if (update_vel_accel) {
        // Calculate robot velocity
        double this_speed = current_pos.translation().distance(old_pos.translation()) / tmr.time(sec);
        ema.add_entry(this_speed);
        speed = ema.get_value();
        // Calculate robot acceleration
        accel = (speed - last_speed) / tmr.time(sec);

        // Calculate robot angular velocity (deg/sec)
        ang_speed_deg = smallest_angle(current_pos.rotation().degrees(), old_pos.rotation().degrees()) / tmr.time(sec);

        // Calculate robot angular acceleration (deg/sec^2)
        ang_accel_deg = (ang_speed_deg - last_ang_speed) / tmr.time(sec);

        tmr.reset();
        old_pos = current_pos;
        last_speed = speed;
        last_ang_speed = ang_speed_deg;
    }

  last_angle = angle;

    return current_pos;
}

/**
 * Using information about the robot's mechanical structure and sensors, calculate a new position
 * of the robot, relative to when this method was previously ran.
 */
Pose2d OdometryOneWheel::calculate_new_pos(
  robot_specs_t &config, Pose2d &old_pos, Translation2d offset, double dist_driven_delta, double angle_deg
) {

  double delta_theta = deg2rad(angle_deg);

  // printf("delta theta: %.2f\n", delta_theta.radians());

  double dx = dist_driven_delta - offset.y() * delta_theta;
  double dy = 0;

  Translation2d chg_point(dx, dy);
  Twist2d twist{dx, dy, delta_theta};

  // std::cout << twist << std::endl;

  Pose2d new_pos = old_pos.exp(twist);
  // std::cout << new_pos << std::endl;
  // new_pos = old_pos + transform;
  // new_pos = Pose2d(new_point, from_degrees(angle_deg));

  return new_pos;
}
