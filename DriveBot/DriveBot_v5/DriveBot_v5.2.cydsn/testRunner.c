/***************************************************************************
*                                       MICA
* File: testRunner.c
* Workspace: micaComponents
* Project Name: libMica
* Version: v1.0
* Author: Craig Cheney
*
* Brief:
*  Test suite for running unit tests
* 
* Authors:
*   Craig Cheney
*
* Change Log:
*   2018.08.03 CC - Document created
********************************************************************************/
#include "testRunner.h"
#include "micaCommon.h"
#include "usbUart.h"

uint16 passCount = ZERO;
uint16 testCount = ZERO;


/*******************************************************************************
* Function Name: runTest()
****************************************************************************//**
* \brief
*  Records the results of a unit test
*
* \param passCount
*   Pointer to the number of test that have passed
*
* \param testCount
*   Pointer to the number of test run
*
* \return
*   None
*******************************************************************************/
void runTest(bool testResult) {
    testCount++;
    passCount += testResult;
}



/*******************************************************************************
* Function Name: resetTestCount()
****************************************************************************//**
* \brief
*  Resets the recorded values of the test
*
* \return
*   None
*******************************************************************************/
void resetTestCount(void) {
    testCount = ZERO;
    passCount = ZERO;
}

/*******************************************************************************
* Function Name: printTestCount()
****************************************************************************//**
* \brief
*  Displays the results of the testing suite
*
* \param testName
*   Name of the test in question
*
* \param error
*   Actual value of error
*
* \param expectedResult, 
*   Expected value of the error
*
* \param optErr
*   An optional Error to display if the test failed for reasons other
*   than a non-zero error code
*
* \return
*   Whether of not the test passed
*******************************************************************************/
void printTestCount(void){
    usbUart_print("\r\n\n> Passed %d/%d tests\r\n", passCount, testCount);
    /* Reset count */
    resetTestCount();
}


/*******************************************************************************
* Function Name: printTestResults()
****************************************************************************//**
* \brief
*  Displays the results of the test
*
* \param testName
*   Name of the test in question
*
* \param error
*   Actual value of error
*
* \param expectedResult, 
*   Expected value of the error
*
* \param optErr
*   An optional Error to display if the test failed for reasons other
*   than a non-zero error code
*
* \return
*   Whether of not the test passed
*******************************************************************************/
bool printTestResults(char* testName, uint32_t error, uint32_t expectedResult, char* optErr){
    bool result = false;
    /* Print results */
    usbUart_print(testName);
    usbUart_print(": ");
    if(error != expectedResult){
        usbUart_print("Failed, Expected: 0x%x, Got: 0x%x\r\n", expectedResult, error);   
    } else if(optErr[0] != ZERO){
        usbUart_print("Failed: ");
        usbUart_print(optErr);
        usbUart_print("\r\n");
    } else {
        usbUart_print("Passed\r\n");   
        result = true;
    }
    return result;
}

/* [] END OF FILE */
