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
#include <time.h>

/******************INTERNAL VARIABLES******************/
static uint8_t UART_RxBuffer[BUFFER_SIZE];
static uint8_t RxBufLen = 0;

static uint8_t UART_TxBuffer[BUFFER_SIZE];
static uint8_t TxBufLen = 0;

static uint8_t Temperature[3*MAX_TEM_RECORD];  /**< Array for storing temperature measures*/
static uint8_t Humidity[3*MAX_HUM_RECORD];         /**< Array for storing humidity measures*/
static uint8_t Air_CO2[5*MAX_AIR_RECORD];            /**< Array for storing dioxide measures*/

/*********************************************************/

/***********************FUNCTONS***********************/
int SS_InitMeasures(void)
{
    int i;

    for(i=0;i<(int)sizeof(Temperature);i++)
    {
        Temperature[i] = 0;
        Humidity[i] = 0;
    }

    for(i=0;i<(int)sizeof(Air_CO2);i++)
    {
        Air_CO2[i] = 0;
    }
    return SS_SUCCESS;
}

int SS_ResetRxBuffer(void)
{
    for(int i=0;i<BUFFER_SIZE;i++)
    {
        UART_RxBuffer[i] = 0;
    }
    RxBufLen = 0;
    return SS_SUCCESS;
}

int SS_ResetTxBuffer(void)
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
    UART_TxBuffer[TxBufLen] = aux;
    TxBufLen += 1;
    return SS_SUCCESS;
}

void getTxBuffer(uint8_t * buf, uint8_t *len)
{
    int i;
    *len = TxBufLen;

    // Gets the TxBuffer
	if(TxBufLen > 0) {
		memcpy(buf,UART_TxBuffer, *len);
	}

    // Proceeds to clear the TxBuffer
    for(i=TxBufLen;i>-1;i--)
    {
        UART_TxBuffer[i]=0;
        TxBufLen -= 1;
    }		
	return;
}

