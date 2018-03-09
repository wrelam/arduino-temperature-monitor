/***************************************************************************//**
    @file   TempMonitor.h

    @brief  Temperature Monitor class
*******************************************************************************/
#ifndef TEMPMONITOR_H
#define TEMPMONITOR_H

#include <DHT_U.h>

/** @brief  Temperature Monitor class */
class TempMonitor
{
    private:
        int pin;            /**< Pin connected to DHT sensor */
        int type;           /**< Type of DHT sensor */
        DHT_Unified dht;    /**< DHT object */
        float tempC;        /**< Temperature in degrees Centigrade */
        float tempF;        /**< Temperature in degrees Fahrenheit */

    public:
        /** @brief  Default constructor
         *  @return TempMonitor object
         */
        TempMonitor(void);

        /** @brief  TempMonitor constructor
         *  @param  sPin    Pin to which a sensor is connected
         *  @param  sType   Type of sensor connected
         *  @return TempMonitor object
         */
        TempMonitor(int sPin, int sType);

        /** @brief  Update temperature readings */
        void update(void);

        /** @brief  Retrieve the last read temperature in degrees Centigrade
         *  @return Temperature in degrees Centigrade
         */
        float getTempC(void);

        /** @brief  Retrieve the last read temperature in degrees Fahrenheit
         *  @return Temperature in degrees Fahrenheit
         */
        float getTempF(void);
};

#endif // TEMPMONITOR_H

