/*********************************************************//**
 * \file smartsensor.h
 *  
 * \brief Set of functions that implements a Doubly Linked List (DLL)
 *
 * This file defines a set of functions for a standard configuration of the ADC presented in the PIC32MX795F512L with capabilty to read the signal value.
 * 
 * \author Daniel Escadas
 * \author Renato Pouseiro
 * \version 1.0
 * \date 
*/
/*********************************************************/
#ifndef __SMARTSENSOR_H__
#define __SMARTSENSOR_H__

/*********************************************************//**
 * \brief Maximum size for the temperature measures array.
*/
#define MAX_TEM_RECORD 20

/**
 * \brief Maximum size for the humidity measures array.
*/
#define MAX_HUM_RECORD 20

/**
 * \brief Maximum size for the air measures data array.
*/
#define MAX_AIR_RECORD 20

/**
 * \brief Maximum size of transmitter and receiver buffer.
*/
#define BUFFER_SIZE 128

/**
 * \brief Number of samples that can be sended back by the L command
*/
#define LOG_SAMPLES 20

/**
 * \brief Maximum size of transmitter and receiver buffer.
*/

/**
 * \brief Symbol that identifies the start of frame.
*/
#define StartFrame '#'

/**
 * \brief Symbol that identifies the end of frame.
*/
#define EndFrame '!'

/**
 * \brief Symbol that identifies the temperature sensor.
*/
#define TEMP_SENSOR 'T'

/**
 * \brief Symbol that identifies the humidity sensor.
*/
#define HUM_SENSOR 'H'

/**
 * \brief Symbol that identifies the air sensor.
*/
#define AIR_SENSOR 'C'

/**
 * \brief Symbol that identifies the all sensors.
*/
#define ALL 'A'

/************************ERRORS************************/
/**
 * \brief Alias for sucessfull return.
*/
#define SS_SUCCESS 0

/**
 * \brief Alias for failed return due to buffer being full
*/
#define SS_FAILURE_BUFFERFULL -1

/**
 * \brief Alias for failed return due to buffer being empty
*/
#define SS_FAILURE_BUFFEREMPTY -2

/**
 * \brief Alias for failed return due to invalid data format
*/
#define SS_FAILURE_INVALIDDATA -3

/**
 * \brief Alias for failed return due to end symbol not existing or at wrong position. 
*/
#define SS_FAILURE_ENDFRAMENOTFOUND -4

/**
 * \brief Alias for failed return for command not existing.
*/
#define SS_FAILURE_COMMANDNOTFOUND -5

/**
 * \brief Alias for failed return due to start symbol not existing.
*/
#define SS_FAILURE_STARTFRAMENOTFOUND -6

/**
 * \brief Alias for failed return due to measure array being full. 
*/
#define SS_FAILURE_RECORDFULL -7

/**
 * \brief Alias for failed return due to checksum not corresponding 
*/
#define SS_FAILURE_CHECKSUM -8
/*********************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*********************************************************//**
 * \brief Initialize the arrays for storing temperature, humidity and air measures.
 * 
 * Allocates in memory space for three arrays of fixed size to allow the sensor to store measures of temperature, humidity and CO2 measures.
 * 
 * \return If the function has sucesfully reserved space in memory for the struct Measures,  it returns the pointer to this struct. Otherwise returns NULL.
*/
int SS_InitMeasures(void); //DONE

/**
 * \brief Reset the contents of the Receiver Buffer of the Emulated UART.
 * 
 *  Cleans up the contents of the RxBuffer and resets the RxBufLen variable.
 * \return XPTO
*/
int SS_ResetRxBuffer(void);

/**
 * \brief Resets the contents of the Transmitter Buffer of the Emulated UART.
 * 
 * Cleans up the contents of the TxBuffer and resets the TxBufLen variable.
*/
int SS_ResetTxBuffer(void);

/**
 * \brief Add an character to the RxBuffer
 * \param aux Character that will be introduced to the buffer.
*/
int SS_AddCharRx(uint8_t aux);

/**
 * \brief Add an character to the TxBuffer
 * \param aux Character that will be introduced to the buffer.
*/
int SS_AddCharTx(uint8_t aux);

/**
 * \brief Get the characters presented at TxBuffer.
 * \param buf Pointer to the buffer
 * \param len Pointer to the last position ocuppied by the buffer.
*/
void getTxBuffer(uint8_t * buf, uint8_t *len);

/**
 * \brief Check if the CheckSum is valid.
 * \param soma Sum of the CMD and Data bytes.
 * \param CS pointer to the byte array of CS field.
 */
int SS_CheckSum(int soma,uint8_t *CS);

/**
 * \brief Calculates the MOD256 for the checksum and insert it on TxBuffer.
 * \param soma Sum of the CMD and Data bytes.
 */
int SS_CalcCheckSumTx(int soma);

/**
 * \brief Calculates the MOD256 for the checksum and insert it on RxBuffer.
 * \param soma Sum of the CMD and Data bytes.
 */
int SS_CalcCheckSumRx(int soma);

/**
 * \brief Command control algorithm.
*/
int SS_ProcessCom(void);
/*********************************************************/

/*****************SENSOR FUNCTIONS********************/
/**
 * \brief Emulates a temperature sensor reading.
*/
int SS_RealTimeTemperature(uint8_t CMD);

/**
 * \brief Emulates a humidity sensor reading
*/
int SS_RealTimeHumidity(uint8_t CMD);

/**
 * \brief Emulates a CO2 sensor reading.
*/
int SS_RealTimeCO2(uint8_t CMD);

/**
 * \brief Prints out the LOG_SAMPLES last measures of temperature.
*/
int SS_LogTemperature(void);

/**
 * \brief Prints out the LOG_SAMPLES last measures of humidity.
*/
int SS_LogHumidity(void);

/**
 * \brief Prints out the LOG_SAMPLES last measures of CO2.
*/
int SS_LogCO2(void);

/**
 * \brief Clears the temperatures measures stored.
*/
int SS_ResetTemperature(void);

/**
 * \brief Clears the humidity measures stored.
*/
int SS_ResetHumidity(void);

/**
 * \brief Clears the CO2 measures stored.
*/
int SS_ResetCO2(void);

/**
 * \brief Clears all measures stored.
*/
int SS_ResetALL(void);
#endif