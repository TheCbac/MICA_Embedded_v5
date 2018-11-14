/***************************************************************************
*                                       MICA
* File: micaConfig.h
* Workspace: IMU_v4.0
* Project Name: 01_IMU_Stack_v4.0
* Version: v4.0.0
* Author: Craig Cheney
* 
* PCB: MICA_IMU_v3.5.2
* PSoC: CYBLE-214015-01 (EZBLE, 4.2, 256k)
* Sensors: BOSCH IMU (BMX055)
*
* Brief:
*  Provides all of the settings required to configure the MICA library
* 
* Authors:
*   Craig Cheney
*
* 2017.10.04 CC - Changed name from "micaConfig.h" to "configMica.h" to comply 
*   with libMica 1.0.0 API specification
* 2017.08.13 CC - Document Created
********************************************************************************/
#ifndef MICA_CONFIG_H
    #define MICA_CONFIG_H
    /***************************************
    * Included files
    ***************************************/
    #include "project.h"
    /***************************************
    * LEDs
    ***************************************/
    /* Don't use PWM */
    #define configLED_USE_PWM               (0u)   
    
    /* ------------ NAMES ------------- */
    /* These names must match the names in top design */
    /* Name of the LED components */
    #define configLED_RED_PIN_NAME          led_R_pin
    #define configLED_GREEN_PIN_NAME        led_G_pin
    #define configLED_BLUE_PIN_NAME         led_B_pin
    
    /* ------------ Constants ------------- */
    #define configLED_PIN_ON                (0u)
    #define configLED_PIN_OFF               (1u)
    
#endif /* MICA_CONFIG_H */
/* [] END OF FILE */
