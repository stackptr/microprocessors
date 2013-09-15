/*
 * File:        s12slk.c
 * Purpose:     S12SLK initialization routines to configure APSLK I/O interfaces
 *
 * Notes:
 */

#include "PB_s12c128slk.h"
#include "device_headers.h"

/* Local function prototypes */

/* On-board interfaced components */
void PBMCUSLK_PB_Init(void);
void PBMCUSLK_LED_Init(void);
void PBMCUSLK_POT_Init(void);
void PBMCUSLK_BUZZER_Init(void);
void PBMCUSLK_COM_Init(void);
void PBMCUSLK_LCD_Init(void);
void PBMCUSLK_Timer_Init(void);
void PBMCUSLK_SW_Init(void);

/* External interfaced components */
//void PBMCUSLK_MOTOR_Init(void);
void PBMCUSLK_KEYPAD_Init(void);
//void PBMCUSLK_STEPMOTOR_Init(void);

/************************************************************************************************/

/* Initialization routine to configure pins as inputs to read PBx level. */
void PBMCUSLK_PB_Init(void)
{
  /* Enables "Direct Connect" feature for PBMCUSLK Push Buttons by making Port P Bit[5]
     an output pin and driving it low. This turns on a FET switch on PBMCUSLK
     that connects PBMCUSLK_PBx to a defined MCU port pin. */
  DDRP_DDRP5 = 1; //Enable as Output          
  PTP_PTP5 = 0;   //Drive LOW to enable FET                   
  
  
  /* Makes PTP[0:1] an input pin to allow the user to read PBMCUSLK_PB1-2 in application. */
  DDRP_DDRP0 = 0;  //Enable as Input (PB1)
  DDRP_DDRP1 = 0;  //Enable as Input (PB2)
  //PERP_PERP0 = 1;  //Enable Internal Pull-ups
 // PERP_PERP1 = 1;  //Enable Internal Pull-ups
         
  /* Makes PORTA[4:5] an input pin to allow the user to read PBMCUSLK_PB3-4 in application. */
  DDRA_BIT4 = 0;   //Enable as Input (PB3)
  DDRA_BIT5 = 0;   //Enable as Input (PB4)      
  //PUCR_PUPAE = 1;   //Enable Internal Pull-ups on Port A
  
  /* Insert user code here to initialize UNDEFINED PBMCUSLK Push Buttons */ 
}

/************************************************************************************************/

/* Initialization routine to configure pins as outputs to write LEDx level. */
void PBMCUSLK_LED_Init(void)
{
  /* Enables "Direct Connect" feature for PBMCUSLK LEDs by making Port P Bit[4]
     an output pin and driving it low. This turns on a FET switch on PBMCUSLK
     that connects PBMCUSLK_LEDx to a defined MCU port pin. */  
  DDRP_DDRP4 = 1;  //Enable as Output          
  PTP_PTP4 = 0;    //Drive LOW to enable FET          
  
  /* Makes PTA[0:3] output pins to allow the user to turn on/off PBMCUSLK_LED[1-4]
     in application. */
     
  DDRA_BIT0 = 1;  //Enable as Output (LED4)
  DDRA_BIT1 = 1;  //Enable as Output (LED3)
  DDRA_BIT2 = 1;  //Enable as Output (LED2)
  DDRA_BIT3 = 1;  //Enable as Output (LED1)
  DDRA_BIT4 = 1;
  /* Insert user code here to initialize UNDEFINED PBMCUSLK LEDs */
}

/************************************************************************************************/

