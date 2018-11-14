/***************************************************************************
*                                       MICA
* File: debug.h
* Workspace: IMU_v4.0
* Project Name: 01_IMU_Stack_v4.0
* Version: v4.0.0
* Author: Craig Cheney
* 
* PCB: MICA_IMU_v3.5.2
* PSoC: CYBLE-214015-01 (EZBLE, 4.2, 256k)
* Sensors: BOSCH IMU (BMX055)
* 
*
* Brief:
*   Contains macros for debugging over the UART. The SW transmit UART is 
*   a firmware only component, and does not require "UART_Start()"
* 
* Date Written:  2017.07.31
* Last Modified: 2017.08.13
********************************************************************************/
/* Header Guard */
#ifndef DEBUG_H
    #define DEBUG_H
    
    /***************************************
    * Included files
    ***************************************/
    #include "project.h"
    /***************************************
    * Macro definitions 
    ***************************************/
    /* No Longer valid since porting to MICA IMU */
    //#define MICA_DEBUG           /* Define for UART Debugging */
    
    /* ------------- UART ----------------- */
    #ifdef MICA_DEBUG
        #define DBG_PRINT(a)        do { UART_PutString(a); } while (0)
        void DBG_PRINT_DEC(uint32 val);
        void DBG_PRINT_DEC_TEXT(uint32 val, char* text);
        void PrintHeader(void);
    #else
        #define DBG_PRINT(a)                do { (void)0; } while (0)
        #define DBG_PRINT_DEC(a)            do { (void)0; } while (0)
        #define DBG_PRINT_DEC_TEXT(a, b)    do { (void)0; } while (0)
        #define PrintHeader()               do { (void)0; } while (0)
    #endif /* MICA_DEBUG */
    
    /* Constants */
    #define RESET_INDEX             (0u)
    #define ZERO                    (0u)
    #define ONE                     (1u)
    #define TWO                     (2u)
    #define TEN                     (10u)
    #define TWELVE                  (12u)
    #define ZERO_INDEX              (ONE)
    #define BUFF_LEN_32_BIT_DEC     (TWELVE)
    /***************************************
    * Function declarations 
    ***************************************/
    
    /***************************************
    * Enumerated types
    ***************************************/
    
#endif /* DEBUG_H */
/* [] END OF FILE */