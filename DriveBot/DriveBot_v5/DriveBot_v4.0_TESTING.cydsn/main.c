/***************************************************************************
*                                 MICA  © 2017
*                           MIT BioInstrumentation Lab
*
* File: main.c
* Workspace: DriveBot_v4
* Project: DriveBot_v4.0
* Version: v1.0.0
* Authors: Craig Cheney, Scott McCuen
* 
* PCB: DriveBot MCU v1.0.0
* PSoC: CY8C4245LQI-483
*
* Brief:
*   This is the top-level application file for the DriveBot microcontoller. It
*   is responsible for receiving commands from the IMU Cube over the UART, and 
*   controlling the Drivebot based on these commands. It outputs the velocity
*   data to IMU, which streams it to the host device. 
* 
* 2017.08.13 SM - Add motor debugging
* 2017.08.11 CC - Document Created
********************************************************************************/
#include "main.h"
#include "common.h"
#include "boardTesting.h"

/*  -------------- DEBUGGING --------------
* Uncomment MICA_DEBUG to enable ONE of the below Debug levels
*/
#define MICA_TEST

/* -------------- DEBUG LEVEL --------------
* Uncomment ONE of the following
* Debugging will only occur when MICA_DEBUG is defined
*/
//#define MICA_TEST_LEDS  /* Cycle through the RGB led */
#define MICA_TEST_BTNS /* Test the user Button */
//#define MICA_TEST_PWM
//#define MICA_TEST_MOTORS
//#define MICA_TEST_QUAD_DEC
/* -------------- END DEBUG CONFIG -------------- */

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
    
     /* %%%%%%%%%%%%%%%%%% Begin Debugging %%%%%%%%%%%%%%%%%% */
#ifdef MICA_TEST
    #ifdef MICA_TEST_LEDS /*  Cycle through the RGB led */
       /* Expected Outcome: 
        0. All LEDs off
        1. Red LED solid on
        2. Green LED solid on
        3. Blue LED solid on
        4. RGB (White) LEDs on */
        test_DebugLeds();
    #elif defined MICA_TEST_BTNS /* Test the user Button */
        /* Expected Outcome: 
        0. Blue LED on
        1. Button Press
        2. Green LED on 
        3. Button Release*/
        test_button();
    #elif defined MICA_TEST_MOTORS /* Testing the motors */
        /* Enable global interrupts */
        CyGlobalIntEnable; 
        /* Assign Button ISR vector */
        button_interrupt_StartEx(ISR_button);
        led_R_pin_Write(LED_OFF);
        led_G_pin_Write(LED_OFF);
        led_B_pin_Write(LED_ON);
        
        PWM_L_Init();
        PWM_R_Init();
        PWM_L_Start();
        PWM_R_Start();
        PWM_L_WriteCompare(200);
        PWM_R_WriteCompare(200);
        
        motorControlReg_Write(MICA_MOTOR_RIGHT_FORWARD | MICA_MOTOR_LEFT_FORWARD );
        
    #elif defined MICA_TEST_PWM
        
    
    #elif defined MICA_TEST_QUAD_DEC
        led_DB_pin_Write(LED_ON);
        led_R_pin_Write(LED_OFF);
        led_G_pin_Write(LED_OFF);
        led_B_pin_Write(LED_OFF);
        /* Enable global interrupts */
        CyGlobalIntEnable;
        /* Assign Button and QDL ISR vectors */
        button_interrupt_StartEx(ISR_button);
        QDL_interrupt_StartEx(ISR_QDL);
        
        
        QuadDec_L_Start();
        
        QuadDec_L_TriggerCommand(QuadDec_L_MASK, QuadDec_L_CMD_RELOAD);
        
        PWM_L_Init();
        PWM_L_Start();
        PWM_L_WriteCompare(127);
        motorControlReg_Write(0b00000000);
    #else
        #error "MICA_DEBUG_<case> must be defined if MICA_DEBUG is defined"
    #endif /* MICA_DEBUG_<case> */
#endif /* MICA_DEBUG */
    /* %%%%%%%%%%%%%%%%%% End Debugging %%%%%%%%%%%%%%%%%% */

    /* Infinite Loop */
    for(;;)
    {
        /* See interrupt service routines */

    }
}

/*******************************************************************************
* ISR Name: ISR_button()
********************************************************************************
* Summary:
*        Triggers whenever user presses the button 
*
* Interrupt: 
*       button_interrupt
*
* Use:
*       <Use case for ISR_isrName> 
*******************************************************************************/
CY_ISR(ISR_button){
    /* Clear Interrupt */
    button_pin_ClearInterrupt();
    /* Toggle LED */
    led_R_pin_Write( ~led_R_pin_Read() );
    /* toggle LED on DB */
    led_DB_pin_Write( ~led_DB_pin_Read() );
    /* Read the current state of the control reg */
    uint8 motorState = motorControlReg_Read();
    /* Toggle the enable bits */
    motorState ^= (MICA_MOTOR_LEFT_EN | MICA_MOTOR_RIGHT_EN);
    /* Write the new state */
    motorControlReg_Write( motorState );
}

/*******************************************************************************
* ISR Name: ISR_QDL()
********************************************************************************
* Summary:
*        Triggers whenever the quadrature decoder counter exceeds the compare value 
*
* Interrupt: 
*       QDL_interrupt
*
* Use:
*       <Use case for ISR_isrName> 
*******************************************************************************/
CY_ISR(ISR_QDL){
    /* Clear Interrupt */
    QuadDec_L_ClearInterrupt(QuadDec_L_INTR_MASK_CC_MATCH);
    /* Toggle LED */
    led_B_pin_Write(LED_ON);
    CyDelay(500);
    led_B_pin_Write(LED_OFF);
}
    

/* [] END OF FILE */