/***************************************************************************
*                                       MICA
* File: main.c
* Workspace: IMU_v5.0
* Project Name: 00_IMU_Launcher_v5.0
* Version: v5.0
* Author: Craig Cheney
* 
* PCB: MICA_IMU_v3.5.2
* PSoC: CYBLE-214015-01 (EZBLE, 4.2, 256k)
* Sensors: BOSCH IMU (BMX055)
*
* Brief:
*   Launcher project for Over the Air Updates. This project will be included
*   in with the upgradable stack project. The Heap has been increased to 
*   512 bytes (0x200). 
* 
* Authors:
*   Craig Cheney
*
* 2018.03.01 CC - Document upgrade to v5.0
********************************************************************************/
#include "project.h"

#if CYDEV_FLASH_SIZE != 0x00040000u
    #error "This design is specifically targeted for parts with 256k of flash"
#endif /* CYDEV_FLASH_SIZE */

/*******************************************************************************
* Function Name: main()
********************************************************************************
* Summary:
*   The top-level application function for the project. Starts the bootloader
*   component in launcher + copier mode. 
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
int main(void) {   
    /* Turn on the Cyan LEDS */
    LEDS_Write(LEDS_ON_CYAN);
    /*  Start the bootloader in Launcher+Copier mode - inifinite loop, will never return */
    launcher_Start();       
    /* Dummy infinite loop */
    for(;;){/* Should never be called */}
}

/* [] END OF FILE */