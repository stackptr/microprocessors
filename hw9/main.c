#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#define BYTES 6

// Function to compute powers of numbers: 10^4 = pow(10,4)
unsigned long int pow(unsigned int base, unsigned int power){
  unsigned long int r = base; // Result
  unsigned char j = 0;
  for (j = 0; j < power-1; j++)
    r *= base;
  return r;
}

// Given a pointer to a string, return the decimal value in an int
unsigned long int strToDec(char * str){
  unsigned long int r = 0;
  unsigned int length = sizeof(str) / sizeof(char *);
  unsigned int power = 
  int i = 0;
  for (int i = 1; i < length/2 - 2; i += 2)
    r += pow((*(str+i) - 0x30), power--);
  return r;
}

void main(void) {
  unsigned long int r = pow(6,4);
  
  unsigned long int p = strToDec("363030303300");
  
  
  
  EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
