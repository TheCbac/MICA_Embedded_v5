/***************************************************************************
*                                       MICA
* File: debug.c
* Workspace: IMU_v4.0
* Project Name: 01_IMU_App_v4.0
* Version: v4.0.0
* Author: Craig Cheney
* 
* PCB: MICA_IMU_v3.5.2
* PSoC: CYBLE-214015-01 (EZBLE, 4.2, 256k)
* Sensors: BOSCH IMU (BMX055)
*
* Brief:
*   *** DEPRECATED - NO LONGER VALID SINCE PORTING OVER TO IMU ***
*   Contains functions for debugging over the UART. The SW transmit UART is 
*   a firmware only component, and does not require "UART_Start()"
* 
* Date Written:  2017.07.31
* Last Modified: 2017.08.13
********************************************************************************/
#include "debug.h"

#ifdef MICA_DEBUG


/*******************************************************************************
* Function Name: divmod10()
********************************************************************************
*
* Summary:
*   Returns the result of value/10. Provides a pointer to the remainder
*
* Parameters:
*   value - value to divide by 10
*   remainder - pointer to the remainder
*
* Return:
*   value/10
*
*******************************************************************************/
static uint32 divmod10(uint32 value, uint32 *remainder){
    /* Perform division */
    uint32 result = value / TEN;
    /* Calculate the remainder and point to it */
    *remainder = value - (result * TEN);
    /* Return the result */
    return result;
}


/*******************************************************************************
* Function Name: DBG_PRINT_DEC()
********************************************************************************
*
* Summary:
*   Prints a number out in an ASCII readable format decimal format
*
* Parameters:
*   val - value to print in decimal
*
* Return:
*   None
*
*******************************************************************************/
void DBG_PRINT_DEC(uint32 val){
    /* declare internal variables */
    char buf[BUFF_LEN_32_BIT_DEC];
    uint32 len = ZERO;
    uint32 i = RESET_INDEX;
    /* Calculate each digit, and place into buffer - Little Endian  */
    do{
        uint32 rem;
        /* Divide by 10 */
        val = divmod10(val, &rem);
        /* Convert to ASCII & place the remainder into the buffer */
        buf[len++] = rem + '0';
    } while ( val != ZERO);
    /* Convert buffer to Big Endian */
    for( i = RESET_INDEX; i < len/TWO; i++){
        /* Copy the last element (MSB) */
        char tmp = buf[len - i - ZERO_INDEX];
        /* Change last element to LSB */
        buf[len -i - ZERO_INDEX] = buf[i];
        /* Move MSB to begining */
        buf[i] = tmp;
    }
    /* Add null termination */
    buf[len] = '\0';
    /* Print the string */
    UART_PutString(buf);
}

/*******************************************************************************
* Function Name: DBG_PRINT_DEC_TEXT()
********************************************************************************
*
* Summary:
*   Prints a number follow by text
*
* Parameters:
*   val - value to print in decimal
*   text - pointer to a string
*
* Return:
*   None
*
*******************************************************************************/
void DBG_PRINT_DEC_TEXT(uint32 val, char* text){
    DBG_PRINT_DEC(val);
    DBG_PRINT(text);
}
#endif /* MICA_DEBUG */

/* [] END OF FILE */
