/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: main.c
* Workspace: IMU_v5.0
* Project: dev_basicUartComm_v5.0
* Version: v5.0.0
* Authors: Craig Cheney
* 
* PCB: MICA IMU v3.5.2
* PSoC: CYBLE214015-01
*
* Brief:
*   This is the top-level application file for dev work towards
*   a self-balancing DriveBot. The goal of the project is to establish basic 
*   UART comms with the DriveBot mcu, first with simple UART, then with MICA Packets.
*
* 2018.04.25 CC - Document Created
********************************************************************************/
#include "project.h"
#include "math.h"
/*  -------------- DEBUGGING --------------
* Uncomment MICA_DEBUG to enable general debugging.
* Uncomment MICA_DEBUG_<case> below to
* enable ONE of the debug levels.
*/
//#define MICA_DEBUG

/* -------------- DEBUG LEVEL --------------
* Uncomment ONE of the following
* Debugging will only occur when MICA_DEBUG is defined
*/
//#define MICA_DEBUG_LEDS           /* Test the onboard RGB LED */
//#define MICA_DEBUG_UART_TX        /* Send data through the UART to test connectivity */
//#define MICA_DEBUG_ECHO           /* Echo to the USB UART */
#define MICA_DEBUG_IMU          /* Test for the IMU */
//#define MICA_DEBUG_FULL           /* Run a composite set of debugging functions */

/* -------------- END DEBUG CONFIG --------------  */


/* --------- Local Variables --------- */


/*******************************************************************************
* Function Name: main()
********************************************************************************
* Summary:
*   The top-level application function for the project.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
int main(void)
{
    /* Enable global interrupts. */
    CyGlobalIntEnable;
    
    /* %%%%%%%%%%%%%%%%%% Begin Debugging %%%%%%%%%%%%%%%%%% */
#ifdef MICA_DEBUG
    #if defined MICA_DEBUG_LEDS
        /* Test the onboard LEDs
        Expeted outcome: 
        0. All LEDs off
        1. Red LED on
        2. Green LED on
        3. Blue LED on
        4. Yellow (RB)
        5. Cyan (BG)
        6. Magenta (RG)
        7. White (RGB) LEDs on
        */
        LEDS_Test(MICA_TEST_INFINITE);
    /* End MICA_DEBUG_LEDS */
    #elif defined MICA_DEBUG_UART_TX
        /* Test the Transmit portion of the UART
        Expected outcome:
        UART constantly writes out the compile date 
        and time. 
        */
        /* Start the UART */
        UART_Start();
        /* Turn on the Green LED */
        LEDS_Write(LEDS_ON_GREEN);
        /* Run the transmit test indefinitely */
        for(;;){
            /* Log, if eneabled */
            uartApi_log("a");
            /* Slow Down */
            CyDelay(MICA_DELAY_MS_SEC_HALF);
            /* Toggle Blue LED */
            LEDS_B_Toggle();
        }
    #elif defined MICA_DEBUG_IMU
        /* Test for the IMU */
        /* Expected outcome:
        * 1. G LED on
        * 2. Delay (0.5 s)
        * 3. Check IMU device ID
        * 4a. ID Correct - White LED
        * 4b. ID Incorrect - Red LED
        * 4c. Other Error - blue LED
        * 5. Delay (1 s)
        */
        /* Start Components */
        I2C_Start();
        UART_Start();
//        BMX055_Start();
//        LEDS_Write(LEDS_ON_GREEN);   
        
