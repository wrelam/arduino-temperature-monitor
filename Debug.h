/***************************************************************************//**
    @file   Debug.h

    @brief  Debugging facilities
*******************************************************************************/
#ifndef DEBUG_H
#define DEBUG_H

#include "Arduino.h"

/** Debug print macros, must provide newline if wanted, the printf(3) way */
#define DBG_PRINT(_s)       do                              \
                            {                               \
                                Serial.print(_s);           \
                            } while (0);

#define DBG_PRINTB(_v, _b)  do                              \
                            {                               \
                                Serial.print(_v, _b);       \
                            } while (0);

#endif // DEBUG_H

