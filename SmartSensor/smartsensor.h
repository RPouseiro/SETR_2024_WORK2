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
#define MAX_TEM_RECORD 50

/**
 * \brief Maximum size for the humidity measures array.
 * 
 * Indicates the maximum number of humidity measures that can will be saved.
*/
#define MAX_HUM_RECORD 50

/**
 * \brief Maximum size for the air measures data array.
 * 
 * Indicates the maximum number of air measures measures that can will be saved.
*/
#define MAX_AIR_RECORD 50

/**
 * \brief Maximum size of transmitter and receiver buffer.
 * 
 * XPTO
*/
#define BUFFER_SIZE 20

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

/*********************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*********************************************************//**
 * \brief Struct for the pointers of each unity that can be stored by the smart-sensor
 * 
 * Struct for storing the information about the pointers of each unity measure stored (temperature, humidity and Co2)
*/
typedef struct Measures Measures;
struct Measures
{
    uint8_t Temperature[3*MAX_TEM_RECORD];  /**< Array for storing temperature measures*/
    uint8_t Humidity[3*MAX_HUM_RECORD];         /**< Array for storing humidity measures*/
    uint8_t Air_CO2[5*MAX_AIR_RECORD];            /**< Array for storing dioxide measures*/
};

/*********************************************************/

/*********************************************************//**
 * \brief Initialize the arrays for storing temperature, humidity and air measures.
 * 
 * Allocates in memory space for three arrays of fixed size to allow the sensor to store measures of temperature, humidity and CO2 measures.
 * 
 * \return If the function has sucesfully reserved space in memory for the struct Measures,  it returns the pointer to this struct. Otherwise returns NULL.
*/
Measures* SS_InitMeasures(); //DONE

/**
 * \brief Frees the memory used for storing measures.
 * 
 * Must be done after terminate the execution on the main program in order to free up the memory used for storing the values
 * \return Nothing
*/
void SS_ClearMeasures(Measures *pnt); //DONE

/**
 * \brief Reset the contents of the Receiver Buffer of the Emulated UART.
 * 
 *  Cleans up the contents of the RxBuffer and resets the RxBufLen variable.
 * \return XPTO
*/
int SS_ResetRxBuffer();

/**
 * \brief Resets the contents of the Transmitter Buffer of the Emulated UART.
 * 
 * Cleans up the contents of the TxBuffer and resets the TxBufLen variable.
 * \return XPTO
*/
int SS_ResetTxBuffer();

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

#endif