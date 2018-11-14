/********************************************************************************
*                                       MICA
*                              Craig Cheney © 2017
*                                ccheney@mit.edu
*                           MIT BioInstrumentation Lab
*
* File: main.h
* Workspace: DriveBot_v4
* Project: DriveBot_v4.0
* Version: v1.0.0
* Author: Craig Cheney
* 
* PCB: DriveBot MCU v1.0.0
* PSoC: CY8C4245LQI-483
*
* Brief:
*   Header file for main.c
* 
* Date Written:  2017.08.11
* Last Modified: 2017.08.11
********************************************************************************/
/* Header Guard */
#ifndef MAIN_H
    #define MAIN_H
    
    /***************************************
    * Included files
    ***************************************/
    #include "project.h"
    /***************************************
    * Macro definitions 
    ***************************************/

    /***************************************
    * Function prototypes 
    ***************************************/
    void processUartPacket(uint8* packet, uint16 len);

    /***************************************
    * Interrupt prototypes 
    ***************************************/
    CY_ISR_PROTO(ISR_button);
    CY_ISR_PROTO(ISR_QDL);
    CY_ISR_PROTO(ISR_Uart);
    /***************************************
    * Enumerated types
    ***************************************/
    
#endif /* MAIN_H */
/* [] END OF FILE */