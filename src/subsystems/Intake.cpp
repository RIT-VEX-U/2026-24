#include "subsystems/Intake.h"
#include <cstdio>
#include <v5_api.h>
#include <vex_optical.h>
#include <vex_triport.h>

IntakeSys::IntakeSys(vex::motor top_roller, vex::motor front_roller, vex::motor back_roller, vex::motor  agitator_roller, vex::optical color_sensor, vex::digital_out lightboard) : 
  top_roller(top_roller), front_roller(front_roller), back_roller(back_roller), agitator_roller(agitator_roller), color_sensor(color_sensor), lightboard(lightboard){
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

void IntakeSys::remove_this_color(BlockColor color_to_remove){
  this->color_to_remove = color_to_remove;
}

IntakeSys::BlockColor IntakeSys::seeing_color(){
  if(color_sensor.hue() > 10 && color_sensor.hue() < 30){
    printf("seeing red\n");
    return BlockColor::RED;
  }
  else if(color_sensor.hue() > 180 && color_sensor.hue() < 220){
    printf("seeing blue\n");
    return BlockColor::BLUE;
  }
  else{
    return BlockColor::NOTHING;
  }
}

void IntakeSys::color_sort(){
  if((seeing_color() == color_to_remove)){
    if(intake_state == IntakeState::IN){
      top_roller.spin(vex::forward, intake_volts, vex::volt);
      front_roller.spin(vex::forward, intake_volts, vex::volt);
      back_roller.spin(vex::forward, intake_volts, vex::volt);
      agitator_roller.spin(vex::reverse, intake_volts, vex::volt);
    }
    else if(intake_state == IntakeState::OUTBOTTOM){
      top_roller.spin(vex::forward, intake_volts, vex::volt);
      front_roller.spin(vex::forward, intake_volts, vex::volt);
      back_roller.spin(vex::forward, intake_volts, vex::volt);
      agitator_roller.spin(vex::reverse, intake_volts, vex::volt);
    }
    else if(intake_state == IntakeState::OUTMIDDLE){
      top_roller.spin(vex::reverse, intake_volts, vex::volt);
      front_roller.spin(vex::forward, intake_volts, vex::volt);
      back_roller.spin(vex::forward, intake_volts, vex::volt);
      agitator_roller.spin(vex::reverse, intake_volts, vex::volt);
    }
    else if(intake_state == IntakeState::OUTTOP){
      top_roller.spin(vex::forward, intake_volts, vex::volt);
      front_roller.spin(vex::forward, intake_volts, vex::volt);
      back_roller.spin(vex::forward, intake_volts, vex::volt);
      agitator_roller.spin(vex::reverse, intake_volts, vex::volt);
    }

  }

}

int IntakeSys::thread_fn(void *ptr){
  IntakeSys &self = *(IntakeSys *)ptr;
  while(true){
    if(self.do_color_sort){
      self.lightboard.set(true);
      self.color_sort();
    }
    else{
      self.lightboard.set(false);
    }
    if(!(self.do_color_sort && self.color_to_remove == self.seeing_color())){
      if(self.intake_state == IntakeState::IN){
        self.top_roller.stop();
        self.front_roller.spin(vex::forward, self.intake_volts, vex::volt);
        self.back_roller.spin(vex::reverse, self.intake_volts, vex::volt);
        self.agitator_roller.spin(vex::forward, self.intake_volts, vex::volt);
      }
      else if(self.intake_state == IntakeState::OUTBOTTOM){
        self.top_roller.stop();
        self.front_roller.spin(vex::reverse, self.intake_volts, vex::volt);
        self.back_roller.spin(vex::forward, self.intake_volts, vex::volt);
        self.agitator_roller.spin(vex::reverse, self.intake_volts, vex::volt);
      }
      else if(self.intake_state == IntakeState::OUTMIDDLE){
        self.top_roller.spin(vex::forward, self.intake_volts, vex::volt);
        self.front_roller.spin(vex::forward, self.intake_volts, vex::volt);
        self.back_roller.spin(vex::forward, self.intake_volts, vex::volt);
        self.agitator_roller.spin(vex::reverse, self.intake_volts, vex::volt);
      }
      else if(self.intake_state == IntakeState::OUTTOP){
        self.top_roller.spin(vex::reverse, self.intake_volts, vex::volt);
        self.front_roller.spin(vex::forward, self.intake_volts, vex::volt);
        self.back_roller.spin(vex::forward, self.intake_volts, vex::volt);
        self.agitator_roller.spin(vex::reverse, self.intake_volts, vex::volt);
      }
      else if(self.intake_state == IntakeState::STOPPED){
        self.top_roller.stop();
        self.front_roller.stop();
        self.back_roller.stop();
        self.agitator_roller.stop();
      }
    }
    vexDelay(10);
  }
  return 0;
}
