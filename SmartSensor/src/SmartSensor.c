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

#include "SmartSensor.h"

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
        // Check if RxBuffer is empty
        if (RxBufLen==0) return SS_FAILURE_BUFFEREMPTY;

        int i,aux;
        uint8_t SensorID; /* Symbol that identifies the type of sensor */

        // Find index of StartFrame
        for (i=0; i<RxBufLen;i++)
        {
            if(UART_RxBuffer[i]==StartFrame) break;
        }

        //If StartFrame exists, analyse the command
        if(i<RxBufLen)//TODO: CONDITION TO VERIFIED CHECKSUM
                    /*
                    if (!SS_CalcCheckSum)
                    */
                   
                    if (UART_RxBuffer[i+4] != EndFrame)
                        return SS_FAILURE_ENDFRAMENOTFOUND; /**< Return Error if EndFrame is not found.*/

                    // Generate Response
                    SS_RealTimeTemperature();
                    SS_RealTimeHumidity();
                    SS_RealTimeCO2();

                    // Clear command and previous positions on the RxBuffer
                    aux = i+4;
                    for(i=0;i<aux;i++)
                    {
                        UART_RxBuffer[i]=NULL;
                        RxBufLen -= 1;
                    }

                    return SS_SUCCESS;
        {
            switch(UART_RxBuffer[i+1])
            {
                case 'A':
                {    
                    //TODO: CONDITION TO VERIFIED CHECKSUM
                    /*
                    if (!SS_CalcCheckSum)
                    */
                   
                    if (UART_RxBuffer[i+4] != EndFrame)
                        return SS_FAILURE_ENDFRAMENOTFOUND; /* Return Error if EndFrame is not found. */

                    // Generate Response
                    SS_RealTimeTemperature();
                    SS_RealTimeHumidity();
                    SS_RealTimeCO2();

                    // Clear command and previous positions on the RxBuffer
                    int aux = i+4;
                    for(i=0;i<aux;i++)
                    {
                        UART_RxBuffer[i]=NULL;
                        RxBufLen -= 1;
                    }

                    return SS_SUCCESS;
                }
                case 'P':
                {
                    SensorID = UART_RxBuffer[i+2];
                    if (SensorID != TEMP_SENSOR && SensorID != HUM_SENSOR && SensorID != AIR_SENSOR)
                        return SS_FAILURE_INVALIDDATA; /* Return Error if SensorID is invalid. */
                    
                    //TODO: CONDITION TO VERIFIED CHECKSUM
                    /*
                    if (!SS_CalcCheckSum)
                    */
                   
                    if (UART_RxBuffer[i+4] != EndFrame)
                        return SS_FAILURE_ENDFRAMENOTFOUND; /* Return Error if EndFrame is not found. */

                    // Generate Response
                    if (SensorID ==TEMP_SENSOR)
                        SS_RealTimeTemperature();
                    if (SensorID == HUM_SENSOR )
                        SS_RealTimeHumidity();
                    if (SensorID == AIR_SENSOR)
                        SS_RealTimeCO2();

                    // Clear command and previous positions on the RxBuffer
                    int aux = i+4;
                    for(i=0;i<aux;i++)
                    {
                        UART_RxBuffer[i]=NULL;
                        RxBufLen -= 1;
                    }

                    return SS_SUCCESS;
                }
                case 'L':
                {
                    //TODO: CONDITION TO VERIFIED CHECKSUM
                    /*
                    if (!SS_CalcCheckSum)
                    */
                   
                    if (UART_RxBuffer[i+4] != EndFrame)
                        return SS_FAILURE_ENDFRAMENOTFOUND; /**< Return Error if EndFrame is not found.*/

                    // Generate Response
                    SS_LogTemperature();
                    SS_LogHumidity();
                    SS_LogCO2();

                    // Clear command and previous positions on the RxBuffer
                    aux = i+4;
                    for(i=0;i<aux;i++)
                    {
                        UART_RxBuffer[i]=NULL;
                        RxBufLen -= 1;
                    }

                    return SS_SUCCESS;
                }
                case 'R':
                {
                    SensorID = UART_RxBuffer[i+2];
                    if (SensorID != TEMP_SENSOR && SensorID != HUM_SENSOR && SensorID != AIR_SENSOR)
                        return SS_FAILURE_INVALIDDATA; /* Return Error if SensorID is invalid. */
                    
                    //TODO: CONDITION TO VERIFIED CHECKSUM
                    /*
                    if (!SS_CalcCheckSum)
                    */
                   
                    if (UART_RxBuffer[i+4] != EndFrame)
                        return SS_FAILURE_ENDFRAMENOTFOUND; /* Return Error if EndFrame is not found. */

                    // Generate Response
                    if (SensorID ==TEMP_SENSOR)
                        SS_ResetTemperature();
                    if (SensorID == HUM_SENSOR )
                        SS_ResetHumidity();
                    if (SensorID == AIR_SENSOR)
                        SS_ResetCO2();
                    if (SensorID == ALL)
                    {
                        SS_ResetALL();
                    }

                    // Clear command and previous positions on the RxBuffer
                    int aux = i+4;
                    for(i=0;i<aux;i++)
                    {
                        UART_RxBuffer[i]=NULL;
                        RxBufLen -= 1;
                    }

                    return SS_SUCCESS;
                }
            }
        }
}
/***********************FUNCTONS***********************/

/*******************SENSOR FUNCTONS******************/
int SS_RealTimeTemperature()
{
    return 0;
}

int SS_RealTimeHumidity()
{
    return 0;
}

int SS_RealTimeCO2()
{
    return 0;
}

int SS_LogTemperature()
{
    return 0;
}

int SS_LogHumidity()
{
    return 0;
}

int SS_LogCO2()
{
    return 0;
}

int SS_ResetTemperature()
{
    return 0;
}

int SS_ResetHumidity()
{
    return 0;
}

int SS_ResetCO2()
{
    return 0;
}
