/*****************************************************************************/
/*                                             COPYRIGHT (c) MOTOROLA 2003   */
/* FILE NAME:  SCI.h	                                        	  	 	       */
/*                                                                           */
/*   						                            					                       */
/* AUTHOR: a20639        LOCATION: Guad, Mex	   							               */
/*                                                                           */
/*****************************************************************************/
typedef unsigned char   UINT8;      /*unsigned 8 bit definition */
typedef unsigned short  UINT16;     /*unsigned 16 bit definition*/
typedef unsigned long   UINT32;     /*unsigned 32 bit definition*/
typedef signed char     INT8;       /*signed 8 bit definition */
typedef short           INT16;      /*signed 16 bit definition*/
typedef long int        INT32;      /*signed 32 bit definition*/
#define TRUE    1
#define FALSE   0

#define FBUS 8000000					// Define the Bus frequency
#define SCALE (unsigned char) (FBUS/((unsigned long) 100* (unsigned long) 8 * (unsigned long)2 * (unsigned long)120 )) 
										/* Sets the scaled clock (FBUS/(Period*Prescale*2*Fpwm)) 
											4 @2Mhz ; 16 @8Mhz;  32 @16 MHz*/
 
#define SCI_FBUS FBUS
#define BDL38400 	(unsigned char) ((unsigned long)SCI_FBUS/(unsigned long)((unsigned long)16*(unsigned long)38400))

#include "device_headers.h"            

#pragma CODE_SEG DEFAULT

void SCIInit(void);
void SCIStartTransmit(UINT8 cData);
void SCITransmitStr(UINT8 *pStr);
void SCITransmitArray(UINT8 *pStr, UINT8 length);

#pragma CODE_SEG __NEAR_SEG NON_BANKED          //JCB
interrupt void Vscirx();
#pragma CODE_SEG DEFAULT


