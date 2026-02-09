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

// ==================== Constructor ======================
  IntakeSys(vex::motor top_roller, vex::motor front_roller, vex::motor back_roller, vex::motor agitator_roller, vex::motor back_score_roller, vex::optical lower_intake_sensor, vex::optical middle_intake_sensor, vex::digital_out lightboard, vex::digital_out matchloader_sol);

// ==================== Helper Enums =====================
  enum IntakeState{
    IN,
    OUTBACK, // Like the restaurant
    OUTTOP,
    OUTMIDDLE,
    OUTBOTTOM,
    STOPPED,
    AUTOLOAD
  };

  enum BlockColor{
    RED,
    BLUE,
    NOTHING,
  };

// ==================== Intake States ====================

  IntakeState get_intake_state(){return intake_state;}

  AutoCommand *IntakeCmd(double volts = 12);
  AutoCommand *OutBottomCmd(double volts = 12);
  AutoCommand *OutMiddleCmd(double volts = 12);
  AutoCommand *OutTopCmd(double volts = 12);
  AutoCommand *OutBackCmd(double volts = 12);
  AutoCommand *AutoLoadCmd();
  AutoCommand *IntakeStopCmd();
  AutoCommand *ColorSortCmd(bool do_color_sort);
  AutoCommand *MatchLoaderCmd(bool do_color_sort);

  void intake(double volts = 12);
  void outbottom(double volts = 12);
  void outmiddle(double volts = 12);
  void outtop(double volts = 12);
  void outback(double volts = 12);
  void autoload();
  void intake_stop();

  void lock_state(bool lock = true);

// ==================== Color Sorting ====================

  bool is_color_sorting();
  void start_color_sorting();
  void stop_color_sorting();

  BlockColor seeing_color(vex::optical color_sensor);
  void remove_this_color(BlockColor color_to_remove);

// =================== Motor Behavior ====================

  void run_state_machine(bool sorting);
  static int thread_fn(void *tr);

  void spin_motor(vex::motor &motor, double volts, bool jammed);

// ==================== Jam Detection ====================

  void auto_fix_jamming(bool fix_jamming);
  bool motor_jammed(vex::motor motor);

// ==================== Match Loading ====================

  void match_load(bool do_match_load);
  bool is_match_loading();

private:
  // Here lied shitty_timer. RIP

// ===================== Color Sort ======================

  bool do_color_sort = false;
  BlockColor color_to_remove = BLUE;

// ================= Motors and Sensors ==================

  vex::motor top_roller;
  vex::motor front_roller;
  vex::motor back_roller;
  vex::motor agitator_roller;
  vex::motor back_score_roller;
  vex::optical lower_intake_sensor;
  vex::optical middle_intake_sensor;
  vex::digital_out lightboard;
  vex::digital_out matchloader_sol;

// ==================== State Machine ====================

  vex::task task;
  IntakeState intake_state;
  bool state_unlocked = true;

  double intake_volts;
  bool do_jam_fix = false;

};