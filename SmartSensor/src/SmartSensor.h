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
 * 
 * Indicates the maximum number of temperature measures that can will be saved.
*/
#define MAX_TEM_RECORD 20

/**
 * \brief Maximum size for the humidity measures array.
 * 
 * Indicates the maximum number of humidity measures that can will be saved.
*/
#define MAX_HUM_RECORD 20

/**
 * \brief Maximum size for the air measures data array.
 * 
 * Indicates the maximum number of air measures measures that can will be saved.
*/
#define MAX_AIR_RECORD 20

/**
 * \brief Maximum size of transmitter and receiver buffer.
 * 
 * XPTO
*/
#define BUFFER_SIZE 500

/**
 * \brief Maximum size of transmitter and receiver buffer.
 * 
 * XPTO
*/
#define LOG_SAMPLES 20

/**
 * \brief Maximum size of transmitter and receiver buffer.
 * 
 * XPTO
*/
#define TEMP_FRAME_SIZE 8

/**
 * \brief Maximum size of transmitter and receiver buffer.
 * 
 * XPTO
*/
#define HUM_FRAME_SIZE 8

/**
 * \brief Maximum size of transmitter and receiver buffer.
 * 
 * XPTO
*/
#define CO2_FRAME_SIZE 10

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
 * \brief Symbol that identifies the air sensor.
*/
#define ALL 'A'

/************************ERRORS************************/
/**
 * \brief Alias for sucessfull return.
 * 
 * Indicates the successful execution of the called function.
*/
#define SS_SUCCESS 0

/**
 * \brief Alias for failed return.
 * 
 * Indicates a failure at the execution of the called function. 
*/
#define SS_FAILURE_BUFFERFULL -1

/**
 * \brief Alias for failed return.
 * 
 * Indicates a failure at the execution of the called function. 
*/
#define SS_FAILURE_BUFFEREMPTY -2

/**
 * \brief Alias for failed return.
 * 
 * Indicates a failure at the execution of the called function. 
*/
#define SS_FAILURE_INVALIDDATA -3

/**
 * \brief Alias for failed return.
 * 
 * Indicates a failure at the execution of the called function. 
*/
#define SS_FAILURE_ENDFRAMENOTFOUND -4

/**
 * \brief Alias for failed return.
 * 
 * Indicates a failure at the execution of the called function. 
*/
#define SS_FAILURE_COMMANDNOTFOUND -5

/**
 * \brief Alias for failed return.
 * 
 * Indicates a failure at the execution of the called function. 
*/
#define SS_FAILURE_STARTFRAMENOTFOUND -6

/**
 * \brief Alias for failed return.
 * 
 * Indicates a failure at the execution of the called function. 
*/
#define SS_FAILURE_RECORDFULL -7

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
 * \return XPTO
*/
int SS_ResetTxBuffer(void);

/**
 * \brief Add an character to the RxBuffer
 * \param aux Character that will be introduced to the buffer.
 * \return -1 if the buffer is bull and does not allow to introduce any new characters or 0 if the execution was flawless.
*/
int SS_AddCharRx(uint8_t aux);

/**
 * \brief Add an character to the RxBuffer
 * \param aux Character that will be introduced to the buffer.
 * \return -1 if the buffer is bull and does not allow to introduce any new characters or 0 if the execution was flawless.
*/
int SS_AddCharTx(uint8_t aux);

/**
 * \brief Add an character to the RxBuffer
 * \param aux Character that will be introduced to the buffer.
 * \return -1 if the buffer is bull and does not allow to introduce any new characters or 0 if the execution was flawless.
*/
void getTxBuffer(uint8_t * buf, uint8_t *len);


/**
 * \brief
 * \param
 * \return
*/
int SS_ProcessCom(void);
/*********************************************************/

/*****************SENSOR FUNCTIONS********************/
/**
 * \brief
 * \param
 * \return
*/
int SS_RealTimeTemperature(uint8_t CMD);

/**
 * \brief
 * \param
 * \return
*/
int SS_RealTimeHumidity(uint8_t CMD);

/**
 * \brief
 * \param
 * \return
*/
int SS_RealTimeCO2(uint8_t CMD);

/**
 * \brief
 * \param
 * \return
*/
int SS_LogTemperature(void);

/**
 * \brief
 * \param
 * \return
*/
int SS_LogHumidity(void);

/**
 * \brief
 * \param
 * \return
*/
int SS_LogCO2(void);

/**
 * \brief
 * \param
 * \return
*/
int SS_ResetTemperature(void);

/**
 * \brief
 * \param
 * \return
*/
int SS_ResetHumidity(void);

/**
 * \brief
 * \param
 * \return
*/
int SS_ResetCO2(void);

/**
 * \brief
 * \param
 * \return
*/
int SS_ResetALL(void);
#endif