#include "competition/autonomous.h"
#include "core/utils/command_structure/auto_command.h"
#include "core/utils/command_structure/command_controller.h"
#include "core/utils/command_structure/delay_command.h"
#include "core/utils/command_structure/drive_commands.h"
#include "core/utils/math/geometry/rotation2d.h"
#include "core/utils/pure_pursuit.h"
#include "robot-config.h"
#include <vex_global.h>

#include "core/utils/trajectory/trajectory.h"

#define LOG 3

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

Trajectory spawn_to_right_loader() {  
  std::vector<HermitePoint> points = {
    {19.500, 55.000, 0.000, -30.000},
    {14.000, 25.000, -90.000, -2.000},
  };

  TrajectoryConfig config(70.000_inps, 70.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(20.000_inps);
  config.set_reversed(false);
  config.set_track_width(11.8_in);
  config.add_constraint(CentripetalAccelerationConstraint(100.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.175_VpInPs, 0.042_VpInPs2, 12.000_V, 12.000_in));
  Trajectory trajectory = TrajectoryGenerator::generate_trajectory(points, config);

  return trajectory;
}

Trajectory right_loader_to_goal() {  
  std::vector<HermitePoint> points = {
    {12.000, 25.000, 30.000, 0.000},
    {32.000, 23.750, 20.000, 0.000},
    {42.000, 23.750, 20.000, 0.000},
  };

  TrajectoryConfig config(80.000_inps, 80.000_inps2);
  config.set_start_velocity(0.000_inps);
  config.set_end_velocity(20.000_inps);
  config.set_reversed(true);
  config.set_track_width(11.8_in);
  config.add_constraint(CentripetalAccelerationConstraint(180.000_inps2));
  config.add_constraint(TankVoltageConstraint(0.175_VpInPs, 0.042_VpInPs2, 12.000_V, 12.000_in));
  Trajectory trajectory = TrajectoryGenerator::generate_trajectory(points, config);

  return trajectory;
}

// --- Paths ---

void right_auto_path() {
  printf("Build %d\n", LOG);
  intake_sys.auto_fix_jamming(true);
  CommandController cc{
    // new Async(new FunctionCommand([]() {
    //   while (true) {
    //     printf(
    //       "ODO X: %f ODO Y: %f, ODO ROT: %f\n", odom.get_position().x(),
    //       odom.get_position().y(), odom.get_position().rotation().degrees()
    //     );
    //     vexDelay(100);
    //   }
    //   return true;
    //   }) ),
    
    // Starts at {19.5, 54, from_degrees(270)}
    // Matchloader

    intake_sys.MatchLoaderCmd(true),
    SunroofSolCmd(true),
    intake_sys.AutoLoadCmd(),
    drive_sys.FollowTrajectoryCmd(spawn_to_right_loader(), trajectory_follower_config),
    DriveTankRawCmd(0.4, 0.4),
    new DelayCommand(600),
    DriveTankRawCmd(0.07, 0.07),
    new DelayCommand(3000),

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
    intake_sys.FrontPurgeCmd(),

    // Long goal (drive to and score)

    new Parallel({
      drive_sys.FollowTrajectoryCmd(right_loader_to_goal(), trajectory_follower_config),
      (new InOrder({new DelayCommand(200), SunroofSolCmd(false), new DelayCommand(550), intake_sys.OutBackCmd()}))->withTimeout(3),
    }),

    // drive_sys.TurnToHeadingCmd(180, 0.8)->withTimeout(.5),
    // new Parallel({
    //   (new InOrder({
    //     drive_sys.DriveToPointCmd({43.25, 23.75}, vex::reverse, 0.8, 0.8)->withTimeout(2), 
    //     DriveTankRawCmd(-0.45, -0.45)}))->withTimeout(3),
    //   (new InOrder({new DelayCommand(650), intake_sys.OutBackCmd()}))->withTimeout(3),
    // }),
    new DelayCommand(2950),

    /*DriveTankRawCmd(0.5, 0.5),
    new DelayCommand(250),
    SunroofSolCmd(true),*/
    DriveTankRawCmd(-0.45, -0.45),
  };

  cc.run();
}

void left_auto_path() {
  printf("Build %d\n", LOG);
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

    // Starts at {19.5, 86.5, from_degrees(90)}

    // Matchloader
    intake_sys.MatchLoaderCmd(true),
    drive_sys.DriveForwardCmd(32.75, vex::forward, 0.8)->withTimeout(1.5),
    drive_sys.TurnToHeadingCmd(180, .8)->withTimeout(2.25),
    SunroofSolCmd(true),
    intake_sys.AutoLoadCmd(),
    DriveTankRawCmd(0.4, 0.4),
    new DelayCommand(600),
    DriveTankRawCmd(0.1, 0.1),
    new DelayCommand(4500),

    // Leaving Matchloader
    intake_sys.MatchLoaderCmd(false),
    SunroofSolCmd(false),
    intake_sys.FrontPurgeCmd(),

    // Long goal (drive to and score)
    drive_sys.TurnToHeadingCmd(181, 0.8)->withTimeout(.5),
    new Parallel({
      (new InOrder({
        //drive_sys.DriveToPointCmd({43.25, 118}, vex::reverse, 0.8, 0.8)->withTimeout(2), 
        drive_sys.DriveForwardCmd(33, vex::reverse, 0.8, 0.8)->withTimeout(2),
        DriveTankRawCmd(-0.45, -0.45)}))->withTimeout(3),
      (new InOrder({new DelayCommand(650), intake_sys.OutBackCmd()}))->withTimeout(3),
    }),
    new DelayCommand(3950),

    DriveTankRawCmd(0.5, 0.5),
    new DelayCommand(250),
    SunroofSolCmd(true),
    DriveTankRawCmd(-0.45, -0.45),
  };

  cc.run();
}

