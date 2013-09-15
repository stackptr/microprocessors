;*************************************************************
; Corey Johns
; HW #10
; Fall 2012
; 10/23/2012
;*************************************************************

            XDEF Entry, _Startup
            ABSENTRY Entry
		        INCLUDE 'derivative.inc' 

;*************************************************************
; Equates
;*************************************************************
ROMStart    EQU  $4000
EOS         EQU  0

;*************************************************************
; RAM
;*************************************************************
            ORG RAMStart
vSub        DS.W 1
vCnt        DS.B 1

;*************************************************************
; Program
;*************************************************************
            ORG   ROMStart
Entry:
_Startup:   LDS   #RAMEnd+1     ; initialize the stack pointer
            CLI                 ; enable interrupts

;  Begin assignment code
            JSR Sub1            ; Call first subroutine
            
            ; Pass parameters to second subroutine through registers
            LDAA vKey
            LDX #cString
            JSR Sub2            ; Call second subroutine
            
            ; Use stack to pass paramters to third subroutine
            LDAA vKey
            LDX #cString
            PSHA
            PSHX
            JSR Sub3             ; Call third subroutine
            
Spin:       BRA  Spin

;*************************************************************
; Subroutine Sub1
; Performs character count and sum of a string, using fixed memory addresses
; for the input string and key.
;
; Parameters list:
;   cString, fixed, input: String to be analyzed
;   vKey, fixed, input: Key used to perform XOR with sum.
;   vCnt, fixed, output: Stores the count of the string
;   vSub, fixed, output: Stores the sum of the string
;
; Registers changed: none (All registers used are saved and then returned after subroutine)
;*************************************************************
Sub1:       PSHA                ; Save any registers being used on the stack
            PSHB
            PSHX
            PSHY
            CLRA                ; Clear A to store the count
            LDY #0              ; Y stores the sum
            LDX #cString        ; X stores the pointer to each char
L0:         LDAB 1, X+          ; B is the current char of the string
            BEQ L1              ; Terminate loop when B is 0
            EORB vKey           ; Otherwise, XOR the char with the key
            ABY                 ; Add B to Y
            INCA                ; Increment the count
            BRA L0              ; Branch back
L1:         INCA                ; When loop terminates, increment count for EOS
            STAA vCnt           ; Store A in vCnt
            STY vSub            ; Store Y in vSub
            PULY                ; Return registers from stack
            PULX
            PULB
            PULA
            RTS                 ; Return from subroutine

;*************************************************************
; Subroutine Sub2
; Performs character count and sum of a string, using registers
; to pass in and return the values.
;
; Parameters list:
;   X, register, input: Address of string to be analyzed
;   A, register, input: Key used to perform XOR with sum.
;   A, register, output: Stores the count of the string
;   Y, register, output: Stores the sum of the string
;
; Registers changed: A, Y
;*************************************************************
Sub2:       PSHB
            PSHA                ; Key is now on SP
            CLRA                ; Clear A to store the count
            LDY #0              ; Y store the sum
L2:         LDAB 1, X+          ; B is the current char of the string
            BEQ L3              ; Terminate loop when B is 0
            EORB SP             ; Otherwise, XOR B with key (which is on the stack)
            ABY                 ; Add B to Y
            INCA                ; Increment the count
            BRA L2              ; Branch back
L3:         INCA                ; When loop terminates, increment count for EOS
            LEAS 1, SP          ; Adjust SP to ignore the key 
            PULB                ; Return B from stack
            RTS                 ; Return from subroutine

;*************************************************************
; Subroutine Sub3
; Performs character count and sum of a string, using the stack to
; pass the input string and key and registers to return the values.
;
; Parameters list:
;   String, stack, input: Address of string to be analyzed
;   Key, stack, input: Key used to perform XOR with sum.
;   A, register, output: Stores the count of the string
;   Y, register, output: Stores the sum of the string
;
; Registers changed: A, Y
;*************************************************************
Sub3:       PSHX                ; Save registers on stack
            PSHB
            LDX 5, SP           ; Load X with pointer to string (on stack)
            CLRA                ; Clear A to store the count
            LDY #0              ; Y store the sum
L4:         LDAB 1, X+          ; B is the current char of the string
            BEQ L5              ; Terminate loop when B is 0
            EORB 7, SP          ; Otherwise, XOR B with key (on stack)
            ABY                 ; Add B to Y
            INCA                ; Increment the count
            BRA L4              ; Branch back
L5:         INCA                ; When loop terminates, increment count for EOS
            PULB                ; Return B, X from stack
            PULX
            RTS                 ; Return from subroutine

;*************************************************************
; Constant data
;*************************************************************
vKey        DC.B  $8F
cString     DC.B "We meet again, at last. The circle is now complete. "
            DC.B "When I left you, I was but the learner; now, I am the master."
            DC.B  EOS


;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
            ORG   $FFFE
            DC.W  Entry           ; Reset Vector