int SS_ProcessCom(void)
{
        // Check if RxBuffer is empty
        if (RxBufLen==0) 
        {
            return SS_FAILURE_BUFFEREMPTY;
        }

        int i,aux;
        uint8_t SensorID; //Symbol that identifies the type of sensor

        // Find index of StartFrame
        for (i=0; i<RxBufLen;i++)
        {
            if(UART_RxBuffer[i]==StartFrame) break;
        }

        //If StartFrame exists, analyse the command
        if(i<RxBufLen)
        {
            switch(UART_RxBuffer[i+1])
            {
                case 'A':
                {    
                    //TODO: CONDITION TO VERIFIED CHECKSUM
                    /*
                    if (!SS_CalcCheckSum)
                    */

                    // Checks if EndFrame is presented
                    if (UART_RxBuffer[i+4] != EndFrame)
                    {
                        // Clears frame size from RxBuffer
                        aux = i+3;
                        for(i=0;i<aux;i++)
                        {
                            UART_RxBuffer[i]=0;
                            RxBufLen -= 1;
                        }
                        return SS_FAILURE_ENDFRAMENOTFOUND;
                    }

                    // Generate Response
                    SS_RealTimeTemperature(UART_RxBuffer[i+1]);
                    SS_RealTimeHumidity(UART_RxBuffer[i+1]);
                    SS_RealTimeCO2(UART_RxBuffer[i+1]);

                    // Clear command and previous positions on the RxBuffer
                    int aux = i+4;
                    for(i=0;i<aux;i++)
                    {
                        UART_RxBuffer[i]=0;
                        RxBufLen -= 1;
                    }
                    return SS_SUCCESS;
                }
                case 'P':
                {
                    SensorID = UART_RxBuffer[i+2];
                    // Checks if the sensor symbol is correct
                    if (SensorID != TEMP_SENSOR && SensorID != HUM_SENSOR && SensorID != AIR_SENSOR)
                    {
                        aux = i+2;
                        for(i=0;i<aux;i++)
                        {
                            UART_RxBuffer[i]=0;
                            RxBufLen -= 1;
                        }
                        return SS_FAILURE_INVALIDDATA;
                    }

                    //TODO: CONDITION TO VERIFIED CHECKSUM
                    /*
                    if (!SS_CalcCheckSum)
                    */
                   
                    // Checks if EndFrame is presented
                    if (UART_RxBuffer[i+4] != EndFrame)
                    {
                        aux = i+3;
                        for(i=0;i<aux;i++)
                        {
                            UART_RxBuffer[i]=0;
                            RxBufLen -= 1;
                        }
                        return SS_FAILURE_ENDFRAMENOTFOUND; /* Return Error if EndFrame is not found. */
                    }

                    // Generate Response matching the sensor
                    if (SensorID ==TEMP_SENSOR)
                        SS_RealTimeTemperature(UART_RxBuffer[i+1]);
                    if (SensorID == HUM_SENSOR )
                        SS_RealTimeHumidity(UART_RxBuffer[i+1]);
                    if (SensorID == AIR_SENSOR)
                        SS_RealTimeCO2(UART_RxBuffer[i+1]);

                    // Clear command and previous positions on the RxBuffer
                    int aux = i+4;
                    for(i=0;i<aux;i++)
                    {
                        UART_RxBuffer[i]=0;
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
                    
                    // Checks if EndFrame is presented
                    if (UART_RxBuffer[i+4] != EndFrame)
                    {
                        aux = i+3;
                        for(i=0;i<aux;i++)
                        {
                            UART_RxBuffer[i]=0;
                            RxBufLen -= 1;
                        }
                        return SS_FAILURE_ENDFRAMENOTFOUND;
                    }

                    // Generate Responses
                    SS_LogTemperature();
                    SS_LogHumidity();
                    SS_LogCO2();

                    // Clear command and previous positions on the RxBuffer
                    aux = i+4;
                    for(i=0;i<aux;i++)
                    {
                        UART_RxBuffer[i]=0;
                        RxBufLen -= 1;
                    }

                    return SS_SUCCESS;
                }
                case 'R':
                {
                    SensorID = UART_RxBuffer[i+2];
                    // Checks if the sensor symbol is correct
                    if (SensorID != TEMP_SENSOR && SensorID != HUM_SENSOR && SensorID != AIR_SENSOR && SensorID != ALL)
                    {
                        aux = i+2;
                        for(i=0;i<aux;i++)
                        {
                            UART_RxBuffer[i]=0;
                            RxBufLen -= 1;
                        }
                        return SS_FAILURE_INVALIDDATA; /* Return Error if SensorID is invalid. */
                    }
                    
                    //TODO: CONDITION TO VERIFIED CHECKSUM
                    /*
                    if (!SS_CalcCheckSum)
                    */
                   
                    // Checks if EndFrame is presented
                    if (UART_RxBuffer[i+4] != EndFrame)
                    {
                        aux = i+3;
                        for(i=0;i<aux;i++)
                        {
                            UART_RxBuffer[i]=0;
                            RxBufLen -= 1;
                        }
                        return SS_FAILURE_ENDFRAMENOTFOUND; /* Return Error if EndFrame is not found. */
                    }

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
                        UART_RxBuffer[i]=0;
                        RxBufLen -= 1;
                    }

                    return SS_SUCCESS;
                }
                default:
                {
                    aux = i+1;
                    for(i=0;i<aux;i++)
                    {
                        UART_RxBuffer[i]=0;
                        RxBufLen -= 1;
                    }
                    return SS_FAILURE_COMMANDNOTFOUND;
                }
            }
        }
        //Clears the characters already read from the RxBufLen
        for(i=0;i<RxBufLen;i++)
        {
            UART_RxBuffer[i]=0;
            RxBufLen -= 1;
        }
        return SS_FAILURE_STARTFRAMENOTFOUND;
}
/***********************FUNCTONS***********************/

