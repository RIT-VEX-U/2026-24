#include "competition/opcontrol.h"

#include "core/subsystems/tank_drive.h"
#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/command_structure/delay_command.h"
#include "robot-config.h"
#include "competition/autonomous.h"
#include <cstdio>
#include <vex_global.h>

bool enable_drive = true;
bool drive_tank = true;

void opcontrol() {
  intake_sys.auto_fix_jamming(false);
  // odompod_solonoid.set(false);
  con.ButtonR1.pressed([](){
    intake_sys.intake();
  });
  con.ButtonL2.pressed([](){
    intake_sys.outtop();
  });
  con.ButtonL1.pressed([](){
    intake_sys.outmiddle();
  });
  con.ButtonR2.pressed([](){
    intake_sys.outbottom();
  });
  con.ButtonDown.pressed([](){
    if(intake_sys.is_color_sorting()){
      intake_sys.stop_color_sorting();
    }
    else{
      intake_sys.start_color_sorting();
    }
  });
  con.ButtonA.pressed([](){
    CommandController cc{
      drive_sys.DriveForwardCmd(30)->withTimeout(3),
    };
    enable_drive = false;
    cc.run();
    enable_drive = true;
  });
  con.ButtonX.pressed([](){
    enable_drive = false;
    CommandController cc{
      drive_sys.TurnDegreesCmd(90, 1),
    };
    cc.run();
    enable_drive = true;
  });
  con.ButtonB.pressed([](){
    intake_sys.match_load(!intake_sys.is_match_loading());
  });
  con.ButtonLeft.pressed([](){
    drive_tank = !drive_tank;
  });

   while(true){
    printf("X: %.2f, Y: %.2f, Rot: %.2f\n", odom.get_position().x(), odom.get_position().y(), odom.get_position().rotation().degrees());
    if(!con.ButtonR1.pressing() && !con.ButtonR2.pressing() &&
      !con.ButtonL1.pressing() && !con.ButtonL2.pressing()){
      intake_sys.intake_stop();
    }
    double left;
    double right;
    if(enable_drive)
    {
      if(drive_tank){
        left = (double)con.Axis3.position() / 100;
        right = (double)con.Axis2.position() / 100;
      }
      else{
        left = (double)con.Axis3.position() / 100;
        right = (double)con.Axis1.position() / 100;
      }
      if(left < 0.2 && left > -0.2){
        left = 0;
      }
      if(right < 0.2 && right > -0.2){
        right = 0;
      }
      if(enable_drive && drive_tank){
        drive_sys.drive_tank(left,right);
      }
      else if (enable_drive && !drive_tank) {
        drive_sys.drive_arcade(left,right);
      }
    }
  vexDelay(10);
  }
}
