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
* 2018.07.25 CC - Abstract out "boardTesting.c"
* 2018.04.10 CC - Upgrade to v5. Switch to MICA Creator components from libMica
* 2017.08.25 CC - Add libMica
* 2017.08.13 SM - Add motor debugging
* 2017.08.11 CC - Document Created
********************************************************************************/
#include "project.h"
#include <stdbool.h>
#include <stdio.h>
#include "packet_testing.h"
#include "testRunner.h"

/*  -------------- DEBUGGING --------------
* Uncomment MICA_DEBUG to enable general debugging.
* Uncomment MICA_DEBUG_<case> below to
* enable ONE of the debug levels.
*/
#define MICA_DEBUG

/* -------------- DEBUG CASE --------------
* Uncomment ONE of the following
* Debugging will only occur when MICA_DEBUG is defined
*/
#ifdef MICA_DEBUG
//    #define MICA_DEBUG_LEDS           /* Test the onboard RGB LED */
//    #define MICA_UART_USB_TX        /* Send data through the UART to test connectivity */
//    #define MICA_DEBUG_ECHO           /* Echo to the USB UART */
//    #define MICA_DEBUG_RELAY_IMU      /* Relay data received from the IMU to the USB */
//    #define MICA_DEBUG_BTN            /* Check the user button */
//    #define MICA_DEBUG_BTN_INT        /* Interrupt based Button testing */
//    #define MICA_DEBUG_BTN_INT_CUSTOM   /* Test the custom interrupt for the button */
//    #define MICA_DEBUG_ENCODER        /* Debug the encoders */
//    #define MICA_DEBUG_MOTORS_BTN     /* Check the motors */
    #define MCIA_DEBUG_MOTORS_DIR     /* Check the direction of the motors */
//    #define MICA_DEBUG_FULL           /* Run a composite set of debugging functions */
#endif

/* -------------- END DEBUG CASE --------------  */

/*  -------------- Testing --------------
* Uncomment MICA_TEST to enable specific tests.
* Uncomment MICA_TEST_<case> below to
* enable ONE of the test levels. */

//#define MICA_TEST

/* -------------- TEST LEVEL --------------
* Uncomment ONE of the following
* Testing will only occur when MICA_TEST is defined
*/
#ifdef MICA_TEST
//    #define MICA_TEST_DIRECTION_CHANGE  /* See how fast/torqy the accelration change is */
//    #define MICA_TEST_PID_SPEED         /* Simple control loop set speed */
//    #define MICA_TEST_PACKETS_SELF         /* Self test (no UART) of the packets */
//    #define MICA_TEST_MALLOC            /* Test heap memory allocation */
//    #define MICA_TEST_PACKET_SPAWN            /*spawning packets */
//    #define MICA_TEST_PACKETS_ERRORS       /* Test various error on packts */
    #define MICA_TEST_PACKETS           /* Test Packet communication */
//    #define MICA_TEST_PACKETS_ISR        /* Receive packets from IMU via interrupt and print results */
#endif
/* -------------- END TEST LEVEL --------------  */

/* --------- Local Variables --------- */

