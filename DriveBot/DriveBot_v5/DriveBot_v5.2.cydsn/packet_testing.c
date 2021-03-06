/***************************************************************************
*                                       MICA
* File: packet_testing.c
* Workspace: micaComponents
* Project Name: libMica
* Version: v1.0
* Author: Craig Cheney
*
* Brief:
*  Unit tests for the packets
* 
* Authors:
*   Craig Cheney
*
* Change Log:
*   2018.08.03 CC - Document created
********************************************************************************/
#include "packet_testing.h"
#include "testRunner.h"
#include <stdio.h>

/*******************************************************************************
* Function Name: test_generateBuffers()
****************************************************************************//**
* \brief
*  Runner that assists with unit tests of generating buffers
*
* \param packetBuffer
*   Pointer to buffers
*
* \param dataArray
*   buffer that contains the data
*
* \param dataLen
*   The length of data to send
*
* \param testName
*   Name of test
*
* \param expectedResult
*   The Error code of the expected result
*
* \return
*  Boolean indicating if the test passed
*******************************************************************************/  
bool test_generateBuffers(packets_BUFFER_FULL_S* packetBuffer, char* testName, uint16 len, uint32_t expectedResult){
    /* Blocks too large are not created */
    uint32_t error = packets_generateBuffers(packetBuffer, len);
    /* Print results */
    bool result = printTestResults(testName, error, expectedResult, "");
    /* Destroy buffers */
    if(!error){
        packets_destoryBuffers(packetBuffer);
    }
    return result;
}

/*******************************************************************************
* Function Name: test_generateBuffers_iteration()
****************************************************************************//**
* \brief
*  Create and destroy buffers repeatedly
*
* \param packetBuffer
*   Pointer to buffers
*
* \param testName
*   Name of test
*
* \param iterations
*  Number of times to run the test
*
* \return
*  Boolean indicating if the test passed
*******************************************************************************/  
bool test_generateBuffers_iteration(packets_BUFFER_FULL_S* packetBuffer, char* testName, uint16 iterations){
    uint16 i;
    /* Create and destroy a whole buch of times */
    for(i = ZERO; i < iterations; i++){
        packets_generateBuffers(packetBuffer, packets_LEN_BLOCK_PACKET);
        packets_destoryBuffers(packetBuffer);
    }
    /* Succeed if we get here */
    return printTestResults(testName, ZERO, ZERO, "");
}



/*******************************************************************************
* Function Name: test_destroyBuffers()
****************************************************************************//**
* \brief
*  Runner that assists with unit tests of generating buffers
*
* \param packetBuffer
*   Pointer to buffers
*
* \param dataArray
*   buffer that contains the data
*
* \param dataLen
*   The length of data to send
*
* \param testName
*   Name of test
*
* \param expectedResult
*   The Error code of the expected result
*
* \return
*   Boolean indicating if the test passed
*******************************************************************************/  
bool test_destroyBuffers(packets_BUFFER_FULL_S* packetBuffer, char* testName, uint32_t expectedResult) {
    uint32_t error = packets_destoryBuffers(packetBuffer);
    /* Print the results */
    return printTestResults(testName, error, expectedResult, "");
}

/*******************************************************************************
* Function Name: test_packetCreation_stateErrors()
****************************************************************************//**
* \brief
*  Test the creation of the packets, evaaluating the state errors
*
* \param packetBuffer
*   Pointer to buffers
*
* \param testName
*   Name of test
*
* \param expectedResult
*   The Error code of the expected result
*
* \return
*   Boolean indicating if the test passed
*******************************************************************************/
bool test_packetCreation_stateErrors(packets_BUFFER_FULL_S* packetBuffer, char* testName, uint32_t expectedResult){
    uint32_t error = packets_constructPacket(packetBuffer);
    /* Print out results */
    bool result = printTestResults(testName, error, expectedResult, "");
    /* Flush the buffer */
    packets_flushTxBuffers(packetBuffer);
    return result;
}

