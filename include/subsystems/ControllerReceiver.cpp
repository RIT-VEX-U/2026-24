#include "subsystems/ControllerReceiver.h"
#include "vex.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <iostream>

#include <fcntl.h>
#include <unistd.h>

static inline bool is_ws(char c) { return c == ' ' || c == '\t'; }

// Tokenizer like in my earlier snippet, but non-static (re-entrant per call).
static char* nextToken(char*& p) {
  if (!p) return nullptr;
  while (*p && is_ws(*p)) ++p;
  if (!*p) return nullptr;
  char* tok = p;
  while (*p && !is_ws(*p)) ++p;
  if (*p) *p++ = '\0';
  return tok;
}

static bool parseU16Hex(const char* str, uint16_t& out) {
  errno = 0;
  char* end = nullptr;
  unsigned long v = std::strtoul(str, &end, 16);
  if (errno != 0 || end == str || *end != '\0') return false;
  if (v > 0xFFFFul) return false;
  out = static_cast<uint16_t>(v);
  return true;
}

static bool parseI8(const char* str, int8_t& out) {
  errno = 0;
  char* end = nullptr;
  long v = std::strtol(str, &end, 10);
  if (errno != 0 || end == str || *end != '\0') return false;
  if (v < -128 || v > 127) return false;
  out = static_cast<int8_t>(v);
  return true;
}

ControllerReceiver::ControllerReceiver() {}

void ControllerReceiver::start() {
  if (!running_) {
    running_ = true;
    last_tick_us_ = vexSystemHighResTimeGet(); // caller can subtract init_us if you want, not required
    handle_ = new vex::task{controller_thread, (void*)this};
  }
}

void ControllerReceiver::stop() {
  running_ = false;
}

bool ControllerReceiver::updateFromLine(const char* line) {
  if (!line) return false;
  // printf("oiwjef\n");

  char buf[256];
  snprintf(buf, sizeof(buf), "%s", line);

  // Strip comments
  if (char* hash = std::strchr(buf, '#')) *hash = '\0';

  // Trim leading whitespace
  char* p = buf;
  while (*p && is_ws(*p)) ++p;
  if (*p == '\0') return false;

  // Tokenize
  char* walk = p;
  char* tButtons = nextToken(walk);
  char* tRX      = nextToken(walk);
  char* tRY      = nextToken(walk);
  char* tLY      = nextToken(walk);
  char* tLX      = nextToken(walk);

  if (!tButtons || !tRX || !tRY || !tLY || !tLX) return false;

  uint16_t bmask = 0;
  int8_t rx = 0, ry = 0, ly = 0, lx = 0;

  if (!parseU16Hex(tButtons, bmask)) return false;
  if (!parseI8(tRX, rx)) return false;
  if (!parseI8(tRY, ry)) return false;
  if (!parseI8(tLY, ly)) return false;
  if (!parseI8(tLX, lx)) return false;

  // Commit atomically-ish: these are single stores; good enough for VEX use.
  buttons_ = bmask;
  rightX_ = (rx / 127.0)*100; // axis1
  rightY_ = (ry / 127.0)*100; // axis2
  leftY_  = (ly / 127.0)*100; // axis3
  leftX_  = (lx / 127.0)*100; // axis4

  return true;
}

// Thread entrypoint (VEX style)
int controller_thread(void* ptr) {
  ControllerReceiver& obj = *((ControllerReceiver*)ptr);

  constexpr uint64_t kPeriodUs = 10000; // 10ms
  uint64_t next_tick = vexSystemHighResTimeGet() + kPeriodUs;

  while (obj.running_) {
    // 1) Poll all currently-available bytes (non-blocking)
    int ch;
    while ((ch = vexSerialReadChar(1)) != -1) {  // channel 1 = USB
      const char c = static_cast<char>(ch);
      
      if (c == '\r') continue;

      if (c == '\n') {
        obj.line_[obj.line_len_] = '\0';
        obj.updateFromLine(obj.line_);
        obj.line_len_ = 0;
      } else {
        if (obj.line_len_ + 1 < sizeof(obj.line_)) {
          obj.line_[obj.line_len_++] = c;
        } else {
          // overflow -> drop the line
          obj.line_len_ = 0;
        }
      }
    }

    // 2) Enforce minimum update cadence (10ms): yield until next tick
    while ((int64_t)(vexSystemHighResTimeGet() - next_tick) < 0) {
      vex::this_thread::yield();
      if (!obj.running_) break;
    }
    next_tick += kPeriodUs;

    // 3) If we fell way behind, resync schedule (prevents runaway catch-up)
    uint64_t now = vexSystemHighResTimeGet();
    if (now - next_tick > 100 * kPeriodUs) {
      next_tick = now + kPeriodUs;
    }
  }

  return 0;
}