void ISR_imuUart(void);
void ISR_Timer(void);
void ISR_toggleMotorState(void);
void ISR_toggleBtnTest(void);
/* State variables */
volatile bool flag_pendingRxByte = false;
volatile bool motorsState = false;
volatile bool timerExpired = false;
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
    #elif defined MICA_UART_USB_TX
        /* Test the Transmit portion of the UART
        Expected outcome:
        UART constantly writes out the compile date 
        and time. 
        */
        /* Start the UART */
        UART_USB_Start();
        /* Turn on the Green LED */
        LEDS_Write(LEDS_ON_GREEN);
        /* Loop count */
        uint8 i = ZERO;
        /* Run the transmit test indefinitely */
        for(;;){
            /* Log, if eneabled */
            usbUart_log("Log: %x ", i++);
            /* Display the compile date and time */
            usbUart_txTest(MICA_TEST_ONCE);
            /* Toggle Blue LED */
            LEDS_B_Toggle();
        }
    /* End MICA_UART_USB_TX */
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
        * the UART should be transmitted out the usbUart
        * 2. Green LED on
        * 3. Blue LED toggles with each received byte
        */
        LEDS_Write(LEDS_ON_GREEN);
        /* Start the component */
        UART_IMU_Start();
        UART_USB_Start();
        for(;;) {
            /* Check the buffer for data */
            char data = imuUart_getChar();
            if(data) {
                /* Echo Data back */
                usbUart_putChar(data);
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
        1. User clicks button
        2. R+G LEDs on 
        3. User clicks button
        4. Green LED on */
        
        LEDS_G_Write(LEDS_ON);
        /* Initialize Button interrupts */
        Button_EnableBtnInterrupts();
        /* Enable global interrupts */
        CyGlobalIntEnable;
        /* Infinite loop */
        for(;;) {
            /* Check if the button was pressed */
            if(Button_wasButtonPressed()) {
                /* toggle Red LED state */
                LEDS_R_Toggle(); 
            }
        }
    /* end MICA_DEBUG_BTN_INT */   
    #elif defined MICA_DEBUG_BTN_INT_CUSTOM
        /* Test the custom ISR
        Expected outcome: 
        0. Green LED on
        1. User clicks button
        2. B+G LEDs on 
        3. User clicks button
        4. Green LED on */
        LEDS_G_Write(LEDS_ON);
        /* Initialize Button interrupts */
        Button_SetBtnPressIsr(ISR_toggleBtnTest);
        Button_EnableBtnInterrupts();
        /* Enable global interrupts */
        CyGlobalIntEnable;
        /* Infinite loop */
        for(;;) {
            /* Everything handled in interrupt */
        }
    /* End  MICA_DEBUG_BTN_INT_CUSTOM*/
    #elif defined MICA_DEBUG_ENCODER
        /* Test the Encoders
        Expected outcome: 
        0. Green LED on
        1. Encoder value is printed over USB
        2. User clicks button
        3. Encoder counts reset
        4. Red LED toggles*/
        /* Start components */
        UART_USB_Start();
        dualEncoder_Start();
        /* Enable Button interrupts */
        Button_EnableBtnInterrupts();
        
        /* Initial state */
        LEDS_G_Write(LEDS_ON);
        /* Infinite loop */
        for(;;) {
            /* Get the encoder values */
            ENCODER_DUAL_T encoder;
            dualEncoder_getEncoderCounts(&encoder);
            /* Clear the screen */
            usbUart_clearScreen();
            /* Write out the values */
            usbUart_print("Left: %d\r\nRight: %d\r\n", encoder.leftCount, encoder.rightCount);
            /* If the button is pressed, reset the counters */
            bool btnPressed = Button_wasButtonPressed();
            if(btnPressed) {
                /* Reset the encoder values */
                dualEncoder_resetEncoderCounts(&encoder);
                /* Toggle the red led */
                LEDS_R_Toggle();
            }
            /* Loop delay */
            CyDelay(TEST_DELAY_LOOP);
        }
        /* end MICA_DEBUG_ENCODER */
    #elif defined MICA_DEBUG_MOTORS_BTN
        /* Test the motors
        Expected outcome: 
        0. Green LED on
        1. Encoder counts printed over the USB UART
        2. User clicks button
        3. Motors Enable (disable)
        4. Blue LED toggles*/
        /* Start the Components */
        UART_USB_Start();
        dualEncoder_Start();
        /* start the motor controller */
        Motors_Start();
        /* Turn off motors */
        Motors_Disable();
        /* Start the buttons */
        Button_EnableBtnInterrupts();
        /* Initial state */
        LEDS_Write(LEDS_ON_GREEN);
        /* Set initial state */
        bool running = false;
        Motors_Move(Motors_DIRECTION_FORWARD, Motors_SPEED_HALF);
        /* Infinite loop */
        for(;;) {
            /* See if the button was pressed */
            if(Button_wasButtonPressed()) {
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
            ENCODER_DUAL_T encoder;
            dualEncoder_getEncoderCounts(&encoder);
            /* Clear the screen */
            usbUart_clearScreen();
            /* Write out the values */
            usbUart_print("Left: %d\r\nRight: %d\r\n", encoder.leftCount, encoder.rightCount);
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

        CyGlobalIntEnable;
        /* Start hardware blocks */
        UART_USB_Start();
        dualEncoder_Start();
        Motors_Start();
        Button_EnableBtnInterrupts();
        /* Initial State */
        LEDS_Write(LEDS_ON_GREEN);
        bool motorState = false;
        Motors_DIRECTION_T direction = Motors_FORWARD;
        /* Infinite Loop */
        for(;;){
            /* variables for storing encoder state */
            ENCODER_DUAL_T encoders;
            /* Read encoder values */
            dualEncoder_getEncoderCounts(&encoders);   
            /* Update screen */
            usbUart_clearScreen();
            usbUart_print("Left: %d\r\nRight: %d\r\n", encoders.leftCount, encoders.rightCount);
            
            /* Check for button pressing */
            bool btnPressed = Button_wasButtonPressed();
            if(btnPressed) {
                /* Toggle state of Red LED */
                LEDS_R_Toggle();
                LEDS_B_Toggle();
                 /* Toggle state of the drivebot baseboard LED */
                led_DB_pin_Write( ~ led_DB_pin_Read());
                /* toggle state of motor */
                motorState = !motorState;
                /* Change direction */
                if(motorState == false){
                    if(direction == Motors_FORWARD){
                        direction = Motors_BACKWARD;
                    } else {
                        direction = Motors_FORWARD;   
                    }
                }
                /* Update new values */
                Motors_SetState(motorState);
                Motors_Move(direction, Motors_SPEED_MAX);
            }
            /* Loop delay */
            MICA_delayMs(MICA_DELAY_MS_SEC_TENTH);
        }
    /* End MICA_DEBUG_FULL */
    #else
        #error "At least ONE MICA_DEBUG_<case> must be defined if MICA_DEBUG is defined"
    #endif /* MICA_DEBUG_<case> */
#endif /* MICA_DEBUG */
#ifdef MICA_TEST
    #if defined MICA_TEST_DIRECTION_CHANGE
        /* Investigate if the motors can flip up drivebot */
        /* Initialize Button interrupts */
        boardTest_EnableBtnInterrupts();
        /* ISR to call when button is pressed */
        boardTest_SetBtnPressIsr( ISR_toggleMotorState );
        CyGlobalIntEnable;
        /* Start hardware blocks */
        Motors_Start();
        LEDS_Write(LEDS_ON_GREEN);

        /* Infinite Loop */
        for(;;){
            if(motorsState){
                /* Forwards */
                LEDS_Write(LEDS_ON_CYAN);
                Motors_Move(Motors_FORWARD, Motors_SPEED_MAX);
                MICA_delayMs(600);
                /* Back */
                LEDS_Write(LEDS_ON_YELLOW);
                Motors_Move(Motors_BACKWARD, Motors_SPEED_MAX);
                MICA_delayMs(600);
            } else {
                /* Idle mode */
                LEDS_Write(LEDS_ON_GREEN);   
            }
        }
        /* End MICA_TEST_DIRECTION_CHANGE */
    #elif defined MICA_TEST_PID_SPEED
        /* Test a simple PID loop */
        /* Initialize Button interrupts */
        boardTest_EnableBtnInterrupts();
        
        /* Assign Interrupt vectors */
        boardTest_SetBtnPressIsr( ISR_toggleMotorState );
        timer_interrupt_StartEx(ISR_Timer);
        CyGlobalIntEnable;
        /* Start hardware blocks */
        Motors_Start();
        QuadDec_L_Start();
        QuadDec_R_Start();
        UART_USB_Start();
        SystemTimer_Start();
        /* Set the timer count (down counter) */
        SystemTimer_WritePeriod(25000);
        /* Initial LED state */
        LEDS_Write(LEDS_ON_GREEN);


        /* Infinite Loop */
        for(;;){
            if(timerExpired){
                /* Clear the flag */
                timerExpired = false;
                /* Get the encoder values */
                encoder_T encoder;
                boardTest_getEncoderCounts(&encoder);
                /* Clear the screen */
                usbUart_clearScreen();
                /* Write out the values */
                usbUart_print("Left: %d\r\nRight: %d\r\n", encoder.leftCount, encoder.rightCount);
                /* Toggle Red led */
                LEDS_R_Toggle();
            }

            
        }
    /* End MICA_TEST_PID_SPEED */
    #elif defined MICA_TEST_PACKETS_SELF
        /* Test packing and unpacking a mica packet */
        LEDS_Write(LEDS_ON_GREEN);
        UART_USB_Start();
        usbUart_clearScreen();
        /* Create a packet object and generate buffers */
        packets_BUFFER_FULL_S packetBuffer;
        /* Generate a new buffer */
        uint32 error = packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);    
        /* Halt on error */
        if(error){
            LEDS_Write(LEDS_ON_RED);
            while(true){}
        }
        
        /* Define dummy data */
        #define DATA_LEN 0x03
        uint8 dummyData[DATA_LEN] = {0x01, 0x03, 0x05};
        packetBuffer.send.packet.moduleId = 5;
        packetBuffer.send.packet.cmd = 0xCC;
        packetBuffer.send.packet.payload = dummyData;
        packetBuffer.send.packet.payloadLen = DATA_LEN;
        
        /* Send data to txProcessBuffer */
        packets_createPacket(&packetBuffer);
        /* "Transfer" the data */
        uint16_t j;
        for(j=ZERO; j < packetBuffer.send.processBuffer.bufferIndex; j++){
            uint8 dataByte = packetBuffer.send.processBuffer.buffer[j];
            packets_processRxByte(&packetBuffer, dataByte);
            usbUart_print("0x%x ", dataByte);
        }

        /* Parse the packet */
        error = packets_parsePacket(&packetBuffer);
        if(error) {
            usbUart_print("\r\nParsing was unsuccessful ");
            if(error == packets_ERR_INCOMPLETE){
                usbUart_print("- Packet was incomplete");
            }
            while(true){}
        }
        
        /* Clear the screen and print out the data */
        packets_PACKET_RECEIVE_S* receivePacket = &(packetBuffer.receive.packet);
        usbUart_print("\r\nModule: %x \r\nCommand: %x\r\nPayload Len: %x\r\n", receivePacket->moduleId, receivePacket->cmd, receivePacket->payloadLen);
        usbUart_print("Payload:");
        uint8 i;
        for(i=ZERO; i< receivePacket->payloadLen; i++){
            usbUart_print(" %x", receivePacket->payload[i]);
        }
        
        /* Infinite loop */
        for(;;) {

        }
    /* End MICA_TEST_PACKETS_SELF */

    #elif defined MICA_TEST_MALLOC            
        /* Test heap memory allocation - Increase heap to 1024 bytes (0x400) */
        #include <stdlib.h>
        #define ARR_LEN 100
        
        /* Start hardware */
        UART_USB_Start();
        LEDS_Write(LEDS_ON_GREEN);
        
    
        /* Loop count */
        uint16 i = ZERO;
        /* Infinite loop */
        for(;;){
            /* Toggle blue led */
            LEDS_B_Toggle();
            /* Allocate from heap */
            uint8 *arrPtr1;
            uint8 *arrPtr2;
            uint8 *arrPtr3;
            
            arrPtr1 = malloc(ARR_LEN);
            arrPtr2 = malloc(ARR_LEN);
            arrPtr3 = malloc(ARR_LEN);
            usbUart_print("%d - 1:0x%x, 2:0x%x, 3:0x%x\r\n", i++, arrPtr1, arrPtr2, arrPtr3);
            free(arrPtr1);
            free(arrPtr2);
            free(arrPtr3);
            
            arrPtr3 = malloc(ARR_LEN);
            arrPtr2 = malloc(ARR_LEN);
            usbUart_print("%d - 1:0x%x, 2:0x%x, 3:0x%x\r\n", i++, arrPtr1, arrPtr2, arrPtr3);
            free(arrPtr2);
            free(arrPtr3);
            
            
            /* Loop delay */
            MICA_delayMs(MICA_DELAY_MS_SEC_ONE);
        }
    /* end MICA_TEST_MALLOC */
    #elif defined MICA_TEST_PACKET_SPAWN
        /* Test packet spawning */
        /* Start the hardware */
        LEDS_Write(LEDS_ON_GREEN);
        UART_USB_Start();
        usbUart_clearScreen();
        /* Create a pointer to a buffer and initialize it */
        packets_BUFFER_FULL_S packetBuffer;
        uint16 i = ZERO;
        /* Infinite loop */
        for(;;) {
            LEDS_B_Toggle();
            /* Generate a new buffer */
            uint32 error = packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);    
            error |= packets_destoryBuffers(&packetBuffer);
        
            /* Check for UART data */
            uint32 data = UART_IMU_UartGetByte();
            /* See if data is available */
            if( !(data & UART_IMU_UART_RX_UNDERFLOW) ){
                /* Process data */
                error |= packets_processRxByte(&packetBuffer, data);
            }
            

            
            usbUart_print("Loop iteration: %d, Addr: %x, Block size: %x Error: %x\r\n", i++, packetBuffer.send.processBuffer.buffer , packets_LEN_BLOCK_PACKET, error);
            /* Red LED on error */
            if(error) {
               LEDS_Write(LEDS_ON_RED);    
                /* Infinite loop */
                while(true){}
            }
            /* Loop Delay */
            MICA_delayMs(MICA_DELAY_MS_SEC_QUARTER);
            
        }
    /* end MICA_TEST_PACKET_SPAWN */
        #elif defined MICA_TEST_PACKETS_ERRORS
        /* Unit tests for MICA Errors */
        LEDS_Write(LEDS_ON_GREEN);
        UART_USB_Start();
        usbUart_clearScreen();

        /* Print Program Header */
        usbUart_printHeader(__TIME__, __DATE__, "      PACKET TESTING UNIT TESTS ");
        
        /* ### Buffer Generate Test Suite ### */
        {
            /* Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            usbUart_print("\r\n*** Buffer generation ***\r\n");
            /* Nominal Case */
            runTest(test_generateBuffers(&packetBuffer, "Normal", packets_LEN_BLOCK_PACKET, packets_ERR_SUCCESS));
            /* Long test */
            runTest(test_generateBuffers(&packetBuffer, "Long", 1000, packets_ERR_MEMORY));
            /* Multiple create */
            packets_generateBuffers(&packetBuffer, 10);
            runTest(test_generateBuffers(&packetBuffer, "Multiple Create", 10, packets_ERR_STATE));
            
            /* Clean Up */
            packets_destoryBuffers(&packetBuffer);
        }
        
        /* ### Buffer Destroy Test Suite ### */
        {
            /* Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            usbUart_print("\r\n*** Buffer Destruction ***\r\n");
            /* Buffer not generated */
            runTest( test_destroyBuffers(&packetBuffer, "Non-generated", packets_ERR_MEMORY));
            /* Normal packet */
            packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
            runTest(test_destroyBuffers(&packetBuffer, "Normal", packets_ERR_SUCCESS));
            /* Too long buffer */
            packets_generateBuffers(&packetBuffer, 2000);
            runTest(test_destroyBuffers(&packetBuffer, "Failed Gen", packets_ERR_MEMORY));
            /* Multiple destroy */
            packets_generateBuffers(&packetBuffer, 10);
            packets_destoryBuffers(&packetBuffer);
            runTest(test_destroyBuffers(&packetBuffer, "Multiple destroy", packets_ERR_MEMORY));
            
            /* Clean Up (Uncessary, here for consistency */
            packets_destoryBuffers(&packetBuffer);
        }
        
        /* ### Memory leak testing ### */
        {
            usbUart_print("\r\n*** Memroy Leak Generation ***\r\n");
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            runTest(test_generateBuffers_iteration(&packetBuffer,"Repeat Generations", 1000));
        }
        
        
        /* ### Packet Creation Test Suite - State Errors ### */
        {
            usbUart_print("\r\n*** Construct Packet Test Suite - State Errors ***\r\n");
            /* Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
            /* Local reference to sending buffer */
            packets_PACKET_S* sendPacket = &(packetBuffer.send.packet); 
            /* Define dummy data */
            #define DATA_LEN 0x03
            uint8 dummyData[DATA_LEN] = {0x01, 0x03, 0x05};
            sendPacket->moduleId = 5;
            sendPacket->cmd = 0xCC;
            sendPacket->payload = dummyData;
            sendPacket->payloadLen = DATA_LEN;
            /* Simple working packet */
            runTest(test_packetCreation_stateErrors(&packetBuffer, "Basic Packet", packets_ERR_SUCCESS));
            /* Incorrect module ID */
            sendPacket->moduleId = 10;
            runTest(test_packetCreation_stateErrors(&packetBuffer, "Module ID: 10", packets_ERR_MODULE));
            /* Module ID 0 */
            sendPacket->moduleId = 0;
            runTest(test_packetCreation_stateErrors(&packetBuffer, "Module ID: 0", packets_ERR_SUCCESS));
            /* No Payload */
            sendPacket->moduleId = 1;
            sendPacket->payloadLen = ZERO;
            runTest(test_packetCreation_stateErrors(&packetBuffer, "No Payload", packets_ERR_SUCCESS));
            /* Long Packet - no memory */
            sendPacket->payloadLen = packets_LEN_BLOCK_PACKET + 1;
            runTest(test_packetCreation_stateErrors(&packetBuffer, "Long Payload - no memory", ( packets_ERR_MEMORY )));
            /* Long Packet - max payload, and memory */
            sendPacket->payloadLen = 1024;
            runTest(test_packetCreation_stateErrors(&packetBuffer, "Long Payload - max payload, memory", (packets_ERR_LENGTH | packets_ERR_MEMORY)));
            
            /* Clean Up */
            packets_destoryBuffers(&packetBuffer);
        }
        
        /* ### Packet Creation Test Suite - Packet Values ### */
        {
            usbUart_print("\r\n*** Construct Packet Test Suite - Packet Values ***\r\n");
            /* Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
            /* Local reference to sending buffer */
            packets_PACKET_S* sendPacket = &(packetBuffer.send.packet); 
            /* Define valid dummy data */
            #define DATA_LEN 0x03
            uint8 dummyData[DATA_LEN] = {0x01, 0x03, 0x05};
            sendPacket->moduleId = 5;
            sendPacket->cmd = 0xCC;
            sendPacket->payload = dummyData;
            sendPacket->payloadLen = DATA_LEN;
            /* Simple working packet */
            uint8 validPacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x03, 0x01, 0x03, 0x05, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x22, 0xAA};
            runTest(test_packetCreation_packetVals(&packetBuffer, "Simple Packet", validPacket)); 
            /* No payload */
            sendPacket->payloadLen = ZERO;
            uint8 noPayloadPacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x2E, 0xAA};
            runTest(test_packetCreation_packetVals(&packetBuffer, "No payload", noPayloadPacket)); 
            /* Reponse Flags */
            sendPacket->flags = packets_FLAG_RESP;
            uint8 respFlagPacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x2D, 0xAA};
            runTest(test_packetCreation_packetVals(&packetBuffer, "Response Flag", respFlagPacket)); 
            /* Response and payload */
            sendPacket->payloadLen = 1;
            dummyData[0] = 0xFF;
            sendPacket->flags = packets_FLAG_RESP;
            uint8 respFlagPacket2[15] = {0x01, 0x05, 0xCC, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x2D, 0xAA};
            runTest(test_packetCreation_packetVals(&packetBuffer, "Response Flag 2", respFlagPacket2)); 
            
            /* Clean Up */
            packets_destoryBuffers(&packetBuffer);
        }
        
        /* ### Send Packet Test Suite ### */
        {
            usbUart_print("\r\n*** Send Packet ***\r\n");
            /* Setup - Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
            /* Basic Packet */
            runTest(test_sendPacket(&packetBuffer, "Send Basic Packet", packets_ERR_SUCCESS));
            runTest(test_sendPacket(&packetBuffer, "Send Basic Packet 2", packets_ERR_SUCCESS));
            
        
            /* Clean Up */
            packets_destoryBuffers(&packetBuffer);
        }
        
        /* ### Process RX Byte Test Suite ### */
        {
            usbUart_print("\r\n*** Process RX Byte - State Errors ***\r\n");
            /* Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            /* No buffers generated */
            uint8 bytes[1] = {packets_SYM_START};
            runTest(test_processRxByte_stateError(&packetBuffer, bytes, 1, "No Buffers", packets_ERR_MEMORY));
            /* Generate buffer */
            packets_generateBuffers(&packetBuffer, 100);
            /* Buffer overflow */
            packetBuffer.receive.processBuffer.bufferIndex = 100;
            bytes[0] = packets_SYM_START;
            runTest(test_processRxByte_stateError(&packetBuffer, bytes, 1, "Buffer overflow", packets_ERR_MEMORY));
            /* Valid Start Byte */
            bytes[0] = packets_SYM_START;
            runTest(test_processRxByte_stateError(&packetBuffer, bytes, 1, "Valid Start", packets_ERR_SUCCESS));
            /* Invalid Start Byte */
            bytes[0] = packets_SYM_START + 1;
            runTest(test_processRxByte_stateError(&packetBuffer, bytes, 1, "invalid Start", packets_ERR_START_SYM));
            
            /* Clean up */
            packets_destoryBuffers(&packetBuffer);
        }
        
        
        /* ### Packet parsing Test Suite - State Errors ### */
        {
            usbUart_print("\r\n*** Packet parsing - State Errors ***\r\n");
            /* Setup - Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer;
            packets_initialize(&packetBuffer);
            packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
            /* Local references */
            packets_BUFFER_PROCESS_S* txBuffer = &(packetBuffer.send.processBuffer);
            /* No Payload, success */
            uint8 noPayloadPacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x2E, 0xAA};
            txBuffer->bufferIndex = 12;
            memcpy(txBuffer->buffer, noPayloadPacket, txBuffer->bufferIndex);
            runTest(test_packetParsing_stateErrors(&packetBuffer, "Valid packet - no payload ", packets_ERR_SUCCESS));
            /* Success, payload */
            uint8 basePacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFD, 0x2E, 0xAA};
            txBuffer->bufferIndex = 14;
            memcpy(txBuffer->buffer, basePacket, txBuffer->bufferIndex);
            runTest(test_packetParsing_stateErrors(&packetBuffer, "Valid packet ", packets_ERR_SUCCESS));
            /* Incomplete packet */
            txBuffer->bufferIndex = 11;
            memcpy(txBuffer->buffer, noPayloadPacket,txBuffer->bufferIndex);
            runTest(test_packetParsing_stateErrors(&packetBuffer, "Short packet", packets_ERR_INCOMPLETE));
            /* Invalid start symbol */
            uint8 badStartPacket[15] = {0x02, 0x05, 0xCC, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFD, 0x2E, 0xAA};
            txBuffer->bufferIndex = 14;
            memcpy(txBuffer->buffer, badStartPacket, txBuffer->bufferIndex);
            runTest(test_packetParsing_stateErrors(&packetBuffer, "Invalid start sym", packets_ERR_START_SYM));
            /* Invalid module */
            uint8 badModulePacket[15] = {0x01, 0x07, 0xCC, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFD, 0x2E, 0xAA};
            txBuffer->bufferIndex = 14;
            memcpy(txBuffer->buffer, badModulePacket, txBuffer->bufferIndex);
            runTest(test_packetParsing_stateErrors(&packetBuffer, "Invalid Module", packets_ERR_MODULE));
            /* Invalid Checksum */
            uint8 badChecksumPacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFD, 0xF0, 0xAA};
            txBuffer->bufferIndex = 14;
            memcpy(txBuffer->buffer, badChecksumPacket, txBuffer->bufferIndex);
            runTest(test_packetParsing_stateErrors(&packetBuffer, "Bad Checksum", packets_ERR_CHECKSUM));
            /* Invalid End symbol */
            uint8 badEndPacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFD, 0x2E, 0xAB};
            txBuffer->bufferIndex = 14;
            memcpy(txBuffer->buffer, badEndPacket, txBuffer->bufferIndex);
            runTest(test_packetParsing_stateErrors(&packetBuffer, "Invalid End Symbol", packets_ERR_END_SYM));
        
            /* Clean up */
            packets_destoryBuffers(&packetBuffer);
        }
        
        
        /* ### Packet parsing Test Suite - Packet vals ### */
        {
            usbUart_print("\r\n*** Packet parsing - Packet Values ***\r\n");
            /* Setup - Create a packet object and initialize */
            packets_BUFFER_FULL_S packetBuffer1;
            packets_initialize(&packetBuffer1);
            packets_generateBuffers(&packetBuffer1, packets_LEN_BLOCK_PACKET);
            /* Local references */
            packets_BUFFER_PROCESS_S* txBuffer = &(packetBuffer1.send.processBuffer);
            /* No Payload, success */
            uint8 noPayloadPacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x2E, 0xAA};
            txBuffer->bufferIndex = 12;
            memcpy(txBuffer->buffer, noPayloadPacket, txBuffer->bufferIndex);
            uint8 payload[15];
            packets_PACKET_S expectedPacket = {
                .moduleId = 0x05,
                .cmd = 0xCC,
                .payload = payload,
                .payloadLen = 0,
                .payloadMax = packets_LEN_BLOCK_PACKET,
                .flags = ZERO,
                .error = ZERO
            };
            runTest(test_packetParsing_packetVals(&packetBuffer1, "No payload ", &expectedPacket));
            
            /* No Payload, success */
            uint8 basePacket[15] = {0x01, 0x05, 0xCC, 0x00, 0x02, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFD, 0x2E, 0xAA};
            uint8 payload2[15] = {0xFF, 0xFF};
            
            txBuffer->bufferIndex = 14;
            memcpy(txBuffer->buffer, basePacket, txBuffer->bufferIndex);
            packets_PACKET_S expectedPacket2 = {
                .moduleId = 0x05,
                .cmd = 0xCC,
                .payload = payload2,
                .payloadLen = 2,
                .payloadMax = packets_LEN_BLOCK_PACKET,
                .flags = ZERO,
                .error = ZERO
            };
            runTest(test_packetParsing_packetVals(&packetBuffer1, "Payload payload ", &expectedPacket2));
            /* Clean up */
            packets_destoryBuffers(&packetBuffer1);
        }
        
        /* Display test suite results */
        printTestCount();

        /* Enable the button */
        Button_EnableBtnInterrupts();
        /* Infinite loop */
        for(;;) {
            /* Reset on buton press */
            if(Button_wasButtonReleased()){
                /*Reset the device*/
                CySoftwareReset();   
            }
        }
        
    /* End MICA_TEST_PACKETS_ERRORS */
    #elif defined MICA_TEST_PACKETS
        /* Receive a packet from the IMU and print the result via the USB uart */
        /* Start the Components */
        UART_USB_Start();
        UART_IMU_Start();
        LEDS_Write(LEDS_ON_GREEN);
        /* Initialize variables */
        packets_BUFFER_FULL_S packetBuffer;
        packets_initialize(&packetBuffer);
        uint32 error = packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
        /* Ensure packet buffers were created properly */
        if(error){
            LEDS_Write(LEDS_ON_RED);
            for(;;){}
        }
        /* Infinite loop */
        for(;;){
            uint32 data = UART_IMU_UartGetByte();
            /* See if data is available */
            if( !(data & UART_IMU_UART_RX_UNDERFLOW) ){
                /* Process packet byte, check if packet complete */
                uint32 err = packets_processRxByte(&packetBuffer, (uint8) data); 
                /* Inidicate errors */
                if(err){
                    LEDS_G_Toggle();   
                    /* Reset the receive packet */
                    packets_flushRxBuffers(&packetBuffer);
                }
                /* Check if complete */
                if(packetBuffer.receive.bufferState == packets_BUFFER_RECEIVE_COMPLETE) {
                    LEDS_B_Toggle();
                    /* Parse the packet */
                    packets_parsePacket(&packetBuffer);
                    packets_PACKET_S* rxPacket = &(packetBuffer.receive.packet);
                    /* Clear the screen and print out the data */
                    usbUart_print("\n\r\nModule: %x \r\nCommand: %x\r\nPayload Len: %x\r\n", rxPacket->moduleId, rxPacket->cmd, rxPacket->payloadLen);
                    usbUart_print("Payload:");
                    uint8 i;
                    for(i=ZERO; i< rxPacket->payloadLen; i++){
                        usbUart_print(" %x", rxPacket->payload[i]);
                    }
                }
            }
        }
    /* End MICA_TEST_PACKETS */
    #elif defined MICA_TEST_PACKETS_ISR
        /* Receive a packet from the IMU Uart via an interrupt and print the result via the USB uart */
        /* Start the Components */
        UART_USB_Start();
        UART_IMU_Start();
        LEDS_Write(LEDS_ON_GREEN);
        /* Initialize variables */
        packets_BUFFER_FULL_S packetBuffer;
        packets_initialize(&packetBuffer);
        uint32 error = packets_generateBuffers(&packetBuffer, packets_LEN_BLOCK_PACKET);
        /* Ensure packet buffers were created properly */
        if(error){
            LEDS_Write(LEDS_ON_RED);
            for(;;){}
        }
        /* Enable the uart interrupts */
        UART_IMU_EnableInt();
        UART_IMU_SetCustomInterruptHandler(ISR_imuUart);

        /* Infinite loop */
        for(;;){
            /* See if there are any pending bytes */
            if(flag_pendingRxByte){
                /* Reset the flag */
                flag_pendingRxByte = false;
                /* Read in the byte */
                uint8 newData = imuUart_getChar();
                uint32 err = packets_processRxByte(&packetBuffer, newData);
                /* Inidicate errors */
                if(err){ 
                    /* Reset the receive packet */
                    packets_flushRxBuffers(&packetBuffer);
                }
                /* Check if complete */
                if(packetBuffer.receive.bufferState == packets_BUFFER_RECEIVE_COMPLETE) {
                    LEDS_B_Toggle();
                    /* Parse the packet */
                    packets_parsePacket(&packetBuffer);
                    packets_PACKET_S* rxPacket = &(packetBuffer.receive.packet);
                    /* Clear the screen and print out the data */
                    usbUart_print("\n\r\nModule: %x \r\nCommand: %x\r\nPayload Len: %x\r\n", rxPacket->moduleId, rxPacket->cmd, rxPacket->payloadLen);
                    usbUart_print("Payload:");
                    uint8 i;
                    for(i=ZERO; i< rxPacket->payloadLen; i++){
                        usbUart_print(" %x", rxPacket->payload[i]);
                    }
                }
            }
        }
    /* End MICA_TEST_PACKETS_ISR */
    #else 
        #error "At least ONE MICA_TEST_<case> must be defined if MICA_TEST is defined"
    #endif
    #if defined(MICA_DEBUG) && defined(MICA_TEST)
        #error "MICA_DEBUG and MICA_TEST may not be defined at the same time"
    #endif
