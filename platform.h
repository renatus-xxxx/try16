#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>

#ifdef MSX
#define CLOCKS_PER_SEC 60
#define JIFFY          0xfc9e

struct regs {
  char     f;
  char     a;
  unsigned bc;
  unsigned de;
  unsigned hl;
};

void calbio(unsigned int address, struct regs *r) {

#asm

CALBIO_CALSLT	equ	001ch
CALBIO_EXPTBL	equ	0fcc1h		;slot address of BIOS ROM

    ld	hl,2
	add	hl,sp
	ld	e,(hl)	;second param to de
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	c,(hl)	;first param to hl
	inc	hl
	ld	b,(hl)
	ld  l,c
	ld	h,b

CALBIO01:
	push	hl
	pop	ix
	push	de
	ex	de,hl
	ld	iy,0
	add	iy,sp
	di
	ld	sp,hl
	pop	af
	pop	bc
	pop	de
	pop	hl
	ei
	ld	sp,iy
	push	iy
	ld	iy,(CALBIO_EXPTBL-1)
	call	CALBIO_CALSLT
	pop	iy
	exx
	pop	hl
	ld	bc,8
	add	hl,bc
	di
	ld	sp,hl
	exx
	push	hl
	push	de
	push	bc
	push	af
	ei
	ld	sp,iy
	pop	af
#endasm
}

clock_t platform_clock(void) {
  uint8_t* ptr = (uint8_t*)0xFC9E;
  return (clock_t)(ptr[0] | (ptr[1] << 8));
}

float platform_elapsed(clock_t start, clock_t end) {
  return ((end - start) & 0xFFFF) / (float)CLOCKS_PER_SEC;
}

unsigned char platform_gttrig(char no) {
  struct regs r;
  r.a = no;
  calbio(0x00d8, &r); // MSX BIOS call 0x00D8 = GTTRIG
  return (r.a != 0);
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

unsigned char platform_gttrig(int no) {
  return 0;
}
#endif

#endif // PLATFORM_H
