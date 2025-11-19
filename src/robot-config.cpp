#include "robot-config.h"
#include "core/subsystems/screen.h"
#include "core/utils/command_structure/auto_command.h"
#include "core/utils/controls/feedforward.h"
#include "core/utils/controls/motion_controller.h"
#include "core/utils/math/geometry/rotation2d.h"
#include "core/utils/controls/pidff.h"
#include <cstdio>
#include <vex_optical.h>
#include <vex_rotation.h>
#include <vex_triport.h>
#include <vex_units.h>
#include "core/device/wrapper_device.hpp"
#include "core/device/vdb/registry-controller.hpp"
//VEX
vex::controller con;
vex::brain Brain;

//MOTORS

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
vex::motor toproller(PORT15, vex::gearSetting::ratio6_1, false);
vex::motor agitatorroller(PORT17, vex::gearSetting::ratio6_1, false);

vex::digital_out zlight_board{Brain.ThreeWirePort.A};
vex::digital_out match_loader{Brain.ThreeWirePort.B};
vex::digital_out odompod_solonoid{Brain.ThreeWirePort.C};

vex::inertial imu(PORT14);
vex::rotation odompod(PORT18, true);

vex::optical lower_intake_sensor(PORT2);

vex::optical middle_intake_sensor(PORT13);

// lidar at 16

PID::pid_config_t drive_pid_cfg{
   .p = 0.09615,
   .d = 0.007,
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
  // .p = 0.02308,
  .p = 0.025,
  // .i = 0.0025,
  // .d = 0.0015,
  .d = 0.0015,
  .deadband = 3,
  .on_target_time = 0.1,
  .error_method = PID::ERROR_TYPE::LINEAR,
};
PID turn_pid(turn_pid_cfg);

robot_specs_t robot_config = {
  .robot_radius = 10,
  .odom_wheel_diam = 2.29,
  .odom_gear_ratio = 1,
  .dist_between_wheels = 12.4,
  .drive_correction_cutoff = 15,
  .drive_feedback = &drive_pid,
  .turn_feedback = &turn_pid,
  .correction_pid = turn_pid_cfg,
};

// OdometryOneWheel odom(&odompod, robot_config, Translation2d(-1.5, 1.4), &imu);
OdometryTank odom(left_motors, right_motors, robot_config, &imu);
TankDrive drive_sys(left_motors, right_motors, robot_config, &odom); //define how robot moves
IntakeSys intake_sys(toproller, frontroller, backroller, agitatorroller, lower_intake_sensor, middle_intake_sensor, zlight_board, match_loader);

AutoCommand *MatchLoaderCmd(bool sol_on) {
  return new FunctionCommand([sol_on]() {
    match_loader.set(sol_on);
    return true;
  });
}

void robot_init() {
 imu.calibrate();
 while(imu.isCalibrating()){
    vexDelay(10);
 }
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
  odom.set_position({20, 88, from_degrees(90)});
  printf("started\n");
}
