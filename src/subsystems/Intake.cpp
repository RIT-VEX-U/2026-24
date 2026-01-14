#include "subsystems/Intake.h"
#include "robot-config.h"
#include <cstdio>
#include <v5_api.h>
#include <vex_global.h>
#include <vex_optical.h>
#include <vex_triport.h>
#include <vex_units.h>

IntakeSys::IntakeSys(vex::motor top_roller, vex::motor front_roller, vex::motor back_roller, vex::motor agitator_roller, vex::motor back_score_roller, vex::optical lower_intake_sensor, vex::optical middle_intake_sensor, vex::digital_out lightboard, vex::digital_out matchloader_sol) : 
  top_roller(top_roller), front_roller(front_roller), back_roller(back_roller), agitator_roller(agitator_roller), back_score_roller(back_score_roller), lower_intake_sensor(lower_intake_sensor), middle_intake_sensor(middle_intake_sensor), lightboard(lightboard), matchloader_sol(matchloader_sol){
  task = vex::task(thread_fn, this);
};

void IntakeSys::intake(double volts){
  this->intake_volts = volts;
  intake_state = IntakeState::IN;
}
void IntakeSys::outbottom(double volts){
  this->intake_volts = volts;
  intake_state = IntakeState::OUTBOTTOM;
}
void IntakeSys::outmiddle(double volts){
  this->intake_volts = volts;
  intake_state = IntakeState::OUTMIDDLE;
}
void IntakeSys::outtop(double volts){
  this->intake_volts = volts;
  intake_state = IntakeState::OUTTOP;
}
void IntakeSys::outback(double volts) {
  this->intake_volts = volts;
  intake_state = IntakeState::OUTBACK;
}
void IntakeSys::intake_stop(){
  intake_state = IntakeState::STOPPED;
}

bool IntakeSys::is_color_sorting(){
  return do_color_sort;
}

void IntakeSys::start_color_sorting(){
  do_color_sort = true;
}

void IntakeSys::stop_color_sorting(){
  do_color_sort = false;
}

void IntakeSys::match_load(bool do_match_load){
  matchloader_sol.set(do_match_load);
}

void IntakeSys::auto_fix_jamming(bool fix_jamming){
  do_jam_fix = fix_jamming;
}

bool IntakeSys::is_match_loading(){
  return matchloader_sol.value();
}

void IntakeSys::remove_this_color(BlockColor color_to_remove){
  this->color_to_remove = color_to_remove;
}

IntakeSys::BlockColor IntakeSys::seeing_color(vex::optical color_sensor){
  if(color_sensor.hue() > 10 && color_sensor.hue() < 30){
    return BlockColor::RED;
  }
  else if(color_sensor.hue() > 180 && color_sensor.hue() < 220){
    return BlockColor::BLUE;
  }
  else{
    return BlockColor::NOTHING;
  }
}

void IntakeSys::front_roller_behavior(bool jammed, bool sorting){
  double volts = intake_volts;
  if(!sorting && intake_state == OUTBOTTOM){
    volts *= -1;
  }
  else if(!sorting && intake_state == OUTMIDDLE){
    volts = 8;
  }
  if(jammed){
    volts *= -1;
  }
  if(intake_state == STOPPED){
    front_roller.stop();
  }
  else{
    front_roller.spin(vex::forward, volts, vex::volt);
  }
}

