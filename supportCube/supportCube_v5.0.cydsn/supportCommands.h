/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: supportCommands.h
* Workspace: supportCube_v5
* Project: supportCube_v5.0
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: supportCube v2.1.1
* PSoC: CYBLE-214015-01
*
* Brief:
*   Header for supportCommands.c
*
* 2018.10.19  - Document Created
********************************************************************************/

/* Header Guard */
#ifndef supportCommands_H
    /***************************************
    * Included files
    ***************************************/
    #include <stdint.h>
    #include "packets.h"
    /***************************************
    * Macro Definitions
    ***************************************/
    #define SUPPORT_ID_DEVICE_MSB           (0x00)
    #define SUPPORT_ID_DEVICE_LSB           (0x01)
    #define SUPPORT_ID_FIRMWARE_MSB         (0x05)
    #define SUPPORT_ID_FIRMWARE_LSB         (0x00)
    
    /***************************************
    * Enumerated Types
    ***************************************/
    
    /***************************************
    * Structures
    ***************************************/

    /***************************************
    * Function declarations 
    ***************************************/
    uint32_t cmdHandler_supportCube(packets_PACKET_S* rxPacket, packets_PACKET_S* txPacket);
    uint32_t ackHandler_supportCube(packets_PACKET_S* packet);
    

    uint32_t cmdHandler_print(packets_PACKET_S* cmdPacket, packets_PACKET_S* ackPacket);
    uint32_t ackHandler_print(packets_PACKET_S* packet);
    
    uint32_t cmdHandler_noop(packets_PACKET_S* cmdPacket, packets_PACKET_S* ackPacket);
    uint32_t ackHandler_noop(packets_PACKET_S* packet);        

#endif /* supportCommands_H */
/* [] END OF FILE */
