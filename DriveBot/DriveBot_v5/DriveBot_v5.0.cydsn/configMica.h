/***************************************************************************
*                                       MICA
* File: configMica.h
* Workspace: DriveBot_v4
* Project Name: DriveBot_v4.1
* Version: v4.0.0
* Authors: Craig Cheney
* 
* PCB: DRIVEBOT MCU v1.0.0
* PSoC: CY8C4245LQI-483
* Sensors: Encoders
*
* Brief:
*  Provides all of the settings required to configure the MICA library
* 
* 2017.08.24 CC - Document created
********************************************************************************/
#ifndef CONFIG_MICA_H
    #define CONFIG_MICA_H
    /***************************************
    * Included files - Included in all MICA files - keep to a minimum
    ***************************************/
    #include "project.h"

    /* ------------ Parameters ------------- 
    * These names must match the names in top design
    * Name of the LED components 
    */
    #define configLED_USE_PWM               (0u)   
    #define configUART_EN                   (1u)
    /* ------------ NAMES ------------- */
    /* These names must match the names in top design */
    /* Name of the LED components */
    #define configLED_RED_PIN_NAME          led_R_pin
    #define configLED_GREEN_PIN_NAME        led_G_pin
    #define configLED_BLUE_PIN_NAME         led_B_pin
    
    #define configLED_PIN_ON                (0u)
    #define configLED_PIN_OFF               (1u)
    
    #define configUART_NAME                 UART

#endif /* CONFIG_MICA_H */                  
/* [] END OF FILE */
