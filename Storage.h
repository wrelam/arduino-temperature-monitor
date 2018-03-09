/***************************************************************************//**
    @file   Storage.h

    @brief  Storage class
*******************************************************************************/
#ifndef STORAGE_H
#define STORAGE_H

template <class T>
class Storage
{
    private:
        size_t size;        /**< Size of storage area, in bytes */
        size_t startAddr;   /**< First address in storage area */
        size_t addr;        /**< Current working address */
        int precision;      /**< Precision of formatted prints */
        T defVal;           /**< Default data value */

    public:
        /** @brief  Storage constructor
         *  @param  n   Number of objects of type T to be stored
         *  @param  def Default value for objects in storage
         *  @return Storage object
         */
        Storage(int n, T def);

        /** @brief  Initializes the storage area
         *  @return Whether initialization succeeded or not
         */
        bool init(void);

        /** @brief  Write data to the current working address
         *  @param  data    Data to be written
         */
        void write(T const& data);
};

#endif // STORAGE_H