/*******************************************************************************
* Function Name: test_packetCreation_packetVals()
****************************************************************************//**
* \brief
*  Test the creation of the packets, looking at the value of the packet created
*
* \param packetBuffer
*   Pointer to buffers
*
* \param testName
*   Name of test
*
* \param expectedPacket
*   Pointer to array that is evaluated against the created txBuffer packet
*
* \return
*   Boolean indicating if the test passed
*******************************************************************************/
bool test_packetCreation_packetVals(packets_BUFFER_FULL_S* packetBuffer, char* testName, uint8_t* expectedPacket){
    uint32_t error = packets_constructPacket(packetBuffer);
    /* There should be no errors returned from the contructPacket function (even for failing testings) */
    if(error){
        /* Log the error */
        return printTestResults(testName, error, ZERO, "");
    }
    
    /* Compare the buffer arrays */
    uint16_t i;
    packets_BUFFER_PROCESS_S* txBuffer  = &(packetBuffer->send.processBuffer);
    bool match = true;
    for(i = ZERO; i < txBuffer->bufferIndex; i++){
        /* Check to see if the values match */
        match &= expectedPacket[i] == txBuffer->buffer[i];   
        /* Stop if they don't match */
        if(!match){ 
            /* Flush the buffer */
            packets_flushTxBuffers(packetBuffer);
            /* Log an error */
            char msg[100];
            sprintf(msg, "Mismatch at index [%d]: expect: 0x%02x, got: 0x%02x", i, expectedPacket[i], txBuffer->buffer[i]);
            return printTestResults(testName, error, ZERO, msg);
        }
    }
    /* Flush the buffer */
    packets_flushTxBuffers(packetBuffer);
    /* Print out success results */
    return printTestResults(testName, error, ZERO, "");
}

/*******************************************************************************
* Function Name: test_sendPacket()
****************************************************************************//**
* \brief
*  Test the sending fucntion
*
* \param packetBuffer
*   Pointer to buffers
*
* \param testName
*   Name of test
*
* \param expectedPacket
*   Pointer to array that is evaluated against the created txBuffer packet
*
* \return
*   Boolean indicating if the test passed
*******************************************************************************/
bool test_sendPacket(packets_BUFFER_FULL_S* packetBuffer, char* testName, uint32_t expectedResult){
    /* Send the packet */
    uint32 error = packets_sendPacket(packetBuffer);
    /* Display the results */
    return printTestResults(testName, error, expectedResult, "");
}

    
/*******************************************************************************
* Function Name: test_processRxByte_stateError()
****************************************************************************//**
* \brief
*  Runner that assists with unit tests of generating buffers
*
* \param packetBuffer
*   Pointer to buffers
*
* \param bytes
*   Next byte of data
*
* \param testName
*   Name of test
*
* \param expectedResult
*   The Error code of the expected result
*
* \return
*   Boolean indicating if the test passed
*******************************************************************************/
bool test_processRxByte_stateError(packets_BUFFER_FULL_S* packetBuffer, uint8_t * dataArr, uint16 len, char* testName, uint32_t expectedResult){
    uint32_t error = ZERO;
    uint16 i;
    for(i = ZERO; i < len; i++){
        error |= packets_processRxByte(packetBuffer, dataArr[i]);
    }

    /* flush the buffers */
    packets_flushBuffers(packetBuffer);
    /* Record the result */
    return printTestResults(testName, error, expectedResult, "");
    
}

/*******************************************************************************
* Function Name: test_packetParsing_stateErrors()
****************************************************************************//**
* \brief
*  Takes the data in the tx process buffer, passes it to the processRxByte,
*   then parses the data. Evaluate based on returned errors
*
* \param packetBuffer
*   Pointer to buffers
*
* \param testName
*   Name of test
*
* \param expectedResult
*   The Error code of the expected result
*
* \return
*   Boolean indicating if the test passed
*******************************************************************************/
bool test_packetParsing_stateErrors(packets_BUFFER_FULL_S* packetBuffer, char* testName, uint32_t expectedResult){
   /* Reset the RX Buffer */
    packets_flushRxBuffers(packetBuffer);
    /* Send data to txProcessBuffer */
    uint32 error = packets_ERR_SUCCESS;
     /* 'Transfer' the data */
    uint16_t j;
    for(j=ZERO; j < packetBuffer->send.processBuffer.bufferIndex; j++){
        if(!error){
            uint8 dataByte = packetBuffer->send.processBuffer.buffer[j];
            error |= packets_processRxByte(packetBuffer, dataByte);
        }
    }
    if(!error){
        /* Parse the packet */
        error |= packets_parsePacket(packetBuffer);
    }   
    /* Flush TX buffers */
    packets_flushTxBuffers(packetBuffer);
        
    return printTestResults(testName, error, expectedResult, "");
}

