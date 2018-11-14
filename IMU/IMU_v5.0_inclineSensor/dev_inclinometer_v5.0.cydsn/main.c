/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: main.c
* Workspace: IMU_v5.0
* Project: dev_basicUartComm_v5.0
* Version: v5.0.0
* Authors: Craig Cheney
* 
* PCB: MICA IMU v3.5.2
* PSoC: CYBLE214015-01
*
* Brief:
*   This is the top-level application file for dev work towards
*   a self-balancing DriveBot. The goal of the project is to establish basic 
*   UART comms with the DriveBot mcu, first with simple UART, then with MICA Packets.
*
* 2018.04.25 CC - Document Created
********************************************************************************/
#include "project.h"
#include "math.h"
#include <stdio.h>
#include "inclinometer.h"
/*  -------------- DEBUGGING --------------
* Uncomment MICA_DEBUG to enable general debugging.
* Uncomment MICA_DEBUG_<case> below to
* enable ONE of the debug levels.
*/
//#define MICA_DEBUG

/* -------------- DEBUG LEVEL --------------
* Uncomment ONE of the following
* Debugging will only occur when MICA_DEBUG is defined
*/
#ifdef MICA_DEBUG
//#define MICA_DEBUG_LEDS           /* Test the onboard RGB LED */
//#define MICA_DEBUG_UART_TX        /* Send data through the UART to test connectivity */
//#define MICA_DEBUG_ECHO           /* Echo to the USB UART */
//#define MICA_DEBUG_IMU          /* Test for the IMU */
//#define MICA_DEBUG_UART_ARRAY      /* Test the UART Arrays fucntion  */
//#define MICA_DEBUG_SPRINTF      /* Test the Sprintf fucntion  */
//#define MICA_DEBUG_TIMER            /* Test the interrupts on the timer  */
//#define MICA_DEBUG_TWOS_COMP_TO_FLOAT      /* Test converting two's compliment to float  */
//#define MICA_DEBUG_GYRO_PRINT       /* Print off the values received by the IMU's gyro */
//#define MICA_DEBUG_ACC_PRINT       /* Print off the values received by the IMU's Acc */
//#define MICA_DEBUG_MAG_PRINT        /* Print off the values received by the IMU's Mag */
//#define MICA_DEBUG_QUATERNIONS      /* Print out the basic Quaternion */
//#define MICA_DEBUG_INCLINOMETER     /* Test the basic sensor fusion algorithm  */ 
//#define MICA_DEBUG_IMU_ACC_POWER    /* Test the new power settings of the Accelerometer */
//#define MICA_DEBUG_IMU_GYR_POWER    /* Test the new power settings of the gyroscope */
//#define MICA_DEBUG_IMU_MAG_POWER      /* Test the new power settings of the Accelerometer */
//#define MICA_DEBUG_FULL           /* Run a composite set of debugging functions */
#endif /* End MICA_DEBUG */

/* -------------- END DEBUG CONFIG --------------  */


/*  -------------- Testing --------------
* Uncomment MICA_TEST to enable specific tests.
* Uncomment MICA_TEST_<case> below to
* enable ONE of the test levels. */

#define MICA_TEST

/* -------------- TEST LEVEL --------------
* Uncomment ONE of the following
* Testing will only occur when MICA_TEST is defined
*/
#ifdef MICA_TEST
    #define MICA_TEST_PACKETS           /* Test Packet communication */
#endif
/* -------------- END TEST LEVEL --------------  */


CY_ISR_PROTO(ISR_systemTimer);
/* --------- Local Variables --------- */
volatile bool systemTimerFlag = false;

/*******************************************************************************
* Function Name: main()
********************************************************************************
* Summary:
*   The top-level application function for the project.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
int main(void)
{
    /* Enable global interrupts. */
    CyGlobalIntEnable;
    
    /* %%%%%%%%%%%%%%%%%% Begin Debugging %%%%%%%%%%%%%%%%%% */
