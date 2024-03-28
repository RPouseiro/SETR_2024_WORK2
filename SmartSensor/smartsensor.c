/*******************************************************//**
 * \file smartsensor.c
 *  
 * \brief 
 *
 * This file defines a set of functions for a smart sensor emulation.
 * 
 * \author Daniel Escadas
 * \author Renato Pouseiro
 * \version 1.0
 * \date 12-03-2024
*/
/*******************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "smartsensor.h"

/******************INTERNAL VARIABLES******************/
static uint8_t UART_RxBuffer[BUFFER_SIZE];
static uint8_t RxBufLen = 0;

static uint8_t UART_TxBuffer[BUFFER_SIZE];
static uint8_t TxBufLen = 0;
/*********************************************************/

/***********************FUNCTONS***********************/
Measures *SS_InitMeasures()
{
    Measures *measures;
    if(measures=malloc(sizeof(Measures)) == NULL) // Allocates memory space for the three measures arrays
    {
        return NULL;                     // If not possible, returns NULL;
    }
    else
    {
        return measures;
    }
}

void SS_ClearMeasures(Measures *pnt)
{
    free(pnt);
}

int SS_ResetRxBuffer()
{
    for(int i=0;i<BUFFER_SIZE;i++)
    {
        UART_RxBuffer[i] = 0;
    }
    RxBufLen = 0;
    return SS_SUCCESS;
}

int SS_ResetTxBuffer()
{
    for(int i=0;i<BUFFER_SIZE;i++)
    {
        UART_TxBuffer[i] = 0;
    }
    TxBufLen = 0;
    return SS_SUCCESS;
}

int SS_AddCharRx(uint8_t aux)
{
    //Check if buffer is full and return error
    if (RxBufLen >= BUFFER_SIZE)    return SS_FAILURE_BUFFERFULL;

    // Add new char to the buffer
    UART_RxBuffer[RxBufLen] = aux;
    RxBufLen += 1;

    return SS_SUCCESS;
}

int SS_AddCharTx(uint8_t aux)
{
    //Check if buffer is full and return error
    if (TxBufLen >= BUFFER_SIZE)    return SS_FAILURE_BUFFERFULL;

    UART_TxBuffer[RxBufLen] = aux;
    TxBufLen += 1;

    return SS_SUCCESS;
}

int SS_ProcessCom()
{

}