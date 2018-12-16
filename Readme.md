# Stepper Motor Driven Air Valves Controller by MIDI Input
This project is part of an idea to create an organ with more dynamic abilities.
Therefore the air vales of each organ pipe must be position controlled to have a variable air volume blow through them.

## Concept
A microcontroller gets a MIDI signal as input and controls as much stepper motors as possible (stepper motor drivers will be used).
The volume (MIDI velocity) will be mapped linearly to the stepper motor position in this first attempt.
We will try to not use dedicated motion controllerst first.

This examples uses an TI Tiva C LauchPad Evaluation Board with a TI TM4C1294NCPDT processor.
It has very many GPIO pins, which are needed, because each stepper motor will require three pins (step, direction and an endstop switch).
The first attempt will use MIDI, because this is a quite simple serial protocol.
For later revisions it will be better to use Open Sound Control (OSC) via the ethernet port of the Tiva board.

The only reason, why TI is used, was that I already had the board at home.


## Other Dependencies
TivaWare from https://github.com/tinkerguy/tivaware


## Project Organization
The code should always be auto formatted with clang-format using the `.clang-format` file provided in this repository.
We use the `CppStyle` eclipse plugin inside TI Code Composer Studio.

