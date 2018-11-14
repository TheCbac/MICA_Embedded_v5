/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: supportCommands.c
* Workspace: supportCube_v5
* Project: supportCube_v5.0
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: supportCube v2.1.1
* PSoC: CYBLE-214015-01
*
* Brief:
*   Command handler for the support cube
*
* 2018.10.19  - Document Created
********************************************************************************/
#include "supportCommands.h"
#include "micaCommon.h"
#include "project.h"
#include "supportBleCallback.h"

/*******************************************************************************
* Function Name: cmdHandler_supportCube()
****************************************************************************//**
* \brief
*  <function description>
*
* \param rxPacket [in]
*  Pointer to the packet containing the command to validate
*
* \param txPacket [out]
*  Pointer to the response packet
* 
* \return
*  Returns the error of associated with 
*******************************************************************************/
uint32_t cmdHandler_supportCube(packets_PACKET_S* rxPacket, packets_PACKET_S* txPacket){
    LEDS_B_Toggle();
    /* State of the BLE */
    CYBLE_STATE_T bleState = CyBle_GetState();
    /* extract command */
    switch(rxPacket->cmd) {
        /* Respond with the device ID */
        case packets_CMD_ID: {
            uint8_t i = ZERO;
            txPacket->payload[i++] = SUPPORT_ID_DEVICE_MSB;
            txPacket->payload[i++] = SUPPORT_ID_DEVICE_LSB;
            txPacket->payload[i++] = SUPPORT_ID_FIRMWARE_MSB;
            txPacket->payload[i++] = SUPPORT_ID_FIRMWARE_LSB;
            txPacket->payloadLen = i;
            break;
        }
        /* Start the scan */
        case packets_CMD_SCAN_START: {
            /* Ensure valid state */
            if(bleState == CYBLE_STATE_DISCONNECTED) {
                CyBle_GapcStartScan(CYBLE_SCANNING_FAST);
            } else {
                /* Set the invalid state flag */
                txPacket->flags |= packets_FLAG_INVALID_STATE;
            }
            break;
        }
        /* Stop the scan */
        case packets_CMD_SCAN_STOP: {
            /* Ensure valid state */
            if(bleState == CYBLE_STATE_SCANNING) {
                CyBle_GapcStopScan();
            } else {
                /* Set the invalid state flag */
                txPacket->flags |= packets_FLAG_INVALID_STATE;
            }
            break;
        }
        /* Connect to a device */
        case packets_CMD_CONNECT: {
            /* Ensure a valid device ID */
            if(rxPacket->payloadLen != CYBLE_GAP_BD_ADDR_SIZE){
                txPacket->flags |= packets_FLAG_INVALID_ARGS;
                break;
            }
            /* Respond with device ID */
            memcpy(txPacket->payload, rxPacket->payload, CYBLE_GAP_BD_ADDR_SIZE);
            txPacket->payloadLen = CYBLE_GAP_BD_ADDR_SIZE;
            /* Connect directly if disconnected */
            if(bleState == CYBLE_STATE_DISCONNECTED) {
                /* Store the device id */
                setConnectingDeviceId( rxPacket->payload);
                /* Initiate the connection */
                CYBLE_GAP_BD_ADDR_T peerAddr;
                memcpy(peerAddr.bdAddr, rxPacket->payload, CYBLE_GAP_BD_ADDR_SIZE);
                peerAddr.type = CYBLE_GAP_ADDR_TYPE_PUBLIC;
                CyBle_GapcConnectDevice(&peerAddr);
            /* Need to stop scan before we can connect */
            } else if( bleState == CYBLE_STATE_SCANNING) {
                /* Indicate that the there is a desired connection pending*/
                setPendingConnection(true);
                setConnectingDeviceId( rxPacket->payload);
                /* Stop the scan. Connection will be under the CYBLE_EVT_GAPC_SCAN_START_STOP callback */
                CyBle_GapcStopScan();
            } else {
                txPacket->flags |= packets_FLAG_INVALID_STATE;   
            }
            break;
        }
        /* Cancel a pending connection */
        case packets_CMD_CONNECT_CANCEL: {
            /* Ensure that there is a connection pending */
            if (bleState != CYBLE_STATE_CONNECTING) {
                txPacket->flags |= packets_FLAG_INVALID_STATE;   
                break;
            }
            /* Request the cancel */
            CYBLE_API_RESULT_T result = CyBle_GapcCancelConnection();
            if (result != CYBLE_ERROR_OK) {
                txPacket->flags |= packets_FLAG_INVALID_STATE;   
                break;
            }
            /* Success, respond with the deviceID */
            memcpy(txPacket->payload, rxPacket->payload, CYBLE_GAP_BD_ADDR_SIZE);
            txPacket->payloadLen = CYBLE_GAP_BD_ADDR_SIZE;
            
            break;
        }
        /* Disconnect from a connected device */
        case packets_CMD_DISCONNECT: {
            /* Ensure a valid device ID */
            if(rxPacket->payloadLen != CYBLE_GAP_BD_ADDR_SIZE){
                txPacket->flags |= packets_FLAG_INVALID_ARGS;
                break;
            }
            /* Respond with device ID */
            memcpy(txPacket->payload, rxPacket->payload, CYBLE_GAP_BD_ADDR_SIZE);
            txPacket->payloadLen = CYBLE_GAP_BD_ADDR_SIZE;
            if(bleState == CYBLE_STATE_CONNECTED) {
            /* Get the peer BD handle from the adddress */
                uint8_t bdHandle;
                CYBLE_GAP_BD_ADDR_T deviceId;
                memcpy(deviceId.bdAddr, rxPacket->payload, CYBLE_GAP_BD_ADDR_SIZE);
                CyBle_GapGetPeerBdHandle(&bdHandle, &deviceId);
                /* Initiate the disconnect */
                CyBle_GapDisconnect(getBleHandle()->bdHandle);
                
            } else {
                txPacket->flags |= packets_FLAG_INVALID_STATE;   
            }
            break;
        }
        /* Write a BLE Characteristic */
        case packets_CMD_CHAR_WRITE: {
            /* Make sure the correct size */
            if (rxPacket->payloadLen <= (CYBLE_GAP_BD_ADDR_SIZE +1)) {
                txPacket->flags |= packets_FLAG_INVALID_ARGS;
                break;
            }
            /* Pass the device ID and handle back */
            memcpy(txPacket->payload, rxPacket->payload, CYBLE_GAP_BD_ADDR_SIZE + 1);
            txPacket->payloadLen = CYBLE_GAP_BD_ADDR_SIZE +1;
            /* Get the peer BD handle from the adddress */
            CYBLE_CONN_HANDLE_T bdHandle;
            CYBLE_GAP_BD_ADDR_T deviceId;
            memcpy(deviceId.bdAddr, rxPacket->payload, CYBLE_GAP_BD_ADDR_SIZE);
            CyBle_GapGetPeerBdHandle(&bdHandle.bdHandle, &deviceId);
            /* Extract the handle */
            uint16_t charHandle =  rxPacket->payload[CYBLE_GAP_BD_ADDR_SIZE];
            
            /* Prepare the write request */
            CYBLE_GATTC_WRITE_REQ_T writeReq;
            writeReq.attrHandle = charHandle;
            writeReq.value.val = &rxPacket->payload[CYBLE_GAP_BD_ADDR_SIZE+1];
            writeReq.value.len = (rxPacket->payloadLen - 7);
            /* Initiate the write */
            CYBLE_API_RESULT_T result = CyBle_GattcWriteCharacteristicValue(*getBleHandle(), &writeReq);
            
            switch(result) {
                case CYBLE_ERROR_OK:{
                    break;
                }
                case CYBLE_ERROR_INVALID_PARAMETER: {
                    txPacket->flags |= packets_FLAG_INVALID_ARGS;
                    break;   
                }
                case CYBLE_ERROR_INVALID_OPERATION: {
                    txPacket->flags |= packets_FLAG_ILLEGAL_OPERATION;
                    break;   
                }
                case CYBLE_ERROR_MEMORY_ALLOCATION_FAILED: {
                    txPacket->flags |= packets_FLAG_MEMORY;
                    break;   
                }
                default: {
                    txPacket->flags |= packets_FLAG_UNKNOWN_ERR;
                    break;
                }
            }
            break;
        }
        /* Read from a characteristic command */
        case packets_CMD_CHAR_READ: {
            /* Make sure the correct size */
            if (rxPacket->payloadLen != (CYBLE_GAP_BD_ADDR_SIZE +1)) {
                txPacket->flags |= packets_FLAG_INVALID_ARGS;
                break;
            }
            /* Pass the device ID and handle back */
            memcpy(txPacket->payload, rxPacket->payload, CYBLE_GAP_BD_ADDR_SIZE + 1);
            txPacket->payloadLen = CYBLE_GAP_BD_ADDR_SIZE +1;
            /* Extract the handle */
            uint16_t charHandle =  rxPacket->payload[CYBLE_GAP_BD_ADDR_SIZE];
            CYBLE_GATTC_READ_REQ_T readReq = charHandle;
            CYBLE_API_RESULT_T result = CyBle_GattcReadCharacteristicValue(*getBleHandle(), readReq);
            switch(result) {
                case CYBLE_ERROR_OK:{
                    break;
                }
                case CYBLE_ERROR_INVALID_PARAMETER: {
                    txPacket->flags |= packets_FLAG_INVALID_ARGS;
                    break;   
                }
                case CYBLE_ERROR_INVALID_OPERATION: {
                    txPacket->flags |= packets_FLAG_ILLEGAL_OPERATION;
                    break;   
                }
                case CYBLE_ERROR_MEMORY_ALLOCATION_FAILED: {
                    txPacket->flags |= packets_FLAG_MEMORY;
                    break;   
                }
                default: {
                    txPacket->flags |= packets_FLAG_UNKNOWN_ERR;
                    break;
                }
            }
            break;   
        }
        /* Change mode (bootloader) command*/
        case packets_CMD_MODE: {
             /* Make sure the correct size, and only bootloader command */
            if ((rxPacket->payloadLen != 1) || (rxPacket->payload[0] != packets_MODE_BOOTLOAD)) {
                /* Command is NACK, but keep for posterity */
                txPacket->flags |= packets_FLAG_INVALID_ARGS;
                break;
            }
            /* Enter bootload mode - This is will never return, device is reset*/
            Bootloadable_Load();
            break;
        }
        /* Command not found */
        default:{
            /* Set the invalid command flag */
            txPacket->flags |= packets_FLAG_INVALID_CMD;
        }
    }
    
    return packets_ERR_SUCCESS;
}

