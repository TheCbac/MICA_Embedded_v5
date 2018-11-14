/***************************************************************************
*                                 MICA  © 2018
*                           MIT BioInstrumentation Lab
*
* File: main.c
* Workspace: DriveBot_v5
* Project: DriveBot_v5.1
* Version: v5.1.0
* Authors: Craig Cheney, Scott McCuen
* 
* PCB: DriveBot MCU v5.1.0
* PSoC: CY8C4245LQI-483
*
* Brief:
*   This is the top-level application file for the DriveBot microcontoller. It
*   is responsible for receiving commands from the IMU Cube over the UART, and 
*   controlling the Drivebot based on these commands. It outputs the velocity
*   data to IMU, which streams it to the host device. 
* 
* 2018.04.10 CC - Upgrade to v5. Switch to MICA Creator components from libMica
* 2017.08.25 CC - Add libMica
* 2017.08.13 SM - Add motor debugging
* 2017.08.11 CC - Document Created
********************************************************************************/
#include "project.h"
#include "boardTesting.h"
#include <stdbool.h>

/*  -------------- DEBUGGING --------------
* Uncomment MICA_DEBUG to enable general debugging.
* Uncomment MICA_DEBUG_<case> below to
* enable ONE of the debug levels.
*/
#define MICA_DEBUG

/* -------------- DEBUG LEVEL --------------
* Uncomment ONE of the following
* Debugging will only occur when MICA_DEBUG is defined
*/
//#define MICA_DEBUG_LEDS           /* Test the onboard RGB LED */
//#define MICA_DEBUG_UART_TX        /* Send data through the UART to test connectivity */
//#define MICA_DEBUG_ECHO           /* Echo to the USB UART */
//#define MICA_DEBUG_RELAY_IMU      /* Relay data received from the IMU to the USB */
//#define MICA_DEBUG_RELAY_IMU_LOG      /* Relay data received from the IMU to the USB but convert to ascii first */
//#define MICA_DEBUG_BTN            /* Check the user button */
//#define MICA_DEBUG_BTN_INT        /* Interrupt based Button testing */
#define MICA_DEBUG_ENCODER        /* Debug the encoders */
//#define MICA_DEBUG_MOTORS_BTN     /* Check the motors */
//#define MCIA_DEBUG_MOTORS_DIR     /* Check the direction of the motors */
//#define MICA_DEBUG_FULL           /* Run a composite set of debugging functions */

/* -------------- END DEBUG CONFIG --------------  */



