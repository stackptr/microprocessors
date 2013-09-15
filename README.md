# Microprocessor-Based System Design

Programming assignments written for EEL4746, taught by Reginald Perry in Fall 2012. The course has a class and lab component, but the lab component was fairly uninteresting C exercises.

Note that this code was written to run within the CodeWarrior IDE using the Student Learning Kit Project Board (SLK-PB). Various libraries have not been included since they would be shipped with the IDE itself.

The followign assignments are included in this repo (since not all assignments required written code):

- hw6: Determines a key code from a supplied String
- hw9: Demonstrates a few basic C functions without using std libraries
- hw10: Performs hw6 using three different styles of subroutines
- hw11: Utilized the 7-segment display of the SLK-PB
- hw12: Displays a count on the 7-segment display
- hw13a: Displays a count using the timer subsystem to delay
- hw13b: Displays a count using timer overflow interupt to delay

The class project is also included, which was created with a group of four others. A keypad and stepper motor are used to simulate a combinational lock. The keypad is scanned through `keypad_scan.asm`. LCD libraries are used to display messages to the user. `step_motor.asm` controls the motor, and `main.c` implements a finite state machine to control the lock while `main.asm` implements low-level timing functions. All necessary third-party libraries are included.