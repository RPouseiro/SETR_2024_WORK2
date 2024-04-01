#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "SmartSensor.h"
#include "unity.h"

// Forward declaration of helper functions
void setUp(void);
void tearDown(void);


// Unity test setup and teardown functions
void setUp(void) {
    return;
}
void tearDown(void) {
    return;
}

void test_SmartSensor_CheckValidCmd(void)
{
    // Check command 'P' for temperature sensor
	SS_AddCharRx('#');
	SS_AddCharRx('P');
	SS_AddCharRx('T');
	SS_AddCharRx('0');
	SS_AddCharRx('!');
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());

    // Check command 'P' for humidity sensor
	SS_AddCharRx('#');
	SS_AddCharRx('P');
	SS_AddCharRx('H');
	SS_AddCharRx('0');
	SS_AddCharRx('!');
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());

    // Check command 'P' for temperature sensor
	SS_AddCharRx('#');
	SS_AddCharRx('P');
	SS_AddCharRx('C');
	SS_AddCharRx('0');
	SS_AddCharRx('!');
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());

    SS_AddCharRx('#');
	SS_AddCharRx('A');
	SS_AddCharRx('T');
	SS_AddCharRx('0');
	SS_AddCharRx('!');
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());

    SS_AddCharRx('#');
	SS_AddCharRx('P');
	SS_AddCharRx('T');
	SS_AddCharRx('0');
	SS_AddCharRx('!');
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());
    
}


void test_SmartSensor_CheckInvalidCmd(void)
{
    SS_AddCharRx('d');
	SS_AddCharRx('P');
	SS_AddCharRx('T');  
	SS_AddCharRx('0');
	SS_AddCharRx('!');
    TEST_ASSERT_NOT_EQUAL_INT(0, SS_ProcessCom());
    SS_AddCharRx('#');
	SS_AddCharRx('P');
	SS_AddCharRx('t');
	SS_AddCharRx('0');
	SS_AddCharRx('!');
    TEST_ASSERT_NOT_EQUAL_INT(0, SS_ProcessCom());
 

}

int main(void) {
    printf("\n Smart Sensor interface emulation Unit Testing \n");
    /* Init UART RX and TX buffers */
	SS_ResetRxBuffer();
	SS_ResetTxBuffer();


    UNITY_BEGIN(); 
    RUN_TEST(test_SmartSensor_CheckInvalidCmd); 
    RUN_TEST(test_SmartSensor_CheckValidCmd);

    return UNITY_END();
}

// Define your test cases below

// Write similar test cases for other functions
