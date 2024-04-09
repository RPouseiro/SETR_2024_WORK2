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
	uint8_t buffer[BUFFER_SIZE];
	uint8_t len;
	SS_ResetRxBuffer();
	SS_ResetTxBuffer();
    // Check command 'A'
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('A');
	SS_AddCharRx('0');  //Irrelevant
	SS_CalcCheckSumRx('A'+'0');
	SS_AddCharRx(EndFrame);
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());
	getTxBuffer(buffer, &len);
    
	// Check command 'P' for temperature sensor
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(TEMP_SENSOR);
	SS_CalcCheckSumRx('P'+TEMP_SENSOR);
	SS_AddCharRx(EndFrame);
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());
	getTxBuffer(buffer, &len);

    // Check command 'P' for humidity sensor
    SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(HUM_SENSOR);
	SS_CalcCheckSumRx('P'+HUM_SENSOR);
	SS_AddCharRx(EndFrame);
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());;
	getTxBuffer(buffer, &len);

    // Check command 'P' for air sensor
	SS_AddCharRx('#');
	SS_AddCharRx('P');
	SS_AddCharRx(AIR_SENSOR);
	SS_CalcCheckSumRx('P'+AIR_SENSOR);
	SS_AddCharRx('!');
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());
	getTxBuffer(buffer, &len);

    // Check command 'L' for temperature
	SS_AddCharRx('#');
	SS_AddCharRx('L');
	SS_AddCharRx(TEMP_SENSOR);
	SS_CalcCheckSumRx('L'+TEMP_SENSOR);
	SS_AddCharRx('!');
	TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());
	getTxBuffer(buffer, &len);

	// Check command 'L' for humidity
	SS_AddCharRx('#');
	SS_AddCharRx('L');
	SS_AddCharRx(HUM_SENSOR);
	SS_CalcCheckSumRx('L'+HUM_SENSOR);
	SS_AddCharRx('!');
	TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());
	getTxBuffer(buffer, &len);

	// Check command 'L' for Co2
	SS_AddCharRx('#');
	SS_AddCharRx('L');
	SS_AddCharRx(AIR_SENSOR);  //Irrelevant
	SS_CalcCheckSumRx('L'+AIR_SENSOR);
	SS_AddCharRx('!');
	TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());
	getTxBuffer(buffer, &len);

    // Check command 'R' for temperature sensor
    SS_AddCharRx(StartFrame);
	SS_AddCharRx('R');
	SS_AddCharRx(TEMP_SENSOR);
	SS_CalcCheckSumRx('R'+TEMP_SENSOR);
	SS_AddCharRx(EndFrame);
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());
	getTxBuffer(buffer, &len);

    // Check command 'R' for humidity sensor
    SS_AddCharRx(StartFrame);
	SS_AddCharRx('R');
	SS_AddCharRx(HUM_SENSOR);
	SS_CalcCheckSumRx('R'+HUM_SENSOR);
	SS_AddCharRx(EndFrame);
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());
	getTxBuffer(buffer, &len);

    // Check command 'R' for air sensor
    SS_AddCharRx(StartFrame);
	SS_AddCharRx('R');
	SS_AddCharRx(AIR_SENSOR);
	SS_CalcCheckSumRx('R'+AIR_SENSOR);
	SS_AddCharRx(EndFrame);
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());
	getTxBuffer(buffer, &len);

    // Check command 'R' for all sensors
    SS_AddCharRx(StartFrame);
	SS_AddCharRx('R');
	SS_AddCharRx(ALL);
	SS_CalcCheckSumRx('R'+ALL);
	SS_AddCharRx(EndFrame);
    TEST_ASSERT_EQUAL_INT(SS_SUCCESS, SS_ProcessCom());
    getTxBuffer(buffer, &len);
}

