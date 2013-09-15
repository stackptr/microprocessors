; Corey Johns
; HW #6
; Fall 2012
; 9/26/2012


; export symbols
            XDEF Entry, _Startup            ; export 'Entry' symbol
            ABSENTRY Entry        ; for absolute assembly: mark this as application entry point



; Include derivative-specific definitions 
		INCLUDE 'derivative.inc' 

ROMStart    EQU  $4000  ; absolute address to place my code/constant data
EOS         EQU  0

; variable/data section

            ORG RAMStart
 ; Insert here your data definition.
vSum        DS.W 1
vCnt        DS.B 1


; code section
            ORG   ROMStart

Entry:
_Startup:
            LDS   #RAMEnd+1       ; initialize the stack pointer
            CLI                     ; enable interrupts

;  Begin assignment code
            CLRA                ; Clear A register
            CLRB                ; Clear B register
            STD vSum            ; Initialize vSum to 0
            STAA vCnt           ; Initialize vCnt to 0
            LDX #cString        ; Set X to point to string address
            LDAA X              ; Initialize A with first char of string

; Start looping through cString          
loop:
            LDAB vCnt           ; Load B with count
            INCB                ; Increment count
            STAB vCnt           ; Store it back
            
            EORA vKey           ; XOR the current char with the key       ; 
            PSHA                ; Push the value to the stack
            LDD vSum            ; Load D with the current sum
            ADDB 1,SP+          ; Add B with the value from stack
            ADCA 0              ; If there was a carry, add it to B
            STD vSum            ; Store D back in sum variable
            
            LDAA 1,+X           ; Point to next value
            BNE loop            ; Branch if next value is not EOS
            
            LDAB vCnt
            INCB
            STAB vCnt           ; Increment for EOS
            

Spin:       BRA  Spin
;*************************************************************
vKey        DC.B  $F0           ; Place key code here
cString     DC.B "We meet again, at last. The circle is now complete. "
            DC.B "When I left you, I was but the learner; now, I am the master."
            DC.B  EOS


;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
            ORG   $FFFE
            DC.W  Entry           ; Reset Vector