/*******************SENSOR FUNCTONS******************/
int SS_RealTimeTemperature(uint8_t CMD)
{
    uint8_t data1, data2, data3;
    int i,aux;

    // Generate random value inside of range for temperature
    srand(time(NULL));
    aux = rand()%2;
    if (aux)
    {
        data1 = '+';
    }
    else
    {
        data1 = '-';
    }

    if (data1=='+')
    {
        data2 =  (uint8_t)(rand()%7);
        if(data2 == 6)
        {
            data3 = 0;
        }
        else
        {
            data3 = (uint8_t)(rand()%10);
        } 
    }
    else
    {
        data2 = (uint8_t)(rand()%6);
        if(data2 == 5)
        {
            data3 = 0;
        }
        else
        {
            data3 = (uint8_t)(rand()%10);
        } 
    }

    // Save measure for log history
    // Finds first empty spot on the log history
    for(i=0;i<MAX_TEM_RECORD;i=i+3)
    {
        if (Temperature[i]!='+' && Temperature[i]!='-')
        {
            Temperature[i] = data1;     // Identifies positive or negative temperature
            Temperature[i+1] = data2;   //
            Temperature[i+3] = data3;   //
            break;
        }
    }
    if (i!=MAX_TEM_RECORD)
    {   
        // Checks if TxBuffer can save response
        if(TxBufLen<=BUFFER_SIZE-8)
        { 
            //Write Response Command
            SS_AddCharTx(StartFrame);   // Startframe Symbol
            SS_AddCharTx(CMD+32);       // Command Response
            SS_AddCharTx(TEMP_SENSOR+32);// Sensor ID
            SS_AddCharTx(data1);        // Temperature sign
            SS_AddCharTx(48+data2);     // Converts decimal to ASCII
            SS_AddCharTx(48+data3);     // Converts decimal to ASCII
            SS_AddCharTx('0');          // Checksum value
            SS_AddCharTx(EndFrame);     // EndFrame Symbol

            //DEBUG
            printf("\n");
            for(i=0;i<TxBufLen;i++)
            {
                printf("%c",UART_TxBuffer[i]);
            }
            
            return SS_SUCCESS;
        } 
        else
        {
            return SS_FAILURE_BUFFERFULL;
        }
    }
    return SS_FAILURE_RECORDFULL;
    
}

int SS_RealTimeHumidity(uint8_t CMD)
{
    uint8_t data1, data2, data3;
    int i,aux;

    // Generate random value inside of range for humidity
    srand(time(NULL));
    aux = rand()%100;
    if (!aux)       // Maximum value for humidity
    {
        data1 = 1;
        data2 = 0;
        data3 = 0;
    }
    else            // Random value between 0 and 99%
    {
        data1 = 0;
        data2 = (uint8_t) (rand()%10);
        data3 = (uint8_t) (rand()%10);
    }

    // Save measure for log history
    // Finds first empty spot on the log history
    for(i=0;i<MAX_HUM_RECORD;i=i+3)
    {
        if (Humidity[i]!=0)
        {
            Humidity[i] = data1;
            Humidity[i+1] = data2;
            Humidity[i+2] = data3;
            break;
        }
    }

    if (i!=MAX_HUM_RECORD)
    {
        if(TxBufLen<=BUFFER_SIZE-8)
        { 
            //Insert Command
            SS_AddCharTx(StartFrame);   // Startframe Symbol
            SS_AddCharTx(CMD+32);       // Command Response
            SS_AddCharTx(HUM_SENSOR+32);// Sensor ID
            SS_AddCharTx(48+data1);     // Converts decimal to ASCII
            SS_AddCharTx(48+data2);     // Converts decimal to ASCII
            SS_AddCharTx(48+data3);     // Converts decimal to ASCII
            SS_AddCharTx('0');          // Checksum value
            SS_AddCharTx(EndFrame);     // EndFrame Symbol
        
            //DEBUG
            printf("\n");
            for(i=0;i<TxBufLen;i++)
            {
                printf("%c",UART_TxBuffer[i]);
            }

            return SS_SUCCESS;
        } 
            else
        {
            return SS_FAILURE_BUFFERFULL;
        } 
    }
    return SS_FAILURE_RECORDFULL;
}

