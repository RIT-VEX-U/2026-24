#include "competition/opcontrol.h"

#include "core/subsystems/tank_drive.h"
#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/command_structure/delay_command.h"
#include "core/utils/math/geometry/rotation2d.h"
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
bool aimbot = false;
bool sunroof_lock = false;
vex::timer input_timer;

namespace {

constexpr double kOpenLoopTurnTargetHeadingDeg = 90.0;
constexpr double kOpenLoopTurnDt = 0.01;
constexpr auto kOpenLoopTurnMaxAngularVelocity = 6.0_radps;
constexpr auto kOpenLoopTurnMaxAngularAcceleration = 10.0_radps2;
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
    intake_sys.outmiddleall(
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
    // CommandController cc{
    //   drive_sys.FollowTrajectoryCmd(spawn_to_right_loader(), trajectory_follower_config),
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
  // con.ButtonX.pressed([](){
  //     aimbot = !aimbot;
  // });
  con.ButtonX.pressed([](){
    #ifndef SKILLS // Disable sunroof locking in skills
    if(!sunroof_lock) 
    #endif
    sunroof_solonoid.set(!sunroof_solonoid.value());
  });
  con.ButtonA.pressed([](){
    intake_sys.match_load(!intake_sys.is_match_loading());
  });
  con.ButtonY.pressed([](){
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
      #ifdef SKILLS
      intake_sys.autoload_prefer = IntakeSys::BlockColor::NOTHING;
      #endif
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
    if(intake_state != IntakeSys::FRONTPURGE && intake_state != IntakeSys::AUTOLOAD && prev_state == IntakeSys::FRONTPURGE) {
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

      aimbot = con.ButtonB.pressing();
      if(enable_drive){
        if (aimbot) {
          if (odom.get_position().y() < 23.75) {
            drive_sys.drive_line(left, {11, 11}, from_degrees(0), line_cfg);
          } else if (odom.get_position().y() < 72) {
            drive_sys.drive_line(left, {36.2, 36.2}, from_degrees(180), line_cfg);
          } else if (odom.get_position().y() < 118) {
            drive_sys.drive_line(left, {104.5, 104.5}, from_degrees(0), line_cfg);
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
  odom.set_position({42, 24, from_degrees(180)});
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