//        uint8 whoAmI = readI2CReg(BMX055_ACC_ADDR, BMX055_ACC_CHIPID_REG);
//        if(whoAmI == BMX055_ACC_CHIPID_VAL){
//            LEDS_Write(LEDS_ON_RED);   
//        } else {
//            LEDS_Write(LEDS_ON_BLUE);   
//            
//        }
//        for(;;) {
//            uartApi("WhoAmI: 0x%x\r\n", whoAmI);
//            MICA_delayMs(MICA_DELAY_MS_SEC_ONE);
//        }
        for(;;){
            /* Initial State */
            LEDS_Write(LEDS_ON_GREEN);
            /* Delay for half second */
            MICA_delayMs(MICA_DELAY_MS_SEC_HALF);
            /* Register an "Other Error" (I2C) in case of hang */
            LEDS_Write(LEDS_ON_BLUE);
            /* Check IMU id value */
            uint32 i2cError;
            uint32 result = BMX055_Test(&i2cError);
            /* Check result */
            if(result == BMX055_ERR_OK) {
                uartApi("BMX ID Valid\r\n");
                /* Success - White LEDS */
                LEDS_Write(LEDS_ON_WHITE);
            } else if (result == BMX055_ERR_WHOAMI) {
                uartApi("BMX ID Invalid\r\n");
                /* ID value doesn't match */
                LEDS_Write(LEDS_ON_RED);
            } else if (result == BMX055_ERR_I2C){
                /* Other Error (I2C) - Redudant, added for clarity  */
                LEDS_Write(LEDS_ON_BLUE);
                /* Write out the Error over the UART */
                uartApi("I2C Error: 0x%x\r\n", i2cError);
            } else {
                LEDS_Write(LEDS_ON_MAGENTA);
                uartApi("Other Error 0x%x", result);   
            }
            /* Delay with error led for 1 second */
            MICA_delayMs(MICA_DELAY_MS_SEC_ONE);
        }
    /* End MICA_DEBUG_IMU */
    #else
        #error "Exactly ONE MICA_DEBUG_<case> must be defined if MICA_DEBUG is defined"
    #endif /* MICA_DEBUG_<case> */
#endif /* MICA_DEBUG */
/* %%%%%%%%%%%%%%%%%%  End Debugging  %%%%%%%%%%%%%%%%%% */
    
    I2C_Start();
    UART_Start();
    
    uint16 theta;
    uint16 accData[4]; 
    uint16 gyrData[4];
    uint8 channels = BMX055_CHANNEL_MASK_X  | BMX055_CHANNEL_MASK_Y | BMX055_CHANNEL_MASK_Z ;
    
    LEDS_Write(LEDS_ON_GREEN);
    /* Infinite Loop */
    for(;;)
    {
        
        
        /* ACCELEROMETER */
        /* Read the data from the accelrometers */
        BMX055_Acc_Read(accData, channels);
        
        /* Get the X value of the accelerometer */
        uint16 xVal = accData[BMX055_CHANNEL_INDEX_DATA + BMX055_CHANNEL_INDEX_X];
        uint8 xMsb = MASK_BYTE_ONE & (xVal >> BITS_ONE_BYTE);
        uint8 xLsb = MASK_BYTE_ONE & xVal;
        /* Get the Y value of the accelerometer */
        uint16 yVal = accData[BMX055_CHANNEL_INDEX_DATA + BMX055_CHANNEL_INDEX_Y];
        uint8 yMsb = MASK_BYTE_ONE & (yVal >> BITS_ONE_BYTE);
        uint8 yLsb = MASK_BYTE_ONE & yVal;
        /* Get the Z value of the accelerometer */
        uint16 zVal = accData[BMX055_CHANNEL_INDEX_DATA + BMX055_CHANNEL_INDEX_Z];
        uint8 zMsb = MASK_BYTE_ONE & (zVal >> BITS_ONE_BYTE);
        uint8 zLsb = MASK_BYTE_ONE & zVal;
        
        /* Debug over the UART (if enabled) */
        uartApi_log("X:0x%x%x Y:0x%x%x Z:0x%x%x\r\n", xMsb, xLsb, yMsb, yLsb, zMsb, zLsb);
        /* write out the values */
        UART_UartPutChar(xMsb);
        UART_UartPutChar(xLsb);
        UART_UartPutChar(yMsb);
        UART_UartPutChar(yLsb);
        UART_UartPutChar(zMsb);
        UART_UartPutChar(zLsb);
        
        
        /* delay set rate to 50 Hz */
        MICA_delayMs(1000);
        /* Toggle Red LED */
        LEDS_R_Toggle();
    }
}

/* [] END OF FILE */
