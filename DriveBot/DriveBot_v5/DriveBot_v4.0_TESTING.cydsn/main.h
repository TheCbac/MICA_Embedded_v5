/***************************************************************************
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
    #define LED_ON                      (0u)
    #define LED_OFF                     (1u)
    
    
    #define MICA_MOTOR_FORWARD          (0b01u)
    #define MICA_MOTOR_BACKWARD         (0b00u)
    #define MICA_MOTOR_EN               (1u << 2u)
 
    #define MICA_MOTOR_LEFT_SHIFT       (0u)
    #define MICA_MOTOR_RIGHT_SHIFT      (4u)
    
    #define MICA_MOTOR_LEFT_FORWARD     (MICA_MOTOR_FORWARD  << MICA_MOTOR_LEFT_SHIFT)
    #define MICA_MOTOR_LEFT_BACKWARD    (MICA_MOTOR_BACKWARD << MICA_MOTOR_LEFT_SHIFT)
    #define MICA_MOTOR_LEFT_EN          (MICA_MOTOR_EN       << MICA_MOTOR_LEFT_SHIFT)
    #define MICA_MOTOR_RIGHT_FORWARD    (MICA_MOTOR_FORWARD  << MICA_MOTOR_RIGHT_SHIFT)
    #define MICA_MOTOR_RIGHT_BACKWARD   (MICA_MOTOR_BACKWARD << MICA_MOTOR_RIGHT_SHIFT)
    #define MICA_MOTOR_RIGHT_EN         (MICA_MOTOR_EN       << MICA_MOTOR_RIGHT_SHIFT)

    /***************************************
    * Function prototypes 
    ***************************************/

    /***************************************
    * Interrupt prototypes 
    ***************************************/
    CY_ISR_PROTO(ISR_button);
    CY_ISR_PROTO(ISR_QDL);
    /***************************************
    * Enumerated types
    ***************************************/
    
#endif /* MAIN_H */
/* [] END OF FILE */