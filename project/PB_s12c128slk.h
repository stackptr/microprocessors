/*
 * File:        s12slk.h
 * Purpose:     S12SLK definitions to APSLK I/O interfaces
 *
 * Notes:
 */

#ifndef PBS12SLK_H
#define PBS12SLK_H

/* External function prototypes */

/* On-board interfaced components */
extern void PBMCUSLK_PB_Init(void);
extern void PBMCUSLK_LED_Init(void);
extern void PBMCUSLK_POT_Init(void);
extern void PBMCUSLK_BUZZER_Init(void);
extern void PBMCUSLK_COM_Init(void);
extern void PBMCUSLK_LCD_Init(void);
extern void PBMCUSLK_KEYPAD_Init(void);
extern void PBMCUSLK_SW_Init(void);

/* External interfaced components */
extern void PBMCUSLK_STEPMOTOR_Init(void);

/************************************************************************************************/

/* Macros define the MCU Student Learning Kit Project Board (PBMCUSLK) user I/O interfaces
   to the appropriate registers / port pins on the pbs12c32slk Application Module.
   Macros simplify the implementation of I/O interfaces in user code.
   Macros labeled with "Direct Connect" label are conveniently connected
   to the specified port pin via the PBMCUSLK and do not require jumper wire
   from MCU PORT to USER I/O header. Refer to the PBMCUSLK User Guide for
   more on this "Direct Connect" feature along with how to activate and/or
   disactivate the feature allowing the user to connect these user I/O
   interfaces to other MCU registers / port pins. Some I/O interfaces are
   left for the user to define. */
   
/* PBMCUSLK PUSH BUTTONS */
#define SLK_PB1  PTP_PTP0            //Direct Connect
#define SLK_PB2  PTP_PTP1			      //Direct Connect
#define SLK_PB3  PORTA_BIT5		      //Direct Connect
#define SLK_PB4  PORTA_BIT4		      //Direct Connect
#define SLK_PB5  UNDEFINED
#define SLK_PB6  UNDEFINED
#define SLK_PB7  UNDEFINED
#define SLK_PB8  UNDEFINED
#define SLK_PBs  UNDEFINED           //Macro can be assigned to a single port for byte-wide decoding
 
/* PBMCUSLK LEDs */
#define SLK_LED1 PORTA_BIT3          //Direct Connect          
#define SLK_LED2 PORTA_BIT2			    //Direct Connect
#define SLK_LED3 PORTA_BIT1			    //Direct Connect
#define SLK_LED4 PORTA_BIT0			    //Direct Connect
#define SLK_LED5 PORTA_BIT4
#define SLK_LED6 PORTA_BIT6
#define SLK_LED7 UNDEFINED
#define SLK_LED8 UNDEFINED
#define SLK_LEDs UNDEFINED           //Macro can be assigned to a single port for byte-wide decoding

/* PBMCUSLK POT */
#define SLK_POT  ATDDR1H             //Direct Connect to PTAD_PTAD1 also ATD Ch.1
                                        
/* PBMCUSLK BUZZER */
#define SLK_BUZZER_TONE   UNDEFINED		//Direct Connect to PTT_PTT0 also PWM Ch.0
#define SLK_BUZZER_VOLUME TC0Lo    		//Direct Connect to PTT_PTT0 also PWM Ch.0
																				  //Frequencies below are calculated for Clock X period = 16us																			  //set by PWM Ch.0 example initialization code, equation:
																				  //Frequency = 1 / (Clock X period * PWMPERx) for left-aligned
#define A4_440Hz  142	                    //Frequency = 1 / (16us * 142) = 440.14Hz 
#define A5_880Hz   71 									  //Frequency = 1 / (16us * 71) = 880.28Hz

/* PBMCUSLK Timer */
#define TIMER_COUNT  PACNT             //Direct Connect to PTAD_PTAD1 also ATD Ch.1
 
/* PBMCUSLK SCI Registers for the S12C Module*/
#define PBMCUSLK_SCIDRL   SCIDRL          //PBMCUSLK Switches do NOT support "Direct Connect" feature 
#define PBMCUSLK_SCIBDH   SCIBDH					//User must use jumper wire to connect to I/O
#define PBMCUSLK_SCIBDL   SCIBDL					//PBMCUSLK_SWx_x macros are placeholders to define I/O pin
#define PBMCUSLK_SCICR1   SCICR1
#define PBMCUSLK_SCICR2   SCICR2
#define PBMCUSLK_SCITC    SCISR1_TC
#define PBMCUSLK_SCITDRE  SCISR1_TDRE
#define PBMCUSLK_SCIRDRF  SCISR1_RDRF
#define PBMCUSLK_SCISR1   SCISR1

/* PBMCUSLK LCD Registers for the S12C Module*/
#define PBMCUSLK_SPIDR    SPIDR          //PBMCUSLK Switches do NOT support "Direct Connect" feature 
#define PBMCUSLK_SPIBR    SPIBR					//User must use jumper wire to connect to I/O
#define PBMCUSLK_SPIBDL   SCIBDL					//PBMCUSLK_SWx_x macros are placeholders to define I/O pin
#define PBMCUSLK_SPICR1   SPICR1
#define PBMCUSLK_SPICR2   SPICR2
#define PBMCUSLK_SPISR    SPISR


