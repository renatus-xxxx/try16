#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>

#ifdef MSX
#define CLOCKS_PER_SEC 60
#define JIFFY          0xfc9e

clock_t platform_clock(void) {
  uint8_t* ptr = (uint8_t*)0xFC9E;
  return (clock_t)(ptr[0] | (ptr[1] << 8));
}

float platform_elapsed(clock_t start, clock_t end) {
  return ((end - start) & 0xFFFF) / (float)CLOCKS_PER_SEC;
}
#else
#include <time.h>

clock_t platform_clock(void) {
  return clock();
}

float platform_elapsed(clock_t start, clock_t end) {
  float sec = (float)(end - start) / CLOCKS_PER_SEC;
  return (sec <= 0.0f) ? (float)1e-6 : sec;
}
#endif

#endif // PLATFORM_H