// THIS SKILLS PATH WILL NOT WORK, AS IT WAS WRITTEN WITH DIFFERENT DRIVE PID VALUES OF .04, .0025, 1, .5 AND TURN PID VALUES OF 2, .5 (STARTING AT DEADBANDS)
void skills_path() {
  printf("Build %d\n", LOG);
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

    // Collect cata's preload
    new DelayCommand(1000),
    intake_sys.IntakeCmd(),
    intake_sys.MatchLoaderCmd(true),
    new DelayCommand(1500),

    // Score preloads
    drive_sys.DriveToPointCmd({23.75, 23}, vex::reverse, .75)->withTimeout(3),
    drive_sys.TurnToHeadingCmd(180, .75)->withTimeout(1.75), 
    drive_sys.DriveToPointCmd({40.5, 23}, vex::reverse, .5, .25)->withTimeout(3),
    intake_sys.OutBackCmd(),
    DriveTankRawCmd(-.35, -.35),
    new DelayCommand(2500),
    new FunctionCommand([](){
      odom.set_position(Pose2d({42.5, 23}, odom.get_position().rotation().radians()));
      return true;
    }),

    // Collect matchloads
    drive_sys.DriveForwardCmd(6, vex::forward, .7)->withTimeout(2),
    drive_sys.TurnToPointCmd({10.125, 23}, vex::forward, .6)->withTimeout(2),
    intake_sys.AutoLoadCmd(),
    new FunctionCommand([]() {
      intake_sys.autoload_prefer = intake_sys.BlockColor::BLUE;
      return true;
    }),
    new Parallel({
      drive_sys.DriveToPointCmd({17, 23}, vex::forward, .7)->withTimeout(2),
      SunroofSolCmd(true),
    }),

    drive_sys.TurnToHeadingCmd(181.5, .6),
    DriveTankRawCmd(0.5, 0.5),
    new DelayCommand(5500),
    DriveTankRawCmd(0, 0),
    new FunctionCommand([](){
      odom.set_position(Pose2d({12.5, 23}, odom.get_position().rotation().radians()));
      return true;
    }),

    // Score matchloads
    drive_sys.TurnToPointCmd({40, 23}, vex::reverse, .6)->withTimeout(2),
    drive_sys.DriveToPointCmd({40, 23}, vex::reverse, .5)->withTimeout(4),
    drive_sys.TurnToHeadingCmd(180, .6)->withTimeout(1),

    SunroofSolCmd(false),
    intake_sys.OutBackCmd(),
    DriveTankRawCmd(-.2, -.2),
    new DelayCommand(4000),
    intake_sys.IntakeStopCmd(),
    DriveTankRawCmd(0,0),
    new FunctionCommand([](){
      odom.set_position(Pose2d({42.5, 23}, odom.get_position().rotation().radians()));
      return true;
    }),

    // Park
    intake_sys.MatchLoaderCmd(false),
    drive_sys.DriveForwardCmd(12, vex::forward, .5)->withTimeout(2),
    SunroofSolCmd(true),
    drive_sys.DriveForwardCmd(12, vex::reverse, .2)->withTimeout(2),
    drive_sys.DriveForwardCmd(12, vex::forward, .5)->withTimeout(2),
    /*drive_sys.TurnToPointCmd({6.5, 40}, vex::forward, .7)->withTimeout(2),
    drive_sys.DriveToPointCmd({6.5, 40}, vex::forward, .5)->withTimeout(2),
    drive_sys.TurnToHeadingCmd(90, .7)->withTimeout(2),
    intake_sys.IntakeCmd(),
    new DelayCommand(10000),
    DriveTankRawCmd(.4, .4),
    new DelayCommand(1750),
    DriveTankRawCmd(0, 0),*/
  };

  cc.run();
}
