/**
 * @title    Logiciel pour carte gp2d120
 * @context  Coupe suisse de robotique 2007
 * @author   P. Gerber <patrick.gerber@heig-vd. *
 * @file     main.c
 * @language ASCII/C
 * @svn      $Id: main.c 198 2007-04-16 11:24:50Z pgerber@heig-vd.ch $
 *
 */
#define C8051F300
#include "silabs.h"
#include "types.h"
#include "i2c.h"
#include "schb_cmd_list.h"

#define I2C_ADDR 0x27
#define MOY_LENGTH 100

sbit LED1 = P0 ^ 6;
sbit LED2 = P0 ^ 5;
sbit LED3 = P0 ^ 4;

typedef struct
{
  unsigned char bcast;
  unsigned char distance;

} i2c_t;

i2c_t i2c;

void set_counter_clockwise_direction();
void set_clockwise_direction();
void dinit_defaults_values();
void set_power_off();
void pca_set_pwm(char speed);

/**
 * Hardware Initialisations
 * P0.0  -  SDA (SMBus), Open-Drain, Digital
 * P0.1  -  SCL (SMBus), Open-Drain, Digital
 * P0.2  -  GP2D120,  Push-Pull,  Analog
 * P0.3  -
 * P0.4  -  LED3
 * P0.5  -  LED2
 * P0.6  -  LED1
 * P0.7  -
 */
void port_io_init()
{
  // Enable CrossBar (All IO PushPull)
  P0MDIN = 0xF3;
  P0MDOUT = 0xF0;
  XBR1 = 0x04;
  XBR2 = 0x40;
}

void adc_init()
{
  AMX0SL = 0x82; // +P0.2 -GND
  ADC0CN = 0x81; // Enable on timer0 overflow

  EIE1 |= 0x04; // Eneble irq
}

void timer_init()
{
  TCON = 0x10; // Enable timer 0
  TMOD = 0x01; // 16 Bit Counter/Timer
  TL0 = 0xFF;
  TH0 = 0xFF;
  IE |= 0x02; // Enable Timer 0 Interrupt
}

void init_device()
{
  // MCU Init
  SILABS_INTERNAL_OSC(OSC_FREQ_MAX)
  SILABS_DISABLE_WATCHDOG

  port_io_init();
  timer_init();
  adc_init();
}

/**
 * Timer 0 Interrupt Service Routine.
 */
void timer0_isr() interrupt 1
{
  static int cpt = 0;

  // Blinky led1
  if (cpt++ >= 1000)
  {
    LED1 = !LED1;
    cpt = 0;
  }

  TL0 = 0x37; // 0.1ms reload
  TH0 = 0xFF;
  TF0 = 0;
}

/**
 * ADC Interrupt Service Routine.
 */
void adc_isr() interrupt 8
{
  static char cpt = 0;
  static int somme = 0;

  somme += ADC0;
  if (cpt++ >= MOY_LENGTH)
  {
    i2c.distance = (unsigned char)(somme / MOY_LENGTH);
    somme = 0;
    cpt = 0;
  }

  ADC0CN &= ~0x20; // Reset pending flag
}

void main()
{
  init_device();
  i2c_init(I2C_ADDR, &i2c, sizeof i2c);

  ENABLE_ALL_INTERRUPTS

  LED2 = 0;
  LED3 = 1;

  for (;;)
  {
    LED3 = !(i2c.distance > 39);
  }
}
