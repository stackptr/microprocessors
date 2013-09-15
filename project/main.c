/*************************************************************
 * main.c
 * Provides main C interface for the project.
 *
 * Version history:
 *   Nov 18: Implemented unlock, lock, and programming states - Corey Johns, Qin Zhu, Paul Kafka
 *   Nov 17: Minor changes to main.c to conform to requirements, created debug.asm for debug mode - Qin Zhu
 *   Nov 17: Hardware testing, input logic - Corey Johns, Qin Zhu, Paul Kafka
 *   Nov 16: State definitions, logic - Corey Johns
 *   Nov 16: Added input handling pseudocode - Corey Johns
 *   Nov 12: File creation, adding pseudocode - Corey Johns
 *************************************************************/

// For reference:
// Group: 109
// X = 80
// PAT = 1
// Range 1: 0 to 60
// Range 2: 61 to 110
// Range 3: 111 to 255


#include <hidef.h>          // common defines and macros
#include <stdio.h>          // provides sprintf()
#include "derivative.h"     // derivative-specific definitions
#include "main_asm.h"       // interface to the assembly module
#include "keypad_scan.h"    // interface to the keypad module
#include "step_motor.h"
#include "LCD.h"            // LCD functions
#include "MCUinit.h"        // For MCU_init(), device initialization function.

void MCU_init(void); /* Device initialization function declaration */

// General definitions
#define NULL_INPUT  0       // Null character
#define BUF_SIZE    5       // Buffer size fo rinput

// State definitions
#define POWER_ON_RESET  0
#define LOCK            1
#define DISPLAY_CODES   2
#define PROG_MODE       3
#define PROG_1          30
#define PROG_CHECK      31
#define PROG_ERROR      39
#define UNLOCK          5
#define UNLOCK_1        50
#define UNLOCK_2        51
#define UNLOCK_3        52
#define ERASE_CODES     6
#define INTERMEDIATE_1  60
#define INTERMEDIATE_2  61
#define INTERMEDIATE_3  62
#define ERROR_STATE     9


unsigned char state;
unsigned char debug;          // Set to 1 for debugging mode
static char topStr[20];       // String buffer for top string of LCD
static char bottomStr[20];    // String buffer for bottom string of LCD
static char cur_input;        // Stores current input location from keypad

static char input_buffer[4];  // Used to store past input

static char unlock1[4];
static char unlock2[4];
static char unlock3[4];
unsigned char unlock_ptr;

unsigned char unlock_check;   // Signals which unlock code has matched
unsigned char unlock_failure; // Signals failure

// Stepper motor variables
unsigned char start;   // Flag to indicate if routine is in start condition
unsigned char direction; // 0 = clockwise, 1 = counterclockwise
unsigned char stopped; // Flag to indicate stopped state
unsigned int motor_count;
unsigned char sec_counter;

/* init_unlock_codes
 * Author: Corey Johns
 *
 * Initializes unlock codes to default values on board startup.
 */
void init_unlock_codes(){
  sprintf(unlock1, "000");
  sprintf(unlock2, "111");
  sprintf(unlock3, "222");
}

/* keypad_debounce
 * Example from textbook
 * 
 * Allows for a variable delay for keypad debounce.
 */
unsigned char keypad_debounce(unsigned int bounce_length){
  int count;
  unsigned char get_key,new_key;
  count = bounce_length/2; // Initialize count
  while ( (count > 0) && (count < bounce_length) ){
    delay_1ms();
    new_key = key_scan();
    if(new_key == get_key) ++count;
    else --count;
  }
  if (count == bounce_length) return (get_key);
  else return(new_key);
}

/* delay
 * Author: Corey Johns
 *
 * Delays for x amount of seconds using the delay_1ms routine.
 */
void delay(unsigned int x){
  for (x = x*1000; x > 0; x--){
    delay_1ms();
  }
}


/* display_codes
 * Author: Corey Johns
 *
 * Flashes through the current unlock codes when in display mode.
 */
void display_codes(void){
  LCDPutString("T109 MU\n");
  LCDPutString("Codes:\n");
  delay(2);
  LCDPutString("T109 MU\n");
  sprintf(bottomStr, "%s\n", unlock1);
  LCDPutString(bottomStr);
  delay(2);
  LCDPutString("T109 MU\n");
  sprintf(bottomStr, "%s\n", unlock2);
  LCDPutString(bottomStr);
  delay(2);
  LCDPutString("T109 MU\n");
  sprintf(bottomStr, "%s\n", unlock3);
  LCDPutString(bottomStr);
  delay(2);
  state = POWER_ON_RESET;
}