int SS_RealTimeCO2(uint8_t CMD)
{
    

    uint8_t data1, data2, data3, data4, data5;
    int i;
    // Generate random value inside of range for Co2 sensor
    srand(time(NULL));
    if (!rand()%19600)  //Maximum value
    {
        data1 = 2;
        data2 = 0;
        data3 = 0;
        data4 = 0;
        data5 = 0;
    }
    else                // Random value between 0 and 19000
    {
        data1 = (uint8_t) (rand()%2);
        data2 = (uint8_t) (rand()%10);
        data3 = (uint8_t) (rand()%10);
        data4 = (uint8_t) (rand()%10);
        data5 = (uint8_t) (rand()%10);
    }
    if(!data1 && !data2) // Sets the mininum value to 400
    {
        data3 = 4;
    } 

    // Save measure for log history
    // Finds first empty spot on the log history
    for(i=0;i<MAX_AIR_RECORD;i=i+5)
    {
        if (Air_CO2[i]!=0 || Air_CO2[i+1]!=0 || Air_CO2[i+2]!=0)
        {
            Air_CO2[i] = data1;
            Air_CO2[i+1] = data2;
            Air_CO2[i+2] = data3;
            Air_CO2[i+3] = data4;
            Air_CO2[i+4] = data5;
            break;
        }
    }

    if (i!=MAX_AIR_RECORD)
    {
        if(TxBufLen<=BUFFER_SIZE-8)
        { 
            //Insert Command
            SS_AddCharTx(StartFrame);// Startframe Symbol
            SS_AddCharTx(CMD+32);// Command Response
            SS_AddCharTx(AIR_SENSOR+32);// Sensor ID
            SS_AddCharTx(48+data1);// Converts decimal to ASCII
            SS_AddCharTx(48+data2);// Converts decimal to ASCII
            SS_AddCharTx(48+data3);// Converts decimal to ASCII
            SS_AddCharTx(48+data4);
            SS_AddCharTx(48+data5);
            SS_AddCharTx('0');// Checksum value
            SS_AddCharTx(EndFrame);// EndFrame Symbol
    
            /*//DEBUG
            printf("\n");
            for(i=0;i<TxBufLen;i++)
            {
                printf("%c",UART_TxBuffer[i]);
            }*/

            return SS_SUCCESS;
        } 
        else
        {
            return SS_FAILURE_BUFFERFULL;
        } 
    }
    return SS_FAILURE_RECORDFULL;
}

int SS_LogTemperature(void)
{
    int i = 0;
    int data1, data2, data3;
    for(i=0;i<LOG_SAMPLES;i=i+3)
    {
        if (Temperature[i]=='+' || Temperature[i]=='-')
        {
            data1 = Temperature[i];
            data2 = Temperature[i+1];
            data3 = Temperature[i+2];
        }
        if (i!=MAX_TEM_RECORD)
        {   
            if(TxBufLen<=BUFFER_SIZE-8)
            { 
            //Insert Command
            SS_AddCharTx(StartFrame);
            SS_AddCharTx('l');
            SS_AddCharTx('t');
            SS_AddCharTx(data1+48);
            SS_AddCharTx(data2+48);
            SS_AddCharTx(data3+48);
            SS_AddCharTx('0');
            SS_AddCharTx('!');
            } 
            else
            {
                return SS_FAILURE_BUFFERFULL;
            } 
        }
        else
        {
            return SS_SUCCESS;
        }  
    }       
       
}

int SS_LogHumidity(void)
{
    return 0;
}

int SS_LogCO2(void)
{
    return 0;
}

int SS_ResetTemperature(void)
{
    return 0;
}

int SS_ResetHumidity(void)
{
    return 0;
}

int SS_ResetCO2(void)
{
    return 0;
}

int SS_ResetALL(void)
{
    return 0;
}