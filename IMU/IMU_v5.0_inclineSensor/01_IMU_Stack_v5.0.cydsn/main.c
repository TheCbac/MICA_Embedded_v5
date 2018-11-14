/***************************************************************************
*                                       MICA
* File: main.c
* Workspace: IMU_v5.0
* Project Name: 01_IMU_Stack_v5.0
* Version: v5.0
* Author: Craig Cheney
* 
* PCB: MICA_IMU_v3.5.2
* PSoC: CYBLE-214015-01 (EZBLE, 4.2, 256k)
* Sensors: BOSCH IMU (BMX055)
*
* Brief:
*   Upgradable stack appliation for Over the air functionality. This program
*   contains the BLE portion of the code for executing both a stack update, as
*   well as the application update. Enable or disable UART debugging in debug.h.
* 
* 2017.07.31 CC - Document Created
* 2018.03.01 CC - Upgraded to v5.0, porting to MICA components, not libMICA
********************************************************************************/
#include "project.h"
#include "debug.h"
#include "stackBle.h"

/*******************************************************************************
* Function Name: main()
********************************************************************************
* Summary:
*   The top-level application function for the project. Turns on LED_B for
*   debugging, and prints debugging info if 'MICA_DEBUG' is defined in debug.h.
*   Main then check to see if a new user application has been loaded, and if so,
*   the new app immediately. If no app is present, then the BLE component is started
*   and awaits bootloading data.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
int main(void) {
    LEDS_Write(LEDS_ON_GREEN);
    /* Print the header about the application */
    PrintHeader();
    /* Enable & Assign interrupts */
    CyGlobalIntEnable; 
    /* Check to see if the Stack project was updated */
    OTA_AfterStackImageUpdate();
    /* See if there is a new user app. If there is boot it immediately */
    OTA_BootUserApp();
    /* No valid user app, Initialize BLE Bootloader */
    initializeBLE(bleOtaCallback);
    /* Start bootloader in a non-blocking manner */
    loader_Initialize();
    
    /* Infinite Loop */
    for(;;)
    {
        /* Mandatory - Process the BLE stack events */
        CyBle_ProcessEvents();
        /* Check if data is being passed in from the BLE component */
        loader_HostLink(TIMEOUT_50_MS);
    }
}



/* [] END OF FILE */
