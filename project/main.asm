;*************************************************************
; main.asm
; Provides assembly routines to interface with stepper motor,
; keypad.
;
; Version history:
;   Nov 12: File creation - Corey Johns
;*************************************************************

		        INCLUDE 'derivative.inc'  ; Include derivative-specific definitions
            XDEF delay_1ms

;*************************************************************
; Equates
;*************************************************************  		



;*************************************************************
; RAM
;*************************************************************
RAM:        SECTION


;*************************************************************
; Program
;*************************************************************
Program:    SECTION

; Creates a delay of approximately one millisecond (1000 operations)
delay_1ms:  PSHA
            PSHB
            LDAA #15
delay_L0:   LDAB #20
delay_L1:   DBNE B, delay_L1
            DBNE A, delay_L0
            PULB
            PULA
            RTC
