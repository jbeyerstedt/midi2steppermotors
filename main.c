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
#include "driverlib/uart.h"

#include "inc/tm4c1294ncpdt.h"

/* GLOBAL COONFIGURATION */
#define TIMER_ONESEC_DIV 20  // timer value = 1 second / TIMER_ONESEC_DIV

/* GLOBAL VARIABLES */
uint32_t g_ui32SysClock;  // System clock rate in Hz.

/* VARIABLES AND DATA TYPES FOR VALVE POSITION CONTROL */

// mapping of stepOutput and dirOutput array's index to a speaking name
typedef enum StepPorts { StepPortE = 0, StepPortN } stepPort_t;
typedef enum DirPorts { DirPortL = 0, DirPortN } dirPort_t;
#define PORTS_CNT 2
#define VALVE_CNT 6

// mapping of the internal array indices to a GPIO port address
typedef struct StepPortMapping {
  stepPort_t portIndex;
  uint32_t   portBase;
} stepPortMap_t;
//stepPortMap_t stepPortMap[PORTS_CNT] = {{StepPortE, GPIO_PORTE_AHB_DATA_R}, {StepPortN, GPIO_PORTN_DATA_R}};

typedef struct DirPortMapping {
  dirPort_t portIndex;
  uint32_t  portBase;
} dirPortMap_t;
//dirPortMap_t dirPortMap[PORTS_CNT] = {{DirPortL, GPIO_PORTL_DATA_R}, {DirPortN, GPIO_PORTN_DATA_R}};

// valve information
typedef struct Valve {  // holds information about each valve
  stepPort_t stepPort;
  uint32_t   stepPin;
  dirPort_t  dirPort;
  uint32_t   dirPin;
  uint8_t    position;
  uint32_t   stepEndTime;
} valve_t;

// step output: PE0:5 - 1B3, 1B4, 1B5, 1B6, 1A2, 1A6
// dir output:  PL0:5 - 1C7, 1C8, 1C9, 1C10, 1C5, 1C6
valve_t valveLUT[VALVE_CNT] = {
    {StepPortE, GPIO_PIN_0, DirPortL, GPIO_PIN_0, 0, 0},  // note middle C
    {StepPortE, GPIO_PIN_1, DirPortL, GPIO_PIN_1, 0, 0},  // note TODO
    {StepPortE, GPIO_PIN_2, DirPortL, GPIO_PIN_2, 0, 0},  // note TODO
    {StepPortE, GPIO_PIN_3, DirPortL, GPIO_PIN_3, 0, 0},  // note TODO
    {StepPortE, GPIO_PIN_4, DirPortL, GPIO_PIN_4, 0, 0},  // note TODO
    {StepPortE, GPIO_PIN_5, DirPortL, GPIO_PIN_5, 0, 0}   // note TODO
};

uint8_t  stepOutput[PORTS_CNT] = {0};  // holds the output value
uint8_t  stepToggle[PORTS_CNT] = {0};  // holds bits for each output, that should take a step
uint8_t  dirOutput[PORTS_CNT]  = {0};
uint32_t currentStepTime       = 0;  // TODO: check if big enough!

/**
 * The error routine that is called if the driver library encounters an error.
 */
#ifdef DEBUG
void __error__(char* pcFilename, uint32_t ui32Line) {}
#endif

/**
 * The interrupt handler for the stepper motor step output.
 */
void Timer0IntHandler(void) {
  ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  // clear the interrupt

  // check, which motors should end
  for (int i = 0; i < VALVE_CNT; i++) {
    if (valveLUT[i].stepEndTime == currentStepTime) {
      stepToggle[valveLUT[i].stepPort] &= ~valveLUT[i].stepPin;
    }
  }

  // write stepper motor step output
  GPIO_PORTE_AHB_DATA_R = GPIO_PORTE_AHB_DATA_R ^ stepToggle[0];
  //GPIO_PORTN_DATA_R     = GPIO_PORTN_DATA_R ^ stepToggle[1];

  // debug output: blink LED same as stepper motor 1
  GPIO_PORTN_DATA_R = GPIO_PORTN_DATA_R ^ stepToggle[valveLUT[0].stepPort];

  // Update the interrupt status.
  ROM_IntMasterDisable();
  ROM_IntMasterEnable();

  // increase step time
  currentStepTime++;
}

/**
 * main.c
 */
