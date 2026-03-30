#include "robot-config.h"
#include "core/subsystems/odometry/odometry_lidar_wrapper.h"
#include "core/subsystems/screen.h"
#include "core/units/types/geometry.h"
#include "core/utils/command_structure/auto_command.h"
#include "core/utils/controls/feedforward.h"
#include "core/utils/controls/motion_controller.h"
#include "core/utils/controls/state_space/tank_drive_sysid.h"
#include "core/utils/math/geometry/rotation2d.h"
#include "core/utils/controls/pidff.h"
#include <cstdio>
#include <v5_api.h>
#include <vex_optical.h>
#include <vex_rotation.h>
#include <vex_triport.h>
#include <vex_units.h>
#include "core/device/wrapper_device.hpp"
#include "core/device/vdb/registry-controller.hpp"
#include "subsystems/DriveParamUKF.h"
#include "subsystems/Lidar.h"
//VEX
vex::controller con;
vex::brain Brain;

//MOTORS
bool DRIVE_FINISHED_FLAG = false;

vex::motor left1(PORT3, vex::gearSetting::ratio6_1, true);
vex::motor left2(PORT4, vex::gearSetting::ratio6_1, true);
vex::motor left3(PORT5, vex::gearSetting::ratio6_1, true);
vex::motor left4(PORT19, vex::gearSetting::ratio6_1, true);
vex::motor_group left_motors(left1, left2, left3, left4);

vex::motor right1(PORT6, vex::gearSetting::ratio6_1, false);
vex::motor right2(PORT7, vex::gearSetting::ratio6_1, false);
vex::motor right3(PORT8, vex::gearSetting::ratio6_1, false);
vex::motor right4(PORT20, vex::gearSetting::ratio6_1, false);
vex::motor_group right_motors(right1, right2, right3, right4);

vex::motor frontroller(PORT9, vex::gearSetting::ratio6_1, false);
vex::motor backroller(PORT1, vex::gearSetting::ratio6_1, false);
vex::motor toproller(PORT16, vex::gearSetting::ratio6_1, false);
vex::motor agitatorroller(PORT17, vex::gearSetting::ratio6_1, false);
vex::motor backscoreroller(PORT11, vex::gearSetting::ratio6_1, false);

vex::digital_out zlight_board{Brain.ThreeWirePort.A};
vex::digital_out upper_light_board{Brain.ThreeWirePort.D};
vex::digital_out match_loader{Brain.ThreeWirePort.B};
//vex::digital_out right_wing_solonoid{Brain.ThreeWirePort.D};
vex::digital_out left_wing_solonoid{Brain.ThreeWirePort.C};
vex::digital_out right_stick_solonoid{Brain.ThreeWirePort.E};
vex::digital_out sunroof_solonoid{Brain.ThreeWirePort.F};

vex::inertial imu(PORT14);

vex::optical low_optical_sensor(PORT2);
vex::optical high_optical_sensor(PORT13);
vex::distance intake_distance_sensor(PORT18);



// lidar at 16

PID::pid_config_t drive_pid_cfg{
   .p = 0.03,
   .d = 0.004,
   .deadband = 1.5,
   .on_target_time = 0.1,
};

FeedForward::ff_config_t drive_ff_cfg{
  .kV = 0.149,
  .kA = 0.032,
};

PID drive_pid(drive_pid_cfg);

MotionController::m_profile_cfg_t drive_motion_cfg{
  .max_v = 80,
  .accel = 150,
  .pid_cfg = drive_pid_cfg,
  .ff_cfg = drive_ff_cfg,
};

MotionController drive_motion(drive_motion_cfg);

PID::pid_config_t turn_pid_cfg{
  .p = 0.05, // 0.025
  .i = 0.001,
  .d = 0.004, // 0.004
  .deadband = 3,
  .on_target_time = 0.1,
  .error_method = PID::ERROR_TYPE::LINEAR,
};
PID turn_pid(turn_pid_cfg);

robot_specs_t config = {
  .robot_radius = 15,
  .odom_wheel_diam = 2.29,
  .odom_gear_ratio = 1,
  .dist_between_wheels = 11.8,
  .drive_correction_cutoff = 15,
  .drive_feedback = &drive_pid,
  .turn_feedback = &turn_pid,
  .correction_pid = turn_pid_cfg,
};

TankDriveModel drive_model(
  Length::from<inch_tag>(11.8),
  12_V,
  0_V,
  0.175_VpInPs,
  0.042_VpInPs2,
  1.23_VpRadPs,
  0.20651_VpRadPs2
);

TankDriveObserver drive_observer(drive_model, 10_ms);

TankTrajectoryFollowerConfig trajectory_follower_config;

TankTrajectoryFollowerConfig line_cfg = [] {
  TankTrajectoryFollowerConfig cfg;
  cfg.q_tolerances = {{5, 1.5, 0.3, 20, 20}};
  cfg.r_tolerances = {{12, 12}};
  cfg.dt = 10_ms;
  cfg.max_velocity = 0_inps;
  cfg.velocity_step = 0.2_inps;
  cfg.stop_at_end = false;
  return cfg;
}();

SerialLogger logger(vex::PORT12);
LidarReceiver lidar(vex::PORT15, 921600, &imu, &left_motors, &right_motors, &config, &logger, &drive_observer);

OdometryLidarWrapper odom(&lidar);
OdometryTank odomtank(left_motors, right_motors, config, &imu);
TankDrive drive_sys(left_motors, right_motors, config, &odom, &logger, &drive_model, &drive_observer); //define how robot moves
IntakeSys intake_sys(toproller, frontroller, backroller, agitatorroller, backscoreroller, low_optical_sensor, high_optical_sensor, intake_distance_sensor, zlight_board, upper_light_board, match_loader);

