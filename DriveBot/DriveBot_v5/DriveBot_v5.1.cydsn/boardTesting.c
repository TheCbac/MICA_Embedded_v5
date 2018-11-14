/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: boardTesting.c
* Workspace: DriveBot_v5
* Project: DriveBot_v5.1
* Version: v5.1.0
* Authors: Craig Cheney, Scott McCuen
* 
* PCB: DriveBot MCU v5.1.0
* PSoC: CY8C4245LQI-483
*
* Brief:
*  Source code for testing the various features of the drivebot
* 
* 2018.04.10 CC - Document Created
********************************************************************************/
#include "boardTesting.h"
#include <stdbool.h>

#define BTN_THRESHOLD       (4u) /**< Number of time to press btns before action */
/* Local varibles */
volatile uint32 btnPressCount = ZERO;   /**< Number of times button has been pressed */
volatile uint32 btnReleaseCount = ZERO; /**< Number of times button has been released */
volatile bool btnPressed = false;       /**< Whether or not a button has been pressed since last check */
/* Interrupt Service Routines */
CY_ISR_PROTO(ISR_testButton_press);
CY_ISR_PROTO(ISR_testButton_release);

/*******************************************************************************
* ISR Name: ISR_testButton_press()
********************************************************************************
* Summary:
*  Triggers whenever user presses the button. Toggles the red led's state,
*  turn the green led on. 
*
* Interrupt: 
*       button_interrupt_press
*
*******************************************************************************/
CY_ISR(ISR_testButton_press){
    /* Clear Interrupt */
    button_pin_ClearInterrupt();
    /* Log the Press */
    btnPressCount++;
    /* Register that a button was pressed */
    btnPressed = true; 
}

/*******************************************************************************
* ISR Name: ISR_testButton_release()
********************************************************************************
* Summary:
*  Triggers whenever user releases the button. Turns the green led off 
*
* Interrupt: 
*       button_interrupt_release
*
*******************************************************************************/
CY_ISR(ISR_testButton_release){
    /* Clear Interrupt */
    button_pin_ClearInterrupt();
    /* Log the release */
    btnReleaseCount++;
}

/*******************************************************************************
* Function Name: boardTest_getPressCount()
********************************************************************************
* Summary: 
*  Checks to see if there has been a button press since the last check
*
* Return:
*   Number of Button presses
*
*******************************************************************************/
uint32 boardTest_getPressCount(void){
    /* Return number of counts */
    return btnPressCount;
    
}  

/*******************************************************************************
* Function Name: boardTest_getReleaseCount()
********************************************************************************
* Summary: 
*  Checks to see if there has been a button press since the last check
*
* Return:
*   Number of Button presses
*
*******************************************************************************/
uint32 boardTest_getReleaseCount(void){
    /* Return number of counts */
    return btnReleaseCount;
    
}  

/*******************************************************************************
* Function Name: boardTest_resetBtnCounts()
********************************************************************************
* Summary: 
*  Checks to see if there has been a button press since the last check
*
* Return:
*   Number of Button presses
*
*******************************************************************************/
void boardTest_resetBtnCounts(void){
    /* Disable interrupts */
    uint8 ints = CyEnterCriticalSection();
    /* Reset counts */
    btnPressCount = ZERO;
    btnReleaseCount = ZERO;
    /* Enable interrupts */
    CyExitCriticalSection(ints);
}  

/*******************************************************************************
* Function Name: boardTest_wasButtonPressed()
********************************************************************************
* Summary: 
*  Checks if the button pressed flag is true and resets it
*
* Return:
*   True if the button has been pressed since the last check
*
*******************************************************************************/
bool boardTest_wasButtonPressed(void){
    /* Disable interrupts */
    uint8 ints = CyEnterCriticalSection();
    /* Check if pressed */
    bool pressed = btnPressed;
    /* Reset the flag */
    btnPressed = false;
    /* Enable interrupts */
    CyExitCriticalSection(ints);
    /* Return if the button was pressed */
    return pressed;
    
}  

/*******************************************************************************
* Function Name: boardTest_EnableBtnInterrupts()
********************************************************************************
* Summary: 
*  Checks to see if there has been a button press since the last check
*
* Return:
*   Number of Button presses
*
*******************************************************************************/
void boardTest_EnableBtnInterrupts(void){
    /* Initialize interrupt vectors */
    button_press_interrupt_StartEx(ISR_testButton_press); 
    button_release_interrupt_StartEx(ISR_testButton_release); 
}  


/* ****************************** UART  ***************************** */


///*******************************************************************************
//* Function Name: boardTest_usbEcho()
//********************************************************************************
//* Summary: 
//*  Poll the USB UART. If character present, echo, and toggle blue LED.
//*
//* Return:
//*   None
//*
//*******************************************************************************/
//void boardTest_usbEcho(void){
//    /* Check the buffer for data */
//    char data = UART_USB_UartGetChar();
//    if(data) {
//        /* Echo Data back */
//        UART_USB_UartPutChar(data);   
//        /* Toggle the Blue led */
//        LEDS_B_Toggle();  
//    }
//}    


/* ****************************** Encoders  ***************************** */


/*******************************************************************************
* Function Name: boardTest_getEncoderCounts()
********************************************************************************
* Summary: 
*  Places the encoder count into the pointer passed in
*
* Return:
*   None
*
*******************************************************************************/
void boardTest_getEncoderCounts(encoder_T * encoderPtr) {
    /* Read the encoders and write to the pointer passed in */
    encoderPtr->leftCount = QuadDec_L_ReadCounter();
    encoderPtr->rightCount = QuadDec_R_ReadCounter();
}

/*******************************************************************************
* Function Name: boardTest_resetEncoderCounts()
********************************************************************************
* Summary: 
*  Resets the hardware encoder values and the encoder values passed in
*
* Return:
*   None
*
*******************************************************************************/
void boardTest_resetEncoderCounts(encoder_T * encoderPtr) {
    /* Reset hardware counter */
    QuadDec_L_WriteCounter(ZERO);
    QuadDec_R_WriteCounter(ZERO);  
    /* Reset the local count */
    encoderPtr->leftCount = ZERO;
    encoderPtr->rightCount = ZERO;
}

/* [] END OF FILE */
