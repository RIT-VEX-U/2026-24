#include "competition/opcontrol.h"

#include "core/subsystems/tank_drive.h"
#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/command_structure/delay_command.h"
#include "robot-config.h"
#include "competition/autonomous.h"
#include <cstdio>
#include <algorithm>
#include "core/utils/trajectory/trajectory.h"
#include "core/utils/controls/trapezoid_profile.h"
#include "core/utils/controls/state_space/linear_plant_inversion_feedforward.h"
#include "core/utils/trajectory/constraints/centripetal_acceleration_constraint.h"
#include "core/utils/trajectory/constraints/tank_voltage_constraint.h"

// #define ARCADE        // Comment for Joe-friendly drive controls.
#define DEADBAND 0.0  // Deadband for joystick inputs
// #define SKILLS        // Comment for driver controls, uncomment for skills
//#define TESTCODE      // Comment competition-ready drive code.
                      // Uncomment for testing/debug/experimental/tuning/etc code.
#ifdef TESTCODE
void PID_Tuning();
#endif

bool enable_drive = true;
bool sunroof_lock = false;
vex::timer input_timer;

namespace {

constexpr double kOpenLoopTurnTargetHeadingDeg = 90.0;
constexpr double kOpenLoopTurnDt = 0.01;
constexpr auto kOpenLoopTurnMaxAngularVelocity = 6.0_radps;
constexpr auto kOpenLoopTurnMaxAngularAcceleration = 10.0_radps2;

void run_open_loop_turn_to_heading(
  double target_heading_deg,
  units::AngularVelocity max_angular_velocity,
  units::AngularAcceleration max_angular_acceleration) {
  const double current_heading_deg = odom.get_position().rotation().degrees();
  const double delta_heading_deg = OdometryBase::smallest_angle(current_heading_deg, target_heading_deg);

  if (std::abs(delta_heading_deg) < 1e-6) {
    drive_sys.stop();
    return;
  }

  TrapezoidProfile profile(
    0.0,
    deg2rad(delta_heading_deg),
    max_angular_velocity.radps(),
    max_angular_acceleration.radps2(),
    max_angular_acceleration.radps2());

  TankDriveModel::Plant plant = drive_model.wheel_plant();
  LinearPlantInversionFeedforward<2, 2> feedforward(plant, kOpenLoopTurnDt);
  vex::timer timer;

  while (true) {
    const double elapsed = timer.time(vex::sec);
    const double next_t = std::min(elapsed + kOpenLoopTurnDt, profile.total_time());
    const motion_t current = profile.calculate(elapsed);
    const motion_t next = profile.calculate(next_t);

    const TankDriveModel::StateVector wheel_ref = drive_model.chassis_to_wheels(
      0_inps,
      units::AngularVelocity::from<units::radians_per_second_tag>(current.vel));
    const TankDriveModel::StateVector next_wheel_ref = drive_model.chassis_to_wheels(
      0_inps,
      units::AngularVelocity::from<units::radians_per_second_tag>(next.vel));

    const EVec<2> ff = feedforward.calculate(wheel_ref, next_wheel_ref, next_t - elapsed);
    const EVec<2> ks = drive_model.wheel_stiction_voltages(next_wheel_ref);
    const double max_voltage = drive_model.max_voltage().V();
    const double commanded_left = clamp(ff(0) + ks(0), -max_voltage, max_voltage);
    const double commanded_right = clamp(ff(1) + ks(1), -max_voltage, max_voltage);

    drive_sys.drive_tank_voltage(commanded_left, commanded_right);

    if (elapsed >= profile.total_time()) {
      break;
    }

    vexDelay(static_cast<int>(kOpenLoopTurnDt * 1000.0));
  }

  drive_sys.stop();
}

}  // namespace