/* add_code
 * Author: Corey Johns
 *
 * Checks if there is room available for unlock codes. if there is room,
 * adds the unlock code and increments current unlock code counter.
 */

void add_code(void){
  if (unlock_ptr > 3){
     // Unlock codes full
     LCDPutString("T109 MP\n");
     LCDPutString("FULL\n");
  } else {
     unlock_ptr++;
     LCDPutString("T109 MP\n");
     LCDPutString("OK\n");
  }
  delay(2);
  state = POWER_ON_RESET;
}

void prog_full(void){
  LCDPutString("T109 MP\n");
  LCDPutString("FULL\n");
  delay(2);
  state = POWER_ON_RESET;
}

void clear_unlock_codes(void){
  sprintf(unlock1, "");
  sprintf(unlock2, "");
  sprintf(unlock3, "");

}

/* erase_codes
 * Author: Corey Johns
 *
 * Initializes unlock codes to default, effectively erasing them.
 */
void erase_codes(void){
  clear_unlock_codes();
  LCDPutString("T109 ME\n");
  LCDPutString("ERASED\n");
  delay(2);
  state = POWER_ON_RESET;
  unlock_ptr = 1; // Reset pointer
}

/* unlock_invalid
 * Author: Corey Johns
 *
 * Flashes when invalid unlock code is tried.
 */
void unlock_invalid(void){
  LCDPutString("T109 MU\n");
  LCDPutString("NOTVALID\n");
  delay(2);
  state = POWER_ON_RESET;
}

/* unlock_mode
 * Author: Corey Johns
 *
 * Spins motor for the unlock state.
 */
void unlock_mode(void){
  // Initialize motor count
  motor_count = 640*80;
  sec_counter = 80;
  direction = 1; // spin counter clockwise
  
  // Turn the motor on
  stopped = 0;
  
  while(motor_count != 0){
    if(SLK_PB4 == 0){
      SLK_LED5 = 1;
      LCDPutString("T109 MZ\n");
      LCDPutString("DEBUG\n");
      debug = 1;
    }
  }
  
  // After motor finishes
  stopped = 1;
  state = POWER_ON_RESET;
}

/* lock_mode
 * Author: Corey Johns
 *
 * Spins motor for the lock state.
 */
void lock_mode(void){
  // Change display
  LCDPutString("T109 ML\n");
  LCDPutString("LOCK\n");
  
  // Initialize motor count
  motor_count = 640*80;
  sec_counter = 80;
  direction = 0; // spin clockwise
  
  // Turn the motor on
  stopped = 0;
  
  while(motor_count != 0){
    if(SLK_PB4 == 0){
      SLK_LED5 = 1;
      LCDPutString("T109 MZ\n");
      LCDPutString("DEBUG\n");
      debug = 1;
    }
  }
  
  // After motor finishes
  stopped = 1;
  state = POWER_ON_RESET;

}

/* invalid_input
 * Author: Corey Johns
 *
 * Provides error state for unknown input.
 */
void invalid_input(void){
  
  LCDPutString("T109\n");
  LCDPutString("USERERR\n");
  state = POWER_ON_RESET;
}


/* main
 * Author: Corey Johns, Qin Zhu, Paul Kafka
 *
 * Provides main functionality for input logic and states.
 */
