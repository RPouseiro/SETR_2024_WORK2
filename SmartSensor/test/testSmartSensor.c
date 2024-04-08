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
    // Check command 'A'
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('A');
	SS_AddCharRx('0');  //Irrelevant
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());

    // Check command 'P' for temperature sensor
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(TEMP_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());
	
    // Check command 'P' for humidity sensor
    SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(HUM_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());;

    // Check command 'P' for humidity sensor
	SS_AddCharRx('#');
	SS_AddCharRx('P');
	SS_AddCharRx(AIR_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx('!');
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());

    // Check command 'L'
	SS_AddCharRx('#');
	SS_AddCharRx('L');
	SS_AddCharRx('0');  //Irrelevant
	SS_AddCharRx('0');
	SS_AddCharRx('!');

    // Check command 'R' for temperature sensor
    SS_AddCharRx(StartFrame);
	SS_AddCharRx('R');
	SS_AddCharRx(TEMP_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());

    // Check command 'R' for humidity sensor
    SS_AddCharRx(StartFrame);
	SS_AddCharRx('R');
	SS_AddCharRx(HUM_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());

    // Check command 'R' for air sensor
    SS_AddCharRx(StartFrame);
	SS_AddCharRx('R');
	SS_AddCharRx(AIR_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());

    // Check command 'R' for all sensors
    SS_AddCharRx(StartFrame);
	SS_AddCharRx('R');
	SS_AddCharRx(ALL);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());
    
    
}

void test_SmartSensor_CheckInvalidCmd(void)
{
    SS_AddCharRx('d');
	SS_AddCharRx('P');
	SS_AddCharRx('T');  
	SS_AddCharRx('0');
	SS_AddCharRx('!');
    TEST_ASSERT_EQUAL_INT(SS_FAILURE_STARTFRAMENOTFOUND, SS_ProcessCom());
    SS_AddCharRx('#');
	SS_AddCharRx('P');
	SS_AddCharRx('t');
	SS_AddCharRx('0');
	SS_AddCharRx('!');
    TEST_ASSERT_EQUAL_INT(SS_FAILURE_INVALIDDATA, SS_ProcessCom());
    SS_AddCharRx('#');
	SS_AddCharRx('P');
	SS_AddCharRx('T');
	SS_AddCharRx('0');
	SS_AddCharRx('E');
    TEST_ASSERT_EQUAL_INT(SS_FAILURE_ENDFRAMENOTFOUND, SS_ProcessCom());
    SS_ResetRxBuffer();
    TEST_ASSERT_EQUAL_INT(SS_FAILURE_BUFFEREMPTY, SS_ProcessCom());

}

void test_SmartSensor_Temperature(void)
{
    uint8_t buffer[BUFFER_SIZE];
	uint8_t len;
    SS_ResetRxBuffer();
	SS_ResetTxBuffer();

	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(TEMP_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();
	
	getTxBuffer(buffer, &len);
	TEST_ASSERT_EQUAL_INT(8, len);
	TEST_ASSERT_EQUAL_INT('#',buffer[0]);
	TEST_ASSERT_EQUAL_INT('!',buffer[7]);
}

void test_SmartSensor_Humidity(void)
{
    uint8_t buffer[BUFFER_SIZE];
	uint8_t len;
    SS_ResetRxBuffer();
	SS_ResetTxBuffer();

	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(HUM_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();

	getTxBuffer(buffer, &len);
	TEST_ASSERT_EQUAL_INT(8, len);
	TEST_ASSERT_EQUAL_INT('#',buffer[0]);
	TEST_ASSERT_EQUAL_INT('!',buffer[7]);
}

void test_SmartSensor_CO2(void)
{
    uint8_t buffer[BUFFER_SIZE];
	uint8_t len;
    SS_ResetRxBuffer();
	SS_ResetTxBuffer();
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(AIR_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();

	getTxBuffer(buffer, &len);
	TEST_ASSERT_EQUAL_INT(10, len);
	TEST_ASSERT_EQUAL_INT('#',buffer[0]);
	TEST_ASSERT_EQUAL_INT('!',buffer[9]);
}

void test_SmartSensor_Log_Temp(void)
{
    uint8_t buffer[BUFFER_SIZE];
	uint8_t len;
    SS_ResetRxBuffer();
	SS_ResetTxBuffer();
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(TEMP_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(TEMP_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();
	TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_LogTemperature());

}



void test_SmartSensor_Log_Hum(void)
{
    uint8_t buffer[BUFFER_SIZE];
	uint8_t len;
    SS_ResetRxBuffer();
	SS_ResetTxBuffer();
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(HUM_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(HUM_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();

	TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_LogHumidity());

}

void test_SmartSensor_Log_CO2(void)
{
    uint8_t buffer[BUFFER_SIZE];
	uint8_t len;
    SS_ResetRxBuffer();
	SS_ResetTxBuffer();
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(AIR_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(AIR_SENSOR);
	SS_AddCharRx('0');
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();

	TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_LogCO2());
}


int main(void) {
    printf("\n Smart Sensor interface emulation Unit Testing \n");
    /* Init UART RX and TX buffers */
	SS_ResetRxBuffer();
	SS_ResetTxBuffer();
	SS_InitMeasures();

    UNITY_BEGIN(); 
	RUN_TEST(test_SmartSensor_CheckInvalidCmd);
    RUN_TEST(test_SmartSensor_CheckValidCmd);
  	RUN_TEST(test_SmartSensor_Temperature); 
    RUN_TEST(test_SmartSensor_Humidity);
    RUN_TEST(test_SmartSensor_CO2);
	RUN_TEST(test_SmartSensor_Log_Temp);
	RUN_TEST(test_SmartSensor_Log_Hum);
	RUN_TEST(test_SmartSensor_Log_CO2);

    return UNITY_END();
}