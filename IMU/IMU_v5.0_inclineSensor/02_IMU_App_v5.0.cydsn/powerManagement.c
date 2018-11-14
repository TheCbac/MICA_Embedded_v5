/***************************************************************************
*                                       SwingTime
* File: powerManagement.c
* Workspace: IMU_v4.0
* Project Name: 02_SwingTime_App_v2.1
* Version: v2.0.1
* Authors: Craig Cheney
* 
* PCB: SwingTime_v2.1.2
* PSoC: CYBLE-214009-00 (EZBLE, 4.1, 256k)
* Sensors: MMA8635 (Accelerometer)
*
* Brief:
* Controls all of the power management states
* 
* 2018.02.16 CC - Document created
********************************************************************************/
#include "powerManagement.h"
/* State of the Application */
volatile APP_POWER_STATE_T appPowerState = STATE_ACTIVE;

/*******************************************************************************
* Function Name: power_getSystemState()
********************************************************************************
* Summary:
*  Returns the current power state of the system
*
* Parameters:
*   None
*
* Return:
*   appPowerState - state of the system
*
*******************************************************************************/
APP_POWER_STATE_T power_getSystemState(void){
    return appPowerState;   
}

/*******************************************************************************
* Function Name: power_setSystemState()
********************************************************************************
* Summary:
*  Sets the power state of the system. Only Certain transitions are valid.
*
* Parameters:
*   nextState - The next power state to go into
*
* Return:
*   appPowerState - state of the system
*
*******************************************************************************/
APP_POWER_STATE_T power_setSystemState(APP_POWER_STATE_T nextState){
    /* Switch on the current state */
    switch(appPowerState) {
        /* DEEPSLEEP valid next states: WAKEUP  */
        case STATE_DEEPSLEEP:{
            if(nextState == STATE_WAKEUP){
                appPowerState = nextState;   
            }
            break;
        }
        /* WAKEUP valid next states: ACTIVE*/
        case STATE_WAKEUP:{
            if(nextState == STATE_ACTIVE){
                appPowerState = nextState;   
            }
            break;
        }
        /* ACTIVE valid next states: PREP_SLEEP*/
        case STATE_ACTIVE:{
            if(nextState == STATE_PREP_SLEEP){
                appPowerState = nextState;   
            }
            break;
        }
        /* PREP_SLEEP valid next states: WAIT_FOR_SLEEP */
        case STATE_PREP_SLEEP:{
            if(nextState == STATE_WAIT_FOR_SLEEP){
                appPowerState = nextState;   
            }
            break;
        }
        /* WAIT_FOR_SLEEP valid next states: SLEEP */
        case STATE_WAIT_FOR_SLEEP:{
            if(nextState == STATE_DEEPSLEEP){
                appPowerState = nextState;   
            }
            break;
        }
    }
    /* Return the actual state */
    return appPowerState;
}


/*******************************************************************************
* Function Name: power_processSystemState()
********************************************************************************
* Summary:
*   handles the usb events. System is woken up the wakeup flag, and put to sleep
*   by the sleep flag. These need to be seperate as the HFCLK (which controls
*   the debouncer) is turned off in deep sleep
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void power_processSystemState(void) {
    /* Disable interrupts */
    uint8 interrupts = CyEnterCriticalSection();
    /* Get the current state */
    APP_POWER_STATE_T currentState = power_getSystemState();
    /* handle wakeup */
    if(currentState == STATE_WAKEUP ) {
        /* Wakeup from Sleep */
        // TODO: wake up blocks
        /* Set active */
        power_setSystemState(STATE_ACTIVE);
    } else if (currentState == STATE_PREP_SLEEP) {
        /* Put Device to sleep  */
        // TODO: put blocks to sleep
        /* Advance to next state */
        power_setSystemState(STATE_WAIT_FOR_SLEEP);
    } else if (currentState == STATE_WAIT_FOR_SLEEP) {
        /* Get the state of the BLE Sub-System (BLESS) */
        CYBLE_BLESS_STATE_T bleState = CyBle_GetBleSsState();
        if(bleState == CYBLE_BLESS_STATE_DEEPSLEEP) {
            /* State to wakeup in */
            power_setSystemState(STATE_DEEPSLEEP);
            /* Put to sleep */
            CySysPmDeepSleep();
            /* RESUME HERE on wakeup */
        }
    }
    /* Enable interrupts */
    CyExitCriticalSection(interrupts);
}
/* [] END OF FILE */
