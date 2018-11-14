/***************************************************************************
*                                       MICA
* File: otaUpdate.c
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
*   Code for executing an OTA update. Contains both optional and mandatory steps.
* 
* Date Written:  2017.08.01
* Last Modified: 2017.08.13
********************************************************************************/
#include "otaUpdate.h"
#include "cytypes.h"

#if CYDEV_FLASH_SIZE != 0x00040000u
 #error "This design is specifically targeted to parts with 256k of flash."
#endif

/*******************************************************************************
* Following code implements re-initialization of separate RAM segment containing
* data that is required for BLE Stack operation.
* This code is used in case of GCC compiler
*******************************************************************************/
extern void Bootloader___libc_init_array(void);
typedef unsigned char __cy_byte_align8 __attribute ((aligned (8)));
struct __cy_region
{
    __cy_byte_align8 *init; /* Initial contents of this region.  */
    __cy_byte_align8 *data; /* Start address of region.  */
    size_t init_size;       /* Size of initial data.  */
    size_t zero_size;       /* Additional size to be zeroed.  */
};
extern const struct __cy_region Bootloader___cy_regions[];
extern const char Bootloader___cy_region_num __attribute__((weak));
#define Bootloader___cy_region_num ((size_t)&Bootloader___cy_region_num)
/*******************************************************************************
* Function Name: OTA_initializeCodeSharing()
********************************************************************************
*
* Summary:
*   This function triggers initialization of bootloader RAM .bss section.
*   It is required for BLE Stack operation. This function should be called from
*   main() prior BLE component start.
*
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void OTA_initializeCodeSharing(void){
    unsigned regions = Bootloader___cy_region_num;
    const struct __cy_region *rptr = Bootloader___cy_regions;
    /* Initialize memory */
    for( regions = Bootloader___cy_region_num; regions != ZERO; regions--){
        uint32 *src = (uint32 *)rptr->init;
        uint32 *dst = (uint32 *)rptr->data;
        unsigned limit = rptr->init_size;
        unsigned count;
        /* Copy the contents src address to the dst addresses */
        for(count = ZERO; count != limit; count += sizeof (uint32)){
            *dst = *src;
            dst++;
            src++;
        }
        /* Zero the folowing sections */
        limit = rptr->zero_size;
        for(count = ZERO; count != limit; count += sizeof (uint32)){
            *dst = ZERO;   
            dst++;
        }
        rptr++;
    }
    DBG_PRINT("> Code sharing initialized\r\n");
}

/*******************************************************************************
* Function Name: OTA_writeFlashByte()
********************************************************************************
*
* Summary:
*   This API writes to flash the specified data. This over
*
*
* Parameters:
*    address    - The address in flash.
*    inputValue - One-byte data.
*
* Return:
*   A status of the writing to flash procedure.
*
*******************************************************************************/
cystatus OTA_writeFlashByte(const uint32 address, const uint8 inputValue){
    /* intialize private variables */
    cystatus result = CYRET_SUCCESS;
    uint8 rowData[CYDEV_FLS_ROW_SIZE];
    /* Calculate the flash address */
    uint32 flsAddr = address - CYDEV_FLASH_BASE;
    /* Calculate the row number */
    uint16 rowNum = (uint16) (flsAddr / CYDEV_FLS_ROW_SIZE);
    /* Calculate the base address */
    uint32 baseAddr = address - (address % CYDEV_FLS_ROW_SIZE);
    uint16 idx;
    /* Get the row data */
    for(idx = ZERO; idx < CYDEV_FLS_ROW_SIZE; idx++){
        rowData[idx] = (uint8)bootloadable_GET_CODE_DATA(baseAddr + idx);   
    }
    /* Set the new value */
    rowData[address % CYDEV_FLS_ROW_SIZE] = inputValue;
    /* Write the data */
    result = CySysFlashWriteRow((uint32) rowNum, rowData);
    /* Return the result */
    return result;
}

/*******************************************************************************
* Function Name: OTA_EnterBootloadMode()
********************************************************************************
*
* Summary:
*   Sets the active application to the stack bootloader, and then resets
*   the device. The active application is written in the Flash metadata section.
*   This function will not return. 
*
*   NOTE The active application number is not set directly, but the boolean 
*   mark instead means that the application is active or not for the relative 
*   metadata. Both metadata sections are updated. For example, if the second 
*   application is to be set active, then in the metadata section for the first 
*   application there will be a "0" written, which means that it is not active, and 
*   for the second metadata section there will be a "1" written, which means that it is 
*   active. TODO: more efficient and direct implementation that doesn't waste a Flash 
*   Write cycle on the entire device
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void OTA_EnterBootloadMode(void){
    DBG_PRINT("> Entering Bootload mode...\r\n");
    uint8 idx;
    /* Set the active application by updating the metadata section */
    for( idx = ZERO; idx < bootloadable_MAX_NUM_OF_BTLDB; idx++){
        OTA_writeFlashByte((uint32) OTA_MD_BTLDB_ACTIVE_OFFSET(idx), (uint8)(idx == ID_PROGRAM_STACK) );
    }
    /* Reset the device in bootload mode */
    bootloadable_Load();
}

/*******************************************************************************
* Function Name: OTA_afterImageUpdate()
********************************************************************************
*
* Summary:
*   This functions checks if the Self Project Image has been Updated and is 
*   Running for the first time. If so, it clears the new image flag. If bonded 
*   devices are used, then the bonded data must be cleared.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void OTA_afterImageUpdate(void){
    /* See if the image was updated */
    if( FLAG_UPDATE_APP_GET == FLAG_SET){
        DBG_PRINT("> *** App Image was updated ***\r\n");
        //TODO: deal with bonded devices
        /* Clear the flag */
        bootloadable_SetFlashByte(FLAG_UPDATE_APP, FLAG_CLEAR);
    }
}

/* [] END OF FILE */
