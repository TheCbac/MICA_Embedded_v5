/***************************************************************************
*                                       MICA
* File: bleImu.c
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
*   Code for controlling the BLE stack while using the MICA IMU. 
* 
* ChangeLog: 
* 2017.08.24 CC - Changed name to bleImu.h (from micaBle_imu.h)             ✓
*               - Moved Prorgram and BLE definitions to micaBle 
* 2017.08.13 CC - Added Program and BLE definitions, MVP                    ✓
* 2017.08.01 CC - Document created                                          ✓
* 
********************************************************************************/
/* Header Guard */
#ifndef BLE_IMU_H
    #define BLE_IMU_H
    
    /***************************************
    * Included files
    ***************************************/
    #include "project.h"
    #include "debug.h"
    /***************************************
    * Macro definitions 
    ***************************************/
//    /* Progam IDs */
//    #define ID_PROGRAM_STACK            (0u)
//    #define ID_PROGRAM_APP              (1u)
    /* BLE definitions */
    #define PASSKEY_IGNORE              (0u)
//    
//    /* MTU Definitions */
//    #define MICA_BLE_MTU_MAX_SIZE                       (512)       /**< Largest Maximum Transmission Unit (MTU) size allowed */
//    #define MICA_BLE_MTU_DEFAULT_SIZE                   (23)        /**< Default Maximum Transmission Unit (MTU) size */
//    #define MICA_BLE_MTU_UNUSABLE                       (0x03u)     /**< Packet overhead */
    
    /***************************************
    * Function declarations 
    ***************************************/
    void imuBle_init(void);
    void imuBle_processEvents(void);
    /***************************************
    * Enumerated types
    ***************************************/
    
#endif /* BLE_IMU_H */
/* [] END OF FILE */