AutoCommand *MatchLoaderCmd(bool sol_on) {
  return new FunctionCommand([sol_on]() {
    match_loader.set(sol_on);
    return true;
  });
}

uint64_t init_us;
struct pose_timestamp {
  uint64_t timestamp;
  float x;
  float y;
  float deg;
};




/*                 144
 * y /---------------\ 144
 *   |    90deg      |
 *   |      ^        |
 *   |    < o > 0deg |
 *   |      v        |
 *   |    270deg     |
 * 0 \_______________/
 *   0               x
*/
Pose2d right_auto_pose(19.5, 54, from_degrees(270));
Pose2d left_auto_pose(19.5, 86.5, from_degrees(90));
Pose2d skills_auto_pose(24, 71.25, from_degrees(90));
Pose2d curve_start(71.25, 95.0, from_degrees(0));
Pose2d &auto_start_pose = curve_start;
void robot_init() {
 imu.calibrate();
 while (!logger.is_connected() && (vexSystemHighResTimeGet() - init_us) < 5000000) {
   logger.update();
 }

 while(imu.isCalibrating()){
    vexDelay(10);
 }
    drive_observer.set_measurement_provider(
      [](units::Length &left_pos, units::Velocity &left_vel, units::Length &right_pos, units::Velocity &right_vel) {
        const double left_pos_in =
          (left_motors.position(vex::rotationUnits::rev) / config.odom_gear_ratio) * M_PI * config.odom_wheel_diam;
        const double right_pos_in = (right_motors.position(vex::rotationUnits::rev) / config.odom_gear_ratio) * M_PI * config.odom_wheel_diam;
        const double left_vel_inps = (left_motors.velocity(vex::velocityUnits::rpm) / 60.0 / config.odom_gear_ratio) *
                                     M_PI * config.odom_wheel_diam;
        const double right_vel_inps = (right_motors.velocity(vex::velocityUnits::rpm) / 60.0 / config.odom_gear_ratio) *
                                      M_PI * config.odom_wheel_diam;

        left_pos = units::Length::from<units::inch_tag>(left_pos_in);
        right_pos = units::Length::from<units::inch_tag>(right_pos_in);
        left_vel = units::Velocity::from<units::inches_per_second_tag>(left_vel_inps);
        right_vel = units::Velocity::from<units::inches_per_second_tag>(right_vel_inps);
      });
    drive_observer.start_async();

  std::vector<screen::Page *> pages = {
    new screen::StatsPage({
      {"left1", left1},
      {"left2", left2},
      {"left3", left3},
      {"left4", left4},
      {"right1", right1},
      {"right2", right2},
      {"right3", right3},
      {"right4", right4}
    }),
    new screen::PIDPage(turn_pid, "turnpid"),
    new screen::PIDPage(drive_pid, "drivepid"),
    new screen::OdometryPage(odom, 15, 15, true),
  };
  screen::start_screen(Brain.Screen, pages);
  odom.set_position(auto_start_pose);
  printf("started\n");

  //
  // Time t = 0_s;
  // Time end = 5_s;
  // int i = 0;
  // std::vector<std::tuple<Time, Voltage, Velocity>> out;
  // while (t < end) {
  //   left_motors.spin(vex::forward, 1 * t.s(), voltageUnits::volt);
  //   right_motors.spin(vex::forward, 1 * t.s(), voltageUnits::volt);
  //
  //
  //   double lvel = deg2rad(left_motors.velocity(vex::dps)) * config.odom_wheel_diam / 2.0;
  //   double rvel = deg2rad(right_motors.velocity(vex::dps)) * config.odom_wheel_diam / 2.0;
  //   double vel_inps = (lvel + rvel) / 2.0;
  //
  //   out.push_back(std::tuple<Time, Voltage, Velocity>{t, 1_V * t.s(), Velocity::from<inches_per_second_tag>(vel_inps)});
  //
  //   vexDelay(10);
  //   if (i % 10 == 0) {
  //     printf("volts: %0.03f\n", 1 * t.s());
  //   }
  //
  //   t += 10_ms;
  //   i++;
  // }
  // left_motors.stop(vex::brakeType::brake);
  // right_motors.stop(vex::brakeType::brake);
  //
  // for (auto var : out) {
  //   printf("%0.03f, %0.03f, %0.03f\n", std::get<0>(var).seconds(), std::get<1>(var).volts(), std::get<2>(var).inps());
  //   vexDelay(100);
  // }


  init_us = vexSystemHighResTimeGet();
  
  lidar.start();
  lidar.reset_ukf(auto_start_pose);
  bool logger_schema_sent = false;
  while (true) {
    if (!logger.is_connected()) {
      logger.update();
      logger_schema_sent = false;
      vexDelay(10);
      continue;
    }

    if (!logger_schema_sent) {
      logger.define_and_send_schema(0x01, "time:u64, x:f32, y:f32, t:f32");
      logger.define_and_send_schema(0x02, "time:u64, l:f32, r:f32");
      logger_schema_sent = true;
    }

    // lidar.resetUKF({48, 96, 180});
    Pose2d pose = drive_sys.get_position();
    uint64_t timestamp = vexSystemHighResTimeGet() - init_us;
    float x((float)pose.x());
    float y((float)pose.y());
    float t((float)pose.rotation().wrapped_degrees_360());
    float l((float)drive_sys.get_left_velocity());
    float r((float)drive_sys.get_right_velocity());
    logger.build(0x01)
       .add(timestamp)
       .add(x)
       .add(y)
       .add(t)
       .send();
    const double observer_data[3] = {(double)timestamp, (double)l, (double)r};
    logger.log(0x02, observer_data, 3);
    // printf("loop\n");
    vexDelay(10);
  }
}
