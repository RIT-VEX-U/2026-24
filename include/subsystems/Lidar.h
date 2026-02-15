#pragma once

#include "core/device/cobs_device.h"
#include "core/robot_specs.h"
#include "core/utils/math/geometry/pose2d.h"
#include "core/utils/math/geometry/rotation2d.h"
#include "core/utils/math/estimator/unscented_kalman_filter.h"
#include "core/utils/math/numerical/numerical_integration.h"
#include "logger/logger.h"
#include <cstdint>
#include <cmath>
#include <limits>
#include <vex_motorgroup.h>
#include <vex_task.h>

constexpr double FIELD_OFFSET = 0.5;
constexpr double FIELD_SIZE = 142.5;
constexpr double WALL_MIN = FIELD_OFFSET;
constexpr double WALL_MAX = FIELD_SIZE - (FIELD_OFFSET * 2);

constexpr double LIDAR_OFFSET_X = -4.5;
constexpr double LIDAR_OFFSET_Y = 6.2;
constexpr double LIDAR_OFFSET_ANGLE = M_PI;

// ughies
namespace lidar_ukf {
    EVec<3> dynamics(const EVec<3>& x, const EVec<3>& u);
    EVec<2> measurement(const EVec<3>& xhat, const EVec<3>& u);
    EVec<3> mean_state(const EMat<3, 5>& sigmas, const EVec<5>& Wm);
    EVec<2> mean_meas(const EMat<2, 5>& sigmas, const EVec<5>& Wm);
    EVec<3> residual_state(const EVec<3>& a, const EVec<3>& b);
    EVec<2> residual_meas(const EVec<2>& a, const EVec<2>& b);
    EVec<3> add_state(const EVec<3>& a, const EVec<3>& b);
    UnscentedKalmanFilter<3, 3, 2> createUKF();
}

class LidarReceiver : public COBSSerialDevice {
public:
    LidarReceiver(int port, int baudrate, vex::inertial *imu, vex::motor_group *left_motors, vex::motor_group *right_motors, robot_specs_t *config, SerialLogger *logger);

    void start();
    
    void stop();

    Pose2d get_internal_pose() const;

    Pose2d get_pose() const;
    
    void set_pose(const Pose2d& pose);

    void reset_ukf(const Pose2d& initial_pose);

    bool is_running() const { return running_; }

    double BEAM_TOLERANCE = 20;
private:
    vex::inertial *imu;
    vex::motor_group *left_motors;
    vex::motor_group *right_motors;
    robot_specs_t *config;
    SerialLogger *logger;

    vex::task* lidar_handle_ = nullptr;
    bool running_ = false;
    
    uint64_t last_predict_us_ = 0;
    
    // x [x, y, theta], u [vx, vy, omega], y [distance, lidar_angle]
    UnscentedKalmanFilter<3, 3, 2> ukf_;

    Pose2d pose_out_; // updates every 10ms for odom compatability

    EVec<3> get_robot_velocity(); // from encoders

    friend int lidar_thread(void* ptr);

};

int lidar_thread(void* ptr);