/*******************************************************************************
* Function Name: test_packetParsing_packetVals()
****************************************************************************//**
* \brief
*  Takes the data in the tx process buffer, passes it to the processRxByte,
*   then parses the data
*
* \param packetBuffer
*   Pointer to buffers
*
* \param testName
*   Name of test
*
* \param expectedResult
*   The Error code of the expected result
*
* \return
*   Boolean indicating if the test passed
*******************************************************************************/
bool test_packetParsing_packetVals(packets_BUFFER_FULL_S* packetBuffer, char* testName, packets_PACKET_S* expectedPacket){
       /* Reset the RX Buffer */
    packets_flushRxBuffers(packetBuffer);
    /* Send data to txProcessBuffer */
    uint32 error = packets_ERR_SUCCESS;
     /* 'Transfer' the data */
    uint16_t j;
    for(j=ZERO; j < packetBuffer->send.processBuffer.bufferIndex; j++){
        if(!error){
            uint8 dataByte = packetBuffer->send.processBuffer.buffer[j];
            error |= packets_processRxByte(packetBuffer, dataByte);
        }
    }
    if(!error){
        /* Parse the packet */
        error |= packets_parsePacket(packetBuffer);
    }
    
    /* Compare the packets */
    bool match = comparePackets( &(packetBuffer->receive.packet), expectedPacket);
    char msg[20] = "";
    if(!match){
        sprintf(msg, "Packets do not match");
    }
    /* Flush TX buffers */
    packets_flushTxBuffers(packetBuffer);
    return printTestResults(testName, error, ZERO, msg);
}

/*******************************************************************************
* Function Name: comparePacketBuffer()
****************************************************************************//**
* \brief
*  Compares two full buffers
*
* \param p1
*   Pointer to the first buffer
*
* \param p2
*   Pointer to the second buffer
*
* \return
*   Boolean indicating if the buffers match
*******************************************************************************/  
bool comparePacketBuffer(packets_BUFFER_FULL_S* b1, packets_BUFFER_FULL_S* b2){
    bool match = true;
    /* State */
    match &= b1->receive.bufferState == b2->receive.bufferState;
    /* Receive packet and buffer */
    match &= comparePackets(&(b1->receive.packet), &(b2->receive.packet));
    match &= compareProcessBuffer(&(b1->receive.processBuffer), &(b2->receive.processBuffer));
    /* send packet and buffer */
    match &= comparePackets(&(b1->send.packet), &(b2->send.packet));
    match &= compareProcessBuffer(&(b1->send.processBuffer), &(b2->send.processBuffer));

    return match;
}


/*******************************************************************************
* Function Name: comparePackets()
****************************************************************************//**
* \brief
*  Compares two Send or Receive Buffers
*
* \param p1
*   Pointer to the first buffer
*
* \param p2
*   Pointer to the second buffer
*
* \return
*   Boolean indicating if the buffers match
*******************************************************************************/  
bool comparePackets(packets_PACKET_S* p1, packets_PACKET_S* p2){
    bool match = true;

    match &= p1->moduleId == p2->moduleId;
    match &= p1->cmd == p2->cmd;
    match &= p1->payloadMax == p2->payloadMax;
    match &= p1->payloadLen == p2->payloadLen;
    match &= p1->flags == p2->flags;
    match &= p1->error == p2->error;
    /* Compare each element of the payload if everything valid so far */
    if(match){
        uint16 i;
        for(i = ZERO; i < p1->payloadLen; i++){
            match &= p1->payload[i] == p2->payload[i];

        }
    }

    return match;
}

/*******************************************************************************
* Function Name: compareProcessBuffer()
****************************************************************************//**
* \brief
*  Compares two process Buffers
*
* \param p1
*   Pointer to the first buffer
*
* \param p2
*   Pointer to the second buffer
*
* \return
*   Boolean indicating if the buffers match
*******************************************************************************/  
bool compareProcessBuffer(packets_BUFFER_PROCESS_S* p1, packets_BUFFER_PROCESS_S* p2) {
    bool match = true;

    match &= p1->bufferLen == p2->bufferLen;
    match &= p1->bufferIndex == p2->bufferIndex;
    match &= p1->timeCount == p2->timeCount;

    uint16 i;
    for(i = ZERO; i < p1->bufferLen; i++){
        match &= p1->buffer[i] == p2->buffer[i];

    }

    return match;
}

/* [] END OF FILE */
