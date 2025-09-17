#include "robot-config.h"

    vex::brain Brain;
    vex::controller con;

    //MOTORS
    vex::motor front_left(PORT1, vex::gearSetting::ratio6_1, true);
    vex::motor back_left(PORT2, vex::gearSetting::ratio6_1, true);
    vex::motor middle_left(PORT3, vex::gearSetting::ratio6_1, true);
    vex::motor_group left_motors(front_left, back_left, middle_left);

    vex::motor back_right(PORT8, vex::gearSetting::ratio6_1, false);
    vex::motor middle_right(PORT7, vex::gearSetting::ratio6_1, false);
    vex::motor front_right(PORT20, vex::gearSetting::ratio6_1, false);
    vex::motor_group right_motors(back_right, middle_right, front_right);

    //SENSORS
    vex::inertial imu(PORT12);
    PID::pid_config_t drive_pid_cfg{
        .p = 0.0,
        .i = 0.0,
        .d = 0.0,
        .deadband = 0.5,
        .on_target_time = 0.1,
        .error_method = PID::ERROR_TYPE::LINEAR,
    };

    PID drive_pid{drive_pid_cfg};

    PID::pid_config_t turn_pid_cfg{
        .p = 0.0,
        .i = 0.0,
        .d = 0.0,
        .deadband = 1,
        .on_target_time = 0.1,
        .error_method = PID::ERROR_TYPE::ANGULAR,
    };

    PID turn_pid{turn_pid_cfg};

    PID::pid_config_t correction_pid_cfg{
        .p = 0.0,
        .i = 0.0,
        .d = 0.0,
        .deadband = 0.5,
        .on_target_time = 0.1,
        .error_method = PID::ERROR_TYPE::ANGULAR,
    };

    PID correction_pid{correction_pid_cfg};

    robot_specs_t robot_cfg{
        .robot_radius = 10,
        .correction_pid = correction_pid_cfg,
        .drive_feedback = &drive_pid,
        .turn_feedback = &turn_pid,
        .odom_wheel_diam = 1.75,
        .dist_between_wheels = 12.25,
        .odom_gear_ratio = 0.75,
    };

    OdometryTank odom(left_motors, right_motors, robot_cfg, &imu);

    TankDrive drive_sys(left_motors, right_motors, robot_cfg, &odom);

    VDB::Device dev1{vex::PORT11, 115200 * 2};

    VDP::RegistryController registry_contoller{&dev1};
    

    PIDTuner tuner(drive_pid, drive_sys);

    VDP::PartPtr tuner_data = (std::shared_ptr<VDP::TimestampedRecord>)new VDP::TimestampedRecord(
    "tuner_record", new VDP::PIDTunerRecord("tuner_record", tuner));

    VDP::PartPtr odom_data = (std::shared_ptr<VDP::TimestampedRecord>)new VDP::TimestampedRecord(
    "odom_record", new VDP::OdometryDataRecord("odom_record", odom));

void robot_init() {
    printf("STARTED\n");

    VDP::ChannelID chan1 = registry_contoller.open_channel(tuner_data);
    VDP::ChannelID chan2 = registry_contoller.open_channel(odom_data);

    odom.set_position({0,0,0});
    while(imu.isCalibrating()){
        vexDelay(10);
    }
    printf("IMU Calibrated\n");

    bool ready = registry_contoller.negotiate();

    if (!ready) {
        Brain.Screen.printAt(20, 20, "FAILED");
        while (true) {
            vexDelay(1000);
        };
    }
    Brain.Screen.printAt(20, 20, "STSRTED");
    printf("Negotiation Finished\n");
    

    while (true) {
        // printf("P: %f, I: %f, D: %f, Error: %f\n", drive_pid.config.p, drive_pid.config.i, drive_pid.config.d, drive_pid.get_error());
        registry_contoller.send_data(chan1);
        registry_contoller.send_data(chan2);
        printf("%s\n", tuner_data->pretty_print_data().c_str());
        vexDelay(100);
    }
}
