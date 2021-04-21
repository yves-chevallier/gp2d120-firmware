#ifdef C8051F300
#ifndef C8051F300_INCL
#include <c8051f300.h>
#define C8051F300_INCL
#endif

#define OSC_FREQ_30625 0x04
#define OSC_FREQ_61250 0x05
#define OSC_FREQ_122500 0x06
#define OSC_FREQ_245000 0x07
#define OSC_FREQ_MAX OSC_FREQ_245000

#define SILABS_INTERNAL_OSC(_x) \
  OSCICN = _x;

#define SILABS_DISABLE_WATCHDOG \
  PCA0MD &= ~0x40;              \
  PCA0MD = 0x00;

#define SILABS_ENABLE_WATCHDOG \
  PCA0MD &= ~0x40;             \
  PCA0MD = 0x00;               \
  PCA0CPL2 = 0x00;             \
  PCA0MD |= 0x20;

#define SILABS_INIT_ADC \
  XBR0 |= 0x04;         \
  \
    P0MDIN &= ~0x04;    \
  ADC0CN = 0x80;        \
  ADC0CF |= 0x01;       \
  AMX0SL = 0x89;        \
  EIE1 |= 0x04;

#define ADC_IRQ 8
#define ADC_START_CONV \
  AD0BUSY = 1;
#define ENABLE_ALL_INTERRUPTS \
  EA = 1;

#define ENABLE_TEMPERATURE_SENSOR \
  REF0CN = 0x0C;

#define TO_SLEEP \
  PCON |= 0x01;  \
  PCON = PCON;   \
  for (;;)       \
    ;

#define SILABS_TIMER0_INIT \
  TCON = 0x10;             \
  TMOD = 0x01;             \
  CKCON = 0x01;            \
  TL0 = 0x11;              \
  TH0 = 0xE8;              \
  IP |= 0x02;              \
  IE |= 0x02;

#define TIMER0_RELOAD \
  TL0 = 0x11;         \
  TH0 = 0xE8;

#define TIMER0_IRQ 1
#endif
#ifdef C8051F330
#include <c8051f330.h>
#endif
#ifdef C8051F120
#include <c8051f120.h>
#endif
