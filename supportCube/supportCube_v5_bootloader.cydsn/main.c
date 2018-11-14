/***************************************************************************
*                                 MICA Embedded
*                              Craig Cheney © 2018
*                                ccheney@mit.edu
*                           MIT BioInstrumentation Lab
*                                       
* File: main.c
* Workspace: SupportCube_v5
* Project: supportCube_v5_bootloader
* Version: v5.0.0
* Author: Craig Cheney
* 
* PCB: MICA Support v2.1.0
* PSoC: EZBLE-PSoC - CYBLE-215009-01 (BLE 4.2, 256k)
* USB-Serial Bridge: CY7C65211-24LTXI
*
* Brief:
*   main.c is the top most, application level, file for the supportBootloader_v4.0 project. 
*   This project is responsible for loading a new bootloadable project into device memory
*   via the usbUART (UART connected the USB-Serial Chip via the USB-C plug).
*    
* Changelog:
* 2018.08.24 CC - Updated to v5, switch to MICA components
* 2017.08.09 CC - v4.0 created
*   
********************************************************************************/
#include "project.h"

/*******************************************************************************
* Function Name: main()
********************************************************************************
* Summary:
*   The top-level application function for the project. Enables the bootloader
*   component, which waits for data from the usbUart.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
int main(void){
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    /* Start the UART */
    UART_USB_Start();
    /* Turn on the RED Led */
    LEDS_Write(LEDS_ON_RED);
    /* Wait for bootloader data. This will never return */
    Bootloader_Start();
    /* Infinite Loop */
    for(;;){}
}

/* [] END OF FILE */
