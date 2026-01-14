#include "competition/opcontrol.h"

#include "core/subsystems/tank_drive.h"
#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/command_structure/delay_command.h"
#include "robot-config.h"
#include "competition/autonomous.h"
#include <cstdio>
#include <vex_global.h>

#define DEADBAND 0.0

bool enable_drive = true;

bool wings_out = false;
bool right_stick_out = false;

bool l2_initial_press = true;
vex::timer double_tap_window;
bool double_tap = false;

void opcontrol() {
  intake_sys.auto_fix_jamming(false);

  con.ButtonR1.pressed([](){
    intake_sys.intake();
  });
  con.ButtonR2.pressed([](){
    intake_sys.outbottom();
  });
  con.ButtonL1.pressed([](){
    intake_sys.outmiddle();
  });

  con.ButtonL2.pressed([](){
    if(l2_initial_press) {
      if(double_tap_window.time() <= 500) double_tap = true;
      else double_tap_window.clear();

      l2_initial_press = false;
    }

    if(double_tap) intake_sys.outback();
    else intake_sys.outtop();
  });
  con.ButtonL2.released([](){
    l2_initial_press = true;
    double_tap = false;
  });

  con.ButtonDown.pressed([](){
    wings_out = !wings_out;
    right_wing_solonoid.set(wings_out);
    left_wing_solonoid.set(wings_out);
  });
  con.ButtonX.pressed([](){
    right_stick_solonoid.set(right_stick_out = !right_stick_out);
  });
  con.ButtonB.pressed([](){
    intake_sys.match_load(!intake_sys.is_match_loading());
  });

  while(true){
    printf("X: %.2f, Y: %.2f, Rot: %.2f\n", odom.get_position().x(), odom.get_position().y(), odom.get_position().rotation().degrees());
    if(!con.ButtonR1.pressing() && !con.ButtonR2.pressing() &&
      !con.ButtonL1.pressing() && !con.ButtonL2.pressing()){
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
        drive_sys.drive_tank(left,right);
      }
    }
    
  vexDelay(10);
  }
}
