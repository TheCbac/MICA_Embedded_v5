/***************************************************************************
*                                       SwingTime
* File: powerManagement.c
* Workspace: SwingTime_v2.1
* Project Name: 02_SwingTime_App_v2.1
* Version: v2.0.1
* Authors: Craig Cheney
* 
* PCB: SwingTime_v2.1.2
* PSoC: CYBLE-214009-00 (EZBLE, 4.1, 256k)
* Sensors: MMA8635 (Accelerometer)
*
* Brief:
*   Definintions for power management
* 
* 2018.02.16 CC - Document created
********************************************************************************/
/* Header Guard */
#ifndef POWER_MANAGEMENT_H
    #define POWER_MANAGEMENT_H
    /***************************************
    * Included source files
    ***************************************/
    #include "project.h"
        
    /***************************************
    * Macro definitions 
    ***************************************/
    
    /***************************************
    * Enumerated types
    ***************************************/
    /* Power state of the appication */
    typedef enum {
        STATE_ACTIVE,
        STATE_PREP_SLEEP,
        STATE_WAIT_FOR_SLEEP,
        STATE_DEEPSLEEP,
        STATE_WAKEUP
    } APP_POWER_STATE_T;

    /***************************************
    * Function declarations 
    ***************************************/
    void power_processSystemState(void);
    APP_POWER_STATE_T power_getSystemState(void);
    APP_POWER_STATE_T power_setSystemState(APP_POWER_STATE_T nextState);
    

#endif /*POWER_MANAGEMENT_H*/

/* [] END OF FILE */
