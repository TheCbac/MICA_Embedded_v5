/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: supportBleCallback.c
* Workspace: supportCube_v5
* Project: supportCube_v5.0
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: supportCube v1.0.0
* PSoC: CYBLE-214015-01
*
* Brief:
*   BLE Callback application for the Support Cube
*
* 2018.10.19  - Document Created
********************************************************************************/
#include "supportBleCallback.h"
#include "stdlib.h"

/* Store the connecting device ID */
uint8_t connectingDevice[CYBLE_GAP_BD_ADDR_SIZE];
CYBLE_CONN_HANDLE_T bleHandle;
volatile bool pendingConnection = false;

/*******************************************************************************
* Function Name: supportBleHandler()
****************************************************************************//**
* \brief
*  Bluetooth event handler for the support cube
*
* \param eventCode
*  The code indicating which event occured
*
* \param eventParam
*   Pointer to the parameters of the event. User must cast to the approprate type
*
* \return
*  None
*******************************************************************************/
void supportBleHandler(uint32 eventCode, void *eventParam){
    /* Act depending on the event*/
    switch(eventCode){
        /**********************************************************
        *                       General Events
        ***********************************************************/
        /* Stack initialized */
        case CYBLE_EVT_STACK_ON:{
            break;
        }

        /**********************************************************
        *                       GAP Events
        ***********************************************************/
        /* A device was found from the scan */
        case CYBLE_EVT_GAPC_SCAN_PROGRESS_RESULT: {
            CYBLE_GAPC_ADV_REPORT_T *advReport = (CYBLE_GAPC_ADV_REPORT_T *) eventParam;
            /* Process only for advertisement packets, not on scan response packets */
            if(advReport->eventType != CYBLE_GAPC_SCAN_RSP) {
                /* Extract parameters - This is the order they are passed in */
                uint8_t eventType = advReport->eventType;
                uint8_t peerType = advReport->peerAddrType;
                uint8_t *addr = advReport->peerBdAddr;
                uint8_t dataLen = advReport->dataLen;
                uint8_t *data = advReport->data;
                uint8_t rssi = advReport->rssi;
                /* Create Payload */
                uint8_t payload[100];
                uint8_t i = ZERO;
                payload[i++] = eventType;
                payload[i++] = peerType;
                /* BD address */
                memcpy(&payload[i], addr, CYBLE_GAP_BD_ADDR_SIZE);
                i += CYBLE_GAP_BD_ADDR_SIZE;
                /* Length of the advertisement packet */
                payload[i++] = dataLen;
                /* Adv packet */
                memcpy(&payload[i], data, dataLen);
                i += dataLen;
                /* RSSI */
                payload[i++] = rssi;
                /* Send the packet */
                packets_usb_sendPacket(packets_RSP_DEVICE_FOUND, i, payload, packets_FLAG_NONE);
            }
            break;   
        }
        /* The scan was either started or stopped */
        /* If the fast scan times out, it then goes to the slow scan */
        case CYBLE_EVT_GAPC_SCAN_START_STOP: {
            CYBLE_STATE_T state = CyBle_GetState();
             if( state != CYBLE_STATE_SCANNING) {
                /* Indicate that the scan Stopped */
                packets_usb_sendPacket(packets_RSP_SCAN_STOPPED, ZERO, NULL, packets_FLAG_NONE);
                /* See if scan was stopped to connect */
                if(getPendingConnection() && state == CYBLE_STATE_DISCONNECTED){
                    /* Clear the flag */
                    setPendingConnection(false);
                    /* Initiate a connection */
                    CYBLE_GAP_BD_ADDR_T deviceAddr;
                    deviceAddr.type = CYBLE_GAP_ADDR_TYPE_PUBLIC;
                    getConnectingDeviceId(deviceAddr.bdAddr);
                    CYBLE_API_RESULT_T result = CyBle_GapcConnectDevice(&deviceAddr);
                    /* Indicate that the connection failed */
                    if (result != CYBLE_ERROR_OK){
                        uint8_t bdAddr[CYBLE_GAP_BD_ADDR_SIZE];
                        getConnectingDeviceId(bdAddr);
                        packets_usb_log("Error connecting: 0x%x", result);
                        /* Send response packet */
                        packets_usb_sendPacket(packets_RSP_CONNECTION_LOST, CYBLE_GAP_BD_ADDR_SIZE, bdAddr, packets_FLAG_NONE);
                    }
   
                }
            }
            break;    
        }
        /* Connected to the device */
        case CYBLE_EVT_GAP_DEVICE_CONNECTED: {
            /* Initiate an MTU exchange request */
            CyBle_GattcExchangeMtuReq(bleHandle, CYBLE_GATT_MTU);
            break;
        }
        /* The peer device responded to the MTU request */
        case CYBLE_EVT_GATTC_XCHNG_MTU_RSP:{
            /* indicate the command was a sucess */
            uint8_t bdAddr[CYBLE_GAP_BD_ADDR_SIZE];
            getConnectingDeviceId(bdAddr);
            /* Send response packet */
            packets_usb_sendPacket(packets_RSP_CONNECTED, CYBLE_GAP_BD_ADDR_SIZE, bdAddr, packets_FLAG_NONE);            
            break;   
        }
        /* Store the BLE handle */
        case CYBLE_EVT_GATT_CONNECT_IND: {
            memcpy(&bleHandle, eventParam, sizeof(CYBLE_CONN_HANDLE_T));
            break;
        }
        /* Disover complete */
        case CYBLE_EVT_GATTC_DISCOVERY_COMPLETE: {
            packets_usb_log("Discovery completed");
            break;   
        }
        
        case CYBLE_EVT_GATTC_LONG_PROCEDURE_END: {
            LEDS_Write(LEDS_ON_WHITE);   
        }
        /* A device was disconnected */
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:{
            uint8_t *disconnectReason = (uint8_t *) eventParam;
            /* If user directed */
            if(*disconnectReason == CYBLE_HCI_CONNECTION_TERMINATED_LOCAL_HOST_ERROR){
                /* indicate to the remote device the disconnect*/
                uint8_t bdAddr[CYBLE_GAP_BD_ADDR_SIZE];
                getConnectingDeviceId(bdAddr);
                /* Send response packet */
                packets_usb_sendPacket(packets_RSP_DISCONNECTED, CYBLE_GAP_BD_ADDR_SIZE, bdAddr, packets_FLAG_NONE);
            } else {
                /* indicate to the remote device the disconnect*/
                uint8_t bdAddr[CYBLE_GAP_BD_ADDR_SIZE];
                getConnectingDeviceId(bdAddr);
                /* Send response packet */
                packets_usb_sendPacket(packets_RSP_CONNECTION_LOST, CYBLE_GAP_BD_ADDR_SIZE, bdAddr, packets_FLAG_NONE);
            }
            break;   
        }
        /* Response to ther read request */
        case CYBLE_EVT_GATTC_READ_RSP: {
            CYBLE_GATTC_READ_RSP_PARAM_T *readRsp = (CYBLE_GATTC_READ_RSP_PARAM_T *) eventParam;
            uint16_t dataLen = readRsp->value.len;
            uint8_t charHandle = readRsp->connHandle.bdHandle;
            uint16_t bufferLen =  CYBLE_GAP_BD_ADDR_SIZE+ sizeof(charHandle) + sizeof(dataLen) + dataLen;
            uint8_t * outBuffer = malloc(bufferLen);
            if (outBuffer != NULL) {
                packets_usb_log("Read successful");
                /* Pack the buffer */
                /* Device ID */
                getConnectingDeviceId(outBuffer);
                uint16_t i = CYBLE_GAP_BD_ADDR_SIZE;
                /* Characteristic handle  */
                outBuffer[i++] = charHandle;
                /* data length */
                outBuffer[i++] = (dataLen >> BITS_ONE_BYTE) & MASK_BYTE_ONE;
                outBuffer[i++] = (dataLen & MASK_BYTE_ONE);
                /* Data */
                memcpy(&outBuffer[i], readRsp->value.val, dataLen);
                /* Send response packet */
                packets_usb_sendPacket(packets_RSP_READ, bufferLen, outBuffer, packets_FLAG_NONE);
                /* Destroy buffer */
                free(outBuffer);
                /* failed to create the buffer, send everything but the data */
            } else {
                packets_usb_log("Read failed");
                uint16_t failedLen = CYBLE_GAP_BD_ADDR_SIZE+ sizeof(charHandle) + sizeof(dataLen);
                uint8_t failedBuffer[failedLen];
                getConnectingDeviceId(outBuffer);
                uint16_t i = CYBLE_GAP_BD_ADDR_SIZE;
                /* Characteristic handle  */
                outBuffer[i++] = charHandle;
                /* data length */
                outBuffer[i++] = (dataLen >> BITS_ONE_BYTE) & MASK_BYTE_ONE;
                outBuffer[i++] = (dataLen & MASK_BYTE_ONE);
                /* Send out the data */
                packets_usb_sendPacket(packets_RSP_READ, failedLen, failedBuffer, packets_FLAG_MEMORY);
                
            }
            break;
        }
        /* Notification data received from server device */
        case CYBLE_EVT_GATTC_HANDLE_VALUE_NTF: {
            CYBLE_GATTC_HANDLE_VALUE_NTF_PARAM_T * notification = (CYBLE_GATTC_HANDLE_VALUE_NTF_PARAM_T *) eventParam;
            /* Unpack data */
            uint8_t charHandle = notification->handleValPair.attrHandle;
            uint16_t dataLen = notification->handleValPair.value.len;
            uint16_t bufferLen = CYBLE_GAP_BD_ADDR_SIZE + sizeof(charHandle) + sizeof(dataLen) + dataLen;
            /* Create the buffer */
            uint8_t *outBuffer = malloc(bufferLen);
            if(outBuffer != NULL) {
                /* Pack the buffer */
                /* Device ID */
                getConnectingDeviceId(outBuffer);
                uint16_t i = CYBLE_GAP_BD_ADDR_SIZE;
                /* Characteristic handle */
                outBuffer[i++] = charHandle;
                /* data length */
                outBuffer[i++] = (dataLen >> BITS_ONE_BYTE) & MASK_BYTE_ONE;
                outBuffer[i++] = (dataLen & MASK_BYTE_ONE);
                /* Data */
                memcpy(&outBuffer[i], notification->handleValPair.value.val, dataLen);
                /* Send response packet */
                uint32_t err = packets_usb_sendPacket(packets_RSP_NOTIFY, bufferLen, outBuffer, packets_FLAG_NONE);
                if(err) {
                    packets_usb_log("Notify send err: 0x%x", err);      
                }
                /* Destroy buffer */
                free(outBuffer);
                /* failed to create the buffer, send everything but the data */
            } else {
                packets_usb_log("Notify buffer err");      
            }
            break;   
        }
        /* An unhandled event occured */
        default:{
            break;   
        }
    } /* End switch(eventCode) */
}

