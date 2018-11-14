/***************************************************************************
*                                       MICA
* File: Inclinometer.h
* Workspace: micaComponents
* Project Name: libMica
* Version: v1.0
* Author: Craig Cheney
*
* Brief:
*   Header for sensor fusion library
* 
* Authors:
*   Craig Cheney
*
* Change Log:
*   2018.03.17 CC - Document created
********************************************************************************/
/* Header Guard */
#ifndef INCLINOMETER_H
    #define INCLINOMETER_H
    /***************************************
    * Included Files
    ***************************************/
    #include "micaCommon.h"
    /***************************************
    * Macro Definitions
    ***************************************/
    /* System Constants */
    #define DELTA_T             (0.1f) /**< Sampling period in seconds */
    #define GYRO_MEAS_ERROR     (3.14159265358979f) * (5.0f / 180.0f) /**< Gyroscope measurement error */
    #define BETA                sqrt(3.0f / 4.0f) * GYRO_MEAS_ERROR /**< Compute beta */
    
    /***************************************
    * Structs
    ***************************************/

    // /* Struct for representing a Quaternion */
    // typedef struct {
    //     float beta; /**< Value used in filter */
    //     float deltaT; /**< Second element (i) */
    // } FILTER_SETTING_T;
    
    /***************************************
    * Functions Prototypes
    ***************************************/
    void inclinometer_updateFilter(ACC_DATA_F* accData, GYR_DATA_RAD_F* gyroData, QUATERNION_T* State);
    void quaternionToEuler(QUATERNION_T * quat, EULER_ANGLE_T * eAngle);


#endif /* INCLINOMETER_H */
/* [] END OF FILE */