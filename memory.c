/**
 * @file memory.c
 *
 * @brief This file implements the modeling of the physical memory
 *
 * This is the implementation of the file access modeling the
 * physical memory. If this was to be changed to a real device,
 * we only need to provide this same functions able to read from
 * and write to the real memmory
 *
 */

#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "nvm.h"



/**********************************
 * Exported module variables
 **********************************
*/
//volatile


/**********************************
 * Local module variables
 **********************************
*/
FILE *pMemory; //< The file modeling the Flash/EEPROM

/**********************************
 * Exported module variables
 **********************************
*/
//static ...


/**
 * @brief Function to initialize the memory
 *
 * This function initializes the memory, making it ready
 * to be used. This procedure erase all the data on it.
 * Actually, this version creates a new empty file, then fill
 * the allocation table area with 0xFF, meaning the memory
 * holds no data. It also sets the next available address to
 * the beggining of the values area of the memory and sets this area
 * to 0xFF.
 *
 * @return Error status: 0 for success, 0xFF for error
 */
UInt8 memInit (void)
{
    alloc_reg_t allFF[MAX_REG_ALLOC];
    UInt16 valueStartAddress = ALLOC_TABLE_LEN + SIZE_OF_MEM_ADDRESSING;
    UInt8 memValuesFF[MEM_VALUES_LEN];

    pMemory = fopen(".\\mem.bin", "wb"); //Create new, empty file
    if (!pMemory)
      return -1;

    memset((UInt8 *)allFF, 0xFF, sizeof(allFF));

    //Fill up the allocation table with 0xFF
    fwrite(allFF, 1, sizeof(allFF), pMemory);

    //Initialize the next available address.
    fwrite((UInt16 *)&valueStartAddress, 1, sizeof(UInt16), pMemory);

    //Fill up the values area with 0xFF
    memset((UInt8 *)memValuesFF, 0xFF, sizeof(memValuesFF));
    fwrite(memValuesFF, 1, sizeof(memValuesFF), pMemory);

    fclose(pMemory);

    return 0;
} //memInit (


/**
 * @brief Function to read bytes from the memory
 *
 * This function retrieves bytes from memory. Actually
 * it reads from a file that is modeling a physical memory.
 *
 * @param[in] start The start address for reading
 * @param[in] length Number of bytes to be read
 * @param[out] *buffRead Pointer to the buffer that will receive the data
 * @return Error code: Number of bytes read
 *                     0xFF for unrecoverable error
 */
UInt8 memRead (UInt16 start, UInt8 length, UInt8 *buffRead)
{
    Int8 ret = -1;
    pMemory = fopen(".\\mem.bin", "rb");
    if (!pMemory)
      return ret;
    if (fseek(pMemory, start, SEEK_SET))
      return ret;
    ret = fread(buffRead, 1, length, pMemory);
    fclose(pMemory);
    return ret;
} //memRead (

/**
 * @brief Function to write bytes to the memory
 *
 * This function writes bytes to memory. Actually
 * it writes to a file that is modeling a physical memory.
 *
 * @param[in] start The start address for writting
 * @param[in] length The length of data to be written
 * @param[out] *buffWrite Pointer to the buffer containing data to be written
 * @return Error code: 0 for writing success
 *                     -1 for writting error
 */
UInt8 memWrite (UInt16 start, UInt8 length, UInt8 *buffWrite)
{
    UInt8 ret = -1;

    pMemory = fopen(".\\mem.bin", "rb+");
    if (!pMemory)
      return ret;
    if (fseek(pMemory, start, SEEK_SET))
      return ret;
    ret = fwrite(buffWrite, 1, length, pMemory);
    fclose(pMemory);
    return ret;
} //memWrite (