#endif /* MICA_TEST */
/* %%%%%%%%%%%%%%%%%%  End Debugging & Testing  %%%%%%%%%%%%%%%%%% */
    
    /* Infinite Loop */
    for(;;)
    {

    }
}

/*******************************************************************************
* ISR Name: ISR_processRxByte()
********************************************************************************
* Summary:
*   ISR when data is received over the imu UART
* Interrupt: 
*       Button pressed
*
*******************************************************************************/
void ISR_imuUart(void){
    /* Check if new data is available */
    if( UART_IMU_GetRxInterruptSourceMasked() & UART_IMU_INTR_RX_NOT_EMPTY){
        flag_pendingRxByte = true;
    }
}

/*******************************************************************************
* ISR Name: ISR_toggleBtnTest()
********************************************************************************
* Summary:
*   Function to call when button is pressed. Toggle the mot
*
* Interrupt: 
*       Button pressed
*
*******************************************************************************/
void ISR_toggleBtnTest(void){
    /* Toggles the blue LED */
    LEDS_B_Toggle();
}

/*******************************************************************************
* ISR Name: ISR_toggleMotorState()
********************************************************************************
* Summary:
*   Function to call when button is pressed. Toggles whether the motor is
*   Enabled or not
*
* Interrupt: 
*       Button pressed
*
*******************************************************************************/
void ISR_toggleMotorState(void){
    /* Motors running */
    if(motorsState == true){
        Motors_Disable();   
    } else {
        Motors_Enable();   
    }
    /* Toggle state */
    motorsState = !motorsState;  
}

/*******************************************************************************
* ISR Name: ISR_Timer()
********************************************************************************
* Summary:
*   Triggers when the Timer expires
*
* Interrupt: 
*       timer_interrupt
*
*******************************************************************************/
void ISR_Timer(void){
    /* Clear the interrupts */
    SystemTimer_STATUS;
    /* Set the flag */
    timerExpired = true;
}


/* [] END OF FILE */