void IntakeSys::top_roller_behavior(bool jammed, bool sorting){
  double volts = intake_volts;
  if(!sorting && (intake_state == OUTTOP || intake_state == OUTBACK)){
    volts *= -1;
  }
  else if(sorting && intake_state == OUTMIDDLE){
    volts *= -1;
  }
  if(jammed){
    volts *= -1;
  }
  if(intake_state == STOPPED || (!sorting && (intake_state == IN || intake_state == OUTBOTTOM))){
    top_roller.stop();
  }
  else{
    top_roller.spin(vex::forward, volts, vex::volt);
  }
}
void IntakeSys::back_roller_behavior(bool jammed, bool sorting){
  double volts = intake_volts;
  if(!sorting && intake_state == IN){
    volts *= -1;
  }
  if(jammed){
    volts *= -1;
  }
  if(intake_state == STOPPED){
    back_roller.stop();
  }
  else{
    back_roller.spin(vex::forward, volts, vex::volt);
  }
}
void IntakeSys::agitator_roller_behavior(bool sorting){
  double volts = intake_volts;
  if(!sorting && (intake_state == OUTBOTTOM || intake_state == OUTMIDDLE || intake_state == OUTTOP || intake_state == OUTBACK)){
    volts *= -1;
  }
  else if(sorting){
    volts *= -1;
  }
  if(intake_state == STOPPED){
    agitator_roller.stop();
  }
  else{
    agitator_roller.spin(vex::forward, volts, vex::volt);
  }
}
void IntakeSys::back_score_roller_behavior(bool jammed, bool sorting) {
  double volts = intake_volts;
  if(!sorting && intake_state == OUTBACK) {
    volts *= -1;
  }
  if(jammed){
    volts *= -1;
  }
  if(intake_state == STOPPED || intake_state == IN || intake_state == OUTBOTTOM || intake_state == OUTMIDDLE) {
    back_score_roller.stop();
  } else {
    back_score_roller.spin(vex::forward, volts, vex::volt);
  }
}

bool IntakeSys::motor_jammed(vex::motor motor){
  if(motor.current(vex::currentUnits::amp) >= 0.5 && fabs(motor.velocity(vex::velocityUnits::rpm)) <= 0.1){
    return true;
  }
  else{
    return false;
  }
}



int IntakeSys::thread_fn(void *ptr){
  IntakeSys &self = *(IntakeSys *)ptr;
  while(true){
    bool sort_this_block = false;
    if(self.do_color_sort){
      self.lightboard.set(true);
      if(self.seeing_color(self.lower_intake_sensor) == self.color_to_remove){
        sort_this_block = true;
      }
    }
    else{
      self.lightboard.set(false);
    }
    self.front_roller_behavior(self.motor_jammed(self.front_roller), sort_this_block);
    self.top_roller_behavior(self.motor_jammed(self.top_roller), sort_this_block);
    self.back_roller_behavior(self.motor_jammed(self.back_roller), sort_this_block);
    self.agitator_roller_behavior(self.motor_jammed(self.agitator_roller));
    self.back_score_roller_behavior(self.motor_jammed(self.back_score_roller), sort_this_block);
    vexDelay(10);
  }
  return 0;
}

AutoCommand *IntakeSys::IntakeCmd(double volts) {
    return new FunctionCommand([this, volts]() {
      this->intake_volts = volts;
      this->intake_state = IntakeState::IN;
        return true;
    });
}
AutoCommand *IntakeSys::ColorSortCmd(bool do_color_sort) {
    return new FunctionCommand([this, do_color_sort]() {
    this->do_color_sort = do_color_sort;
        return true;
    });
}
AutoCommand *IntakeSys::OutBottomCmd(double volts) {
    return new FunctionCommand([this, volts]() {
      this->intake_volts = volts;
      this->intake_state = IntakeState::OUTBOTTOM;
        return true;
    });
}
AutoCommand *IntakeSys::OutMiddleCmd(double volts) {
    return new FunctionCommand([this, volts]() {
      this->intake_volts = volts;
      this->intake_state = IntakeState::OUTMIDDLE;
        return true;
    });
}
AutoCommand *IntakeSys::OutTopCmd(double volts) {
    return new FunctionCommand([this, volts]() {
      this->intake_volts = volts;
      this->intake_state = IntakeState::OUTTOP;
        return true;
    });
}
AutoCommand *IntakeSys::OutBackCmd(double volts) {
  return new FunctionCommand([this, volts]() {
    this->intake_volts = volts;
    this->intake_state = IntakeState::OUTBACK;
    return true;
  });
}
AutoCommand *IntakeSys::IntakeStopCmd() {
    return new FunctionCommand([this]() {
      this->intake_state = IntakeState::STOPPED;
        return true;
    });
}
AutoCommand *IntakeSys::MatchLoaderCmd(bool do_match_load) {
  return new FunctionCommand([this, do_match_load]() {
    matchloader_sol.set(do_match_load);
    return true;
  });
}
