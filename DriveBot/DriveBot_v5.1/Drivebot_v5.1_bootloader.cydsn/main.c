/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: main.c
* Workspace: DriveBot_v5.1_bootloader
* Project: DriveBot_v5.1.0
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: PSoC 4 MCU 5.3.0
* PSoC: CY8C4245LQI-483
*
* Brief:
*   This is the bootloader application for the drivebot PSoC 4 MCU
*
* 2018.11.15  - Document Created
********************************************************************************/
#include <stdbool.h>
#include "project.h"


/* ####################### BEGIN PROGRAM CONFIGURATION ###################### */

//#define MICA_DEBUG
/* ---------------- DEBUG CASE ----------------
* Uncomment ONE of the following
* Debugging will only occur when MICA_DEBUG is defined
*/
#ifdef MICA_DEBUG
    #define MICA_DEBUG_LEDS     /* Run through the testings of the LEDS */
#endif
/* -------------- END DEBUG CASE --------------  */
   

//#define MICA_TEST
/* ---------------- TEST CASE -----------------
* Uncomment ONE of the following
* Testing will only occur when MICA_TEST is defined
*/
#ifdef MICA_TEST
    #define MICA_TEST_case      /* Test Description */
#endif
/* -------------- END TEST CASE ---------------  */


/* ############################# BEGIN PROGRAM ############################## */
/* Interrupt Service Routines */
void ISR_isrName(void);

/* Global Variables */
volatile bool flag_isrFlag = false;

/* Main Program */
#if !defined(MICA_DEBUG) && !defined(MICA_TEST)
/*******************************************************************************
* Function Name: main()
********************************************************************************
* Summary:
*    Enables the bootloader component, which waits for data from the usbUart.
*
*******************************************************************************/
int main(void)
{
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    /* Start the USB uart */
    usbUart_Start();
    /* Turn on the Red LED */
    LEDS_Write(LEDS_ON_RED);
    /* Wait for bootloader data - This will never return */
    usbBootloader_Start();
    /* Infinite Loop */
    for(;;) {
        
    }
}
#endif /* !defined(MICA_DEBUG) && !defined(MICA_TEST) */
/* End Main Program */

/* ############################ BEGIN DEBUGGER ############################## */
#ifdef MICA_DEBUG
/*******************************************************************************
* DEBUGGER: main()
********************************************************************************
* Summary:
*   Debugging function for the PCB hardware
*******************************************************************************/
int main(void){
    #warning "MICA_DEBUG is enabled"
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    #ifdef MICA_DEBUG_LEDS
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
        /* Infinite loop */
        LEDS_Test(MICA_TEST_INFINITE);
    /* End MICA_DEBUG_LEDS */
    #else
        #error "At least ONE MICA_DEBUG_<case> must be defined if MICA_DEBUG is defined"
    #endif /* End MICA_DEBUG_<case> */
    /* Infinite loop */
    for(;;){}
}
#endif /* MICA_DEBUG */

/* ############################# BEGIN TESTER ############################### */
#ifdef MICA_TEST
/*******************************************************************************
* TESTER: main()
********************************************************************************
* Summary:
*   Test functions for features and algorithms
*******************************************************************************/
int main (void) {
    #warning "MICA_TEST is enabled"
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    #ifdef MICA_TEST_case
        /* Test descriptions */
    /* End MICA_TEST_case*/
    #else 
        #error "At least ONE MICA_TEST_<case> must be defined if MICA_TEST is defined"
    #endif /* End MICA_TEST_<case> */    
    for(;;){}
}    
#endif /* MICA_TEST */

/* ####################### BEGIN FUNCTION DEFINITIONS ####################### */

/*******************************************************************************
* ISR Name: ISR_isrName()
********************************************************************************
* Summary:
*   <isrUserCase>
* Interrupt: 
*   <nameOfInterruptComponent>
*
*******************************************************************************/
void ISR_isrName(void){
    /* Clear the Interrupt */
    
    /* Set the flag */
    flag_isrFlag = true;
}

/* [] END OF FILE */
