#include "subsystems/Intake.h"
#include "robot-config.h"
#include <cstdio>
#include <v5_api.h>
#include <vex_global.h>
#include <vex_optical.h>
#include <vex_triport.h>
#include <vex_units.h>

// ==================== Constructor ====================

IntakeSys::IntakeSys(
  vex::motor top_roller, vex::motor front_roller, vex::motor back_roller,
  vex::motor agitator_roller, vex::motor back_score_roller,
  vex::optical lower_intake_sensor, vex::optical middle_intake_sensor,
  vex::digital_out lightboard, vex::digital_out matchloader_sol
) : top_roller(top_roller), front_roller(front_roller), back_roller(back_roller),
    agitator_roller(agitator_roller), back_score_roller(back_score_roller),
    lower_intake_sensor(lower_intake_sensor), middle_intake_sensor(middle_intake_sensor),
    lightboard(lightboard), matchloader_sol(matchloader_sol), intake_state(STOPPED), intake_volts(12)
{
  task = vex::task(thread_fn, this);
}

// ==================== Intake States ====================

void IntakeSys::intake(double volts) {
  if(state_unlocked) { intake_volts = volts; intake_state = IN; } }
void IntakeSys::outbottom(double volts) { 
  if(state_unlocked) { intake_volts = volts; intake_state = OUTBOTTOM; } }
void IntakeSys::outmiddle(double volts) {
  if(state_unlocked) { intake_volts = volts; intake_state = OUTMIDDLE; } }
void IntakeSys::outtop(double volts) {
  if(state_unlocked) { intake_volts = volts; intake_state = OUTTOP; } }
void IntakeSys::outback(double volts) {
  if(state_unlocked) { intake_volts = volts; intake_state = OUTBACK; } }
void IntakeSys::autoload(double volts) {
  if(state_unlocked) { intake_volts = volts; intake_state = AUTOLOAD; } }
void IntakeSys::frontpurge(double volts) {
  if(state_unlocked) { intake_volts = volts; intake_state = FRONTPURGE; } }
void IntakeSys::intake_stop() { if(state_unlocked) intake_state = STOPPED;  }

void IntakeSys::lock_state(bool lock) {
  state_unlocked = !lock;
}

// ==================== Color Sorting ====================

bool IntakeSys::is_color_sorting()                  { return do_color_sort;    }
void IntakeSys::start_color_sorting()               { do_color_sort = true;    }
void IntakeSys::stop_color_sorting()                { do_color_sort = false;   }
void IntakeSys::remove_this_color(BlockColor color) { color_to_remove = color; }

// ==================== Match Loading ====================

bool IntakeSys::is_match_loading()             { return matchloader_sol.value();     }
void IntakeSys::match_load(bool do_match_load) { matchloader_sol.set(do_match_load); }

// ==================== Jam Detection ====================

void IntakeSys::auto_fix_jamming(bool fix_jamming) { do_jam_fix = fix_jamming; }

bool IntakeSys::motor_jammed(vex::motor motor) {
  return motor.current(vex::currentUnits::amp) >= 0.5 &&
                       fabs(motor.velocity(vex::velocityUnits::rpm)) <= 0.1;
}

// ==================== Sensor Utils ====================

IntakeSys::BlockColor IntakeSys::seeing_color(vex::optical sensor) {
  double hue = sensor.hue();
  if (hue > 10 && hue < 30) return RED;
  if (hue > 180 && hue < 220) return BLUE;
  return NOTHING;
}

// ==================== Motor Helper(s) ====================

void IntakeSys::spin_motor(vex::motor &motor, double volts, bool jammed) {
  if(motor.power() < 0.1) motor.stop();
  if (jammed) volts *= -1;
  motor.spin(vex::forward, volts, vex::volt);
}

// ==================== Motor Behavior ====================