int main(void) {
  uint8_t midiCmd  = 0;
  uint8_t midiNote = 0;
  uint8_t midiVel  = 0;

  printf("MIDI to Stepper Motor Positioning\n\n");

  /* SETUP THE CLOCK (run directly from the crystal at 120 MHz)*/
  g_ui32SysClock = MAP_SysCtlClockFreqSet(
      (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

  /* SETUP THE ON BOARD USER LED (D2) */
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

  /* SETUP STEP FREQUENCY INTERRUPT */
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  ROM_IntMasterEnable();

  ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
  ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, g_ui32SysClock / TIMER_ONESEC_DIV);

  ROM_IntEnable(INT_TIMER0A);                           // enable interrupt of timer0a
  ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  // get interrupt when timeout

  // Enable the timers.
  ROM_TimerEnable(TIMER0_BASE, TIMER_A);

  /* SETUP UART0 (connected to debug interface, 115200 baud) */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  UARTConfigSetExpClk(UART0_BASE, g_ui32SysClock, 115200,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

  /* SETUP GPIO OUTPUTS FOR STEPPER DRIVERS */
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, 0x3F);  // PE0:5
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, 0x3F);  // PL0:5

  /* RUN MIDI INTERPETAION */
  while (1) {
    // MIDI is 31250 baud, 8bit, 1 start, 1 stop, no parity
    // always 3 byte: command + 2 byte data (note, velocity)
    // command: 4 bit command + 4 bit channel (1 to 16)
    //    Note off:              0x8n
    //    Note on:               0x9n
    //    Polyphonic Aftertouch: 0xAn
    // note: 0 to 127 are C-1 to G9 (piano is A0 to C8, 0x3C is the middle C)
    // velocity: 0x00 silent, 0x45 middle, 0x7F high

    // TODO: use state machine, start if (incoming byte > 0x7F)

    /* get MIDI input */
    midiCmd  = UARTCharGet(UART0_BASE);
    midiNote = UARTCharGet(UART0_BASE);
    midiVel  = UARTCharGet(UART0_BASE);
    SysCtlDelay(g_ui32SysClock / (1 * 3));  // delay 1us to get newline char, if it was sent
    if (UARTCharsAvail(UART0_BASE)) {       // catch additional newline, when sent via bash with echo
      if (UARTCharGet(UART0_BASE) != 0x0A) {
        printf("error receiving\n");
      }
    }
    printf("MIDI: 0x%02X 0x%02X 0x%02X -> ", midiCmd, midiNote, midiVel);

    /* modify output value */
    if (0x90 == midiCmd) {  // if NoteOn
      switch (midiNote) {
      case 0x3C:
        printf("middle C!\n");
        uint8_t valveLUTidx = 0;

        // TODO: convert velocity to position value (linear mapping)

        // calculate position difference
        int16_t positionOffset = (int16_t)midiVel - (int16_t)valveLUT[valveLUTidx].position;

        // set stepping direction
        if (positionOffset < 0) {  // set dir to 0 (delete the bit)
          dirOutput[valveLUT[valveLUTidx].dirPort] =
              dirOutput[valveLUT[valveLUTidx].dirPort] & ~(valveLUT[valveLUTidx].dirPin);
          valveLUT[valveLUTidx].stepEndTime = (uint8_t)(-positionOffset);
        } else {  // set dir to 1
          dirOutput[valveLUT[valveLUTidx].dirPort] =
              dirOutput[valveLUT[valveLUTidx].dirPort] | valveLUT[valveLUTidx].dirPin;
          valveLUT[valveLUTidx].stepEndTime = (uint8_t)positionOffset;
        }

        // activate stepping output
        //ROM_IntMasterDisable();
        valveLUT[valveLUTidx].stepEndTime += currentStepTime;  // TODO: is this sufficient with fast notes?
        stepToggle[valveLUT[valveLUTidx].dirPort] =
            stepToggle[valveLUT[valveLUTidx].dirPort] | valveLUT[valveLUTidx].dirPin;

        switch (valveLUT[valveLUTidx].dirPort) {
        case DirPortL:  // only change bit 0:5 (protect others with 0xC0)
          GPIO_PORTL_DATA_R = (GPIO_PORTL_DATA_R & 0xC0) | dirOutput[DirPortL];
          break;
        }
        //ROM_IntMasterEnable();

        // update position
        valveLUT[0].position += positionOffset;

        break;
      default:
        printf("other note 0x%02x\n", midiNote);
      }
    }
  }

  return 0;
}

/*
 * coherent GPIO pins on the booster pack headers
 *
 * Pack 1:
 * PE0:5 - B3, B4, B5, B6, A2, A6
 * PL0:5 - C7, C8, C9, C10, C5, C6
 * PC4:7 - A3, A4, A5, A8
 *
 * Pack 2:
 * PK0:7 - B5, B6, B7, B8, C2, C3, C9, C10
 * PA0:5 - A5 (JP4 to CAN), A6 (JP4 to CAN), D7 (I2C), D6 (I2), B9, B10
 * PQ0:3 - A7, D9, D6 (SPI), D7 (SPI)
 *
 * Both Packs:
 * PM0:7 - 2C4, 2C5, 2C6, 1D2, 1B9, 1B10, 2D10, 2D2
 * PP0:5 - 2A3, 2A4, 1D10, 2D8, 2A8, 2D3
 * PD0:5 - 1D7, 1D6, 2A2, 1A7, 2A2 (JP4 UART), 2A3 (JP5 UART)
 * PH0:3 - 2C7, 2C8, 1D3, 1D4
 */
