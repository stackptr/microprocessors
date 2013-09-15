;*************************************************************
; Corey Johns
; HW 11
; Fall 2012
; 10/30/12
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
SEGHEXC     EQU   %11111001
SEGHEXD     EQU   %11011110
SEGHEXE     EQU   %11111001
SEGHEXF     EQU   %11110001

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
            
L0:         LDAA PORTT
            JSR HEX2SEVEN
            BRA L0

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
