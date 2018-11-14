/***************************************************************************
*                                       MICA
* File: configMica.h
* Workspace: IMU_v4.0
* Project Name: 02_IMU_App_v4.0
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
* Date Written:  2017.08.13
* Last Modified: 2017.08.24
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
    #define configI2C_EN                    (1u)
    #define configLED_USE_PWM               (1u)   
    #define configIMU_USE_I2C               (1u)
    #define configTIMING_EN                 (1u)
    #define configADC_EN                    (1u)
    #define configUseBLE                    (1u)
    /* ------------ NAMES ------------- */
    /* These names must match the names in top design */
    /* Names of the PWM components */
    #define configLED_RED_PWM_NAME          PWM_R
    #define configLED_GREEN_PWM_NAME        PWM_G
    #define configLED_BLUE_PWM_NAME         PWM_B
    /* Name of the LED control registers */
    #define configLED_CONTROL_REG_NAME      ledControl
    /* Name of the I2C master */
    #define configI2C_MASTER_NAME           I2C
    /* Name of the Timer */
    #define configTIMER_TIMESTAMP_NAME      timeStampTimer
    #define configTIMER_DATA_NAME           dataSampleTimer
    #define configTIMER_SECONDS_IRQ_NAME    seconds_Interrupt
    #define configTIMER_SAMPLE_IRQ_NAME     sample_Interrupt
    /* Name of the ADC */
    #define configADC_NAME                  ADC
    /* ------------ Constants ------------- */
    #define configLED_PWM_MAX               (254u)
    #define configLED_PWM_OFF               (0u)
    
    
    #define configTIMER_DOWN                (1u)
#endif /* CONFIG_MICA_H */                  
/* [] END OF FILE */
