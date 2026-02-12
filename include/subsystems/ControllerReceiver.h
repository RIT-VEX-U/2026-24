#pragma once

#include <cstdint>
#include <cstddef>
#include <vex_task.h>

// Forward declaration of thread entrypoint
int controller_thread(void* ptr);

/**
 * Receives controller state from stdin using a terminal-friendly line protocol.
 *
 * Line format (one update per line):
 *   <buttons_hex4> <RX> <RY> <LY> <LX>
 * Example:
 *   0011 10 -5 127 0
 *
 * buttons bits:
 *  0 Up, 1 Down, 2 Left, 3 Right,
 *  4 A, 5 B, 6 X, 7 Y,
 *  8 L1, 9 L2, 10 R1, 11 R2
 *
 * Axes are signed int8 in [-128,127]:
 *  RX=axis1 (right X), RY=axis2 (right Y), LY=axis3 (left Y), LX=axis4 (left X)
 */
class ControllerReceiver {
public:
  ControllerReceiver();

  void start();
  void stop();

  bool isRunning() const { return running_; }

  // Snapshot of latest decoded state
  uint16_t buttons() const { return buttons_; }
  double rightX() const { return rightX_; }
  double rightY() const { return rightY_; }
  double leftY()  const { return leftY_; }
  double leftX()  const { return leftX_; }

  // Convenience booleans
  bool up()    const { return getBit(0); }
  bool down()  const { return getBit(1); }
  bool left()  const { return getBit(2); }
  bool right() const { return getBit(3); }

  bool a()  const { return getBit(4); }
  bool b()  const { return getBit(5); }
  bool x()  const { return getBit(6); }
  bool y()  const { return getBit(7); }

  bool l1() const { return getBit(8); }
  bool l2() const { return getBit(9); }
  bool r1() const { return getBit(10); }
  bool r2() const { return getBit(11); }

private:
  // Thread control
  vex::task* handle_ = nullptr;
  bool running_ = false;

  // Timing
  uint64_t last_tick_us_ = 0;

  // State (updated by thread)
  volatile uint16_t buttons_ = 0;
  volatile double rightX_ = 0;
  volatile double rightY_ = 0;
  volatile double leftY_  = 0;
  volatile double leftX_  = 0;

  // Line assembly
  char line_[256]{};
  size_t line_len_ = 0;

  // Parsing helpers
  bool updateFromLine(const char* line);
  bool getBit(unsigned bit) const {
    return (buttons_ & (static_cast<uint16_t>(1u) << bit)) != 0;
  }

  friend int controller_thread(void* ptr);
};