/* PBMCUSLK SWITCHES */
#define SLK_SW1_1 UNDEFINED          //PBMCUSLK Switches do NOT support "Direct Connect" feature 
#define SLK_SW1_2 UNDEFINED					//User must use jumper wire to connect to I/O
#define SLK_SW1_3 UNDEFINED					//PBMCUSLK_SWx_x macros are placeholders to define I/O pin
#define SLK_SW1_4 UNDEFINED
#define SLK_SW2_1 UNDEFINED
#define SLK_SW2_2 UNDEFINED
#define SLK_SW2_3 UNDEFINED
#define SLK_SW2_4 UNDEFINED
#define SLK_SWs   UNDEFINED          //Macro can be assigned to a single I/O port for byte-wide decoding

/* PBMCUSLK KEYPAD */
#define SLK_KPWIRE1 UNDEFINED         //PBMCUSLK KEYPAD is NOT support by "Direct Connect" feature 
#define SLK_KPWIRE2 UNDEFINED 		    //User must use jumper wire to connect to I/O
#define SLK_KPWIRE3 UNDEFINED		    //PBMCUSLK_KP macros are placeholders to define I/O pin
#define SLK_KPWIRE4 UNDEFINED
#define SLK_KPWIRE5 UNDEFINED        //I/O pins jump wired to MCU and FPGA which outputs interrupt 
#define SLK_KPWIRE6 UNDEFINED        //when any of the keypads is pressed
#define SLK_KPWIRE7 UNDEFINED
#define SLK_KPWIRE8 UNDEFINED
#define SLK_KPWIRE9 UNDEFINED            //Macro can be assigned to a single port for byte-wide decoding             

/* PBMCUSLK STEPMOTOR */
//#define PBMCUSLK_MOTOR_SPEED PWMDTY1      //Wired Connect to PWM Ch. 1
//#define PBMCUSLK_MOTOR_A     PTAD_PTAD2       //Four outputs to the stepper motor
//#define PBMCUSLK_MOTOR_NOTA  PTAD_PTAD3
//#define PBMCUSLK_MOTOR_B     PTJ_PTJ7
//#define PBMCUSLK_MOTOR_NOTB  PTJ_PTJ6

//#define F_245Hz     255 									//Frequency = 1 / (16us * 255) = 245Hz

/* PBMCUSLK INITIALIZATION */
#define INIT_SLK_PBs       PBMCUSLK_PB_Init()         //Initializes pins as inputs to read PBx level
#define INIT_SLK_LEDs      PBMCUSLK_LED_Init()        //Initializes pins as outputs to write LEDx level
#define INIT_SLK_POT       PBMCUSLK_POT_Init()        //Initializes ATD to sample POT value
#define INIT_SLK_BUZZER    PBMCUSLK_BUZZER_Init()     //Initializes PWM to drive BUZZER freq and duty cycle
#define INIT_SLK_COM       PBMCUSLK_COM_Init()        //Initializes SCI to Rx/Tx data thru COM port to terminal
#define INIT_SLK_LCD       PBMCUSLK_LCD_Init()        //Initializes SPI to display characters to 2x8 LCD
#define INIT_SLK_SWs			 PBMCUSLK_SW_Init()         //Initializes pins as inputs to read SWx_x level
#define INIT_SLK_KEYPAD    PBMCUSLK_KEYPAD_Init()     //Initializes pins as inputs to read KEYPAD entry
//#define INIT_PBMCUSLK_MOTOR   PBMCUSLK_MOTOR_Init()      //Initializes PWM to drive Motor duty cycle
//#define INIT_SLK_STEPMOTOR PBMCUSLK_STEPMOTOR_Init()  //Initializes PWM to drive Motor duty cycle
#define INIT_SLK_FRTimer   PBMCUSLK_Timer_Init();      //Initalized the Free Running Timer

/* Miscellaneous Macros */
#define DDRE_DDRE0_MASK 1
#define ON  1
#define OFF 0

/* Example use of macro definitions in user application:
   
   //Code Example
   if (!PBMCUSLK_PB1)
   {
      PBMCUSLK_LED1 = ON;
      PBMCUSLK_BUZZER_VOLUME = PBMCUSLK_POT;
   }
   else
   {
      PBMCUSLK_LED2 = ON;
      PBMCUSLK_BUZZER_TONE = PBMCUSLK_POT;
   }
   
  **Explanation of Code Example**
  If PBMCUSLK_PB1 is equal to zero, turn on PBMCUSLK_LED1. Then read the current PBMCUSLK_POT
  value sampled by ATD Ch.1 and write it to the PBMCUSLK_BUZZER_VOLUME, which sets the PWM Ch.0
  duty cycle. If PBMCUSLK_PB1 is not equal to zero, turn on PBMCUSLK_LED2 and write PBMCUSLK_POT 
  value sampled by ATD Ch. 1 and write it to the PBMCUSLK_BUZZER_TONE, which sets the PWM Ch.0
  period. */
  
#endif /* pbs12dslk_H */