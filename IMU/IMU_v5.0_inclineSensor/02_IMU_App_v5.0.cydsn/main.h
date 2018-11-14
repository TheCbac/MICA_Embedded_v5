/***************************************************************************
*                                       MICA
* File: main.h
* Workspace: IMU_v4.0
* Project Name: 01_IMU_App_v4.0
* Version: v4.0.0
* Author: Craig Cheney
* 
* PCB: MICA_IMU_v3.5.2
* PSoC: CYBLE-214015-01 (EZBLE, 4.2, 256k)
* Sensors: BOSCH IMU (BMX055)
*
* Brief:
*  Application program for the upgradeable stack app. 
* 
* Date Written:  2017.08.01
* Last Modified: 2017.08.13
********************************************************************************/
/* Header Guard */
#ifndef MAIN_H
    #define MAIN_H
    
    /***************************************
    * Included files
    ***************************************/
    //#include "project.h" /
//    #include "configMica.h"
    #include "mica.h"
    
    /***************************************
    * Macro definitions 
    ***************************************/
    /* Counter match for the Watchdog timer */
    #define WDT_COUNTER0_MATCH_VAL  (0xFFFFu)
    #define WDT_COUNTER1_MATCH_VAL  (0x03u)
    /***************************************
    * Function declarations 
    ***************************************/
    void initializeDevice(void);
    /***************************************
    * Interrupt prototypes 
    ***************************************/
    //CY_ISR_PROTO(ISR_dataReady);

    /***************************************
    * Enumerated types
    ***************************************/
    
#endif /* MAIN_H */
/* [] END OF FILE */
