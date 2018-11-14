/***************************************************************************
*                                       MICA
* File: otaUpdate.h
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
*   Code for executing an OTA update. Contains both optional and mandatory steps.
* 
* Date Written:  2017.08.01
* Last Modified: 2017.08.13
********************************************************************************/
/* Header Guard */
#ifndef OTA_UPDATE_H
    #define OTA_UPDATE_H
    
    /***************************************
    * Included files
    ***************************************/
    #include "project.h"
    #include "debug.h"
    /***************************************
    * Macro definitions 
    ***************************************/
    #define ZERO                                (0u)
    /* Offset for flags */
    #define OFFSET_FLAG_STACK                   (10u)
    #define OFFSET_FLAG_APP                     (20u)
    /* Base addresses */
    #define BASE_ADDR_STACK                     (CYDEV_FLASH_BASE + (CYDEV_FLASH_SIZE - ((uint32)CYDEV_FLS_ROW_SIZE)))
    #define BASE_ADDR_APP                       (CYDEV_FLASH_BASE + (CYDEV_FLASH_SIZE - ((uint32)CYDEV_FLS_ROW_SIZE * 2u)))
    /* Bootloading MetaData */
    #define OTA_MD_BTLDB_ACTIVE_1               (0x01u)
    #define OTA_MD_SIZEOF                       (64u)
    #define OTA_MD_BASE_ADDR(appId)             (CYDEV_FLASH_BASE + (CYDEV_FLASH_SIZE - ((uint32)(appId) * CYDEV_FLS_ROW_SIZE) - \
                                                                        OTA_MD_SIZEOF))
    #define OTA_MD_BTLDB_ACTIVE_OFFSET(appId)   (OTA_MD_BASE_ADDR(appId) + 16u)
    /* Bonding status */
    #define BONDING_ENABLED                     ((CYBLE_GAP_ROLE_PERIPHERAL || CYBLE_GAP_ROLE_CENTRAL) && (CYBLE_BONDING_REQUIREMENT == CYBLE_BONDING_YES))
    /* Progam IDs */
    #define ID_PROGRAM_STACK                    (0u)
    #define ID_PROGRAM_APP                      (1u)
    /* Flags */
    #define FLAG_SET                            (0u)
    #define FLAG_CLEAR                          (1u)
    #define FLAG_UPDATE_STACK                   (BASE_ADDR_STACK + OFFSET_FLAG_STACK)
    #define FLAG_UPDATE_APP                     (BASE_ADDR_APP   + OFFSET_FLAG_APP)
    /* Macro to get the flag status */
    #define FLAG_UPDATE_STACK_GET               ( (uint32) CY_GET_XTND_REG8( (volatile uint8 *)FLAG_UPDATE_STACK) )
    #define FLAG_UPDATE_APP_GET                 ( (uint32) CY_GET_XTND_REG8( (volatile uint8 *)FLAG_UPDATE_APP) )
    /***************************************
    * Function declarations 
    ***************************************/
    void OTA_initializeCodeSharing(void);
    void OTA_afterImageUpdate(void);
    void OTA_EnterBootloadMode(void);
    /***************************************
    * Enumerated types
    ***************************************/
    
#endif /* OTA_UPDATE_H */
/* [] END OF FILE */
