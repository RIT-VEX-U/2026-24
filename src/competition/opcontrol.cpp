#include "competition/opcontrol.h"

#include "core/subsystems/tank_drive.h"
#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/command_structure/delay_command.h"
#include "robot-config.h"
#include "competition/autonomous.h"
#include <cstdio>
#include <vex_global.h>

#define DEADBAND 0.0  // Deadband for joystick inputs
#define TESTCODE      // Comment for "competition-ready" (Joe-friendly) drive code.
                      // Uncomment for testing/debug/experimental/tuning/etc code.
#ifdef TESTCODE
void PID_Tuning();
#endif

bool enable_drive = true;

bool right_stick_out = false;
bool sunroof_down = false;

void opcontrol() {
  intake_sys.auto_fix_jamming(false);

  con.ButtonR1.pressed([](){
    intake_sys.intake();
  });
  con.ButtonR2.pressed([](){
    intake_sys.outbottom(7);
  });
  con.ButtonL1.pressed([](){
    intake_sys.outmiddle(8);
  });
  con.ButtonL2.pressed([](){
    intake_sys.outtop();
  });

  con.ButtonLeft.pressed([](){
    intake_sys.outbottom();
  });
  con.ButtonDown.pressed([](){
    right_wing_solonoid.set(true);
    left_wing_solonoid.set(true);
  });
  con.ButtonDown.released([](){
    right_wing_solonoid.set(false);
    left_wing_solonoid.set(false);
  });
  con.ButtonRight.pressed([](){
    intake_sys.outback();
  });
  #ifdef TESTCODE
  con.ButtonUp.pressed([](){
    enable_drive = false;
    PID_Tuning();
    enable_drive = true;
  });
  #endif

  con.ButtonX.pressed([](){
    right_stick_solonoid.set(right_stick_out = !right_stick_out);
  });
  con.ButtonB.pressed([](){
    sunroof_solonoid.set(sunroof_down = !sunroof_down);
  });
  con.ButtonY.pressed([](){
    intake_sys.match_load(!intake_sys.is_match_loading());
  });
  con.ButtonA.pressed([](){
    if(intake_sys.get_intake_state() == IntakeSys::AUTOLOAD) {
      intake_sys.lock_state(false);
      intake_sys.intake_stop();
    }
    else {
      intake_sys.autoload();
      intake_sys.lock_state();
    }
  });

  while(true){
    printf("X: %.2f, Y: %.2f, Rot: %.2f\n", odom.get_position().x(), odom.get_position().y(), odom.get_position().rotation().degrees());
    if(!con.ButtonR1.pressing() && !con.ButtonR2.pressing() &&
      !con.ButtonL1.pressing() && !con.ButtonL2.pressing() && !con.ButtonRight.pressing() && !con.ButtonLeft.pressing()){
      intake_sys.intake_stop();
    }

    double left;
    double right;
    if(enable_drive) {
      left = (double)con.Axis3.position() / 100;
      right = (double)con.Axis2.position() / 100;
      if(left < DEADBAND && left > -DEADBAND){
        left = 0;
      }
      if(right < DEADBAND && right > -DEADBAND){
        right = 0;
      }
      if(enable_drive){
        #ifdef TESTCODE
        drive_sys.drive_arcade(left, (double)con.Axis1.position() / 100);
        #else
        drive_sys.drive_tank(left,right);
        #endif
      }
    }
    
  vexDelay(10);
  }
}

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