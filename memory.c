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

#include "memory.h"
#include "nvm.h"



/**********************************
 * Exported module variables
 **********************************
*/
volatile


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
 * Actually, this version creates a new empty file all fill
 * the allocation table area with 0xFF, meaning the memory
 * holds no data.
 *
 * @return Error code: 0 for success, -1 for error
 */
Int8 memInit (void)
{
    alloc_reg_t allFF[MAX_REG_ALLOC];
    UInt16 valueStartAddress = ALLOC_TABLE_LEN + SIZE_OF_MEM_ADDRESSING;

    pMemory = fopen(".\\mem.bin", "wb");
    if (!pMemory)
      return -1;

    memset((UInt8 *)allFF, 0xFF, sizeof(allFF));

    fwrite(allFF, sizeof(allFF), 1, pMemory); //Fill up the allocation
                                              //table with 0xFF
    //Initialize the next available address.
    fwrite((UInt16 *)&valueStartAddress,sizeof(UInt16),1,pMemory);
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
 *                     -1 for unrecoverable error
 */
Int8 memRead (UInt16 start, UInt8 length, UInt8 *buffRead)
{
    pMemory = fopen(".\\mem.bin", "rb");
    if (!pMemory)
      return -1;
    if (fseek(pMemory, start, SEEK_SET))
      return -1;
    return (fread(buffRead, length, 1, pMemory));
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
Int8 memWrite (UInt16 start, UInt8 length, UInt8 *buffWrite)
{
    pMemory = fopen(".\\mem.bin", "ab+");
    if (!pMemory)
      return -1;
    if (fseek(pMemory, start, SEEK_SET))
      return -1;
    return (fwrite(buffWrite, length, 1, pMemory));
} //memWrite (
