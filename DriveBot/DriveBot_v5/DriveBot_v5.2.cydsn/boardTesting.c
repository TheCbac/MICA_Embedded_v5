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
//void boardTest_getEncoderCounts(encoder_T * encoderPtr) {
//    /* Read the encoders and write to the pointer passed in */
//    encoderPtr->leftCount = QuadDec_L_ReadCounter();
//    encoderPtr->rightCount = QuadDec_R_ReadCounter();
//}

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
//void boardTest_resetEncoderCounts(encoder_T * encoderPtr) {
//    /* Reset hardware counter */
//    QuadDec_L_WriteCounter(ZERO);
//    QuadDec_R_WriteCounter(ZERO);  
//    /* Reset the local count */
//    encoderPtr->leftCount = ZERO;
//    encoderPtr->rightCount = ZERO;
//}

/* [] END OF FILE */
