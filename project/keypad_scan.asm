;*************************************************************
; keypad_scan.asm
; Interfaces with keypad.
;
; Version history:
;   Nov 16: Fixed minor mistakes in code - Corey Johns
;   Nov 16: Finished formatting, added comments - Corey Johns
;   Nov 12: Import into project, formatting - Corey Johns
;   Nov 10: File creation - Eugene Qin
;*************************************************************

		        INCLUDE 'derivative.inc'  ; Include derivative-specific definitions
            XDEF key_scan             ; Export subroutine

;*************************************************************
; Equates
;*************************************************************

; Define keypad
NUM_ROWS:	  EQU   4	              ;number of rows
NUM_KEYS:	  EQU   16              ;number of keys - 123456789*0#

; Define constants
ROW3:		    EQU	  %00001110       ;row 3 scan code
ROW2:	      EQU	  %00001101       ;row 2 scan code
ROW1:	      EQU	  %00001011       ;row 1 scan code
ROW0:		    EQU	  %00000111       ;row 0 scan code

OUTPUTS:    EQU   %00001111       ; Bits 0-3 are outputs, for row codes to be written to
INPUTS:     EQU   %11110000       ; Bits 4-7 are inputs, for col codes to be read
PAD_CONF    EQU   %00001111       ; Reflects the above

COL3:       EQU   %01110000       ;col 3 scan code
COL2:       EQU   %10110000       ;col 2 scan code
COL1:       EQU   %11010000       ;col 1 scan code
COL0:       EQU   %11100000       ;col 0 scan code

KEY_MASK:   EQU   %11110000
NO_KEYS:    EQU   %11110000       ;code for no keys pressed

;*************************************************************
; Program
;*************************************************************

SubCode:    SECTION

;*************************************************************
; Subroutine key_scan
; Delays using the timer subsystem.
;
; Parameters list:
;   PortAD, input/output, fixed - Bits 0-3 are output bits, bits
;     4-7 are input bits. Connected to keypad.
;   B, output, register - ASCII char of 1-9, A-D, *, or # if the
;     corresponding button is pressed on the keypad, otherwise
;     a null character.
;   CCR, output, register - carry bit is set if button is pressed.
;
; Registers used: A, B, X, Y
;*************************************************************

key_scan:   PSHA                  ; Save registers
            PSHX                  ; No need to save B since it is used to
            PSHY                  ; return the pressed key.
            
            LDAA DDRB            ; Read the current direction for port AD
            CMPA #OUTPUTS         ; Check if the current configuration matches what is required
            BEQ skip_init         ; If it does, skip initialization
            
AD_init:    BSET DDRB,OUTPUTS    ; Set Port AD as output on bits 0-3, input on bits 4-7
            BSET PUCR,INPUTS     ; Enable input pull-ups
            
            ; Initialize counter and pointer to scan all cols
skip_init:  LDAA #NUM_ROWS
            LDX #Row_Codes
            
            ; Output each row code and read the col code each time
scan_cols:  MOVB 0,X,PORTB         ; Output the row code to Port AD
            NOP                   ; Delay a little before reading
            NOP
            LDAB PORTB
            MOVB #OUTPUTS, PORTB   ; Reset row output
            
            PSHB                  ; Push to register before checking if a key is pressed
            ANDB #KEY_MASK        ; Mask off row code
            CMPB #NO_KEYS         ; Compare with no keys
            PULB
            
            BNE key_pressed       ; If key pressed, handle it
            INX                   ; Otherwise, point to next row code
            DBNE A,scan_cols      ; Decrement A and continue scanning as long as it's not 0
            CLRB                  ; At this point, there is no key pressed, so clear B and carry,
            BRA all_done          ; such that the returned value is $00
            
key_pressed:
            LDX #Good_Codes       ; Point to table of valid codes
            LDY #Key_Codes        ; Point to tables of ASCII chars corresponding to valid codes
            LDAA #NUM_KEYS        ; The number of keys gives the max amount of loops
            
check_codes:
            CMPB 0,X              ; Compare the byte from keypad to a valid code
            BNE continue          ; If it's not equal, branch
            LDAB 0,Y              ; Otherwise, load B with the ASCII code from the lookup table
            SEC                   ; And set the carry bit, indicating a key was pressed
            BRA all_done          ; All done!

continue:   INX                   ; Increment through code table
            INY                   ; Increment through ASCII table (since the offset of the two tables are equal)
            DBNE A,check_codes    ; Decrement A and continue checking
            CLRB                  ; More than one key is pressed, so clear B and carry
            
all_done:   PULY                  ; When done, return registers
            PULX
            PULA
            RTC                   ; Return to caller
            
;*************************************************************
; Constant data
;*************************************************************
subConst:   SECTION
Row_Codes:	DC.B	ROW3 ;row 3 scan code
		DC.B	ROW2 ;row 2 scan code
		DC.B	ROW1 ;row 1
		DC.B	ROW0 ;row 0

Good_Codes:	DC.B 	COL3 | ROW3 ;"1"
		DC.B	COL2 | ROW3 ;"2"
		DC.B	COL1 | ROW3 ;"3"
		DC.B 	COL0 | ROW3 ;"A"
		DC.B	COL3 | ROW2 ;"4"
		DC.B	COL2 | ROW2 ;"5"
		DC.B 	COL1 | ROW2 ;"6"
		DC.B	COL0 | ROW2 ;"B"
		DC.B	COL3 | ROW1 ;"7"
		DC.B 	COL2 | ROW1 ;"8"
		DC.B	COL1 | ROW1 ;"9"
		DC.B	COL0 | ROW1 ;"C"
		DC.B 	COL3 | ROW0 ;"*"
		DC.B	COL2 | ROW0 ;"0"
		DC.B	COL1 | ROW0 ;"#"
		DC.B	COL0 | ROW0 ;"D"

Key_Codes:  DC.B  "ABCD369#2580147*"
            