/***************************************************************************
*                                    MICA
*                              Craig Cheney © 2018
*                                ccheney@mit.edu
*
* File: boardTesting.c
* Workspace: DriveBot_V4.0
* Project Name: DriveBot_v4.0_TESTING
* Version: v2.0.1
* Authors: Craig Cheney
*
* Brief:
*   Source for PCB population testing functiongs
* 
* 2018.02.13 CC - Document Created
********************************************************************************/
#include "boardTesting.h"
#include "project.h"
#include "common.h"
#include <stdbool.h>
/* Delay in ms between tests */
#define DELAY_TESTING       2000
#define DELAY_TESTING_LONG  4000
#define DELAY_TESTING_SHORT 500
#define DELAY_TESTING_FAST  10

/*******************************************************************************
* Function Name: test_DebugLeds()
********************************************************************************
* Summary:
*   Loops through testing of the onboard debugging LEDs. This function will
*   never return. 
*
* Expected Outcome: 
*   0. All LEDs off
*   1. Red LED solid on
*   2. Green LED solid on
*   3. Blue LED solid on
*   4. RGB (White) LEDs on
*
* Parameters:
*   None
*
* Return:
*   Never (Infinite loop)
*
*******************************************************************************/
void test_DebugLeds(void) {
    /* Infinite loop */
    for(;;) {
        /* 0 - All off */
        led_R_pin_Write(LED_OFF);
        led_G_pin_Write(LED_OFF);
        led_B_pin_Write(LED_OFF);
        CyDelay(DELAY_TESTING);
        /* 1 - Red ON */
        led_R_pin_Write(LED_ON);
        CyDelay(DELAY_TESTING);
        /* 2 - Green ON */
        led_R_pin_Write(LED_OFF);
        led_G_pin_Write(LED_ON);
        CyDelay(DELAY_TESTING);
        /* 3 - Blue ON */
        led_G_pin_Write(LED_OFF);
        led_B_pin_Write(LED_ON);
        CyDelay(DELAY_TESTING);
        /* 4 - RGB (white) ON */
        led_R_pin_Write(LED_ON);
        led_G_pin_Write(LED_ON);
        led_B_pin_Write(LED_ON);
        CyDelay(DELAY_TESTING);
    }
}

/*******************************************************************************
* Function Name: test_button()
********************************************************************************
* Summary:
*   Tests the button functionality
*
* Expected Outcome: 
*   0. Blue LED on
*   1. Button Press
*   2. Green LED on
*   3. Button Release
*
* Parameters:
*   None
*
* Return:
*   Never (Infinite loop)
*
*******************************************************************************/
void test_button(void) {
    led_R_pin_Write(LED_OFF);   
    /* Infinite loop */
    for(;;){
        bool btnPress = button_pin_Read();
        if(btnPress) {
            led_G_pin_Write(LED_ON); 
            led_B_pin_Write(LED_OFF);   
        } else {
            led_G_pin_Write(LED_OFF); 
            led_B_pin_Write(LED_ON);    
        }
    }
}

/* [] END OF FILE */
