/*
 * Prototype to position control air valves with stepper motors for an experimental organ
 * The data input is MIDI (later it should be OSC) and as much motors as possible should be controlled
 * An octave of 12 (or 13) valves should be a good starting point, but more is better
 *
 * @author Jannik Beyerstedt <code@jannikbeyerstedt.de>
 * @license GNU GPL v3
 */

#define TARGET_IS_TM4C129_RA2

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#define TIMER_ONESEC_DIV 2  // timer value = 1 second / TIMER_ONESEC_DIV

// System clock rate in Hz.
uint32_t g_ui32SysClock;

// Flags that contain the current value of the interrupt indicator as displayed on the UART.
uint32_t g_ui32Flags;

// The error routine that is called if the driver library encounters an error.
#ifdef DEBUG
void __error__(char* pcFilename, uint32_t ui32Line) {}
#endif

// The interrupt handler for the first timer interrupt.
void Timer0IntHandler(void) {
  char cOne, cTwo;

  // Clear the timer interrupt.
  ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

  // Toggle the flag for the first timer.
  HWREGBITW(&g_ui32Flags, 0) ^= 1;

  // Use the flags to Toggle the LED for this timer
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, g_ui32Flags);

  // Update the interrupt status.
  ROM_IntMasterDisable();
  ROM_IntMasterEnable();
}

/**
 * main.c
 */
int main(void) {
  printf("Hello World!\n");

  // Set the clocking to run directly from the crystal at 120MHz.
  g_ui32SysClock = MAP_SysCtlClockFreqSet(
      (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

  // Enable the GPIO port that is used for the on-board LEDs.
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

  // Enable the GPIO pins for the LEDs (PN0 & PN1).
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  // Enable the peripherals used by this example.
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

  // Enable processor interrupts.
  ROM_IntMasterEnable();

  // Configure the two 32-bit periodic timers.
  ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
  ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, g_ui32SysClock / TIMER_ONESEC_DIV);

  // Setup the interrupts for the timer timeouts.
  ROM_IntEnable(INT_TIMER0A);
  ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

  // Enable the timers.
  ROM_TimerEnable(TIMER0_BASE, TIMER_A);

  // Loop forever while the timers run.
  while (1) {
    ;
  }

  return 0;
}
