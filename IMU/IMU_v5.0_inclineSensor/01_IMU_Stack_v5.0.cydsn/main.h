/***************************************************************************
*                                       MICA
* File: main.h
* Workspace: IMU_v4.0
* Project Name: 01_IMU_Stack_v4.0
* Version: v4.0.0
* Author: Craig Cheney
* 
* PCB: MICA_IMU_v3.5.2
* PSoC: CYBLE-214015-01 (EZBLE, 4.2, 256k)
* Sensors: BOSCH IMU (BMX055)
*
* Brief:
*   Upgradable stack appliation for Over the air functionality. This program
*   contains the BLE portion of the code for executing both a stack update, as
*   well as the application update. 
* 
* Date Written:  2017.07.31
* Last Modified: 2017.08.13
********************************************************************************/
/* Header Guard */
#ifndef MAIN_H
    #define MAIN_H
    
    /***************************************
    * Included files
    ***************************************/
    #include "project.h"
    #include "debug.h"
    #include "micaOta.h"
    /***************************************
    * Macro definitions 
    ***************************************/
    #define LED_ON              (0u)
    #define LED_OFF             (1u)
    /***************************************
    * Function declarations 
    ***************************************/

    /***************************************
    * Interrupt prototypes 
    ***************************************/
    //CY_ISR_PROTO(ISR_dataReady);

    /***************************************
    * Enumerated types
    ***************************************/
    
#endif /* MAIN_H */
/* [] END OF FILE */