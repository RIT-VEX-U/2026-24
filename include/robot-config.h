#pragma once

#include "core.h"
#include "logger/logger.h"
#include "core/subsystems/odometry/odometry_lidar_wrapper.h"
#include "core/subsystems/odometry/odometry_tank.h"
#include "subsystems/ControllerReceiver.h"
#include "subsystems/Lidar.h"
#include "vex.h"
#include "subsystems/Intake.h"
#include "core/subsystems/odometry/odometry_onewheel.h"
#include <vex_optical.h>
#include <vex_triport.h>

extern vex::controller con;
extern vex::brain Brain;

//MOTORS

extern robot_specs_t config;
extern LidarReceiver lidar;
extern OdometryTank odom_tank;
extern OdometryLidarWrapper odom;
extern TankDrive drive_sys;
extern IntakeSys intake_sys;


extern vex::motor_group left_motors;
extern vex::motor_group right_motors;
extern vex::inertial imu;
extern bool DRIVE_FINISHED_FLAG;
extern uint64_t init_us;

extern SerialLogger logger;
extern ControllerReceiver tcp_con;
// ================ INPUTS ================
// Digital sensors

// Analog sensors

// ================ OUTPUTS ================
// Motors

// Pneumatics
extern vex::digital_out right_wing_solonoid;
extern vex::digital_out left_wing_solonoid;
extern vex::digital_out right_stick_solonoid;
extern vex::digital_out sunroof_solonoid;

// ================ SUBSYSTEMS ================

// ================ UTILS ================

void robot_init();
    //get match load
    // drive_sys.DriveForwardCmd(10, vex::reverse, 0.6),
