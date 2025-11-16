#include "competition/autonomous.h"
#include "core/utils/command_structure/command_controller.h"
#include "robot-config.h"

void autonomous() {
  red_auto_path();
}

void red_auto_path(){
  odom.set_position({7,55, 0});
  CommandController cc{
    drive_sys.DriveForwardCmd(20),
    drive_sys.TurnToPointCmd({72, 48}),
    drive_sys.DriveToPointCmd({72, 48}),
  };
  cc.run();
}

void skills() {}

