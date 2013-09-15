/*************************************************************
 * keypad_scan.h
 * Provides assembly function interfaces for handling keypad.
 *
 * Version history:
 *   Nov 16: File creation - Corey Johns
 *************************************************************/

#ifndef _KEYPAD_SCAN_ASM_H
#define _KEYPAD_SCAN_ASM_H

#ifdef __cplusplus
    extern "C" { /* our assembly functions have C calling convention */
#endif

unsigned char key_scan(void);

#ifdef __cplusplus
    }
#endif

#endif /* _KEYPAD_SCAN_ASM_H */
