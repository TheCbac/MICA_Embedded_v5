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
* 2017.08.25 CC - Add libMica
* 2017.08.13 SM - Add motor debugging
* 2017.08.11 CC - Document Created
********************************************************************************/
#include "main.h"
//#include "micaLeds.h"
//#include "micaUart.h"
#include "stdbool.h"
//#include "micaPackets.h"
/*  -------------- DEBUGGING --------------
* Uncomment MICA_DEBUG to enable ONE of the below Debug levels
*/
//#define MICA_DEBUG

/* -------------- DEBUG LEVEL --------------
* Uncomment ONE of the following
* Debugging will only occur when MICA_DEBUG is defined
*/


/* Static Function prototypes */
static void processActuationCmd(uint8 command, uint8* payload);
static void processControlCmd(uint8 command, uint8* payload);
/* --------- Local Variables --------- */
/* Flag indiacting a packet has been received */
volatile bool packetAwaiting = false;
/* Buffer for storing the data  */
uint8 packetBuffer[MICA_UART_LEN_PACKET];

uint8_t packetBuffer[MICA_PACKET_LEN_MAX_PACKET];
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
    /* Start the LEDs */
    micaLedStart();
    micaLedEnable(MICA_LED_GREEN);
    /* Start the UART  */
    micaUartStart();
    /* Start the PWM Components */
    PWM_L_Start();
    PWM_R_Start();
    /* Start the UART component */
    UART_Start();
    uart_interrupt_StartEx(ISR_Uart);
    /* Hook up the button interrupt */
    button_interrupt_StartEx(ISR_button);
    motorControlReg_Write(MICA_MOTOR_LEFT_FORWARD | MICA_MOTOR_RIGHT_FORWARD);
    /* Enable global interrupts */
    CyGlobalIntEnable;
    
    /* %%%%%%%%%%%%%%%%%% Begin Debugging %%%%%%%%%%%%%%%%%% */
#ifdef MICA_DEBUG
    #if defined DB_TEST_MOTORS
        /* Enable global interrupts */
        CyGlobalIntEnable; 
        /* Assign Button ISR vector */
        button_interrupt_StartEx(ISR_button);
        led_R_pin_Write(LED_OFF);
        led_G_pin_Write(LED_ON);
        led_B_pin_Write(LED_OFF);
        
        PWM_L_Init();
        PWM_R_Init();
        PWM_L_Start();
        PWM_R_Start();
        PWM_L_WriteCompare(200);
        PWM_R_WriteCompare(200);
        
        
    #elif defined DB_TEST_PWM
        
    #elif defined DB_TEST_QUAD_DEC
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
        
    #endif
#endif
    /* %%%%%%%%%%%%%%%%%% End Debugging %%%%%%%%%%%%%%%%%% */
    
    /* Infinite Loop */
    for(;;)
    {
        /* Wait until a packet is ready */
        if(packetAwaiting == true){
//            /* Process the packet */
//            processUartPacket(packetBuffer , MICA_UART_LEN_PACKET);
            /* Parse the packet */
            MICA_PACKET_T packet = mica_parsePacket(packetBuffer);
            /* If the packet is valid */
            if (packet.cmd == PACKET_SUCCESS) {
                
            }
            /* Reset the Flag */
            packetAwaiting = false;
        }

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
//void processUartPacket(uint8* packet, uint16 len){
//    /* Calculate the Checksum (excluding the existing checksum) */
//    uint16_t calcChecksum = mica_computeChecksum16(packet, len - MICA_UART_LEN_CHECKSUM);
//    /* if valid, process the command */
//    if( calcChecksum == packetBuffer[len - MICA_UART_LEN_CHECKSUM] ){
//        /* Indicate sucess */
//        micaLedEnable(MICA_LED_CYAN);
//        /* Get the command and payload */
//        uint8 cmd = packet[MICA_UART_PACKET_INDEX_CMD];
//        uint8* payload = &packet[MICA_UART_PACKET_INDEX_PAYLOAD];
//        /* Act according the module impacted */
//        switch(packet[MICA_UART_PACKET_INDEX_MODULE]){
//            /* Writing to the control module */
//            case MICA_ID_MODULE_CONTROL:{
//                /* Handle ControlModule actions */
//                processControlCmd(cmd, payload);
//                break;
//            }
//            /* Writing to the Actuation Module */
//            case MICA_ID_MODULE_ACTUATION:{
//                /* handle Actuation module commands */
//                processActuationCmd(cmd, payload);
//                break;
//            }
//        } /* packet[MICA_UART_PACKET_INDEX_MODULE] */
//        
//    }
//    /* Do not process a bad packet */
//    else{
//        /* Flush the UART buffer */
//        UART_SpiUartClearRxBuffer();
//        return;
//    }
//}



/*******************************************************************************
* Function Name: processActuationCmd()
********************************************************************************
* Summary:
*   Processes the data passed in by the UART, targeted at the actuation module
*
* Parameters:
*   packet - Array of the data is stored
*   len - Length of the array, including the checksum
*
* Return:
*   None
*
*******************************************************************************/
void processActuationCmd(uint8 command, uint8* payload){
    /* Act according to command issed */
    switch(command){
        /* Writing to the LEDs */
        case MICA_ACT_CMD_MOTORS:{
            /* Write the control register */
            motorControlReg_Write(payload[MICA_ACT_CMD_MOTORS_INDEX_CTRL]);
            /* Set the PWM values */
            PWM_R_WriteCompare(payload[MICA_ACT_CMD_MOTORS_INDEX_PWM_R]);
            PWM_L_WriteCompare(payload[MICA_ACT_CMD_MOTORS_INDEX_PWM_L]);
            uint8_t cat;
            break;
        }
    } /* command */
}



/*******************************************************************************
* Function Name: processControlCmd()
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
void processControlCmd(uint8 command, uint8* payload){
    /* Act according to command issed */
    switch(command){
        /* Writing to the LEDs */
        case MICA_CTRL_ID_LED:{
            /* Set the LEDs */
            micaLedEnable(*payload);
            break;
        }
    } /* command */
}

/*******************************************************************************
* ISR Name: ISR_Uart()
********************************************************************************
* Summary:
*        Triggers when the UART RX Buffer is not empty
*
* Interrupt: 
*       uart_interrupt
*
* Use:
*       Processes the data and sets the packetAwaiting flag if the packet is complete
*******************************************************************************/
CY_ISR(ISR_Uart){
//    /* Determine cause of the interrupt */
//    if( UART_GetRxInterruptSourceMasked() & UART_INTR_RX_FULL ) {
//        /* Index variable */
//        uint8 idx;
//        /* Move the packet from the UART buffer to the software buffer */
//        for(idx = ZERO; idx < MICA_UART_LEN_PACKET; idx++){
//            /* Get a byte and place into the buffer */
//            packetBuffer[idx] = UART_UartGetByte();
//        }
//        /* Set the packet awaiting flag */
//        packetAwaiting = true;
//        /* Clear the interrupt */
//        UART_ClearRxInterruptSource(UART_INTR_RX_FULL);
//        
//
//    }
    /* Data available */
    if (UART_GetRxInterruptSourceMasked() & UART_INTR_RX_NOT_EMPTY) {
        /* Process the byte */
        packetAwaiting = mica_processPacketByte(UART_UartGetByte(), packetBuffer);
        /* Clear the interrupt */
        UART_ClearRxInterruptSource(UART_INTR_RX_NOT_EMPTY);
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
}
    

/* [] END OF FILE */