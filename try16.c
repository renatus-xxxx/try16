#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include "platform.h"

void print_status(int elapsed, int count) {
  char buf[32];
  buf[0] = '\r';
  buf[1] = '0' + (elapsed / 10);
  buf[2] = '0' + (elapsed % 10);
  buf[3] = '.';
  buf[4] = '0';
  buf[5] = ' ';
  buf[6] = 's';
  buf[7] = 'e';
  buf[8] = 'c';
  buf[9] = ' ';
  buf[10] = '0' + (count / 100);
  buf[11] = '0' + (count / 10 % 10);
  buf[12] = '0' + (count % 10);
  buf[13] = ' ';
  buf[14] = 's';
  buf[15] = 'h';
  buf[16] = 'o';
  buf[17] = 't';
  buf[18] = 's';
  buf[19] = ' ';
  buf[20] = ' ';
  buf[21] = ' ';
  buf[22] = '\0';
  fputs(buf, stdout);
}


int main(int argc, char* argv[]) {
  printf("Try16: A retro-style rapid fire counter.\n");
  printf("Strike SPACE key or Trigger-A button to start\n");
  int trigger_source = -1;
  while (trigger_source < 0) {
    if (platform_gttrig(0)) trigger_source = 0;      // SPACE key
    else if (platform_gttrig(1)) trigger_source = 1; // Port A
    else if (platform_gttrig(2)) trigger_source = 2; // Port B
  }
  clock_t start = platform_clock();
  clock_t now   = start;
  clock_t last  = start;
  unsigned int count = 0;
  unsigned char prev = 0;
  uint8_t show_flag = 0;
  while (platform_elapsed_ticks(start, platform_clock()) < 600) {
    now = platform_clock();
    unsigned char trig = platform_gttrig(trigger_source);
    if (!prev && trig) count++;
    prev = trig;
    while(platform_clock() == now) { // wait 1/60 sec
    }
    if (platform_elapsed_ticks(last, now) >= 60) {
      show_flag = 1;
      last = now;
    }
    if (show_flag) {
      show_flag = 0;
      uint16_t ticks = platform_elapsed_ticks(start, now);
      int elapsed = (ticks) / CLOCKS_PER_SEC;
#if 1
      printf("\r%2d.0 sec %3d shots     ", elapsed, count);
      fflush(stdout);
#else
      print_status(elapsed, count);
#endif
    }
    
  }
  printf("\r10.0 sec %3d shots     \n", count); // Ensure the final "10.0 sec" status is displayed after measurement ends
  printf("Avg %d.%01d shots/sec\n", count / 10, count % 10); // Avoid float usage
  printf("Done.");
  return 0;
}