void test_SmartSensor_CheckInvalidCmd(void)
{
	uint8_t buffer[BUFFER_SIZE];
	uint8_t len;
    SS_AddCharRx('d');
	SS_AddCharRx('P');
	SS_AddCharRx('T');  
	SS_CalcCheckSumRx('P'+TEMP_SENSOR);
	SS_AddCharRx('!');
    TEST_ASSERT_EQUAL_INT(SS_FAILURE_STARTFRAMENOTFOUND, SS_ProcessCom());
	getTxBuffer(buffer, &len);
    SS_AddCharRx('#');
	SS_AddCharRx('P');
	SS_AddCharRx('t');
	SS_CalcCheckSumRx('P'+TEMP_SENSOR);
	SS_AddCharRx('!');
    TEST_ASSERT_EQUAL_INT(SS_FAILURE_INVALIDDATA, SS_ProcessCom());
	getTxBuffer(buffer, &len);
    SS_AddCharRx('#');
	SS_AddCharRx('P');
	SS_AddCharRx('T');
	SS_CalcCheckSumRx('P'+TEMP_SENSOR);
	SS_AddCharRx('E');
    TEST_ASSERT_EQUAL_INT(SS_FAILURE_ENDFRAMENOTFOUND, SS_ProcessCom());
	getTxBuffer(buffer, &len);
    SS_ResetRxBuffer();
    TEST_ASSERT_EQUAL_INT(SS_FAILURE_BUFFEREMPTY, SS_ProcessCom());
	getTxBuffer(buffer, &len);
	SS_AddCharRx('#');
	SS_AddCharRx('P');
	SS_AddCharRx('H');
	SS_CalcCheckSumRx('P'+TEMP_SENSOR);
	SS_AddCharRx('!');
    TEST_ASSERT_EQUAL_INT(SS_FAILURE_CHECKSUM, SS_ProcessCom());
}

void test_SmartSensor_Temperature(void)
{
    uint8_t buffer[BUFFER_SIZE];
	uint8_t len;
    SS_ResetRxBuffer();
	SS_ResetTxBuffer();

	// Add a new value
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(TEMP_SENSOR);
	SS_CalcCheckSumRx('P'+TEMP_SENSOR);
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();
	getTxBuffer(buffer, &len);
	TEST_ASSERT_EQUAL_INT(10, len);
	TEST_ASSERT_EQUAL_INT('#',buffer[0]);
	TEST_ASSERT_EQUAL_INT('!',buffer[9]);
}

void test_SmartSensor_Humidity(void)
{
    uint8_t buffer[BUFFER_SIZE];
	uint8_t len;
    SS_ResetRxBuffer();
	SS_ResetTxBuffer();

	// Add a new value
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(HUM_SENSOR);
	SS_CalcCheckSumRx('P'+HUM_SENSOR);
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();
	getTxBuffer(buffer, &len);
	TEST_ASSERT_EQUAL_INT(10, len);
	TEST_ASSERT_EQUAL_INT('#',buffer[0]);
	TEST_ASSERT_EQUAL_INT('!',buffer[9]);
}

void test_SmartSensor_CO2(void)
{
    uint8_t buffer[BUFFER_SIZE];
	uint8_t len;
    SS_ResetRxBuffer();
	SS_ResetTxBuffer();

	// Add a new value
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(AIR_SENSOR);
	SS_CalcCheckSumRx('P'+AIR_SENSOR);
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();

	getTxBuffer(buffer, &len);
	TEST_ASSERT_EQUAL_INT(12, len);
	TEST_ASSERT_EQUAL_INT('#',buffer[0]);
	TEST_ASSERT_EQUAL_INT('!',buffer[11]);
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
	SS_CalcCheckSumRx('P'+TEMP_SENSOR);
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();
	getTxBuffer(buffer, &len);
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(TEMP_SENSOR);
	SS_CalcCheckSumRx('P'+TEMP_SENSOR);
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();
	getTxBuffer(buffer, &len);
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
	SS_CalcCheckSumRx('P'+HUM_SENSOR);
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();
	getTxBuffer(buffer, &len);
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(HUM_SENSOR);
	SS_CalcCheckSumRx('P'+HUM_SENSOR);
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();
	getTxBuffer(buffer, &len);
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
	SS_CalcCheckSumRx('P'+AIR_SENSOR);
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();
	getTxBuffer(buffer, &len);
	SS_AddCharRx(StartFrame);
	SS_AddCharRx('P');
	SS_AddCharRx(AIR_SENSOR);
	SS_CalcCheckSumRx('P'+AIR_SENSOR);
	SS_AddCharRx(EndFrame);
	SS_ProcessCom();
	getTxBuffer(buffer, &len);
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