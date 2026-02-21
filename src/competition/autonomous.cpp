#include "competition/autonomous.h"
#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/command_structure/delay_command.h"
#include "core/utils/command_structure/drive_commands.h"
#include "core/utils/math/geometry/rotation2d.h"
#include "core/utils/pure_pursuit.h"
#include "robot-config.h"
#include <vex_global.h>

void (*autonomous)() = right_auto_path;

// --- AutoCommands ---

AutoCommand *SunroofSolCmd(bool sol_on) {
  return new FunctionCommand([sol_on]() {
    sunroof_solonoid.set(sol_on);
    return true;
  });
}

AutoCommand *DriveTankRawCmd(double left, double right) {
  return new FunctionCommand([left, right]() {
    drive_sys.drive_tank_raw(left, right);
    return true;
  });
}

// --- Paths ---

void right_auto_path() {
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
      }) ),
    
    // Starts at {19.5, 54, from_degrees(270)}
    // Matchloader

    intake_sys.MatchLoaderCmd(true),
    drive_sys.DriveForwardCmd(33, vex::forward, 0.8)->withTimeout(1.5),
    drive_sys.TurnToHeadingCmd(180, .8)->withTimeout(2.25),
    SunroofSolCmd(true),
    intake_sys.AutoLoadCmd(),
    DriveTankRawCmd(0.4, 0.4),
    new DelayCommand(600),
    DriveTankRawCmd(0.1, 0.1),
    new DelayCommand(4500),

    // QUICK MATCHLOAD
    // intake_sys.MatchLoaderCmd(true),
    // new FunctionCommand([]() {
    //     sunroof_solonoid.set(true);
    //     return true;
    // }),
    // intake_sys.AutoLoadCmd(),
    // DriveTankRawCmd(1, 1),
    // new DelayCommand(372),
    // DriveTankRawCmd(0.7, 0.05),
    // new DelayCommand(600),
    // DriveTankRawCmd(0.2, 0.2),


    // Leaving Matchloader
    intake_sys.MatchLoaderCmd(false),
    SunroofSolCmd(false),
    intake_sys.FrontPurgeCmd(),

    // Long goal (drive to and score)
    new Parallel({
      (new InOrder({drive_sys.DriveToPointCmd({44, 23.75}, vex::reverse, 0.8, 0.8)->withTimeout(2), 
                   DriveTankRawCmd(-0.45, -0.45)}))->withTimeout(3),
      (new InOrder({new DelayCommand(650), intake_sys.OutBackCmd()}))->withTimeout(3),
    }),
    new DelayCommand(2750),

    DriveTankRawCmd(0.5, 0.5),
    new DelayCommand(250),
    SunroofSolCmd(true),
    DriveTankRawCmd(-0.45, -0.45),
  };

  cc.run();
}

void left_auto_path() {
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
      }) ),

      // Starts at {tbd, tbd, from_degrees(tbd)}
      // Matchloader

      // Long Goal
  };

  cc.run();
}

void skills_path() {
  /*odom.set_position({20, 88, from_degrees(90)});
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
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
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
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
    new DelayCommand(200),
    DriveTankRawCmd(0.4, 0.4)->withTimeout(0.2),
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
  cc.run();*/
}
