/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: boardTesting.h
* Workspace: DriveBot_v5
* Project: DriveBot_v5.1
* Version: v5.1.0
* Authors: Craig Cheney, Scott McCuen
* 
* PCB: DriveBot MCU v5.1.0
* PSoC: CY8C4245LQI-483
*
* Brief:
*  Header for testing the various features of the drivebot
* 
* 2018.04.10 CC - Document Created
********************************************************************************/

/* Header Guard */
#ifndef BOARD_TESTING_H
    #define BOARD_TESTING_H
    
    /***************************************
    * Included files
    ***************************************/
    #include "project.h"
    /***************************************
    * Macro definitions 
    ***************************************/
    #define TEST_DELAY_LOOP      (250u)
    #define TEST_DELAY_LONG      (1500)
        /***************************************
    * Enumerated types
    ***************************************/
    typedef struct {
        uint32 rightCount;  /**< Right encoder count value */
        uint32 leftCount;   /**< Left encoder count value */
    } encoder_T;
    /***************************************
    * Function prototypes 
    ***************************************/
    
    /* MICA UART Api */
//    void boardTest_usbEcho(void);
    /* MICA encoder functions */
    void boardTest_getEncoderCounts(encoder_T * encoderPtr) ;
    void boardTest_resetEncoderCounts(encoder_T * encoderPtr);
    /* MICA Button functions */
    uint32 boardTest_getPressCount(void);
    uint32 boardTest_getReleaseCount(void);
    void boardTest_resetBtnCounts(void);
    bool boardTest_wasButtonPressed(void);
    void boardTest_EnableBtnInterrupts(void);
    /***************************************
    * Interrupt prototypes 
    ***************************************/

    

    
    
#endif /* BOARD_TESTING_H */

/* [] END OF FILE */