/*******************************************************************************
* Function Name: setConnectingDeviceId()
****************************************************************************//**
* \brief
*  Store the device ID of the peer device attempting to connect to
*
* \param bdAddr [in]
*  Pointer to the bdAddr location
*
* \return
*  None
*******************************************************************************/
void setConnectingDeviceId(uint8_t *bdAddr){
    memcpy(connectingDevice, bdAddr, CYBLE_GAP_BD_ADDR_SIZE);
}

    
/*******************************************************************************
* Function Name: getConnectingDeviceId()
****************************************************************************//**
* \brief
*  Retrieve the deviceID of the connecting device
*
* \param bdAddr [out]
*  Pointer to the location to place the address into
*
* \return
*  None
*******************************************************************************/
void getConnectingDeviceId(uint8_t *bdAddr){
    memcpy(bdAddr, connectingDevice, CYBLE_GAP_BD_ADDR_SIZE);   
}   

/*******************************************************************************
* Function Name: setPendingConnection()
****************************************************************************//**
* \brief
*  Sets a flag to determine if the scan was stopped because of a desired connection
*
* \param newState [in]
*  Value to set the flag to
*
* \return
*  None
*******************************************************************************/
void setPendingConnection(bool newState) {
    pendingConnection = newState;   
}

/*******************************************************************************
* Function Name: getPendingConnection()
****************************************************************************//**
* \brief
*  Get the value of the flag. Used to determine if a connection should be initiated
*   once the stop scan is received
*
* \return
*  Value of the flag
*******************************************************************************/
bool getPendingConnection(void) {
    return pendingConnection;   
}

/*******************************************************************************
* Function Name: getBleHandle()
****************************************************************************//**
* \brief
*  Returns a pointer to the BLE handle
*
* \return
*  Pointer to the BLE handle
*******************************************************************************/
CYBLE_CONN_HANDLE_T *getBleHandle(void){
    return &bleHandle;   
}



/* [] END OF FILE */
