;**************************************************************
;* This stationery serves as the framework for a              *
;* user application. For a more comprehensive program that    *
;* demonstrates the more advanced functionality of this       *
;* processor, please see the demonstration applications       *
;* located in the examples subdirectory of the                *
;* Freescale CodeWarrior for the HC12 Program directory       *
;**************************************************************

; export symbols
            XDEF asm_step_motor
            ; we use export 'Entry' as symbol. This allows us to
            ; reference 'Entry' either in the linker .prm file
            ; or from C/C++ later on
            XREF start
            XREF direction
            XREF stopped
            ;XREF delay

; Include derivative-specific definitions 
		INCLUDE 'derivative.inc'  		

;*************************************************************
; Equates
;*************************************************************
PACONF      EQU  %00001111      ; PortA Bits 0-3 outputs, 4-7 inputs
TCONF       EQU  %00000000      ; Timer configuration

BIT7        EQU  %10000000      ; Define Bit masks 0 to 7
BIT6        EQU  %01000000
BIT5        EQU  %00100000
BIT4        EQU  %00010000
BIT3        EQU  %00001000
BIT2        EQU  %00000100
BIT1        EQU  %00000010
BIT0        EQU  %00000001

delay       EQU  $FF7F

; Note,  you need to physically connect PA6 and PA7 to SW1 and SW2 on project board
; They are NOT hardwired.
;
		    
; variable/data section
MY_EXTENDED_RAM: SECTION
; Insert here your data definition. For demonstration, temp_byte is used.
mySeq_ptr   DS.B 1              ; Index pointer to myCount array

; code section
MyCode:     SECTION

asm_step_motor:
            TST stopped
            BNE Count_ret         ; If the motor is stopped, do not do anything in this routine
            PSHA                  ; Otherwise, push registers to stack
            PSHB
            PSHX
            LDX #mySeq            ; Load X with address of count sequence
            TST start             ; If start flag is raised, proceed to initialize
            BEQ begin_count       ; count sequence: skip over the first byte, which is
initialize: LDAA #0               ; used to indicate the beginning of the sequence
            STAA start            ; Unset start flag
            LDAA #1               ; Then load A with 1
            STAA mySeq_ptr        ; And store 1 into the count offset
            BRA set_LED           ; Since A already loaded with offset, skip to setting LED
begin_count:LDAA mySeq_ptr        ; If start flag unset, load A with offset from mySeq_ptr
set_LED:    MOVB A, X, PORTA      ; Move the byte X+A into PORTA
            LDAB direction        ; Load B with the direction
            TST direction
            BEQ CW_inc            ; If direction is 0, then increment (clockwise)
            DECA                  ; Otherwise direction is 1, decrement (CCW)
            BRA CountTest
CW_inc:     INCA
CountTest:  TST A, X              ; Test X[A] to see if it is negative
            BMI CountReset        ; If it's negative, reset offset
            STAA mySeq_ptr        ; Store A in pointer
            BRA COUNT_END
CountReset: TST direction
            BEQ CW_reset
            LDAA #4
            STAA mySeq_ptr
            BRA COUNT_END
CW_reset:   LDAA #1
            STAA mySeq_ptr
COUNT_END:  MOVW #delay, PACNT
            PULX                        ; Return registers from stack
            PULB
            PULA
Count_ret:  RTC                         ; Return from call

mySeq       DC.B  $80,12,6,3,9,$80    ; Count sequence using negative number as EOS          
;mySeq       DC.B  $80,8,12,4,6,2,3,1,9,$80    ; Count sequence using negative number as EOS