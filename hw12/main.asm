;*************************************************************
; Corey Johns
; HW 12
; Fall 2012
; 11/6/12
;*************************************************************

            XDEF Entry, _Startup
            ABSENTRY Entry
		        INCLUDE 'derivative.inc' 

;*************************************************************
; Equates
;*************************************************************
ROMStart    EQU   $4000

PORTT       EQU   $0241
DDRPA       EQU   $0002
DDRPB       EQU   $0003
DDRPT       EQU   $0242

CONFOUT     EQU   %11111111

; Seven segment display hex masks
;                 %1gfedcba (active high)
SEGHEX0     EQU   %10111111
SEGHEX1     EQU   %00000110
SEGHEX2     EQU   %11011011
SEGHEX3     EQU   %11001111
SEGHEX4     EQU   %11100110
SEGHEX5     EQU   %11101101
SEGHEX6     EQU   %11111101
SEGHEX7     EQU   %10000111
SEGHEX8     EQU   %11111111
SEGHEX9     EQU   %11100111
SEGHEXA     EQU   %11110111
SEGHEXB     EQU   %11111100
SEGHEXC     EQU   %10111001
SEGHEXD     EQU   %11011110
SEGHEXE     EQU   %11111001
SEGHEXF     EQU   %11110001

; Bit masks
BIT0        EQU   %00000001
BIT1        EQU   %00000010
BIT2        EQU   %00000100
BIT3        EQU   %00001000
BIT4        EQU   %00010000
BIT5        EQU   %00100000
BIT6        EQU   %01000000
BIT7        EQU   %10000000

; Software delays
DELAY1      EQU   620  ; Corresponds to a 1 second delay
DELAY2      EQU   310  ; Corresponds to a 0.5 second delay
DELAYY      EQU   100 ; Use same inner loop max value regardless of delay

;*************************************************************
; RAM
;*************************************************************
            ORG RAMStart

;*************************************************************
; Program
;*************************************************************
            ORG   ROMStart

Entry:
_Startup:   LDS   #RAMEnd+1     ; Initialize the stack pointer
            CLI                 ; Enable interrupts

; Begin assignment code
            LDAA #CONFOUT       ; Set all ports as output
            STAA DDRPA
            STAA DDRPB
            STAA DDRPT          ; T is used as an input, but set as output
            
            CLRA                ; Use A as the 8-bit counter
            JSR HEX2SEVEN       ; Initialize the display to 0
            
MAIN:       BRCLR PORTT,BIT0,C_RESET    ; Reset is first priority
            BRSET PORTT,BIT1,C_HOLD     ; Hold is second priority
            JSR SWDelay         ; Delay before counting up
            BRCLR PORTT,BIT0,C_RESET    ; Check bits again to make sure they
            BRSET PORTT,BIT1,C_HOLD     ; can be activated during delay
            JSR Count           ; Perform the count up or down
            JSR HEX2SEVEN       ; Refresh display
            BRA MAIN            ; Loop back

C_RESET:    TSTA                ; If A is already clear (if it was just reset)
            BEQ MAIN         ; then there is no point in writing to 7seg display
            CLRA                ; Otherwise, clear counter and
            JSR HEX2SEVEN       ; refresh the display
            BRA MAIN
            
C_HOLD:     BRCLR PORTT,BIT1,MAIN ; Branch back to counter when BIT1 set
            BRA C_HOLD          ; Otherwise, just continue to loop

;*************************************************************
; Subroutine HEX2SEVEN
; Displays a hex byte on two seven segment displays.
;
; Parameters list:
;   hex, register A, input: Hex byte to be displayed
;   PortA, fixed, output: Memory address of MSD
;   PortB, fixed, output: Memory address of LSD
;
; Registers used: A, X
;*************************************************************
HEX2SEVEN:  PSHX                ; Save registers
            PSHA
            LDX #segbyte        ; X points to the address of segment code table
            ANDA #$0F           ; A now contains the lower digit
            MOVB A, X, PORTB    ; Write the segment code into PORTB
            LDAA SP             ; Load original input back into A
            LSRA
            LSRA
            LSRA
            LSRA                ; A now contains the higher digit
            MOVB A, X, PORTA    ; Write the segment code into PORTA
            PULA                ; Restore registers
            PULX
            RTS                 ; Return from subroutine

;*************************************************************
; Subroutine SWDelay
; Performs a variable delay given by the state of PT5
;
; Parameters list:
;   PortT, fixed, input: Bit 5 specifies a delay of either 1s
;                        if unset, 0.5 sec if set.
;
; Registers used: X, Y
;*************************************************************
 SWDelay:   PSHX                ; Save registers on stack
            PSHY
            BRSET PORTT, BIT5, SW_Delay2
            LDX #DELAY1         ; PT5 is unset, set delay to 1s
            BRA SWL0
 SW_Delay2: LDX #DELAY2         ; PT5 is set, set delay to 0.5s
 SWL0:      LDY #DELAYY         ; Y is the same value regardless of delay used
 SWL1:      DBNE Y, SWL1
            DBNE X, SWL0
            PULY                ; Restore registers from stack
            PULX
            RTS                 ; Return from subroutine

;*************************************************************
; Subroutine Count
; Counts up or down in hex or decimal based on the state of
; PortT.
;
; Parameters list:
;   A, register, input: Provides the current count
;   PortT, fixed, input: Sets count up or down in decimal or hex
;
; Registers used: A
;*************************************************************
Count:      BRSET PORTT, BIT4, CDown    ; Count down if PT4 is set
            INCA                        ; Otherwise count up
            BRSET PORTT, BIT6, C_Done   ; If counting in hex, we're finished
            PSHA                        ; Otherwise push A
            ANDA #$0F                   ; Get lower digit
            CMPA #$0A                   ; And check if it is equal to A
            BNE C_Restore               ; If not equal, restore A and return
            PULA                        ; Otherwise, restore A...
            ANDA #$F0                   ; Clear lower hex digit
            ADDA #$10                   ; Add to upper digit
            RTS                         ; And return
CDown:      DECA                        ; Count down
            BRSET PORTT, BIT6, C_Done   ; If counting in hex, we're finished
            PSHA                        ; Otherwise, push A
            CMPA #$FF
            BEQ CDownRoll               ; If equal to 0, and counting in dec, roll
                                        ; back to 99
            PULA                        ; Otherwise, restore A
            PSHA                        ; Save A
            ANDA #$0F                   ; Get lower digit
            CMPA #$0F                   ; And check if it is equal to F
            BNE C_Restore               ; If not equal, restore A and return
            PULA                        ; Otherwise, restore A
            ANDA #$F0                   ; Clear lower digit
            ADDA #$09                   ; And replace it with 9
            RTS                         ; Return
C_Restore:  PULA
C_Done:     RTS
CDownRoll   PULA
            LDAA #$99
            RTS

;*************************************************************
; Constant data
;*************************************************************
segbyte     DC.B SEGHEX0,SEGHEX1,SEGHEX2,SEGHEX3,SEGHEX4,SEGHEX5
            DC.B SEGHEX6,SEGHEX7,SEGHEX8,SEGHEX9,SEGHEXA,SEGHEXB
            DC.B SEGHEXC,SEGHEXD,SEGHEXE,SEGHEXF

;*************************************************************
; Interrupt vectors
;*************************************************************
            ORG   $FFFE
            DC.W  Entry           ; Reset Vector
