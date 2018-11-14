/***************************************************************************
*                                       MICA
* File: main.c
* Workspace: IMU_v5.0
* Project Name: 02_IMU_App_v5.0
* Version: v4.0.1
* Authors: Craig Cheney
* 
* PCB: MICA_IMU_v3.5.2
* PSoC: CYBLE-214015-01 (EZBLE, 4.2, 256k)
* Sensors: BOSCH IMU (BMX055)
*
* Brief:
*  Application program for the upgradeable stack app. 
* 
* 2018.03.02 CC - Begin porting to Components based MICA instead of libMica
* 2018.02.28 CC - Update for 2.671
* 2017.10.04 CC - Incorporate change name funtionality
* 2017.08.22 CC - Integrating libMica
* 2017.08.13 CC - Application runs OTA and can be triggered to receive updates via BLE
* 2017.08.01 CC - Document Created 
********************************************************************************/

/*  -------------- DEBUGGING --------------
* Uncomment MICA_DEBUG to enable ONE of the below Debug levels
* DEBUGGING GENERALLY DISABLES OTA FUNCTIONALITY
*/
#define MICA_DEBUG

/* -------------- DEBUG LEVEL --------------
* Uncomment ONE of the following
* Debugging will only occur when MICA_DEBUG is defined
*/
//#define MICA_DEBUG_LEDS /* Test the onboard LEDS */
//#define MICA_DEBUG_OTA /* Force an OTA conversion */
//#define MICA_DEBUG_SFLASH /* Force a name change */
#define MICA_DEBUG_IMU  /* Make contact with the IMU. Test I2C as well */
/* -------------- END DEBUG CONFIG -------------- */


/* Counter match for the Watchdog timer */
#define WDT_COUNTER0_MATCH_VAL  (0xFFFFu)
#define WDT_COUNTER1_MATCH_VAL  (0x03u)

#include "project.h"
#include "bleImu.h"
#include "powerManagement.h"

/* Private function declaration */
static void initializeDevice(void);
/* Local variable declarations */

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
int main(void) {
    /* Initialize the IMU */
    initializeDevice();
    
    /* %%%%%%%%%%%%%%%%%% Begin Debugging %%%%%%%%%%%%%%%%%% */
#ifdef MICA_DEBUG
    #ifdef MICA_DEBUG_LEDS
        /* Expected Outcome:
        0. All LEDs off
        1. Red LED on
        2. Green LED on
        3. Blue LED on
        4. Yellow (RB)
        5. Cyan (BG)
        6. Magenta (RG)
        7. White (RGB) LEDs on */
        LEDS_Test(MICA_TEST_INFINITE);
    #elif defined MICA_DEBUG_OTA
        /* Launch into OTA bootloader mode */
        OTA_EnterBootloadMode();
        /* Device Resets */
    #elif defined MICA_DEBUG_SFLASH  
        /* Force a name change */
        #define NAME_LEN (10u)
        Sflash_WriteLocalName(NAME_LEN, "DEV IMU 01");
        /* Read the local name from SFlash, and set that as the local name */
        char nameArray[Sflash_NAME_LEN];
        uint32 result = Sflash_ReadLocalName(nameArray);
        /* Ensure the read was successful */
        if(result == Sflash_ERR_OK) {
            /* Set the new local name */
            CyBle_GapSetLocalName(nameArray);   
        }
        /* Infinite loop */
        for(;;){imuBle_processEvents();}
    #elif defined MICA_DEBUG_IMU /* Check Connectivity to I2C */
        /* Expected outcome: 
        0. White LED ON
        1a. Green LED on - success
        1b. Blue LED ON - Whoam I value failed
        1c. Red LED on - I2C Error
        1d. No leds on - Unknown error
        */
        LEDS_Write(LEDS_ON_WHITE);
        uint32 i2cError = ZERO;
        /* Check the I2C Value */
        uint32 testResult = BMX055_Test(&i2cError);
        switch(testResult){
            case BMX055_ERR_OK:
                LEDS_Write(LEDS_ON_GREEN);
                break;
            case BMX055_ERR_WHOAMI:
                LEDS_Write(LEDS_ON_BLUE);   
                break;
            case BMX055_ERR_I2C:
                LEDS_Write(LEDS_ON_RED);
                break;
            default:
                LEDS_Write(LEDS_ON_NONE);
                break;
        }
        /* Infinite loop */
        for(;;){}
    #else
        #error "MICA_DEBUG_<CASE> must be defined if MICA_DEBUG is defined"
    #endif /* MICA_DEBUG_<CASE> */
#endif /* MICA_DEBUG */
    /* %%%%%%%%%%%%%%%%%% End Debugging %%%%%%%%%%%%%%%%%% */

    /* Infinite loop */
    for(;;){
        /* Mandatory - Process BLE events */
        imuBle_processEvents();   
    }
}


