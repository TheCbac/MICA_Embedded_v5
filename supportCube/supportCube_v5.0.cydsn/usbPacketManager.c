/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: usbPacketManager.c
* Workspace: supportCube_v5
* Project: supportCube_v5.0
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: supportCube v2.1.1
* PSoC: CYBLE-214015-01
*
* Brief:
*   Interface for sending and receiving packets over the USB UART.
*
* 2018.10.19  - Document Created
********************************************************************************/
#include "usbPacketManager.h"
#include "supportCommands.h"

/* USB Packet instance */
packets_BUFFER_FULL_S usbPackets;

/*******************************************************************************
* Function Name: usbPackets_init()
****************************************************************************//**
* \brief
*  Initialize the USB packet manager
*
* \return
*  The error associated with the initialization
*******************************************************************************/
uint32_t usbPackets_init(void) {
    uint32_t err = packets_initialize(&usbPackets);
    if(!err) {
//        err = packets_generateBuffers(&usbPackets, packets_LEN_PACKET_128);
        err = packets_generateBuffers(&usbPackets, 512);
    }
    /* Register callback functions */
    if(!err){
        usbPackets.comms.rxGetBytesPending = usbUart_getRxBufferSize;
        usbPackets.comms.rxReadByte = usbUart_getChar;
        usbPackets.comms.txPutArray = usbUart_putArray;
        usbPackets.comms.ackCallback = ackHandler_supportCube;
        usbPackets.comms.cmdCallback = cmdHandler_supportCube;
    }
    return err;
}

/*******************************************************************************
* Function Name: usbPackets_processIncoming()
****************************************************************************//**
* \brief
*  Processes the incoming RX buffer. Packets are handled by callback registered
*   during initialization.
*
* \return
*  The error associated with the processing
*******************************************************************************/
uint32_t usbPackets_processIncoming(void) {
    return packets_processRxQueue(&usbPackets);
}

/*******************************************************************************
* Function Name: usbPackets_sendPacket()
****************************************************************************//**
* \brief
*  Queues a packet to send out over the usbUart
*
* \param cmd
*   The command value to write
*
* \param payloadLen
*   Length of the payload
* 
* \param payload
*   Pointer to the payload
*
* \param flags
*   The flags to include
*
* \return
*  The error associated with the processing
*******************************************************************************/
uint32_t usbPackets_sendPacket(uint8_t cmd, uint16_t payloadLen, uint8_t *payload, uint16_t flags) {
    usbPackets.send.packet.cmd = cmd;
    usbPackets.send.packet.payloadLen = payloadLen;
    usbPackets.send.packet.flags = flags;
    if(payloadLen) {
        memcpy(usbPackets.send.packet.payload, payload, payloadLen);
    }
    return packets_sendPacket(&usbPackets);
}


/*******************************************************************************
* Function Name: changeBase()
****************************************************************************//**
*
* \brief Changes the base to hex from decimal
*
* \param Index
*  new value
*
* \return
* None
 *******************************************************************************/
static inline uint8* changeBase(uint32 Index)
{
    return (uint8*)("0123456789ABCDEF"+Index);
}

/*******************************************************************************
* Function Name: usbPackets_log()
****************************************************************************//**
* \brief
*  Send a log packet out
*
* \param msg
*   String to write out
*
* \return
*  The error associated with the processing
*******************************************************************************/
uint32_t usbPackets_log(char *msg, ...){
    usbPackets.send.packet.cmd = packets_RSP_LOG;
    usbPackets.send.packet.flags = packets_FLAG_NONE;
    uint8_t i = ZERO;
    uint32_t xVal, buffer[12], index = 1, j;
    uint32_t *pArg;
    pArg = (uint32_t *) &msg;
    
    while(*msg){
        /* None formatted chars */
        if(*msg != '%')  {
            usbPackets.send.packet.payload[i++] = *msg;   
            msg++;
            continue;
        }
        /* Move to char after '%' */
        msg++;
        /* Format hex */
        if(*msg == 'x') {
            /* Get the input val */
            xVal = pArg[index++];
            j=0;
            do {
                buffer[j++] = xVal % 16;
                xVal /= 16;
            } while(xVal);
            /* Pad */
            if(j%2 !=0 || j < 2) {
                buffer[j++] = 0;   
            }
            while( j > 0) {
                j--;
                usbPackets.send.packet.payload[i++] = *changeBase(buffer[j]);
            }
            msg++;
        }
        
        if(msg == '\0'){break;}
    }
    usbPackets.send.packet.payloadLen = i;
    return packets_sendPacket(&usbPackets);
}



/* [] END OF FILE */
