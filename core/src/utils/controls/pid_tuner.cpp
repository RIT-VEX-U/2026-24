#include "core/utils/controls/pid_tuner.h"
#include <vex_global.h>

PIDTuner::PIDTuner(PID pid, TankDrive &drive_sys): pid(pid), drive_sys(drive_sys){
    task = vex::task(thread_fn, this);
};

double PIDTuner::GetSetpoint(){
    return setpoint; 
}

void PIDTuner::SetSetpoint(double setpoint){
    this->setpoint = setpoint;
}

void PIDTuner::StartTuning(){
    tuning = true;
}

void PIDTuner::StopTuning(){
    tuning = false;
}

bool PIDTuner::IsTuning(){
    return tuning;
}

int PIDTuner::thread_fn(void *ptr) {
    PIDTuner &self = *(PIDTuner *)ptr;
    while (true) {
        if(self.tuning){
            if(self.pid.config.error_method == PID::ERROR_TYPE::ANGULAR){
                self.drive_sys.turn_to_heading(self.setpoint);
            }
            else if(self.pid.config.error_method == PID::ERROR_TYPE::LINEAR){
              if(self.pid.get_error() >= 0){
                self.drive_sys.drive_to_point(self.setpoint, self.drive_sys.get_position().y(), vex::forward, 1, 0);
              }else {
                self.drive_sys.drive_to_point(self.setpoint, self.drive_sys.get_position().y(), vex::reverse, 1, 0);
              }
            }
        }
        vexDelay(100);
    }
    return 0;
}  
