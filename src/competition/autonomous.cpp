#include "competition/autonomous.h"
#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/command_structure/delay_command.h"
#include "core/utils/command_structure/drive_commands.h"
#include "core/utils/math/geometry/rotation2d.h"
#include "core/utils/pure_pursuit.h"
#include "robot-config.h"
#include <vex_global.h>

void autonomous() {
  skills();
}
void skills() {
  odom.set_position({20, 88, from_degrees(90)});
  intake_sys.auto_fix_jamming(true);
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
    //get first match load
    drive_sys.DriveToPointCmd({20, 119}, vex::forward, 0.6),
    drive_sys.TurnToHeadingCmd(180),
    intake_sys.MatchLoaderCmd(true),
    intake_sys.IntakeCmd(),
    drive_sys.DriveForwardCmd(24, vex::forward, 0.4)->withTimeout(2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    odom.SetPositionCmd({12.5, 119, from_degrees(180)}),
    new DelayCommand(200),
    //get second match load
    intake_sys.IntakeStopCmd(),
    drive_sys.DriveForwardCmd(16, vex::reverse, 0.6),
    intake_sys.MatchLoaderCmd(false),
    drive_sys.TurnToHeadingCmd(270),
    drive_sys.DriveToPointCmd({40, 26}, vex::forward, 0.6),
    drive_sys.TurnToHeadingCmd(180),
    intake_sys.MatchLoaderCmd(true),
    intake_sys.IntakeCmd(),
    drive_sys.DriveForwardCmd(24, vex::forward, 0.4)->withTimeout(2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    odom.SetPositionCmd({11, 26.5, from_degrees(180)}),
    drive_sys.DriveForwardCmd(10, vex::reverse, 0.6),
    intake_sys.IntakeStopCmd(),
    intake_sys.ColorSortCmd(true),
    drive_sys.TurnToHeadingCmd(0),
    drive_sys.TurnToPointCmd({43, 23})->withTimeout(2),
    drive_sys.DriveToPointCmd({43, 23}, vex::forward, 0.6),
    intake_sys.OutTopCmd(),
  };
  cc.run();
}

void red_auto_path(){
  // odompod_solonoid.set(true);
  // remeasure
  odom.set_position({22, 88, 0});
  intake_sys.auto_fix_jamming(true);
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
    //go to blocks in center
    // drive_sys.DriveForwardCmd(10, vex::forward, 0.6),
    // drive_sys.TurnToPointCmd({69, 96})->withTimeout(2),
    intake_sys.ColorSortCmd(true),
    intake_sys.IntakeCmd(),
    drive_sys.DriveToPointCmd({68, 102}, vex::forward, 0.4),
    drive_sys.TurnToHeadingCmd(90),
    drive_sys.DriveForwardCmd(12, vex::forward, 0.2),
    new DelayCommand(200),
    intake_sys.IntakeStopCmd(),
    // score in middle
    drive_sys.DriveToPointCmd({49, 99}, vex::reverse, 0.6),
    drive_sys.TurnToHeadingCmd(315)->withTimeout(2),
    drive_sys.DriveForwardCmd(10, vex::forward, 0.6),
    intake_sys.OutMiddleCmd(),
    new DelayCommand(4000),
    intake_sys.IntakeStopCmd(),
    intake_sys.ColorSortCmd(false),
    // get match load
    drive_sys.DriveToPointCmd({24, 118}, vex::reverse, 0.6),
    drive_sys.TurnToHeadingCmd(180)->withTimeout(2),
    intake_sys.IntakeCmd(),
    intake_sys.MatchLoaderCmd(true),
    drive_sys.DriveForwardCmd(24, vex::forward, 0.4)->withTimeout(2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(1,1)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(1,1)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(1,1)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveForwardCmd(10, vex::reverse, 0.6),
    intake_sys.IntakeStopCmd(),
    intake_sys.MatchLoaderCmd(false),
    //go to score
    drive_sys.TurnToHeadingCmd(0, 0.4)->withTimeout(2),
    drive_sys.DriveForwardCmd(21, vex::forward, 0.6),
    intake_sys.OutTopCmd(),
    new DelayCommand(5000),
    intake_sys.IntakeStopCmd(),
    drive_sys.DriveForwardCmd(10, vex::reverse, 0.6),
    drive_sys.TurnToHeadingCmd(180)->withTimeout(1),
    intake_sys.MatchLoaderCmd(true),
    drive_sys.DriveForwardCmd(14),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(1,1)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(1,1)->withTimeout(0.2),
    new DelayCommand(200),
    drive_sys.DriveTankCmd(1,1)->withTimeout(0.2),
    new DelayCommand(200),
  };
  cc.run();
  printf("X: %.2f, Y: %.2f, Rot: %.2f\n", odom.get_position().x(), odom.get_position().y(), odom.get_position().rotation().degrees());
}


