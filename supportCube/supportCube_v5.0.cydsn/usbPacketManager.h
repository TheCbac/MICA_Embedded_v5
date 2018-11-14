/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: usbPacketManager.h
* Workspace: supportCube_v5
* Project: supportCube_v5.0
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: supportCube v2.1.1
* PSoC: CYBLE-214015-01
*
* Brief:
*   Header for usbPacketManager.c
*
* 2018.10.19  - Document Created
********************************************************************************/

/* Header Guard */
#ifndef usbPacketManager_H
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
    uint32_t usbPackets_init(void);
    uint32_t usbPackets_processIncoming(void);
    uint32_t usbPackets_sendPacket(uint8_t cmd, uint16_t payloadLen, uint8_t *payload, uint16_t flags);    
    uint32_t usbPackets_log(char *msg, ...);

#endif /* usbPacketManager_H */
/* [] END OF FILE */