Trajectory make_example_curveo() {
  

  constexpr double radius = 24.0;
  constexpr double cx = 71.25;
  constexpr double cy = 71.0;
  constexpr double tangent_mag = 56;

  std::vector<HermitePoint> points = {
    HermitePoint::from_heading(cx, cy + radius, 0.0, tangent_mag),
    HermitePoint::from_heading(cx + radius, cy, -M_PI_2, tangent_mag),
    HermitePoint::from_heading(cx, cy - radius, M_PI, tangent_mag),
    HermitePoint::from_heading(cx - radius, cy, M_PI_2, tangent_mag),
    HermitePoint::from_heading(cx, cy + radius, 0.0, tangent_mag),
  };

  TrajectoryConfig config(80.000_inps, 20.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(20.000_inps);
  config.set_reversed(false);
  config.set_track_width(11.8_in);
  config.add_constraint(CentripetalAccelerationConstraint(100.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.175_VpInPs, 0.042_VpInPs2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}

Trajectory make_example_lineo() {

  std::vector<HermitePoint> points = {
    HermitePoint(71.25, 96, 10, 0),
    HermitePoint(118, 96, 10, 0),
  };

  TrajectoryConfig config(40.000_inps, 30.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(0.000_inps);
  config.set_reversed(false);
  config.set_track_width(11.8_in);
  config.add_constraint(CentripetalAccelerationConstraint(180.000_inps2));
  // config.add_constraint(TankVoltageConstraint(0.189_VpInPs, 0.0416_VpInPs2, 12.000_V, 12.000_in));
  return TrajectoryGenerator::generate_trajectory(points, config);
}


void opcontrol_normal() {
  intake_sys.auto_fix_jamming(false);

  con.ButtonR1.pressed([](){
    intake_sys.intake();
  });
  con.ButtonR2.pressed([](){
    intake_sys.outbottom(7);
  });
  con.ButtonL1.pressed([](){
    intake_sys.outmiddle(
      #ifdef SKILLS // Set voltage to 8 for skills and 9 for driver
      8
      #else
      9
      #endif
    );
  });
  con.ButtonL2.pressed([](){
    intake_sys.outtop();
  });

  con.ButtonLeft.pressed([](){
    intake_sys.outbottom();
  });
  con.ButtonDown.pressed([](){
    // right_wing_solonoid.set(true);
    left_wing_solonoid.set(true);
  });
  con.ButtonDown.released([](){
    // right_wing_solonoid.set(false);
    left_wing_solonoid.set(false);
  });
  con.ButtonRight.pressed([](){
    #ifdef SKILLS // Allow ButtonRight to end matchload macro in skills
    if(intake_sys.get_intake_state() == IntakeSys::AUTOLOAD) {
      intake_sys.match_load(false); // Raise matchloader
      intake_sys.lock_state(false); // Unlock intake state
    }
    #endif

    // do this when loading
    if(intake_sys.has_blocks_loaded() || intake_sys.get_intake_state() == intake_sys.ESQUEBOT) {
      #ifndef SKILLS
        if(!sunroof_lock || intake_sys.get_intake_state() == intake_sys.ESQUEBOT)
      #endif
      sunroof_solonoid.set(false);

      intake_sys.lock_state(false);
      intake_sys.esquescore();
      input_timer.event([]() { 
        if(con.ButtonRight.pressing()) {
          intake_sys.outback(); 
        }
      }, 200);
    } else {
      intake_sys.esquebot();
      intake_sys.lock_state(true);
      input_timer.event([]() { 
        if(con.ButtonRight.pressing()) {
          intake_sys.lock_state(false);
          intake_sys.outback(); 
        } else {
          #ifndef SKILLS
            sunroof_lock = true;
          #endif
          sunroof_solonoid.set(true);
        }
      }, 200);
    }

    // do this when scoring
  });
  // con.ButtonUp.pressed([](){
      // enable_drive = false;
      // vexDelay(500);
      //
      // imu.calibrate();
      // while (imu.isCalibrating()) {
      //   vexDelay(10);
      // }
      // lidar.set_pose(Pose2d(lidar.get_pose().x(), lidar.get_pose().y(), from_degrees(0)));
      // lidar.BEAM_TOLERANCE = 999;
      // vexDelay(3000);
      // lidar.BEAM_TOLERANCE = 20;
      // enable_drive = true;
      // enable_drive = false;
      // CommandController cc{
      //     intake_sys.MatchLoaderCmd(true),
      //     // drive_sys.DriveForwardCmd(24, vex::forward, 0.75),
      //     new FunctionCommand([]() {
      //         sunroof_solonoid.set(true);
      //         return true;
      //     }),
      //     intake_sys.AutoLoadCmd(),
      //     DriveTankRawCmd(1, 1),
      //     new DelayCommand(370),
      //     DriveTankRawCmd(0.7, 0.08),
      //     new DelayCommand(600),
      //     drive_sys.DriveTankCmd(0.3, 0.3),
      //     new DelayCommand(800),
      //     DriveTankRawCmd(0, 0),
      //
      // };
      // cc.run();
      // vexDelay(2000);
      // enable_drive = true;
      // });
  #ifdef TESTCODE
  con.ButtonUp.pressed([](){
    enable_drive = false;
    PID_Tuning();
    enable_drive = true;
  });
  #endif
  con.ButtonUp.pressed([]() {
    // enable_drive = false;
    // Trajectory traj = make_example_curveo();
    // CommandController cc{
    //   drive_sys.FollowTrajectoryCmd(traj, trajectory_follower_config),
    //   // drive_sys.FollowTrajectoryOpenLoopCmd(make_example_lineo()),
    // };
    // cc.run();
    // enable_drive = true;

    // enable_drive = false;
    // run_open_loop_turn_to_heading(
    //   kOpenLoopTurnTargetHeadingDeg,
    //   kOpenLoopTurnMaxAngularVelocity,
    //   kOpenLoopTurnMaxAngularAcceleration);
    // enable_drive = true;

    right_stick_solonoid.set(!right_stick_solonoid.value());
  });
  // con.ButtonX.pressed([](){
  //   #ifdef SKILLS // Hopperreturn in skills, stick in driver
  //   intake_sys.hopperreturn();
  //   #else
  //   // right_stick_solonoid.set(!right_stick_solonoid.value());
  //
  //
  //   #endif
  // });
  con.ButtonB.pressed([](){
    #ifndef SKILLS // Disable sunroof locking in skills
    if(!sunroof_lock) 
    #endif
    sunroof_solonoid.set(!sunroof_solonoid.value());
  });
  con.ButtonY.pressed([](){
    intake_sys.match_load(!intake_sys.is_match_loading());
  });
  con.ButtonA.pressed([](){
    if(intake_sys.get_intake_state() == IntakeSys::AUTOLOAD) {
      intake_sys.match_load(false); // Raise matchloader
      intake_sys.lock_state(false); // Unlock intake state

      #ifdef SKILLS // Hopperreturn in skills, purge in driver
      intake_sys.hopperreturn();
      #else
      intake_sys.frontpurge();
      #endif
    }
    else {
      #ifndef SKILLS
        sunroof_lock = true;          // Lock sunroof
      #endif
      sunroof_solonoid.set(true);   // Lower sunroof
      intake_sys.match_load(true);  // Lower matchloader
      intake_sys.autoload();        // Automatic Match Load
      intake_sys.lock_state();      // Lock intake state
    }
  });

  #ifdef SKILLS // Start with Matchload Macro (sine matchload solonoid)
  sunroof_solonoid.set(true);   // Lower sunroof
  intake_sys.match_load(true);
  intake_sys.autoload();        // Automatic Match Load
  intake_sys.lock_state();      // Lock intake state
  #endif

  IntakeSys::IntakeState intake_state = intake_sys.get_intake_state();
  while(true){
    if(enable_drive) {
    IntakeSys::IntakeState prev_state = intake_state;
    intake_state = intake_sys.get_intake_state();

    // printf("X: %.2f, Y: %.2f, Rot: %.2f\n", odom.get_position().x(), odom.get_position().y(), odom.get_position().rotation().degrees());

    if(!con.ButtonR1.pressing() && !con.ButtonR2.pressing() && !con.ButtonL1.pressing() &&
      !con.ButtonL2.pressing() && !con.ButtonRight.pressing() && !con.ButtonLeft.pressing() &&
      (signed int) intake_state >= 0)
      { intake_sys.intake_stop(); sunroof_lock = false; }

    #ifndef SKILLS // Lock and unlock sunroof in driver, but not in skills
    if(intake_state == IntakeSys::ESQUESCORE) sunroof_lock = false;
    if(intake_state != IntakeSys::FRONTPURGE && prev_state == IntakeSys::FRONTPURGE) {
      sunroof_lock = false;
      sunroof_solonoid.set(false);
    }
    #endif

    double left;
    double right;
      left = (double)con.Axis3.position() / 100;
      right = 
      #ifndef ARCADE
        (double)con.Axis2.position() / 100;
      #else
        (double)con.Axis1.position() / 100;
      #endif

      if(left < DEADBAND && left > -DEADBAND){
        left = 0;
      }
      if(right < DEADBAND && right > -DEADBAND){
        right = 0;
      }
      if(enable_drive){
        if (con.ButtonX.pressing()) {
          if (odom.get_position().y() < 23.75) {
            drive_sys.drive_line(left, {11, 11}, from_degrees(0), line_cfg);
          } else if (odom.get_position().y() < 72) {
            drive_sys.drive_line(left, {36.2, 36.2}, from_degrees(180), line_cfg);
          } else if (odom.get_position().y() < 118) {
            drive_sys.drive_line(left, {105.8, 105.8}, from_degrees(0), line_cfg);
          } else {
            drive_sys.drive_line(left, {130.5, 130.5}, from_degrees(180), line_cfg);
          }
        } else {
          #ifdef ARCADE
          drive_sys.drive_arcade(left, right);
          #else
          drive_sys.drive_tank(left,right);
          #endif
        }
      }
    }
    
  vexDelay(10);
  }
}

void opcontrol() {
  opcontrol_normal();
}

#ifdef TESTCODE
void PID_Tuning() {
  CommandController cc{
    new Async(new FunctionCommand([]() {
      while (true) {
        printf(
          "ODO X: %f ODO Y: %f, ODO ROT: %f\n", odom.get_position().x(),
          odom.get_position().y(), odom.get_position().rotation().degrees()
        );
        vexDelay(100);
      }
      return true;
    })),
    drive_sys.DriveForwardCmd(24, vex::forward, 1.0)->withTimeout(2),
    new DelayCommand(200),
    drive_sys.TurnDegreesCmd(90, 1)
  };
  cc.run();
}
#endif
