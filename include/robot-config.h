#pragma once

#include "core.h"
#include "core/subsystems/odometry/odometry_tank.h"
#include "vex.h"
#include "subsystems/Intake.h"
#include "core/subsystems/odometry/odometry_onewheel.h"
#include <vex_optical.h>
#include <vex_triport.h>

extern vex::controller con;

//MOTORS

extern robot_specs_t robot_config;
// extern OdometryOneWheel odom;
extern OdometryTank odom;
extern TankDrive drive_sys;
extern IntakeSys intake_sys;

// ================ INPUTS ================
// Digital sensors

// Analog sensors

// ================ OUTPUTS ================
// Motors

// Pneumatics
extern vex::digital_out right_wing_solonoid;
extern vex::digital_out left_wing_solonoid;
extern vex::digital_out right_stick_solonoid;

// ================ SUBSYSTEMS ================

// ================ UTILS ================

void robot_init();
    //get match load
    // drive_sys.DriveForwardCmd(10, vex::reverse, 0.6),
