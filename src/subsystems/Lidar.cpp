#include "robot-config.h"
#include "subsystems/Lidar.h"
#include "logger/logger.h"
#include <cstring>
#include <cmath>
#include <v5_api.h>
#include <v5_apiuser.h>
#include <vex_global.h>
#include <vex_units.h>

namespace {
    // somehow the one in rotation2d is fucked
    inline double wrap_radians(double angle) {
        while (angle > M_PI) angle -= 2.0 * M_PI;
        while (angle < -M_PI) angle += 2.0 * M_PI;
        return angle;
    }
}

namespace lidar_ukf {
    // all this does for now is transform from robot to field frame
    // should be the same regardless of data source, but odom would be better
    EVec<3> dynamics(const EVec<3>& x, const EVec<3>& u) {
        double theta = x(2);
        double c = std::cos(theta);
        double s = std::sin(theta);
        
        double vx_field = u(0) * c - u(1) * s;
        double vy_field = u(0) * s + u(1) * c;
        
        return EVec<3>{vx_field, vy_field, u(2)};
    }
    
    EVec<2> measurement(const EVec<3>& xhat, const EVec<3>& u) {
        double robot_x = xhat(0);
        double robot_y = xhat(1);
        double robot_theta = xhat(2);
        
        // transofrm lidar position to field
        double cos_theta = std::cos(robot_theta);
        double sin_theta = std::sin(robot_theta);
        double lidar_x = robot_x + LIDAR_OFFSET_X * cos_theta - LIDAR_OFFSET_Y * sin_theta;
        double lidar_y = robot_y + LIDAR_OFFSET_X * sin_theta + LIDAR_OFFSET_Y * cos_theta;
        
        // direction in field frame
        double beam_theta = robot_theta + LIDAR_OFFSET_ANGLE + u(1) * M_PI / 180.0;
        
        double c = std::cos(beam_theta);
        double s = std::sin(beam_theta);
        
        double d_left = (c < 0) ? ((lidar_x - WALL_MIN) / -c) : 1e9;
        double d_right = (c > 0) ? ((WALL_MAX - lidar_x) / c) : 1e9;
        double d_bottom = (s < 0) ? ((lidar_y - WALL_MIN) / -s) : 1e9;
        double d_top = (s > 0) ? ((WALL_MAX - lidar_y) / s) : 1e9;
        
        double min_dist = std::min({d_left, d_right, d_bottom, d_top});
        
        return EVec<2>{min_dist, u(1)};
    }
    
    EVec<3> mean_state(const EMat<3, 5>& sigmas, const EVec<5>& Wm) {
        EVec<3> x = EVec<3>::Zero();
        double c = 0, s = 0;
        
        for (int i = 0; i < 5; i++) {
            x(0) += sigmas(0, i) * Wm(i);
            x(1) += sigmas(1, i) * Wm(i);
            c += std::cos(sigmas(2, i)) * Wm(i);
            s += std::sin(sigmas(2, i)) * Wm(i);
        }
        x(2) = std::atan2(s, c);
        return x;
    }
    
    EVec<2> mean_meas(const EMat<2, 5>& sigmas, const EVec<5>& Wm) {
        EVec<2> y = EVec<2>::Zero();
        double c = 0, s = 0;
        
        for (int i = 0; i < 5; i++) {
            y(0) += sigmas(0, i) * Wm(i);
            c += std::cos(sigmas(1, i) * M_PI / 180.0) * Wm(i);
            s += std::sin(sigmas(1, i) * M_PI / 180.0) * Wm(i);
        }
        y(1) = std::atan2(s, c) * 180.0 / M_PI;
        return y;
    }
    
    EVec<3> residual_state(const EVec<3>& a, const EVec<3>& b) {
        return EVec<3>{a(0) - b(0), a(1) - b(1), wrap_radians(a(2) - b(2))};
    }
    
    EVec<2> residual_meas(const EVec<2>& a, const EVec<2>& b) {
        double angle_diff = a(1) - b(1);
        while (angle_diff > 180.0) angle_diff -= 360.0;
        while (angle_diff < -180.0) angle_diff += 360.0;
        return EVec<2>{a(0) - b(0), angle_diff};
    }
    
    EVec<3> add_state(const EVec<3>& a, const EVec<3>& b) {
        return EVec<3>{a(0) + b(0), a(1) + b(1), wrap_radians(a(2) + b(2))};
    }
    
    UnscentedKalmanFilter<3, 3, 2> createUKF() {
        EVec<3> state_stddevs{2.0, 2.0, 0.0000001};
        
        EVec<2> measurement_stddevs{30, 30};
        
        return UnscentedKalmanFilter<3, 3, 2>(
            dynamics, measurement, RK2_with_input<3, 3>,
            state_stddevs, measurement_stddevs,
            mean_state, mean_meas,
            residual_state, residual_meas, add_state
        );
    }
}

LidarReceiver::LidarReceiver(int port, int baudrate, vex::inertial *imu, vex::motor_group *left_motors, vex::motor_group *right_motors, robot_specs_t *config, SerialLogger *logger) 
    : COBSSerialDevice(port, baudrate), imu(imu), left_motors(left_motors), right_motors(right_motors), config(config), logger(logger), running_(false), ukf_(lidar_ukf::createUKF()) {
    // start at center
    EVec<3> initial_state{72, 72, 0};
    ukf_.set_xhat(initial_state);
    
    // high trust of angle
    EMat<3, 3> initialP{{2, 0, 0},
                        {0, 2, 0},
                        {0, 0, 0.00025}};
    ukf_.set_P(initialP);
}