/* Initialization routine to setup ATD to sample POT value. */
void PBMCUSLK_POT_Init(void)
{
  /* Enables "Direct Connect" feature for PBMCUSLK POT by making PTT[3]
     an output pin and driving it high. This turns on a FET switch on PBMCUSLK
     that connects PBMCUSLK_POT to a defined MCU port pin. */  
  DDRP_DDRP3 = 1;  //Enable as Output (POT_EN)          
  PTP_PTP3 = 1;    //Drive High to Enable FET
  
  /* Initializes ATD Channel 1 to continously sample PBMCUSLK Potentiometer to allow the 
     user to read PBMCUSLK_POT, which holds the instantaneous ATD Ch.1 data result. */                
  ATDCTL2 |= (ATDCTL2_ADPU_MASK |       //ATD power up enabled
              ATDCTL2_AFFC_MASK);				//ATD fast flag clear enabled
                                        //ATD interrupts disabled
  ATDCTL3 &= ~ATDCTL3_S4C_MASK;					//ATD conversions per sequence = 8
                                        //ATD in non-fifo mode, freeze disabled
  ATDCTL4 |= ATDCTL4_SRES8_MASK;        //ATD set to 8-bit resolution
                                        //ATD sample time =  2 ATD conv.clock periods
  ATDCTL4 &= ~(ATDCTL4_PRS2_MASK);      //ATD Clock = Bus Clock / [2 * (prescaler + 1)]
                                        //ATD Clock = 8MHz / [2 * (1+1)] = 2MHz
  ATDCTL5 |= (ATDCTL5_SCAN_MASK |				//ATD scan mode enabled
              ATDCTL5_CA_MASK);					//ATD single channel scan of ATD Ch.1
                                        //ATD result is left-justified and unsigned  
}

/************************************************************************************************/

/* Initialization routine to setup PWM to drive BUZZER freq and duty cycle. */
void PBMCUSLK_BUZZER_Init(void)
{
TIOS_IOS0 = 1;    //Enable Output Compare for IOS0
TTOV_TOV0 = 1;    //Enable Pin Toggle on Overflow
TCTL2_OM0 = 0;    //Set output Mode (See table in User Guide)
TCTL2_OL0 = 0;    //Set output Level
TFLG1_C0F = 1;    //Clear IOS0 Flag
TSCR2 = 0x00;     //Divide Bus Clock (2MHz) by 0
TC0 = 0x0000;     //After x pulses toggle the output pin
TSCR1_TEN = 1;    //Start the Timer 

//The time is counting a pulse every .032ms.  or 31.250kHz
//to get it to a lower hertz we count 40 times to get 800Hz. 
}

/************************************************************************************************/

/* Initialization routine to setup IOC1 as a free running Timer*/
void PBMCUSLK_Timer_Init(void)
{
PACTL_PAMOD = 1;  //Enable the PA Clock
PACTL_PEDGE = 1;
PAFLG_PAOVF = 1;  //Clear the Overflow flag
PACTL_PAEN = 1;   //Enable the Pulse Accumulator
PACNT = 0x7FFF; //Overflows at 0XFFFF.  Change this number to vary flash rate.

}

/************************************************************************************************/

/************************************************************************************************/

/* Initialization routine to setup SCI to Rx/Tx data via COM port to terminal */
void PBMCUSLK_COM_Init(void)
{
}

/************************************************************************************************/

/* Initialization routine to setup SPI to drive 2 line by 8 character LCD display. */
void PBMCUSLK_LCD_Init(void)
{
}

/************************************************************************************************/

/* Initialization routine to configure pins as inputs to read KEYPAD entry. */
void PBMCUSLK_KEYPAD_Init(void)
{
  /* Insert user code here to initialize UNDEFINED PBMCUSLK KBWIREs */
}

/************************************************************************************************/

/* Initialization routine to configure pins as inputs to read SWx_x level. */
void PBMCUSLK_SW_Init(void)
{
  /* Insert user code here to initialize UNDEFINED PBMCUSLK Switches */
}

/************************************************************************************************/

/* Initialization routine to configure PLL to generate a 24MHz BUSCLK */
//void pbs12c32slk_24MHz_BUSCLK_Init(void)
//{
  /* Equation 1) BUSCLK = PLLCLK / 2 when PLLSEL = 1 (PLL Engaged)
     Equation 2) PLLCLK = 2 x OSCCLK x [(SYNR + 1)/(REFDV + 1)]
     Therefore, combining Equation 1 & 2 gives us 
     Equation 3) BUSCLK = OSCCLK x [(SYNR + 1)/(REFDV + 1)]
     Variables: desired BUSCLK = 24MHz, OSCCLK = 16MHz, SYNR = 2, REFDV = 1
     Computation: BUSCLK = 16MHz x [(2+1)/(1+1)] = 24MHz  */
      
//  while (PLLSEL);;
//  SYNR = 2;															
//  REFDV = 1;
/************************************************************************************************/

/* Initialization routine to setup GPIO to drive a bipolar Stepper Motor*/

