/*****************************************************************************/
/*                                             COPYRIGHT (c) FREESCALE 2004  */
/* FILE NAME:  SCI.c	                                        	  	 	       */
/*                                                                           */
/*   						                            					                       */
/* AUTHOR: R00235C        LOCATION: Guad, Mex	   							               */
/*                                                                           */
/*****************************************************************************/
#pragma CODE_SEG DEFAULT

#include "SCI.h"
#include "device_headers.h"


UINT8 gu8SCIData[2];
UINT8 gu8SCIDataFlag; 


/*****************************************************************************
 * Method: SCIInit																													 *
 * Used to Initialize the SCI module for debuging purposes									 *
 *****************************************************************************/
 void SCIInit(void){																													      
    PBMCUSLK_SCIBDH = 0;							/* Set BaudRate =38400*/
  	PBMCUSLK_SCIBDL	= BDL38400;
  	  
  	PBMCUSLK_SCICR1 = 0x00;							/*SCI0CR2 = 0x0C;  8 bit, TE and RE set */
   	PBMCUSLK_SCICR2 = 0x2C;              /* Receive Interrupt Enable, TE and RE enable */
  }

void SCITransmitArray(UINT8 *pStr, UINT8 length)
{
    UINT8 i;
    for (i=0; i<length; i++)
    {
        SCIStartTransmit(pStr[i]);
    }
}

void SCIStartTransmit(UINT8 cData)
  {
      while (!PBMCUSLK_SCITDRE);  /* ensure Tx data buffer empty */
      PBMCUSLK_SCIDRL = cData; /* load data to SCI2 register */
      while (!PBMCUSLK_SCITC); /* wait for Tx complete */
 }



void SCITransmitStr(UINT8 *pStr)
{
    UINT16 i;
    UINT16 nStrLen=strlen(pStr);
    
    for (i=0; i<nStrLen; i++)
    {
        SCIStartTransmit(pStr[i]);
    }
}

/*****************************************************************************
 * Method: SCI Interrupt																												 *
 * Interrupt Declaration                    									 *
 *****************************************************************************/
//#pragma CODE_SEG __NEAR_SEG NON_BANKED

interrupt void Vscirx(void)
{    
    UINT8 u8status, u8dummy;
    /* ensure Rx data buffer is full */
      do 
      {
          u8status = PBMCUSLK_SCISR1;
      } while (PBMCUSLK_SCIRDRF == 0);		// 	 u8status & 0x20
      
      /* Check for Errors (Framing, Noise, Parity) */
      if ((u8status & 0x07) != 0) 
      {
          u8dummy = PBMCUSLK_SCIDRL;
          return;
      }

      /* Good Data. */
      gu8SCIData[0] = PBMCUSLK_SCIDRL; /* load SCI register to data */
      gu8SCIData[1] = 0;
    
    gu8SCIDataFlag = 1;
}

//#pragma CODE_SEG DEFAULT      //JCB

UINT8 SCIgets(char *pPrompt, char *pBuffer, UINT8 u8Size) {

   
   UINT8 u8ExitFlag = FALSE;
   UINT8 u8Offset=0;
   
   //Echo the string for the command line
   SCITransmitStr(pPrompt);
   
   //Keep processing SCI inputs until Size is reached or terminating NULL
   //occurs.
   while (u8ExitFlag == FALSE) {
      if (gu8SCIDataFlag == 1) {
         gu8SCIDataFlag = 0;
      
         
         //Check the terminating string
         switch (gu8SCIData[0]) {
         
            //Check for Terminating string
            case 0x0D:
               u8ExitFlag = TRUE;
               pBuffer[u8Offset] = 0; //Terminate sting with null.
               break;
               
            case '\b':
               if (u8Offset != 0)
                  pBuffer[u8Offset--] = 0;
               else
                  pBuffer[u8Offset] = 0;
               break;
            
            case '\n':
            default:
               pBuffer[u8Offset++] = gu8SCIData[0];
               break;
         }
         
         
         
         
         //Check the buffersize not to overrun buffer.
         
         
         if (u8Offset == u8Size) {
            u8ExitFlag = TRUE;
         }
      }
   }
   
   return u8Offset;
   
}
//#pragma CODE_SEG DEFAULT