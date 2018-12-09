/**
 * @file nvm.c
 *
 */


#include <stdio.h>

#include "nvm.h"

typedef unsigned char UInt8;
typedef UInt8 gPNvm_AttrId;
typedef UInt8 gPNvm_Result;

gPNvm_Result gpNvm_GetAttribute (gPNvm_AttrId attrId,
                                 UInt8*       pLength,
                                 UInt8*       pValue);

gPNvm_Result gpNvm_SetAttribute (gPNvm_AttrId attrId,
                                 UInt8        length,
                                 UInt8*       pValue);


/**
 * @brief Function to retrieve a value from memory, based on a attrib
 *
 * This function reads the memory, looking for the information
 * saved under attribute. It returns the Value stored
 *
 * @param[in] attrId The Id of the attribute to be read
 * @param[out] pLength the length of the value retrieved (in bytes)
 * @param[out] pValue the value retrieved
 */
 gPNvm_Result gpNvm_GetAttribute(gPNvm_AttrId attrId,
                                UInt8 *pLength,
                                UInt8 *pValue)
{

}

/**
 * @brief Function to store a value in the memory, based on a attrib
 *
 * This function writes a value into the memory, under attribute.
 *  It returns the Value stored
 *
 * @param[in] attrId The Id of the attribute to be saved
 * @param[in] length the length of the value to be saved (in bytes)
 * @param[out] pValue the value to be saved
 */
gPNvm_Result gpNvm_SetAttribute(gPNvm_AttrId attrId,
                                UInt8 length,
                                UInt8 *pValue)
{

}

/**
 * @brief main is the unit test function
 *
 * Just for unit testing...
 */


void main(void)
{
    FILE *pMemory; //< This is the Flash/EEPROM model
    gPNvm_Result written = 0;
    gPNvm_Result read = 0;
    //Open the file for binary access, crete if doesn't exists
    pMemory = fopen(".\\mem.bin","ab+");

    /**** int 8 test **/
    UInt8 TestInt8 = TEST_VALUE_INT8;
    UInt8 *pTestInt8 = &TestInt8;
    UInt8 *pRead_len;
    UInt8 *pRead_val;

    written = gpNvm_SetAttribute(TEST_8BIT_ID, sizeof(UInt8), (UInt8 *)pTestInt8);
    if (written == sizeof(UInt8))
    {
        printf("Test UInt8 write OK.\n");
    }
    read = gpNvm_GetAttribute(TEST_8BIT_ID, pRead_len, pRead_val);
    if (read == sizeof(UInt8))
    {
        printf("Test UInt8 read OK.\n");
        if (*pRead_val == TEST_VALUE_INT8)
        {
            printf("Test UInt8 read value OK.\n");
        }
    }

    /**** int 16 test **/
    UInt16 TestInt16 = TEST_VALUE_INT16;
    UInt16 *pTestInt16 = &TestInt16;
    UInt16 *pRead_len;
    UInt16 *pRead_val;

    written = gpNvm_SetAttribute(TEST_8BIT_ID, sizeof(UInt8), (UInt8 *)pTestInt8);
    if (written == sizeof(UInt8))
    {
        printf("Test UInt8 write OK.\n");
    }
    read = gpNvm_GetAttribute(TEST_8BIT_ID, pRead_len, pRead_val);
    if (read == sizeof(UInt8))
    {
        printf("Test UInt8 read OK.\n");
        if (*pRead_val == TEST_VALUE_INT8)
        {
            printf("Test UInt8 read value OK.\n");
        }
    }

    /**** int 32 test **/
    /**** float test **/
    /**** int 16 array test **/
    /**** string test **/
    /**** simple struct test **/
    /**** complex struct test **/




    return 0;
}
