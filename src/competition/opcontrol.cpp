#include "competition/opcontrol.h"
#include "robot-config.h"

void opcontrol() {
    con.ButtonX.pressed([](){
        Pose2d Robot_Position = odom.get_position();
        printf("X: %.2f, Y: %.2f, Rot: %.2f\n", Robot_Position.x(), Robot_Position.y(), Robot_Position.rotation().degrees());
    });
    con.ButtonA.pressed([](){
        if(tuner.IsTuning()){
            printf("PID TUNING STOPPED\n");
            tuner.StopTuning();
        }
        else{
            printf("PID TUNING STARTED\n");
            tuner.StartTuning();
        }
    });

    while (true) {
        if(!tuner.IsTuning()){
            double left = (double)con.Axis3.position() / 100;
        double right = (double)con.Axis1.position() / 100;
        drive_sys.drive_arcade(left, right, 1, TankDrive::BrakeType::None);
        vexDelay(100);
        }
    }
}

