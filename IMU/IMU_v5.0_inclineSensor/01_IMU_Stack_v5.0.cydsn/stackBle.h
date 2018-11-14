/***************************************************************************
*                                       MICA
* File: stackBle.h
* Workspace: IMU_v5.0
* Project Name: 01_IMU_Stack_v5.0
* Version: v5.0.0
* Author: Craig Cheney
* 
* PCB: MICA_IMU_v3.5.2
* PSoC: CYBLE-214015-01 (EZBLE, 4.2, 256k)
* Sensors: BOSCH IMU (BMX055)
*
* Brief:
*   Code for executing an OTA update. Contains both optional and mandatory steps.
* 
* 2017.07.31 CC - Document Created
* 2018.03.02 CC - Changed name to stackBle from micaOta
********************************************************************************/
/* Header Guard */
#ifndef STACK_BLE_H
    #define STACK_BLE_H
    
    /***************************************
    * Included files
    ***************************************/
    #include "project.h"
    #include "debug.h"
    /***************************************
    * Macro definitions 
    ***************************************/
    /* Timeout definitions */
    #define TIMEOUT_50_MS           (5u)
    
    /* BLE definitions */
    #define PASSKEY_IGNORE          (0u)
    /***************************************
    * Function declarations 
    ***************************************/
    void initializeBLE(CYBLE_CALLBACK_T bleCallback);
    void bleOtaCallback(uint32 event, void* eventParam);
    /***************************************
    * Enumerated types
    ***************************************/
    
#endif /* STACK_BLE_H */
/* [] END OF FILE */