/*******************************************************************************
* Function Name: ackHandler_print()
****************************************************************************//**
* \brief
*  Support cube handler for received acknowledge packet. Currently just toggles
*   the blue led.
*
* \param packet [in]
*  Pointer to the received packet
* 
* \return
*  Returns the error of associated with the operation
*******************************************************************************/
uint32_t ackHandler_supportCube(packets_PACKET_S* packet){
    (void) packet;
    LEDS_B_Toggle();
    return packets_ERR_SUCCESS;
}

/* ### Helpers ### */

/*******************************************************************************
* Function Name: cmdHandler_print()
****************************************************************************//**
* \brief
*  Prints a Received Command packet
*
* \param cmdPacket [in]
*  Pointer to the received packet
*
* \param ackPacket [out]
*  Pointer to the response packet to send out
*
* \return
*  Returns the error of associated with the operation
*******************************************************************************/
uint32_t cmdHandler_print(packets_PACKET_S* cmdPacket, packets_PACKET_S* ackPacket){
    (void) ackPacket;
    usbUart_print("Packet Command: 0x%x\r\n", cmdPacket->cmd);
    return packets_ERR_SUCCESS;
}
/*******************************************************************************
* Function Name: ackHandler_print()
****************************************************************************//**
* \brief
*  Prints a Received Command packet
*
* \param packet [in]
*  Pointer to the received packet
* 
* \return
*  Returns the error of associated with the operation
*******************************************************************************/
uint32_t ackHandler_print(packets_PACKET_S* packet){
    usbUart_print("Packet Acknowledged: 0x%x\r\n", packet->cmd);
    return packets_ERR_SUCCESS;
}

/*******************************************************************************
* Function Name: cmdHandler_noop()
****************************************************************************//**
* \brief
*  No Operation command handler
*
* \param cmdPacket [in]
*  Pointer to the received packet
*
* \param ackPacket [out]
*  Pointer to the response packet to send out
*
* \return
*  Returns the error of associated with the operation
*******************************************************************************/
uint32_t cmdHandler_noop(packets_PACKET_S* cmdPacket, packets_PACKET_S* ackPacket){
    (void) cmdPacket;
    (void) ackPacket;
    return packets_ERR_SUCCESS;
}
/*******************************************************************************
* Function Name: ackHandler_noop()
****************************************************************************//**
* \brief
*  No Operation ack handler
*
* \param packet [in]
*  Pointer to the received packet
* 
* \return
*  Returns the error of associated with the operation
*******************************************************************************/
uint32_t ackHandler_noop(packets_PACKET_S* packet){
    (void) packet;
    return packets_ERR_SUCCESS;
}



/* [] END OF FILE */
