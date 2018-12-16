/**
 * @file nvm.c
 * @brief This file contains the implementation of the API functions:
 * @ref gpNvm_GetAttribute and @ref gpNvm_SetAttribute.
 *
 * This is the core of the non-volatile memory storage component
 * implementation. Here the functions uses @ref memRead and
 * @ref memWrite, that in this exercise reads from and writes to
 * a file. In order to use an actual Flash/EEPROM memory one just
 * need to provide those lower level basic functions.
 *
 * @author Marcio J Teixeira Jr.
 * @date 09/12/18
 *
 */

#include <stdio.h>

#include "nvm.h"
#include "memory.h"

/**
 * @brief Macro to calculate the address of an AttrId on the
 * allocation table
 *
 * This macro takes an Attribute ID and returns the
 * corresponding memory address where the record is saved at.
 *
 */
#define ID_ADDRESS(x) (x<<2) //Since the record length is 4 bytes, let's
                             //take advantage of bit shifting

/**
 * @brief Function to retrieve a value from memory, based on a attrib
 *
 * This function reads the memory, looking for the information
 * saved under attribute. It first read the allocation register to
 * find out where the actual data is stored and its length.
 * It returns the Lenght and the Value stored for this Attribute.
 * There is an integrity checking on the allocation table, made by a CRC-8
 * If the CRC doesn't match, it means the register in corrupted, so it will
 * return an error.
 * There is another integrity checking on the actual data (value), achieved
 * by a CRC-16. If this CRC doesn't match, it means the data is corrupted.
 * This approach can be improved by using a CRC-correcting algorithm in a way
 * that it could identify 1-bit flip and correct it.
 *
 * @param[in] attrId The Id of the attribute to be read
 * @param[out] pLength the length of the value retrieved (in bytes)
 * @param[out] pValue the value retrieved
 * @return Error code: 0xFF for unrecoverable error,
 *                     positive for number of bits recovered by CRC correction
**/
gPNvm_Result gpNvm_GetAttribute(gPNvm_AttrId attrId,
                                UInt8 *pLength,
                                UInt8 *pValue)
{
    UInt16 start, crcRead;
    alloc_reg_t readReg;

    //retrieve the allocation register
    memRead(ID_ADDRESS(attrId), ALLOC_REG_LEN, (UInt8 *)&readReg);
    //checks the register CRC
    if (calcCRC8((UInt8 *)&readReg, ALLOC_REG_LEN))
        return 0xFF;
    *pLength = memRead(readReg.start, readReg.length, pValue);

    memRead(readReg.start + readReg.length, CRC_LEN, (UInt8 *)&crcRead);
    // To avoid allocating 256 bytes here and putting the read value
    // and CRC appended, and perform a CRC checking with the full data
    // expecting a zero, it will be more efficient to calculate the CRC
    // over the value and compare with the CRC read. This way we need only 2 bytes
    if (crcRead != calcCRC16(pValue, *pLength))
        return 0xFF;

    //TODO: Implement the CRC correction on the data
    // correctedBits = checkCRC(pValue, *pLength);

    return 0;
}

/**
 * @brief Function to store a value in the memory, based on a Attribute
 *
 * This function writes a value into the memory, under an Attribute.
 * It returns the number of bytes actually written.
 * In order to write the data, it first reads the special address memory
 * @ref NEXT_FREE_ADDR which holds the next available address on the memory.
 * Later it updates this value summing the length.
 * Then, it assemble the allocation register using this address as the start
 * and the @p length input parameter as the length. Later it stores the value
 * itself in the determined address.
 * The CRC-8 is calculated over the allocation rergister before storing it.
 * This is meant to check the integrity on the future readings.
 * A CRC-16 is also calculated over the value bytes, and this CRC is appended
 * in the end of the value. This is intended to guarantee the data integrity
 * on the future readings.
 * An improvement could be done here, changing this
 * method to a CRC-correcting, an algorithm that can identify a 1-bit flip
 * and correct it.
 *
 *
 * @param[in] attrId The Id of the attribute to be saved
 * @param[in] length The length of the value to be saved, in bytes.
 *                   0xFF not allowed (reserved).
 * @param[in] pValue Pointer to the value to be saved
 * @return Number of bytes written, 0xFF for error.
 *
**/
gPNvm_Result gpNvm_SetAttribute(gPNvm_AttrId attrId,
                                UInt8 length,
                                UInt8 *pValue)
{
    UInt16 start, crc16Calc;
    alloc_reg_t aReg;

    //retrieve the allocation register to check the length
    memRead(ID_ADDRESS(attrId), ALLOC_REG_LEN, (UInt8 *)&aReg);
    // If there's already a value stored under this Attribute,
    // only updates if the length is the same. Attempts to write
    // the same attribute with different length will return error (0xFF)
    if ((aReg.length != 0xFF) && (aReg.length != length))
        return 0xFF;

    //retrieve the next available address
    memRead(NEXT_FREE_ADDR, SIZE_MEM_ADDRESS, (UInt8 *)&start);
    aReg.start = start;
    aReg.length = length;
    aReg.crc = calcCRC8(((UInt8 *)&aReg), ALLOC_REG_NO_CRC);

    //update the next available address
    start += (length + CRC_LEN);
    memWrite(NEXT_FREE_ADDR, SIZE_MEM_ADDRESS, (UInt8 *)&start);

    //Store the allocation register
    memWrite(ID_ADDRESS(attrId), ALLOC_REG_LEN, (UInt8 *)&aReg);
    //Store the value
    if (aReg.length != memWrite(aReg.start, aReg.length, pValue))
      return 0xFF;
    //Store the CRC-16 of value
    crc16Calc = calcCRC16(pValue, length);
    if (CRC_LEN != memWrite(aReg.start + length, CRC_LEN, (UInt8 *)&crc16Calc))
      return 0xFF;

    return 0;
}