#ifdef MICA_DEBUG
    #if defined MICA_DEBUG_LEDS
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
        LEDS_Test(MICA_TEST_INFINITE);
    /* End MICA_DEBUG_LEDS */
    #elif defined MICA_DEBUG_UART_TX
        /* Test the Transmit portion of the UART
        Expected outcome:
        UART constantly writes out the compile date 
        and time. 
        */
        /* Start the UART */
        UART_Start();
        /* Turn on the Green LED */
        LEDS_Write(LEDS_ON_GREEN);
        /* Run the transmit test indefinitely */
        for(;;){
            /* Log, if eneabled */
            uartApi_txTest(ONE);
            /* Slow Down */
            MICA_delayMs(MICA_DELAY_MS_SEC_HALF);
            /* Toggle Blue LED */
            LEDS_B_Toggle();
        }
    #elif defined MICA_DEBUG_IMU
        /* Test for the IMU */
        /* Expected outcome:
        * 1. G LED on
        * 2. Delay (0.5 s)
        * 3. Check IMU device ID
        * 4a. ID Correct - White LED
        * 4b. ID Incorrect - Red LED
        * 4c. Other Error - blue LED
        * 5. Delay (1 s)
        */
        /* Start Components */
        I2C_Start();
        UART_Start();
        BMX055_Start();
        /* Infinite loop */
        for(;;){
            /* Initial State */
            LEDS_Write(LEDS_ON_GREEN);
            /* Delay for half second */
            MICA_delayMs(MICA_DELAY_MS_SEC_HALF);
            /* Register an "Other Error" (I2C) in case of hang */
            LEDS_Write(LEDS_ON_BLUE);
            /* Check IMU id value */
            uint32 i2cError;
            uint32 result = BMX055_Test(&i2cError);
            /* Check result */
            if(result == BMX055_ERR_OK) {
                uartApi("BMX ID Valid\r\n");
                /* Success - White LEDS */
                LEDS_Write(LEDS_ON_WHITE);
            } else if (result == BMX055_ERR_WHOAMI) {
                uartApi("BMX ID Invalid\r\n");
                /* ID value doesn't match */
                LEDS_Write(LEDS_ON_RED);
            } else if (result == BMX055_ERR_I2C){
                /* Other Error (I2C) - Redudant, added for clarity  */
                LEDS_Write(LEDS_ON_BLUE);
                /* Write out the Error over the UART */
                uartApi("I2C Error: 0x%x\r\n", i2cError);
            } else {
                LEDS_Write(LEDS_ON_MAGENTA);
                uartApi("Other Error 0x%x", result);   
            }
            /* Delay with error led for 1 second */
            MICA_delayMs(MICA_DELAY_MS_SEC_ONE);
        }
    /* End MICA_DEBUG_IMU */
    #elif defined MICA_DEBUG_UART_ARRAY
        /* Test the Sprintf fucntion  */
        /* Expected outcome: Loop
        * 1. Prints out "hello putString"
        * 2. Prints out "hello putArray"
        */
        LEDS_Write(LEDS_ON_GREEN);
        /* Start component */
        UART_Start();
        
        for(;;) {
            /* Print using putstring */
            uartApi_putString("Hello putString\r\n");
            /* Print using putArray */
            uint8 array[40] = "Hello putArray\r\nDon't Print the end";
            uartApi_putArray(array, 16);
            /* Toggle Red LED */
            LEDS_R_Toggle();
            /* Loop Delay */
            MICA_delayMs(MICA_DELAY_MS_SEC_HALF);
        }
    /* End MICA_DEBUG_UART_ARRAY */
    #elif defined MICA_DEBUG_SPRINTF
        /* Test the Sprintf fucntion  */
        /* Expected outcome: Loop
        * 1. Prints out "hello pi = 3.1415"
        */
        LEDS_Write(LEDS_ON_GREEN);
        /* Start component */
        UART_Start();
        
        for(;;) {
            char str[80];
            sprintf(str, "Hello pi = %f \r\n", 3.14159f);
            /* Print using putstring */
            uartApi_putString(str);
            /* Toggle Red LED */
            LEDS_R_Toggle();
            /* Loop Delay */
            MICA_delayMs(MICA_DELAY_MS_SEC_HALF);
        }
    /* End MICA_DEBUG_SPRINTF */
    #elif defined MICA_DEBUG_TIMER
        /* Test the interrupt system timer */
        /* Expected outcome: 
        1. Green LED
        2. Message written every second
        3. Blue led toggle
        */
        LEDS_Write(LEDS_ON_GREEN);
        /* initialize Interrupt vectors  */
        timer_interrupt_StartEx(ISR_systemTimer);
        /* Start hardware blocks  */
        UART_Start();
        Timer_Start();
        /* Set the period - disable interrupts */
        uint8 intState = CyEnterCriticalSection();
        /* Change period */
        Timer_WritePeriod(MICA_DELAY_US_SEC_QUARTER);
        /* Force reload */
        Timer_WriteCounter(ZERO);
        /* Re-enable interrupts */
        CyExitCriticalSection(intState);
        
        /* Infinite loop */
        for(;;) {
            /* Check the system flag */
            if(systemTimerFlag){
                /* Clear the flag */
                systemTimerFlag = false;
                /* Write out the message */
                uartApi_putString("Timer Interrupt\r\n");
                /* Write LEDs */
                LEDS_B_Toggle();
            }
        }
    /* End MICA_DEBUG_TIMER */
    #elif defined MICA_DEBUG_TWOS_COMP_TO_FLOAT
    /* convert between 12 base to 16 base two's compliment */
        /* Expected outcome: 
        1. Write a string comparing various formats
        */
        LEDS_Write(LEDS_ON_GREEN);
        /* Start component */
        UART_Start();
        /* infinite loop */
        for(;;) {
            char str[80];
            /* 12-bit two's comp -20 */
            uint16 base12 = 0b111111101100;
            /* Convert to 16-bit */
            int16 new12 = BMX055_twosComp_12To16(base12);
            /* Baseline */
            int16 base16 = -20;
            /* Convert to floats */
            float f12_twos = (float) base12;
            float f12 = (float) new12;
            float f16 = (float) base16;
            /* Format */
            sprintf(str, "12bit: 0x%x, new12: 0x%x 16bit: 0x%x f12_twos:%f f12:%f, f16:%f\r\n", base12, new12, base16, f12_twos, f12, f16);
            /* Print using putstring */
            uartApi_putString(str);
            /* Toggle Red LED */
            LEDS_R_Toggle();
            /* Loop Delay */
            MICA_delayMs(MICA_DELAY_MS_SEC_ONE);
        }
    /* End MICA_DEBUG_TWOS_COMP_TO_FLOAT */
    #elif defined MICA_DEBUG_GYRO_PRINT
        /* Print off the values received by the IMU's gyro */
        /* Expected outcome:
        1. Green LED on 
        2. Write out formatted string out over the UART
        2a. Toggle Blue led - successful read
        2b. Red led - an error occured 
        */
        LEDS_Write(LEDS_ON_GREEN);
        /* container for gryo data */
        BMX055_STATE_T imuState;
        GYR_DATA_RAD_F gyroRadData;
        
        /* Start blocks */
        I2C_Start();
        UART_Start();
        BMX055_Start(&imuState);

        /* Infinite loop */
        for(;;){
            /* Read in the values for the gyroscope */
            uint32 err = BMX055_Gyr_Readf_rad(&imuState.gyr, &gyroRadData);
            /* Ensure value data */
            if(err != BMX055_ERR_OK) {
                /* Turn on RED LED */
                LEDS_Write(LEDS_ON_RED);
            } else{

                char str[80];
                /* Format the string */
                sprintf(str, "Gyro[rad] X: %f, Y: %f, Z: %f\r\n", gyroRadData.Wx, gyroRadData.Wy, gyroRadData.Wz);
                /* Write the data out */
                uartApi_putString(str);
                /* Toggle Blue led */
                LEDS_B_Toggle();
            }
            /* Loop delay */
            MICA_delayMs(MICA_DELAY_MS_SEC_ONE);
        }
    /* End MICA_DEBUG_GYRO_PRINT */
    #elif defined MICA_DEBUG_ACC_PRINT
        /* Print off the values received by the IMU's Acc */
        /* Expected outcome:
        1. Green LED on 
        2. Write out formatted string out over the UART
        2a. Toggle Blue led - successful read
        2b. Red led - an error occured 
        */
        LEDS_Write(LEDS_ON_GREEN);
        /* container for Acc data */
        BMX055_STATE_T imuState;
        ACC_DATA_F accData;
        /* Start blocks */
        I2C_Start();
        UART_Start();
        BMX055_Start(&imuState);

        /* Infinite loop */
        for(;;){
            /* Read in the values for the Acceleromter */
            uint32 err = BMX055_Acc_Readf(&imuState.acc, &accData);
            /* Ensure value data */
            if(err != BMX055_ERR_OK) {
                /* Turn on RED LED */
                LEDS_Write(LEDS_ON_RED);
            } else{
                char str[80];
                /* Format the string, place into str */
                sprintf(str, "Acc - X: %f, Y: %f, Z: %f\r\n", accData.Ax, accData.Ay, accData.Az);
                /* Write the data out */
                uartApi_putString(str);
                /* Toggle Blue led */
                LEDS_B_Toggle();
            }
            /* Loop delay */
            MICA_delayMs(MICA_DELAY_MS_SEC_ONE);
        }
    /* End MICA_DEBUG_ACC_PRINT */
    #elif defined MICA_DEBUG_MAG_PRINT
        /* Print off the values received by the IMU's Mag */
        /* Expected outcome:
        1. Green LED on 
        2. Write out formatted string out over the UART
        2a. Toggle Blue led - successful read
        2b. Red led - an error occured 
        */
        LEDS_Write(LEDS_ON_GREEN);
        /* container for Acc data */
        BMX055_STATE_T imuState;
        MAG_DATA_F magData;
        /* Start blocks */
        I2C_Start();
        UART_Start();
        BMX055_Start(&imuState);
        /* turn on the Magnetometer */
        uint32 powerErr = BMX055_Mag_SetPowerMode(&imuState.mag, BMX055_MAG_PM_NORMAL);
        if( powerErr != BMX055_ERR_OK) {
            /* Don't advance if not valid */
            LEDS_Write(LEDS_ON_WHITE);
            for(;;){}
        }

        /* Infinite loop */
        for(;;){
            /* Read in the values for the Acceleromter */
            uint32 err = BMX055_Mag_Readf(&imuState.mag, &magData);
            /* Ensure value data */
            if(err != BMX055_ERR_OK) {
                /* Turn on RED LED */
                LEDS_Write(LEDS_ON_RED);
            } else{
                char str[80];
                /* Format the string, place into str */
                sprintf(str, "Mag - X: %f, Y: %f, Z: %f\r\n", magData.X, magData.Y, magData.Y);
                /* Write the data out */
                uartApi_putString(str);
                /* Toggle Blue led */
                LEDS_B_Toggle();
            }
            /* Loop delay */
            MICA_delayMs(MICA_DELAY_MS_SEC_ONE);
        }
    /* End MICA_DEBUG_MAG_PRINT */
    #elif defined MICA_DEBUG_QUATERNIONS
        /* Compares conversions between Euler angles and Quaternion representations */
        /* Expected outcome: 
        1. Green LED on
        2. Print out the conversion (compare against http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/)
        3. Toggle Red LED
        */
        LEDS_Write(LEDS_ON_GREEN);
        /* Start hardware blocks */
        UART_Start();
        /* Infinite loop */
        for(;;){
            /* dummy quaternion */ 
            QUATERNION_T quat = {.q1 = (1.0f), .q2 = (0.1f), .q3 = (0.6f), .q4 = (0.3f)};
            EULER_ANGLE_T eAngle;
            /* Convert to Euler angle */
            quaternionToEuler(&quat, &eAngle);
            /* Format string */
            char str[80];
            sprintf(str, "Yaw: %f, Pitch: %f, Roll: %f\r\n", to_degrees(eAngle.yaw), to_degrees(eAngle.pitch), to_degrees(eAngle.roll) ); 
            /* print out */
            uartApi_putString(str);
            /* Loop delay */
            MICA_delayMs(MICA_DELAY_MS_SEC_ONE);
            /* Toggle LED */
            LEDS_R_Toggle();
        }
    /* End MICA_DEBUG_QUATERNIONS */
    #elif defined MICA_DEBUG_INCLINOMETER
        /* Test the basic sensor fusion algorithm  */ 
        /* Expected outcome: 
        1. Green LED on
        2. Print out the Angle of the sensor
        3. Toggle Red LED
        */
        LEDS_Write(LEDS_ON_GREEN);
        /* Initilize interrupt vectors */
        timer_interrupt_StartEx(ISR_systemTimer);
        /* State variables */ 
        Incline_STATE_T inclineState;
        EULER_ANGLE_T stateAngle;
        BMX055_STATE_T imuState;
        ACC_DATA_F accData;
        GYR_DATA_RAD_F gyrData;
        char str[80];
        const bool verbose = false;

        /* Start hardware blocks */
        UART_Start();
        I2C_Start();
        BMX055_Start(&imuState);
        Timer_Start();
        
        /* Initialize the Incline filter */
        /* Filter sample period in [s] */
        const float samplePeriod = 0.1f;
        /* Gyroscope drift in [deg/s] */
        const float estGyrDrift = 5.0f;
        Incline_Init(&inclineState, samplePeriod, estGyrDrift);
        
        
        /* Set the period - disable interrupts */
        uint8 intState = CyEnterCriticalSection();
        /* Change period */
        Timer_WritePeriod(MICA_DELAY_US_SEC_TENTH);
        /* Force reload */
        Timer_WriteCounter(ZERO);
        /* Re-enable interrupts */
        CyExitCriticalSection(intState);

        /* Infinite loop */
        for(;;){
            /* Check the system flag */
            if(systemTimerFlag) {
                /* Reset flag */
                systemTimerFlag = false;

                /* Read in the values for the Acceleromter & Gyro */
                BMX055_Acc_Readf(&imuState.acc, &accData);
                BMX055_Gyr_Readf_rad(&imuState.gyr, &gyrData);
                /* Update the filter */
                Incline_UpdateFilter(&inclineState, &accData, &gyrData);
                /* Convert to Euler Angle */
                Incline_QuaternionToEuler(&inclineState.state, &stateAngle);
                
                /* Clear the Screen */
                uartApi_clearScreen();
                /* Verbosity level */
                if(verbose){
                    /* New lines */
                    uartApi_putString("\r\n\n");
                    /* Display Gryo data */
                    sprintf(str, "Gyro - X: %f, Y: %f, Z: %f\r\n", gyrData.Wx, gyrData.Wy, gyrData.Wz);
                    uartApi_putString(str);
                    /* Display Accd ata */
                    sprintf(str, "Acc - X: %f, Y: %f, Z: %f\r\n", accData.Ax, accData.Ay, accData.Az);
                    uartApi_putString(str);
                    /* Format string */ 
                    sprintf(str, "Qw: %.4f, Qx: %.4f, Qy: %.4f, Qz: %.4f\r\n", inclineState.state.q1, inclineState.state.q2, inclineState.state.q3, inclineState.state.q4 ); 
                    uartApi_putString(str);
                }
                /* Format string */ 
                sprintf(str, "Yaw: %.1f\r\nPitch: %.1f\r\nRoll: %.1f\r\n", to_degrees(stateAngle.yaw), to_degrees(stateAngle.pitch), to_degrees(stateAngle.roll) ); 
                uartApi_putString(str);
                /* Toggle LED */
                LEDS_R_Toggle();
            }
        }
    /* End MICA_DEBUG_INCLINOMETER */
    #elif defined MICA_DEBUG_IMU_ACC_POWER
        /* Test the power modes of the ACC  */ 
        /* Expected outcome: 
        1. Yellow LED on (ACC in Suspend)
        2. Blue LED on (Gyr in deep suspend)
        3. Green LED on (Gyr in Normal)
            A. Red LED indicates Error
        */
        /* Create a state variable for the IMU */
        BMX055_STATE_T imuState;
        /* Start hardware blocks */
        I2C_Start();
        BMX055_Start(&imuState);
        Timer_Start();
        
        /* Place Gyroscope into deep-suspend */
        BMX055_Gyr_SetPowerMode(&imuState.gyr, BMX055_GYR_PM_DEEP_SUSPEND);
        /* Inifinite Loop */
        for(;;){
            /* Put in  suspend */
            uint32 err = BMX055_Acc_SetPowerMode(&imuState.acc, BMX055_ACC_PM_SUSPEND);
            /* Ensure Write worked */
            if(err == BMX055_ERR_OK){
                LEDS_Write(LEDS_ON_YELLOW);  
            } else {
                LEDS_Write(LEDS_ON_RED);
            }
            /* Delay */
            MICA_delayMs(2500);
            
            /* Put in Deep suspend */
            err = BMX055_Acc_SetPowerMode(&imuState.acc, BMX055_ACC_PM_DEEP_SUSPEND);
            /* Ensure Write worked */
            if(err == BMX055_ERR_OK){
                LEDS_Write(LEDS_ON_BLUE);  
            } else {
                LEDS_Write(LEDS_ON_RED);
            }
            /* Delay */
            MICA_delayMs(2500);
            
            /* Put in Deep suspend */
            LEDS_Write(LEDS_ON_BLUE);
            err = BMX055_Acc_SetPowerMode(&imuState.acc, BMX055_ACC_PM_NORMAL);
            /* Ensure Write worked */
            if(err == BMX055_ERR_OK){
                LEDS_Write(LEDS_ON_GREEN);  
            } else {
                LEDS_Write(LEDS_ON_RED);
            }
            /* Delay */
            MICA_delayMs(2500);
        }
    /* End MICA_DEBUG_IMU_ACC_POWER */
    #elif defined MICA_DEBUG_IMU_GYR_POWER
        /* Test the power modes of the GYR  */ 
        /* Expected outcome: 
        1. Yellow LED on (Gyr in Suspend)
        2. Blue LED on (Gyr in deep suspend)
        3. Green LED on (Gyr in Normal)
            A. Red LED indicates Error
        */
        /* Create a state variable for the IMU */
        BMX055_STATE_T imuState;
        /* Start hardware blocks */
        I2C_Start();
        BMX055_Start(&imuState);
        uint32 err;
        /* Inifinite Loop */
        for(;;){
            /* Put in  suspend */
            err = BMX055_Gyr_SetPowerMode(&imuState.gyr, BMX055_GYR_PM_SUSPEND);
            /* Ensure Write worked */
            if(err == BMX055_ERR_OK){
                LEDS_Write(LEDS_ON_YELLOW);  
            } else {
                LEDS_Write(LEDS_ON_RED);
            }
            /* Delay */
            MICA_delayMs(2500);
            
            /* Put in Deep suspend */
            err = BMX055_Gyr_SetPowerMode(&imuState.gyr, BMX055_GYR_PM_DEEP_SUSPEND);
            /* Ensure Write worked */
            if(err == BMX055_ERR_OK){
                LEDS_Write(LEDS_ON_BLUE);  
            } else {
                LEDS_Write(LEDS_ON_RED);
            }
            /* Delay */
            MICA_delayMs(2500);
            
            /* Put in Deep suspend */
            err = BMX055_Gyr_SetPowerMode(&imuState.gyr, BMX055_GYR_PM_NORMAL);
            /* Ensure Write worked */
            if(err == BMX055_ERR_OK){
                LEDS_Write(LEDS_ON_GREEN);  
            } else {
                LEDS_Write(LEDS_ON_RED);
            }
            /* Delay */
            MICA_delayMs(2500);
        }
    /* End MICA_DEBUG_IMU_GYR_POWER */
    #elif defined MICA_DEBUG_IMU_MAG_POWER
        /* Test the power modes of the MAG  */ 
        /* Expected outcome: 
        1. Yellow LED on (Mag in Suspend)
        2. Blue LED on (Mag in deep suspend)
        3. Green LED on (Mag in Normal)
            A. Red LED indicates Error
        */
        /* Create a state variable for the IMU */
        BMX055_STATE_T imuState;
        /* Start hardware blocks */
        I2C_Start();
        BMX055_Start(&imuState);
        Timer_Start();
        /* Turn off gyro and acc */
        BMX055_Acc_SetPowerMode(&imuState.acc, BMX055_ACC_PM_DEEP_SUSPEND);
        BMX055_Gyr_SetPowerMode(&imuState.gyr, BMX055_GYR_PM_DEEP_SUSPEND);
        
        /* Inifinite Loop */
        for(;;){
            /* Put in suspend */
            uint32 err =  BMX055_Mag_SetPowerMode(&imuState.mag, BMX055_MAG_PM_SUSPEND);
            /* Ensure Write worked */
            if(err == BMX055_ERR_OK){
                LEDS_Write(LEDS_ON_BLUE);  
            } else {
                LEDS_Write(LEDS_ON_RED);
            }
            /* Delay */
            MICA_delayMs(2500);
            
            /* Put in sleep */
            err = BMX055_Mag_SetPowerMode(&imuState.mag, BMX055_MAG_PM_SLEEP);
            /* Ensure Write worked */
            if(err == BMX055_ERR_OK){
                LEDS_Write(LEDS_ON_YELLOW);  
            } else {
                LEDS_Write(LEDS_ON_RED);
            }
            /* Delay */
            MICA_delayMs(2500);
            
            /* Put in Normal */
            err = BMX055_Mag_SetPowerMode(&imuState.mag, BMX055_MAG_PM_NORMAL);
            /* Ensure Write worked */
            if(err == BMX055_ERR_OK){
                LEDS_Write(LEDS_ON_GREEN);  
            } else {
                LEDS_Write(LEDS_ON_RED);
            }
            /* Delay */
            MICA_delayMs(2500);
        }
    /* End MICA_DEBUG_IMU_MAG_POWER */
    #else
        #error "Exactly ONE MICA_DEBUG_<case> must be defined if MICA_DEBUG is defined"
    #endif /* MICA_DEBUG_<case> */
