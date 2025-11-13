#pragma once
#include "vex.h"
#include <vex_motor.h>
#include <vex_task.h>
#include <vex_timer.h>
#include <vex_triport.h>

class IntakeSys{
public:
  IntakeSys(vex::motor top_roller, vex::motor front_roller, vex::motor back_roller, vex::motor agitator_roller, vex::optical color_sensor, vex::digital_out lightboard);

  enum IntakeState{
    IN,
    OUTTOP,
    OUTMIDDLE,
    OUTBOTTOM,
    STOPPED,
  };

  enum BlockColor{
    RED,
    BLUE,
    NOTHING,
  };

  IntakeState get_intake_state(){return intake_state;}

  void intake(double volts = 12);

  void outbottom(double volts = 12);

  void outmiddle(double volts = 12);

  void outtop(double volts = 12);

  void intake_stop();

  void remove_this_color(BlockColor color_to_remove);

  bool is_color_sorting();

  void start_color_sorting();

  void stop_color_sorting();

  void color_sort();

  BlockColor seeing_color();

  static int thread_fn(void *tr);

  private:
  int intake_time = 0;
  int shitty_timer = 500;
  bool do_color_sort = false;
  BlockColor color_to_remove = BLUE;
  int blocks_held = 0;
  vex::motor top_roller;
  vex::motor front_roller;
  vex::motor back_roller;
  vex::motor agitator_roller;
  vex::optical color_sensor;
  vex::digital_out lightboard;
  vex::task task;
  double intake_volts;
  IntakeState intake_state;
};
