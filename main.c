#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include "platform.h"

int main(int argc, char* argv[]) {
  printf("Try16: A retro-style rapid fire counter.\n");
  clock_t start       = platform_clock();
  clock_t last        = platform_clock();
  float   elapsed_sec = platform_elapsed(start, last);
  unsigned char prev_trig = 0;
  unsigned int  count     = 0;
  while (elapsed_sec < 10.0f) {
    clock_t now   = platform_clock();
    while(platform_elapsed(last, now) < (1.0f / 60.0f)) { // NTSC Only
      now = platform_clock(); // wait vsync (60FPS)
    }
    last = platform_clock();
    elapsed_sec = platform_elapsed(start, now);
    unsigned char trig = platform_gttrig(0);
    if (!prev_trig && trig) {
      count++;
    }
    prev_trig = trig;
    printf("\rElapsed: %.1f sec   Count: %d      ", elapsed_sec, count);
    fflush(stdout);
  }
  float avg = count / 10.0f;
  printf("\nAvg %.2f shots/sec\n", avg);
  printf("\nDone.\n");
  return 0;
}
