/***************************************************************************
*                                    MICA
*                              Craig Cheney © 2018
*                                ccheney@mit.edu
*                                       
* File: boardTesting.c
* Workspace: DriveBot_V4.0
* Project Name: DriveBot_v4.0_TESTING
* Version: v2.0.1
* Authors: Craig Cheney
*
* Brief:
*   Source for PCB population testing functiongs
* 
* 2018.02.13 CC - Document Created
********************************************************************************/
#ifndef COMMON_H
    #define COMMON_H
    /***************************************
    * Included files
    ***************************************/
    #include "cytypes.h"
    /***************************************
    * Macro definitions 
    ***************************************/
    #define LED_ON              (0x00u)
    #define LED_OFF             (0x01u)
    
    #define PWM_COUNT_OFF       (0x00u)
    #define PWM_COUNT_LOW       (0x20u)
    #define PWM_COUNT_MID       (0x80u)
    #define PWM_COUNT_ON        (0xFEu)
    #define COUNTER_RESET       (0xFEu)
   
    #define LTC6915_GAIN_4096   (0b1101)
    
//    #define LED_COUNT_BLINK     (0x80u)
//    #define LED_COUNT_ON        (0xFEu)
//    #define LED_COUNT_OFF       (0x00u)
    
    #define ZERO                (0u)
    #define ONE                 (1u)
    #define ONE_BIT             (1u)
    #define ONE_BYTE            (1u)
    #define TWO_BITS            (2u)
    #define SIX_BITS            (6u)
    #define EIGHT_BITS          (8u)
    #define TEN_BITS            (10u)
    #define OFFSET_ONE          (1u)
    
    #define EIGHT_BIT_MASK      (0xFFu)
    #define TEN_BIT_MASK        (0x3FFu)
    #define SIGN_BIT_TEN_BIT    (1u << 9u)
    
    #define clip(val, min, max) ((  ((val < min)? min: val )  > max) ? max: val) 
    /***************************************
    * Enumerated types
    ***************************************/

    /***************************************
    * Function declarations 
    ***************************************/

    /***************************************
    * Interrupt prototypes 
    ***************************************/

#endif /* COMMON_H */

/* [] END OF FILE */
