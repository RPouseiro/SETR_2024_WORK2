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
 * \brief Struct for the pointers of each unity that can be stored by the smart-sensor
 * 
 * Struct for storing the information about the pointers of each unity measure stored (temperature, humidity and Co2)
*/
Measures* SS_InitMeasures(); //DONE

/**
 * \brief Clears the memory used for storing measures.
 * 
 * XPTO XPTO XPTO
*/
void SS_ClearMeasures(Measures *pnt); //DONE

int SS_ResetRxBuffer();

int SS_ResetTxBuffer();

int SS_AddCharRx(uint8_t aux);

int SS_AddCharTx(uint8_t aux);

#endif