/* --------- Local Variables --------- */

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


    /* Enable global interrupts */
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
        DEBUG_UART_Start();
        /* Turn on the Green LED */
        LEDS_Write(LEDS_ON_GREEN);
        /* Loop count */
        uint8 i = ZERO;
        /* Run the transmit test indefinitely */
        for(;;){
            /* Log, if eneabled */
            debugUart_log("Log: %x ", i++);
            /* Display the compile date and time */
            debugUart_txTest(MICA_TEST_ONCE);
            /* Toggle Blue LED */
            LEDS_B_Toggle();
        }
    /* End MICA_DEBUG_UART_TX */
    #elif (defined MICA_DEBUG_ECHO)
        /* Echo by polling the USB UART. Excpected outcome:
        *  0. Byte written via USB are echoed back
        *  1. Blue Led Toggles on each key press */
        LEDS_Write(LEDS_ON_GREEN);
        /* Start the component */
        UART_USB_Start();
        /* Infinite Loop */
        uint8 i = ZERO;
        for(;;) {
            /* Check the buffer for data */
            char data = usbUart_getChar();
            if(data) {
                /* Echo Data back */
                usbUart_putChar(data);
                /* Log if enabled */
                usbUart_log("\r\nThis is a log:%x\r\n", i++);
                /* Toggle the Blue led */
                LEDS_B_Toggle();  
            }
        }
    /* End MICA_DEBUG_ECHO */
    #elif defined MICA_DEBUG_RELAY_IMU
        /* Relay data received from the IMU to the USB */
        /* Expected outcome: 
        * 1. Data received from the IMU over
        * the UART should be transmitted out the debugUart
        * 2. Green LED on
        * 3. Blue LED toggles with each received byte
        */
        LEDS_Write(LEDS_ON_GREEN);
        /* Start the component */
        UART_Start();
        DEBUG_UART_Start();
        for(;;) {
            /* Check the buffer for data */
            char data = UART_UartGetChar();
            if(data) {
                /* Echo Data back */
                DEBUG_UART_PutChar(data);
                /* Toggle the Blue led */
                LEDS_B_Toggle();  
            }
        }
    /* End MICA_DEBUG_RELAY_IMU */
    #elif defined MICA_DEBUG_BTN
        /* Test the user push button
        Expected outcome: 
        0. Green LED on
        1. User depresses button
        2. R+G LEDs on 
        3. User releases button
        4. Green LED on */
        LEDS_G_Write(LEDS_ON);
        /* Infinite Loop */
        for(;;) {
            /* Test the user push button */
            bool btnPressed = button_pin_Read();
            /* Check if Btn was pressed */
            if (btnPressed) {
                /* R+G on if pressed */
                LEDS_R_Write(LEDS_ON);
            } else{
                /* G on if not pressed */
                LEDS_R_Write(LEDS_OFF);
            }
        }
    /* end MICA_DEBUG_BTN */
    #elif defined MICA_DEBUG_BTN_INT
        /* Test the user Button with an interrupt and debouncer
        Expected outcome: 
        0. Green LED on
        1. User depresses button
        2. R+G LEDs on 
        3. User releases button
        4. Green LED on */
        
        LEDS_G_Write(LEDS_ON);
        /* Initialize Button interrupts */
        boardTest_EnableBtnInterrupts();
        
        /* Enable global interrupts */
        CyGlobalIntEnable;
        /* Infinite loop */
        for(;;) {
            /* Check if the button was pressed */
            if(boardTest_wasButtonPressed()) {
                /* toggle Red LED state */
                LEDS_R_Toggle(); 
            }
        }
    /* end MICA_DEBUG_BTN_INT */   
    #elif defined MICA_DEBUG_ENCODER
        /* Test the Encoders */
        DEBUG_UART_Start();
        QuadDec_L_Start();
        QuadDec_R_Start();
        /* Enable Button interrupts */
        boardTest_EnableBtnInterrupts();
        /* Initial state */
        LEDS_G_Write(LEDS_ON);
        /* Infinite loop */
        for(;;) {
            /* Get the encoder values */
            encoder_T encoder;
            boardTest_getEncoderCounts(&encoder);
            /* Clear the screen */
            iprintf_clearScreen();
            /* Write out the values */
            iprintf("Left: %d\r\nRight: %d\r\n", encoder.leftCount, encoder.rightCount);
            /* If the button is pressed, reset the counters */
            bool btnPressed = boardTest_wasButtonPressed();
            if(btnPressed) {
                /* Reset the encoder values */
                boardTest_resetEncoderCounts(&encoder);
                /* Toggle the red led */
                LEDS_R_Toggle();
            }
            /* Loop delay */
            CyDelay(TEST_DELAY_LOOP);
        }
        /* end MICA_DEBUG_ENCODER */
    #elif defined MICA_DEBUG_MOTORS_BTN
        /* Test the motors */
        /* Start the Components */
        UART_USB_Start();
        QuadDec_L_Start();
        QuadDec_R_Start();
        /* start the motor controller */
        Motors_Start();
        /* Turn off motors */
        Motors_Disable();
        /* Start the buttons */
        boardTest_EnableBtnInterrupts();
        /* Initial state */
        LEDS_Write(LEDS_ON_GREEN);
        /* Track the state */
        bool running = false;
        /* Infinite loop */
        for(;;) {
            /* See if the button was pressed */
            if(boardTest_wasButtonPressed()) {
                if(running) {
                    /* Stop motors */
                    Motors_Disable();
                } else {
                    /* Start Motors */
                    Motors_Enable();
                }
                /* Toggle LED */
                LEDS_B_Toggle();
                /* Change state */
                running = !running;
            }
            /* Delay */
            CyDelay(TEST_DELAY_LOOP);
            /* Get the encoder values */
            encoder_T encoder;
            boardTest_getEncoderCounts(&encoder);
            /* Clear the screen */
            iprintf_clearScreen();
            /* Write out the values */
            iprintf("Left: %d\r\nRight: %d\r\n", encoder.leftCount, encoder.rightCount);
        }
        /* End MICA_DEBUG_MOTORS_BTN */
    #elif defined MCIA_DEBUG_MOTORS_DIR
        /* Expected outcome:
        Loop through directions: 
            Forward, reverse, CW, CCW
        With respective LEDS:
            Blue, Red, White
        Each at resp three different speeds:
            Slow, Medium, Fast
        */
        /* Test the directionality of the motors */
        LEDS_G_Write(LEDS_ON);
        /* Start (but don't enable) the motors */
        Motors_Start();
        /* Infinite loop */
        for(;;){
            /* Run through the motor test */
            LEDS_Write(LEDS_ON_BLUE);
            Motors_Test(TWO, Motors_SPEED_SLOW);
            
            LEDS_Write(LEDS_ON_RED);
            Motors_Test(TWO, Motors_SPEED_HALF);

            LEDS_Write(LEDS_ON_WHITE);
            Motors_Test(TWO, Motors_SPEED_MAX);
            
        }
    #elif defined MICA_DEBUG_FULL
        /* Test all of the board functions:
        1. USB UART Display Encoder values
        2. Encoder values change based on wheels
        3. Button Pressed both LEDs and reset encoder
        */
       
        /* Initialize Button interrupts */
        boardTest_EnableBtnInterrupts();
        CyGlobalIntEnable;
        /* Start hardware blocks */
        UART_USB_Start();
        QuadDec_L_Start();
        QuadDec_R_Start();
        /* Initial State */
        LEDS_Write(LEDS_ON_GREEN);
        /* Infinite Loop */
        for(;;){
            /* Read encoder values and write out on UART */
            boardTest_encoders();   
            
            
            /* Check for button pressing */
            bool btnPressed = boardTest_pushBtn();
            if(btnPressed) {
                /* Reset both encoder counters */
                QuadDec_L_WriteCounter(ZERO);
                QuadDec_R_WriteCounter(ZERO);   
                /* Toggle state of Red LED */
                LEDS_R_Toggle();
                 /* Toggle state of the drivebot baseboard LED */
                led_DB_pin_Write( ~ led_DB_pin_Read());
            }
            /* Loop delay */
            CyDelay(TEST_DELAY_LOOP);
        }
    /* End MICA_DEBUG_FULL */
    #else
        #error "Exactly ONE MICA_DEBUG_<case> must be defined if MICA_DEBUG is defined"
    #endif /* MICA_DEBUG_<case> */
#endif /* MICA_DEBUG */
/* %%%%%%%%%%%%%%%%%%  End Debugging  %%%%%%%%%%%%%%%%%% */
    
    /* Infinite Loop */
    for(;;)
    {

    }
}

/*******************************************************************************
* Function Name: processUartPacket()
********************************************************************************
* Summary:
*   Processes the data passed in by the UART, and acts accordingly 
*
* Parameters:
*   packet - Array of the data is stored
*   len - Length of the array, including the checksum
*
* Return:
*   None
*
*******************************************************************************/
 

/* [] END OF FILE */