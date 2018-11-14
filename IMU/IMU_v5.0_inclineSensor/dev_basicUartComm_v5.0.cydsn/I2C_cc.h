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
/* Header Guard */
#ifndef I2C_cc_H
    #define I2C_cc_H
    /***************************************
    * Included source files
    ***************************************/
    #include "project.h"
        
    /***************************************
    * Macro definitions 
    ***************************************/
    #define I2C_READ        (0x01u)
    #define I2C_WRITE       (0x00u)
    #define PULLUP_ENABLE   (0x00u)
    #define PULLUP_DISABLE  (0x01u)

    /***************************************
    * Function declarations 
    ***************************************/
    uint8 readI2CReg (uint8 deviceAddr, uint8 registerAddr);
    void writeI2CReg (uint8 deviceAddr, uint8 registerAddr, uint8 registerData);

    /***************************************
    * Interrupt Prototype declaration
    ***************************************/

#endif /*I2C_H*/

/* [] END OF FILE */