#endif /* MICA_DEBUG */
#ifdef MICA_TEST
    #if defined MICA_TEST_PACKETS
        /* Test the ability to send packets back and forth to a baseboard */
        /* Start the hardware blocks */
        UART_Start();
        LEDS_Write(LEDS_ON_GREEN);
        /* Initialize variables */
        packets_BUFFER_FULL_S packetBuffer;
        packets_initialize(&packetBuffer);
        uint32 error = packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
        /* Ensure packet buffers were created properly */
        if(error){
            LEDS_Write(LEDS_ON_WHITE);
            for(;;){}
        }
        /* Local reference to the TX packet */
        packets_PACKET_S* txPacket =  &(packetBuffer.send.packet);
        
        /* Define dummy data into the command */
        #define DATA_LEN 0x03
        uint8 dummyData[DATA_LEN] = {0x01, 0x03, 0x05};
        txPacket->moduleId = 1;
        txPacket->cmd = 0xCC;
        txPacket->payload = dummyData;
        txPacket->payloadLen = DATA_LEN;

        uint8 i = ZERO;
        /* Infinite loop */
        for(;;){
            LEDS_G_Toggle();
            dummyData[0] = i++;
            /* Pack data into buffer */
            /* Write out if successful */
            uint32 error = packets_sendPacket(&packetBuffer);
            if(error){
                /* Turn on RED LED */
                LEDS_Write(LEDS_ON_RED);
            }
            /* Delay */
            MICA_delayMs(MICA_DELAY_MS_SEC_ONE);
        }
        
    /* End MICA_TEST_PACKETS */
    #else 
        #error "At least ONE MICA_TEST_<case> must be defined if MICA_TEST is defined"
    #endif
    #if defined(MICA_DEBUG) && defined(MICA_TEST)
        #error "MICA_DEBUG and MICA_TEST may not be defined at the same time"
    #endif
#endif /* MICA_TEST */
/* %%%%%%%%%%%%%%%%%%  End Debugging  %%%%%%%%%%%%%%%%%% */
    /* LEDs initial state */    
    LEDS_Write(LEDS_ON_GREEN);
    /* Start Hardware blocks */
    I2C_Start();
    UART_Start();
    
    /* Infinite Loop */
    for(;;)
    {

    }
}

/*******************************************************************************
* ISR Name: ISR_systemTimer
********************************************************************************
* Summary:
*        Triggers when the TC of the timer expires. Used for scheduling events
*
* Interrupt: 
*       timer_interrupt
*
*******************************************************************************/
CY_ISR(ISR_systemTimer){
    /* Clear Interrupt */
    Timer_STATUS;
    /* Indicate system flag */
    systemTimerFlag = true;
}

/* [] END OF FILE */