void main(void) {

  // Set default unlock codes
  clear_unlock_codes();
  
  // Set unlock pointer to first unlock code
  unlock_ptr = 1;
  
  // Initialize microcontroller
  MCU_init();
  DisableInterrupts;    // Disable interrupts while initializing
  INIT_SLK_LEDs;        // Initialize LEDs
  INIT_SLK_PBs;         // Initialize buttons
  INIT_SLK_POT;
  LCDInit();            // Initialize LCD
  PAFLG_PAOVF = 1; // Clear pulse accumulator overflow flag by setting it to one
  start = 1; // Set start flag
  debug = 0;
  EnableInterrupts;     // Enable interrupts when finished
  
  direction = 0;
  stopped = 1; // Stop the motor when program starts
  
  // Initialize state to power on reset
  state = POWER_ON_RESET;
  // Set power on reset LCD state
  LCDPutString("T109 MS\n");
  LCDPutString("POR\n");
  
  // Enter main loop
  for(;;){
    // Get input
    cur_input = keypad_debounce(500); // Allow 500ms debounce
    
    // Input logic
    if (SLK_PB4 == 0){
      LCDPutString("T109 MZ\n");
      LCDPutString("DEBUG\n");
      debug = 1;
    } else if (cur_input != 0){  // Check if input is not null\
      debug = 0;
      //*************************
      // Power on reset state
      //*************************
      if (state == POWER_ON_RESET){ // Check the current state
        // Update the LCD
        LCDPutString("T109 MS\n");
        LCDPutString("POR\n");
        
        if (cur_input == '*'){       // Check the current input
          lock_mode();
        }
        else if (cur_input == '#'){
          LCDPutString("T109 MS\n");
          LCDPutString("Int1\n");
          state = INTERMEDIATE_1;
        }
        else if (cur_input >= '0' && cur_input <= '9') {
          //*************************
          // Enter in unlock mode
          //*************************
          // initialize unlock check
          unlock_check = 0b00000000;
          
          // Set unlock check bit
          if (unlock1[0] == cur_input)
            unlock_check |= 0b00000001;
          if (unlock2[0] == cur_input)
            unlock_check |= 0b00000010;
          if (unlock3[0] == cur_input)
            unlock_check |= 0b00000100;
          if ((unlock_check & 0b00000111) > 0){ // If any bits are set
            state = UNLOCK_1;                   // Proceed to next unlock step
            
            LCDPutString("T109 MU\n");
            sprintf(bottomStr, "N-%c\n", cur_input);
            LCDPutString(bottomStr);
            input_buffer[0] = cur_input;
          }
          else
            unlock_invalid();
        }
      }
      //*************************
      // Unlock 1 state
      //*************************
      else if (state == UNLOCK_1){
        
        if (cur_input >= '0' && cur_input <= '9'){
          unlock_failure = 1;
          if ((unlock_check & 0b00000001) > 0 ){  // Check if first bit is set
            if (unlock1[1] = cur_input){
              state = UNLOCK_2;
              unlock_failure = 0;  // Clear flag
            }
            else  // If input does not match code
              unlock_check = ~(~unlock_check & 0b00000001);  // Unset bit
          }
          if ((unlock_check & 0b00000010) > 0 ){
            if (unlock2[1] = cur_input){
              state = UNLOCK_2;
              unlock_failure = 0;
            }
            else
              unlock_check = ~(~unlock_check & 0b00000010);
          }
          if ((unlock_check & 0b00000100) > 0 ){
            if (unlock3[1] = cur_input){
              state = UNLOCK_2;
              unlock_failure = 0;
            }
            else
              unlock_check = ~(~unlock_check & 0b00000100);
          }
          if (unlock_failure == 1)
            unlock_invalid();
          else {
            LCDPutString("T109 MU\n");
            sprintf(bottomStr, "N-%c%c\n", input_buffer[0], cur_input);
            LCDPutString(bottomStr);
            input_buffer[1] = cur_input;
          }
       } else if (cur_input = '#'){
          unlock_mode();
       }
        else
          invalid_input();
      }
      //*************************
      // Unlock 2 state
      //*************************
      else if (state == UNLOCK_2){
        if (cur_input >= '0' && cur_input <= '9'){
          unlock_failure = 1;
          if ((unlock_check & 0b00000001) > 0 ){
            if (unlock1[2] = cur_input){
              state = UNLOCK_3;
              unlock_failure = 0;
            }
            else
              unlock_check = ~(~unlock_check & 0b00000001);  // Unset bit
          }
          if ((unlock_check & 0b00000010) > 0 ){
            if (unlock2[2] = cur_input){
              state = UNLOCK_3;
              unlock_failure = 0;
            }
            else
              unlock_check = ~(~unlock_check & 0b00000010);
          }
          if ((unlock_check & 0b00000100) > 0 ){
            if (unlock3[2] = cur_input){
              state = UNLOCK_3;
              unlock_failure = 0;
            }
            else
              unlock_check = ~(~unlock_check & 0b00000100);
          }
          if (unlock_failure == 1)
            unlock_invalid();
          else {
            LCDPutString("T109 MU\n");
            sprintf(bottomStr, "N-%c%c%c\n", input_buffer[0], input_buffer[1], cur_input);
            LCDPutString(bottomStr);
          }
        } else if (cur_input = '#'){
          unlock_mode();
       } else
          invalid_input();
      }
      //*************************
      // Unlock 3 state
      //*************************
      else if (state == UNLOCK_3){
        if (cur_input == '#'){
          LCDPutString("T109 MU\n");
          LCDPutString("Unlock\n");
          unlock_mode();  // Enter unlock mode
        }
        else
          unlock_invalid();
      }
      //*************************
      // Intermediate state 1
      //*************************
      else if (state == INTERMEDIATE_1){
        if (cur_input == '#'){
          LCDPutString("T109 MS\n");
          LCDPutString("Int2\n");
          state = INTERMEDIATE_2;
        }
        else if (cur_input == '*')
          state = INTERMEDIATE_3;
        else
          invalid_input();
      }
      //*************************
      // Intermediate state 2
      //*************************
      else if (state == INTERMEDIATE_2){
        if (cur_input == '#')
          display_codes();
        else if (cur_input >= '0' && cur_input <= '9'){
          if (unlock_ptr < 4){
            // If there is still room for unlock codes
            LCDPutString("T109 MP\n");
            sprintf(bottomStr, "N-%c\n", cur_input);
            LCDPutString(bottomStr);
            input_buffer[0] = cur_input;
            
            state = PROG_MODE;
            if (unlock_ptr == 1)
              unlock1[0] = cur_input;
            else if (unlock_ptr == 2)
              unlock2[0] = cur_input;
            else if (unlock_ptr == 3)
              unlock3[0] = cur_input;
            
          
          
          } else
            prog_full();
        }
        else
          invalid_input();
      }
      //*************************
      // Intermediate state 3
      //*************************
      else if (state == INTERMEDIATE_3){
        if (cur_input == '#')
          erase_codes();
        else
          invalid_input();
      }
      //*************************
      // Programming mode states
      //*************************
      else if (state == PROG_MODE){
        if(cur_input >= '0' && cur_input <= '9'){
          state = PROG_1;
          
          LCDPutString("T109 MP\n");
          sprintf(bottomStr, "N-%c%c\n", input_buffer[0], cur_input);
          LCDPutString(bottomStr);
          
          input_buffer[1] = cur_input;
          
          // Store the second number of the unlock code
          if (unlock_ptr == 1)
            unlock1[1] = cur_input;
          else if (unlock_ptr == 2)
            unlock2[1] = cur_input;
          else if (unlock_ptr == 3)
            unlock3[1] = cur_input;
          else {
            LCDPutString("Unlock Ptr\n");
            LCDPutString("Exception\n");
            delay(5);
            state = POWER_ON_RESET;
          }
        } else if (cur_input == '#') {
          add_code();
        }
        else
          state = PROG_ERROR;
      } else if (state == PROG_1){
        if(cur_input >= '0' && cur_input <= '9'){
          state = PROG_CHECK;
          
          LCDPutString("T109 MP\n");
          sprintf(bottomStr, "N-%c%c%c\n", input_buffer[0], input_buffer[1], cur_input);
          LCDPutString(bottomStr);
          
          input_buffer[2] = cur_input;
          
          // Store the third number of the unlock code
          if (unlock_ptr == 1)
            unlock1[2] = cur_input;
          else if (unlock_ptr == 2)
            unlock2[2] = cur_input;
          else if (unlock_ptr == 3)
            unlock3[2] = cur_input;
          else {
            LCDPutString("Unlock Ptr\n");
            LCDPutString("Exception\n");
            delay(5);
            state = POWER_ON_RESET;
          }
          
          add_code();
        } else if (cur_input == '#') {
          add_code();
        }
        else
          state = PROG_ERROR;
      } else if (state == PROG_ERROR){
        LCDPutString("T109 MP\n");
        LCDPutString("ERR\n");
        if (cur_input >= '0' && cur_input <= '9'){
          // Change state
          state = PROG_MODE;
          
          // Store the first number of the unlock code
          if (unlock_ptr == 1)
            unlock1[0] = cur_input;
          else if (unlock_ptr == 2)
            unlock2[0] = cur_input;
          else if (unlock_ptr == 3)
            unlock3[0] = cur_input;
          else {
            LCDPutString("Unlock Ptr\n");
            LCDPutString("Exception\n");
            delay(5);
            state = POWER_ON_RESET;
          }
        }
        
      } else if (state == PROG_CHECK){
        if (unlock_ptr > 3)
          prog_full();
        else {
          if (cur_input == '#')
            add_code();
          else if (cur_input >= '0' && cur_input <= '9'){
            if (unlock_ptr == 1)
              unlock1[0] = cur_input;
            else if (unlock_ptr == 2)
              unlock2[0] = cur_input;
            else if (unlock_ptr == 3)
              unlock3[0] = cur_input;
          state = PROG_MODE;
          }
        }
      }
      //*************************
      // Display code state
      //*************************
      //else if (state == DISPLAY_CODES){
      //  display_codes();
      //}
    }
    if (debug == 0 && state == POWER_ON_RESET){
      LCDPutString("T109 MS\n");
      LCDPutString("POR\n");
    }
  } // Never leave main()
}