void LidarReceiver::start() {
    if (!running_) {
        running_ = true;
        last_predict_us_ = vexSystemHighResTimeGet() - init_us;
        lidar_handle_ = new vex::task{lidar_thread, (void*)this};
    }
}

void LidarReceiver::stop() {
    running_ = false;
}

Pose2d LidarReceiver::get_internal_pose() const {
    return Pose2d(ukf_.xhat(0), ukf_.xhat(1), from_radians(ukf_.xhat(2)));
}

Pose2d LidarReceiver::get_pose() const {
    return pose_out_;
}

void LidarReceiver::set_pose(const Pose2d& pose) {
    EVec<3> state;
    state << pose.x(), pose.y(), pose.rotation().radians();
    ukf_.set_xhat(state);
    pose_out_ = pose;
}

void LidarReceiver::reset_ukf(const Pose2d& initial_pose) {
    set_pose(initial_pose);
    
    // high trust of angle
    EMat<3, 3> initialP{{2, 0, 0},
                        {0, 2, 0},
                        {0, 0, 0.00025}};
    ukf_.set_P(initialP);
    
    last_predict_us_ = vexSystemHighResTimeGet() - init_us;
}

EVec<3> LidarReceiver::get_robot_velocity() {
    double left_rpm = left_motors->velocity(vex::velocityUnits::rpm);
    double right_rpm = right_motors->velocity(vex::velocityUnits::rpm);
    
    double wheel_circumference = M_PI * config->odom_wheel_diam;
    double left_vel = (left_rpm / 60.0) * wheel_circumference / config->odom_gear_ratio;
    double right_vel = (right_rpm / 60.0) * wheel_circumference / config->odom_gear_ratio;
    
    double vx = (left_vel + right_vel) / 2.0;
    double vy = 0.0;
    double omega = deg2rad(imu->gyroRate(vex::axisType::yaxis, vex::dps));
    
    return EVec<3>{vx, vy, omega};
}

int lidar_thread(void* ptr) {
    LidarReceiver& obj = *((LidarReceiver*)ptr);
    
    vexDelay(100);
    
    obj.logger->define_and_send_schema(0x04, "time:u64,d:f32,a:f32");

    int i = 0;
    while (obj.running_) {
        uint64_t now_us = vexSystemHighResTimeGet() - init_us;
        
        // minimum 10ms encoder update
        double dt_s = (now_us - obj.last_predict_us_) / 1.0e6;
        if (dt_s >= 0.01) {
            EVec<3> velocity = obj.get_robot_velocity();
            obj.ukf_.predict(velocity, dt_s);
            obj.last_predict_us_ = now_us;
            obj.pose_out_ = obj.get_internal_pose();
        }
        
        // Poll for incoming lidar data
        if (obj.poll_incoming_data_once()) {
            // Got a complete packet, decode it
            auto packet = obj.get_last_decoded_packet();
            
            if (packet.size() != 4) {
                continue;
            }
            
            uint16_t angle_q6;
            uint16_t dist;
            memcpy(&angle_q6, packet.data(), sizeof(angle_q6));
            memcpy(&dist, packet.data() + sizeof(angle_q6), sizeof(dist));
            
            double angle = fmod(angle_q6 * 0.015625, 360);
            angle = wrap_degrees_360(-angle);
            double distance = (dist / 25.4);
            
            // no fake angles
            if (angle > 360 || angle < 0) {
                continue;
            }
            
            // no inside robot
            if (angle > 5 && angle < 200) {
                continue;
            }
            
            // no fake distances
            if (distance < 1.0 || distance > 200.0) {
                continue;
            }

            // ignore if outside tolerance (20in default, increases for pose reset)
            EVec<2> expected = lidar_ukf::measurement(obj.ukf_.xhat(), EVec<3>{0, angle, 0});
            if (std::abs(distance - expected(0)) > obj.BEAM_TOLERANCE) {
                continue;
            }

            // predict up to now
            uint64_t meas_time = vexSystemHighResTimeGet() - init_us;
            double dt_meas = (meas_time - obj.last_predict_us_) / 1.0e6;
            if (dt_meas > 0) {
                EVec<3> velocity = obj.get_robot_velocity();
                obj.ukf_.predict(velocity, dt_meas);
                obj.last_predict_us_ = meas_time;
            }

            // log every other beam
            if (i % 2 == 0) {
                obj.logger->build(0x04)
                    .add(meas_time)
                    .add((float)distance)
                    .add((float)angle)
                    .send();
            }

            // finally call correct
            EVec<2> measurement;
            measurement << distance, angle;
            
            // I hate ts... fill it with fake data cuz it's a workaround to pass it into the measurement Function
            // blame my stupid ass
            EVec<3> u_meas{0.0, angle, 0.0};
            
            obj.ukf_.correct(u_meas, measurement);
            i++;
        }
        
        vex::this_thread::yield();
    }

    return 0;
}
