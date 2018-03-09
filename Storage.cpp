/***************************************************************************//**
    @file   Storage.cpp

    @brief  Storage class implementation
*******************************************************************************/
#include <EEPROM.h>

#include "Debug.h"
#include "Storage.h"

/** @brief  Times to try getting a response to clear the storage area */
#define MAX_TRIES   (5)

/*******************************************************************************
    Storage
*//**
    @brief  Storage constructor
    @param  n   Number of elements to be stored

    @return A Storage object
*******************************************************************************/
template <class T>
Storage<T>::Storage(int n, T def) :
    size{n * sizeof(T)},
    startAddr{0},
    addr{startAddr},
    precision{4},
    defVal{def}
{}


/*******************************************************************************
    init
*//**
    @brief  Initialize the storage area

    This needs to be separate to prevent a constructor from failing (e.g. in the
    case of too much memory being requested).

    @return Whether initialization was successful or not
*******************************************************************************/
template <class T>
bool
Storage<T>::init(void)
{
    T data;
    char ans = '\0';
    int count = 0;

    if (size > EEPROM.length())
    {
        return false;
    }

    Serial.begin(9600);
    while (!Serial)
    {
    }

    DBG_PRINT("Checking storage data\n");
    while (addr <= size)
    {
        EEPROM.get(addr, data);
        if (defVal == data)
        {
            break;
        }

        DBG_PRINTB(data, precision);
        DBG_PRINT(",");
        addr += sizeof(data);
    }
    DBG_PRINT("\n");

    /* Only offer chance to clear if data was found */
    if (startAddr != addr)
    {
        DBG_PRINT("Clear storage? [y/n] ");
        do
        {
            if (MAX_TRIES == ++count)
            {
                ans = 'n';
                break;
            }

            if (Serial.available())
            {
                ans = tolower(Serial.read());

                // Consume remaining input
                while (Serial.available())
                {
                    Serial.read();
                }

                break;
            }

            delay(1000);
        } while (1);

        Serial.write(ans);
        DBG_PRINT("\n");

        if ('y' == ans)
        {
            DBG_PRINT("\nClearing storage");
            for (size_t i = startAddr; i < size; i += sizeof(T))
            {
                EEPROM.put(i, defVal);
                // Display one dot per 8 items cleared (arbitrary)
                if (0 == (i % (8 * sizeof(T))))
                {
                    Serial.print(".");
                }
            }
            addr = startAddr;
            DBG_PRINT("\nStorage cleared\n");
        }
    }
    else
    {
        DBG_PRINT("No data found\n");
    }

    return true;
}


/*******************************************************************************
    write
*//**
    @brief  Writes a piece of data to the storage area
    @param  &data   Data to be written
*******************************************************************************/
template <class T>
void
Storage<T>::write(T const& data)
{
    EEPROM.put(addr, data);
    addr += sizeof(data);
    if (addr >= startAddr + size)
    {
        addr = startAddr;
    }
}


// Instantiate the one type we know will be used. Otherwise the whole template
// implementation needs to go in the header (I think?)
template class Storage<float>;

