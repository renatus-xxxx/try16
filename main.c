#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include "platform.h"

int main(int argc, char* argv[]) {
  printf("Try16: A retro-style rapid fire counter.\n");
  clock_t start = platform_clock();
  clock_t now   = start;
  clock_t last  = start;
  unsigned int count = 0;
  unsigned char prev = 0;
  uint8_t show_flag = 0;
  while (platform_elapsed_ticks(start, platform_clock()) < 600) {
    unsigned char trig = platform_gttrig(1);
    if (!prev && trig) count++;
    prev = trig;
    now = platform_clock();
    if (platform_elapsed_ticks(last, now) >= 60) {
      show_flag = 1;
      last = now;
    }
    if (show_flag) {
      show_flag = 0;
      uint16_t ticks = platform_elapsed_ticks(start, now);
      int elapsed10 = (ticks * 10) / CLOCKS_PER_SEC;
      printf("\rElapsed: %d.%d sec Count: %d     ", elapsed10 / 10, elapsed10 % 10, count);
      fflush(stdout);
    }
  }
  printf("Avg %.2f shots/sec\n", count / 10.0f);
  printf("Done.");
  return 0;
}