/* OLD MAIN */
//    /* Variables for the infinite loop */
//    MICA_ERROR_CODE_T result;
//    MICA_TIMER_DATA_T timerData;
//    MICA_TIMER_STAMP_T timeStamp;
//    MICA_SENSOR_LIST activeSensors;
//    /* Infinite Loop */
//    for(;;){
//        /* Mandatory - Process BLE events */
//        micaBle_processEvents();
//        
//        /* Check the timer to see if an event is queued */
//        result =  micaTimer_Data_Get(&timerData);
//        /* Show an error LED */
//        if (result == ERR_NO_ERROR) {
//            /* Check to see if a sample is pending */
//             if (timerData.pendingSample) {
//                /* Clear the pending sample */
//                micaTimer_Data_ClearPendingSample();
//                /* Get the active sensors */
//                mica_sensorGetActive(&activeSensors);
//                /* Iterate thorugh the active sensors */
//                uint8 i;
//                for (i=ZERO; i< activeSensors.count; i++){
//                    uint16 sensorData[SENSING_MAX_CHANNELS];   
//                    /* Gather data from the sensor and place into sensorData[] */
//                    result = mica_sensorRead(sensorData, activeSensors.sensorId[i], activeSensors.activeChannels[i]);
//                    if(result == ERR_NO_ERROR) {
//                        /* Toggle the LEDs */
////                        micaLedToggle(MICA_LED_WHITE);
//                        /* Get the time stamp */
//                        micaTimer_Stamp_Get(&timeStamp);
//                        /* Pack the data into packets */
//                        mica_blePackPayload(&sensorData[MICA_CHANNEL_INDEX_DATA], sensorData[MICA_CHANNEL_INDEX_NUM_CHAN], &timeStamp, activeSensors.periodCount[i]);
//                    }
//                }
//            }
//               
//        } 
//        
//        /* Enter the low power BLE state */
//        CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
//        /* Clear the WDT C0 & C1 so device does not reset */
//        CySysWdtResetCounters(CY_SYS_WDT_COUNTER0_RESET | CY_SYS_WDT_COUNTER1_RESET);
//        /* Manage system power */
//        power_processSystemState();
//    }
//}

/*******************************************************************************
* Function Name: initializeDevice()
********************************************************************************
* Summary:
*   Initialize the IMU. Starts the components, checks to see if an OTA update
*   occured, and then initializes the name change. 
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void initializeDevice(void){
    /* Turn on LEDs  */
    LEDS_Write(LEDS_ON_WHITE);
    /* Start the I2C component */
    I2C_Start(); // Move this behind micaI2c_Start() 
    UART_Start();
    /* Enable & Assign interrupts */
    CyGlobalIntEnable; 
    /* See if this is the first time the app is being run */
    OTA_AfterAppImageUpdate();
    /* Start the code sharing for the BLE component */
    OTA_InitializeCodeSharing();

    /* Initialize the BLE component */
    imuBle_init();
    
//    /* Initialize the TIMER ISR */
//    micaTimer_Init();
//    /* Set the Watch Dog Timer (WDT) */
//    CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_NONE);
//    /* Set WDT counter 1 to generate reset on match */
//    CySysWdtWriteMode(CY_SYS_WDT_COUNTER1, CY_SYS_WDT_MODE_RESET);
//    /* Set the value at which WDT C0 triggers */
//    CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, WDT_COUNTER0_MATCH_VAL);
//    /* Set the value at which WDT C1 triggers */
//    CySysWdtWriteMatch(CY_SYS_WDT_COUNTER1, WDT_COUNTER1_MATCH_VAL);
//    /* Cascade the WDT C0 -> C1 */
//    CySysWdtWriteCascade(CY_SYS_WDT_CASCADE_01);
//    /* Enable the WDT */
//    CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK | CY_SYS_WDT_COUNTER1_MASK);
//    /* Put device into low power state */
////    power_setSystemState(STATE_PREP_SLEEP);
}




/* [] END OF FILE */
