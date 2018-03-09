/***************************************************************************//**
    @file   TempMonitor.cpp

    @brief  Temperature Monitor class implementation
*******************************************************************************/
#include "TempMonitor.h"

/*******************************************************************************
    TempMonitor
*//**
    @brief  Default constructor

    The created monitor is probably unusable.

    @retval TempMonitor object
*******************************************************************************/
TempMonitor::TempMonitor(void) :
    pin{0}, type{0}, dht{(uint8_t) pin, (uint8_t) type}
{}

/*******************************************************************************
    TempMonitor
*//**
    @brief  Constructs a TempMonitor object
    @param  sPin    Pin to which a sensor is connected
    @param  sType   Type of sensor connected

    @return TempMonitor object
*******************************************************************************/
TempMonitor::TempMonitor(int sPin, int sType) :
    pin{sPin}, type{sType}, dht{(uint8_t) pin, (uint8_t) type}
{
    dht.begin();
}


/*******************************************************************************
    update
*//**
    @brief  Updates temperatures readings
*******************************************************************************/
void
TempMonitor::update(void)
{
    sensors_event_t event;

    dht.temperature().getEvent(&event);
    if (!isnan(event.temperature))
    {
        tempC = event.temperature;
        tempF = (tempC * ((float) 9 / 5)) + 32;
    }
}


/*******************************************************************************
    getTempC
*//**
    @brief  Retrieve the last read temperature in degrees Centigrade

    @return Temperature in degrees Centigrade
*******************************************************************************/
float
TempMonitor::getTempC(void)
{
    return tempC;
}


/*******************************************************************************
    getTempF
*//**
    @brief  Retrieve the last read temperature in degrees Fahrenheit

    @return Temperature in degrees Fahrenheit
*******************************************************************************/
float
TempMonitor::getTempF(void)
{
    return tempF;
}