void IntakeSys::run_state_machine(bool sorting) {
  static BlockColor autoload_prefer = BlockColor::NOTHING;
  double v = intake_volts;
  bool front_jammed = motor_jammed(front_roller);
  bool top_jammed = motor_jammed(top_roller);
  bool back_jammed = motor_jammed(back_roller);
  bool back_score_jammed = motor_jammed(back_score_roller);

  if(intake_state != AUTOLOAD) {
    autoload_prefer = BlockColor::NOTHING; lightboard.set(do_color_sort); }
  switch (intake_state) {
    case STOPPED:
      front_roller.stop();
      top_roller.stop();
      back_roller.stop();
      agitator_roller.stop();
      back_score_roller.stop();
      break;

    case IN:
      spin_motor(front_roller, v, front_jammed);
      if (sorting) {
        spin_motor(top_roller, v, top_jammed);
      } else {
        top_roller.stop();
      }
      spin_motor(back_roller, -v, back_jammed);
      spin_motor(agitator_roller, v, false);
      back_score_roller.stop();
      break;

    case OUTBOTTOM:
      spin_motor(front_roller, -v, front_jammed);
      if (sorting) {
        spin_motor(top_roller, v, top_jammed);
      } else {
        top_roller.stop();
      }
      spin_motor(back_roller, v, back_jammed);
      spin_motor(agitator_roller, -v, false);
      back_score_roller.stop();
      break;

    case OUTMIDDLE:
      spin_motor(front_roller, v, front_jammed); // Set to 8 for skills
      spin_motor(top_roller, sorting ? -v : v, top_jammed);
      spin_motor(back_roller, v, back_jammed);
      spin_motor(agitator_roller, -12, false); // Set to -v for skills
      back_score_roller.stop();
      break;

    case OUTTOP:
      spin_motor(front_roller, v, front_jammed);
      spin_motor(top_roller, -v, top_jammed);
      spin_motor(back_roller, v, back_jammed);
      spin_motor(agitator_roller, -v, false);
      spin_motor(back_score_roller, v, back_score_jammed);
      break;

    case OUTBACK:
      spin_motor(front_roller, v, front_jammed);
      spin_motor(top_roller, -v, top_jammed);
      spin_motor(back_roller, v, back_jammed);
      spin_motor(agitator_roller, -v, false);
      spin_motor(back_score_roller, -v, back_score_jammed);
      break;

    case AUTOLOAD: {
      // Color-Independent Motors
      lightboard.set(true);
      spin_motor(front_roller, v, front_jammed);
      spin_motor(back_score_roller, -8, back_score_jammed);
      spin_motor(top_roller, -v, top_jammed);
      spin_motor(agitator_roller, v, false);
      
      // Color-Dependent Motors
      BlockColor currentBlock = seeing_color(lower_intake_sensor);
      if(autoload_prefer == BlockColor::NOTHING) {      // Beginning of Macro
        autoload_prefer = currentBlock;
        back_roller.stop();
      } else if(currentBlock == autoload_prefer) {      // Seeing preferred
        spin_motor(back_roller, -v, back_jammed);
      } else if(currentBlock != BlockColor::NOTHING) {  // Seeing opposing
        spin_motor(back_roller, v, back_jammed);
      } // Seeing nothing defaults to previous state
      
      break; }

    case FRONTPURGE:
      //spin_motor(front_roller, -v, front_jammed);
      front_roller.stop();
      spin_motor(agitator_roller, v, false);
      back_roller.stop();
      spin_motor(top_roller, -v, top_jammed);
      spin_motor(back_score_roller, v, back_score_jammed);
      break;
  }
}

// ==================== Background Thread ====================

int IntakeSys::thread_fn(void *ptr) {
  IntakeSys &self = *static_cast<IntakeSys *>(ptr);

  while (true) {
    bool sorting = self.do_color_sort &&
                   self.seeing_color(self.lower_intake_sensor) == self.color_to_remove;

    self.lightboard.set(self.do_color_sort);
    self.run_state_machine(sorting);

    vexDelay(10);
  }
  return 0;
}

// ==================== AutoCommands ====================

AutoCommand *IntakeSys::IntakeCmd(double volts)    { return new FunctionCommand([this, volts]() { intake(volts);    return true; }); }
AutoCommand *IntakeSys::OutBottomCmd(double volts) { return new FunctionCommand([this, volts]() { outbottom(volts); return true; }); }
AutoCommand *IntakeSys::OutMiddleCmd(double volts) { return new FunctionCommand([this, volts]() { outmiddle(volts); return true; }); }
AutoCommand *IntakeSys::OutTopCmd(double volts)    { return new FunctionCommand([this, volts]() { outtop(volts);    return true; }); }
AutoCommand *IntakeSys::OutBackCmd(double volts)   { return new FunctionCommand([this, volts]() { outback(volts);   return true; }); }
AutoCommand *IntakeSys::IntakeStopCmd()            { return new FunctionCommand([this]()        { intake_stop();    return true; }); }
AutoCommand *IntakeSys::AutoLoadCmd()              { return new FunctionCommand([this]()        { autoload();       return true; }); }
AutoCommand *IntakeSys::FrontPurgeCmd()            { return new FunctionCommand([this]()        { frontpurge();     return true; }); }

AutoCommand *IntakeSys::ColorSortCmd(bool enabled)         { return new FunctionCommand([this, enabled]()       { do_color_sort = enabled;   return true; }); }
AutoCommand *IntakeSys::MatchLoaderCmd(bool do_match_load) { return new FunctionCommand([this, do_match_load]() { match_load(do_match_load); return true; }); }
