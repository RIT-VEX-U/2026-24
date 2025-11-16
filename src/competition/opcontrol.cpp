#include "competition/opcontrol.h"

#include "robot-config.h"
#include <cstdio>

bool enable_drive = true;
bool drive_tank = true;

void opcontrol() {
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
  con.ButtonB.pressed([](){
    match_loader.set(!match_loader.value());
  });
  con.ButtonA.pressed([](){
    enable_drive = false;
    odom.set_position({0,0,0});
    CommandController cc{
      drive_sys.DriveToPointCmd({24, 24}),
    };
    cc.run();
    enable_drive = true;
  });
  con.ButtonB.pressed([](){
    enable_drive = false;
    CommandController cc{
      drive_sys.TurnDegreesCmd(90)
    };
    cc.run();
    enable_drive = true;
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