//void PBMCUSLK_STEPMOTOR_Init(void)
//{
   
  
  /* Initializes GPIO Ports to drive the stepper motor.
     
     Simplified Truth Table for a stepper motor
      
           Line 1 | Line 2 | Line 3 | Line 4
          ----------------------------------
   Step 1 |  0    | !Line1 |   1    | !Line3
   Step 2 |  0    | !Line1 |   0    | !Line3
   Step 3 |  1    | !Line1 |   0    | !Line3
   Step 4 |  1    | !Line1 |   1    | !Line3
   
   To implement in MCU we will need 4 GPIO pins.  Two of which always do the opposite of there partner.
   For low voltage motors you can drive directly from MCU.  For high voltages you should use operation amplifiers.
   The speed of the motor is determined by how quickly you can cycle through the steps. Direction is determined 
   by either going 1,2,3,4 or 4,3,2,1 through the steps.

    PB4 = Red
    PA0 = Grey
    PM1 = Yellow
    PM0 = Black
 */	

//DDRAD_DDRAD2 = 1;  //Configure PB4 as output
//PTAD_PTAD2 = 0; //Default to logic low
//DDRAD_DDRAD3 = 1;  //Configure PA0 as output
//PTAD_PTAD3 = 0; //Default to logic low
//DDRJ_DDRJ7 = 1; //Configure PM1 as output
//PTJ_PTJ7 = 0 ;  //Default to logic low
//DDRJ_DDRJ6 = 1; //Configure PM0 as output
//PTJ_PTJ6 = 0;   //Default to logic low

//}


/************************************************************************************************/

/* Initialization routine to setup PWM to drive Motor duty cycle. */

//void PBMCUSLK_MOTOR_Init(void)
//{
  /* To use PWM feature on the MC9S12C32 you need to re-route the PWM Ch.1 functionality over the Timer Ch.1
     on PTT[1] pin with the following instruction. */
     
//  MODRR |= MODRR_MODRR1_MASK;
  
  /* Initializes PWM Channel 1 to drive the Motor with a fixed
     frequency/period and user defined duty cycle (speed) PBMCUSLK_MOTOR_SPEED value
     set by the user in their application.  The initialization routine assumes 
     several factors and can be modified to accomodate a different PWM polarity,
     clock source, clock prescaler, alignment,resolution, scalar values, 
     default period and duty cycle.
     
     C source lines like, PWMCLK |= (0); are place holders that do not perform any action.
     They are there for the user to modify this PWM initialization routine. To modify, simply
     replace the "0" with the appropriate value or mask setting. */
 /*	
 	PWMPOL |= PWMPOL_PPOL1_MASK;          //PWM[1] polarity begins HIGH
 	PWMCLK |= (0);                        //PWM[x] clock source is set to Clock A
 	PWMPRCLK |= (PWMPRCLK_PCKA0_MASK |		//PWM[x] Clock (A or B) = Bus Clock / prescaler
 	             PWMPRCLK_PCKA1_MASK | 		//PWM[x] Clock A = 8MHz / 128 = 62.5KHz 
 	             PWMPRCLK_PCKA2_MASK);    //PWM[x] Clock B =  8MHz / 1 = 8MHz
 	PWMCAE |= (0);                        //PWM[x] set to be left-aligned
 	PWMCTL |= (0);                        //PWM[x] set to 8-bit resolution
 	PWMSCLA = 1;                          //PWM[x] Clock SA = Clock A / (2 * PWMSCLA)
 	                                      //PWM[x] Clock SA = 62.5KHz / (2*1) = 31.25KHz
 	PWMSCLB = 1;                          //PWM[x] Clock SB = Clock B / (2 * PWMSCLB)
 	                                      //PWM[x] Clock SB = 8MHz / (2*1) = 4MHz 
 	PWMPER1 = F_245Hz;                    //PWM[1] Period = Clock X period * PWMPERx for left-aligned
 	                                      //PWM[1] Period = Clock X period * (2 * PWMPERx) for center-aligned
 	                                      //PWM[1] Period set to 0xFF
 	PWMDTY1 = 0x3F;                       //PWM[1] Duty Cycle = [PWMDTYx / PWMPERx] * 100% for a polarity that begins high
 	                                      //PWM[1] Duty Cycle = [(PWMPERx - PWMDTYx)/PWMPERx] * 100% for a polarity that begins low
 	                                      //PWM[1] Duty Cycle set to 0 allowing user to define in application
 	PWME |= PWME_PWME1_MASK;              //PWM[1] enabled
}
*/
   
