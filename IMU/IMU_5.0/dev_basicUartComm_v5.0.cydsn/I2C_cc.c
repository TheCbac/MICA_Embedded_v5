/***************************************************************************
*                                       lib_cc
* File: I2C_cc.h
* Project Name: lib_CC
* Version: 0.2
* Author: Craig Cheney
* 
* PCB: N/A
* Sensors: N/A
*
* Brief:
*   API for I2C components 
* 
* Date Written: 3/8/16
********************************************************************************/

#include "I2C_cc.h"

/*******************************************************************************
* Function Name: writeI2CReg
********************************************************************************
* Summary:
*        Writes a byte to data to a given register of the target I2C device 
*
* Params: 
*       deviceAddr - device I2C address
*       registerAddr - address of register to write to
*       registerData - byte of data to be written to the register
*   
*
* Returns
*       void
*
*******************************************************************************/
void writeI2CReg (uint8 deviceAddr, uint8 registerAddr, uint8 registerData){
    // ST & SAD + W
    
    I2C_I2CMasterSendStart(deviceAddr, I2C_WRITE);
    // SUB
    I2C_I2CMasterWriteByte(registerAddr);
    // DATA
    I2C_I2CMasterWriteByte(registerData);
    // SP
    I2C_I2CMasterSendStop();
    
}


/*******************************************************************************
* Function Name: readI2CReg
********************************************************************************
* Summary:
*        Reads a given register of the target I2C device and returns that value 
*
* Params: 
*       deviceAddr - device I2C address
*       registerAddr - address of register to write to
*   
*
* Returns
*       value of register
*
*******************************************************************************/
uint8 readI2CReg (uint8 deviceAddr, uint8 registerAddr){
    // ST & SAD + W
    I2C_I2CMasterSendStart(deviceAddr, I2C_I2C_WRITE_XFER_MODE);
    // SUB
    I2C_I2CMasterWriteByte(registerAddr);
    // SR & SAD + R
    I2C_I2CMasterSendRestart(deviceAddr, I2C_I2C_READ_XFER_MODE);
    // READ
    uint8 data = I2C_I2CMasterReadByte(I2C_I2C_NAK_DATA);
    // SP
    I2C_I2CMasterSendStop();
    return data;
}


/* [] END OF FILE */
