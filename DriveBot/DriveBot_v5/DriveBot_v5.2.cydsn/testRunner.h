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
#ifndef TEST_RUNNER_H
    #define TEST_RUNNER_H
    
    /***************************************
    * Included files
    ***************************************/
    #include "cytypes.h"
    #include <stdbool.h>
    /***************************************
    * Function Prototypes 
    ***************************************/
//    void runTest(bool testResult, uint16* passCount, uint16* testCount);
    void runTest(bool testResult);
    void resetTestCount(void);
    void printTestCount(void);
    
    
    bool printTestResults(char* testName, uint32_t error, uint32_t expectedResult, char* optErr);

#endif /* TEST_RUNNER_H */

/* [] END OF FILE */
