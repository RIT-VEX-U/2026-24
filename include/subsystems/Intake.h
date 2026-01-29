#pragma once
#include "core/utils/command_structure/auto_command.h"
#include "vex.h"
#include "core/utils/command_structure/auto_command.h"
#include <vex_motor.h>
#include <vex_task.h>
#include <vex_timer.h>
#include <vex_triport.h>

class IntakeSys{
public:
  IntakeSys(vex::motor top_roller, vex::motor front_roller, vex::motor back_roller, vex::motor agitator_roller, vex::motor back_score_roller, vex::optical lower_intake_sensor, vex::optical middle_intake_sensor, vex::digital_out lightboard, vex::digital_out matchloader_sol);

  enum IntakeState{
    IN,
    OUTBACK, // Like the restaurant
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

  AutoCommand *IntakeCmd(double volts = 12);
  AutoCommand *OutBottomCmd(double volts = 12);
  AutoCommand *OutMiddleCmd(double volts = 12);
  AutoCommand *OutTopCmd(double volts = 12);
  AutoCommand *OutBackCmd(double volts = 12);
  AutoCommand *IntakeStopCmd();
  AutoCommand *ColorSortCmd(bool do_color_sort);
  AutoCommand *MatchLoaderCmd(bool do_color_sort);

  void intake(double volts = 12);

  void outbottom(double volts = 12);

  void outmiddle(double volts = 12);

  void outtop(double volts = 12);

  void outback(double volts = 12);

  void intake_stop();

  void remove_this_color(BlockColor color_to_remove);

  void run_state_machine(bool sorting);

  void spin_motor(vex::motor &motor, double volts, bool jammed);

  bool is_color_sorting();

  void start_color_sorting();

  void stop_color_sorting();

  void auto_fix_jamming(bool fix_jamming);

  bool motor_jammed(vex::motor motor);

  void match_load(bool do_match_load);

  bool is_match_loading();

  BlockColor seeing_color(vex::optical color_sensor);

  static int thread_fn(void *tr);

  private:
  int intake_time = 0;
  int shitty_timer = 500;
  bool do_color_sort = false;
  bool do_jam_fix = false;
  BlockColor color_to_remove = BLUE;
  int blocks_held = 0;
  vex::motor top_roller;
  vex::motor front_roller;
  vex::motor back_roller;
  vex::motor agitator_roller;
  vex::motor back_score_roller;
  vex::optical lower_intake_sensor;
  vex::optical middle_intake_sensor;
  vex::digital_out lightboard;
  vex::digital_out matchloader_sol;
  vex::task task;
  double intake_volts;
  IntakeState intake_state;
};
