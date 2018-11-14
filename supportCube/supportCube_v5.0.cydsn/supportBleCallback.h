/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: supportBleCallback.h
* Workspace: supportCube_v5
* Project: supportCube_v5.0
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: supportCube v1.0.0
* PSoC: CYBLE-214015-01
*
* Brief:
*   Header for supportBleCallback.c
*
* 2018.10.19  - Document Created
********************************************************************************/

/* Header Guard */
#ifndef supportBleCallback_H
    /***************************************
    * Included files
    ***************************************/
    #include "project.h"
    /***************************************
    * Macro Definitions
    ***************************************/
    
    /***************************************
    * Enumerated Types
    ***************************************/

    
    /***************************************
    * Structures
    ***************************************/

    /***************************************
    * Function declarations 
    ***************************************/
    void supportBleHandler(uint32 eventCode, void * eventParam);
    
    void setConnectingDeviceId(uint8_t *bdAddr);
    void getConnectingDeviceId(uint8_t *bdAddr);
    
    void setPendingConnection(bool newState);
    bool getPendingConnection(void);
    CYBLE_CONN_HANDLE_T *getBleHandle(void);
        

#endif /* supportBleCallback_H */
/* [] END OF FILE */
