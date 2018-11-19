/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: main.c
* Workspace: supportCube_v5
* Project: supportCube_v5.0
* Version: 1.0.0
* Authors: Craig Cheney
* 
* PCB: Support Cube 2.1.1
* PSoC: CYBLE-214015-01
*
* Brief:
*   This is the top-level application file for Support Cube.
*
* 2018.10.19  - Document Created
********************************************************************************/
#include "project.h"
//#include "usbPacketManager.h"
#include "supportBleCallback.h"
#include "supportCommands.h"

/* ####################### BEGIN PROGRAM CONFIGURATION ###################### */

//#define MICA_DEBUG
/* ---------------- DEBUG CASE ----------------
* Uncomment ONE of the following
* Debugging will only occur when MICA_DEBUG is defined
*/
#ifdef MICA_DEBUG
//    #define MICA_DEBUG_LEDS     /* Run through the testings of the LEDS */
    #define MICA_DEBUG_UART_USB /* Write data out of the UART */
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
#define USB_PACKET_LEN_MAX  512

/* Main Program */
#if !defined(MICA_DEBUG) && !defined(MICA_TEST)
/*******************************************************************************
* Function Name: main()
********************************************************************************
* Summary:
*   The top-level application function for the project.
*
*******************************************************************************/
int main(void)
{
    /* Enable global interrupts. */
    CyGlobalIntEnable; 
    
    /* Start components */
    LEDS_Write(LEDS_ON_GREEN);
    usbUart_Start();
    imuUart_Start();
    CyBle_Start(supportBleHandler);
    
    /* Setup Packet */
    
    packets_usbComms.rxGetBytesPending = usbUart_getRxBufferSize;
    packets_usbComms.rxReadByte = usbUart_getChar;
    packets_usbComms.txPutArray = usbUart_putArray;
    packets_usbComms.ackCallback = ackHandler_supportCube;
    packets_usbComms.cmdCallback = cmdHandler_supportCube;
    
    uint32_t err = packets_usb_init(USB_PACKET_LEN_MAX);
    if(err){
        LEDS_Write(LEDS_ON_MAGENTA);
        for(;;){}
    }

    /* Infinite Loop */
    for(;;){
        /* Process the recieved packet */
        packets_usb_processIncoming();
        /* Process BLE events */
        CyBle_ProcessEvents();
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
    #elif defined MICA_DEBUG_UART_USB
    /* Write data out of the UART */
        usbUart_Start();
        LEDS_Write(LEDS_ON_GREEN);
        
        /* Infinite loop */
        for(;;){
            /* Write out a character */
            usbUart_putChar('a');
            /* Echo any typed characters */
            uint8_t data = usbUart_getChar();
            if(data) {
                usbUart_putChar(data);   
                LEDS_B_Toggle();
            }
            /* Loop delay */
            MICA_delayMs(MICA_DELAY_MS_SEC_ONE);
        }
    /* End MICA_DEBUG_UART_USB */
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
