/**
 * @file nvm.c
 *
 */

#include <stdio.h>

#include "nvm.h"
#include "memory.h"

//Macro to calculate the address of an AttrId
#define ID_ADDRESS(x) (x<<2)

/**
 * @brief Function to retrieve a value from memory, based on a attrib
 *
 * This function reads the memory, looking for the information
 * saved under attribute. It returns the Value stored
 *
 * @param[in] attrId The Id of the attribute to be read
 * @param[out] pLength the length of the value retrieved (in bytes)
 * @param[out] pValue the value retrieved
 * @return Error code: 0 for reading success,
 *                     -1 for unrecoverable error,
 *                     positive for number of bits recovered by CRC correction
**/
gPNvm_Result gpNvm_GetAttribute(gPNvm_AttrId attrId,
                                UInt8 *pLength,
                                UInt8 *pValue)
{
    UInt16 start;
    alloc_reg_t readReg;

    //retrieve the allocation register
    memRead(ID_ADDRESS(attrId), ALLOC_REG_LEN, (UInt8 *)&readReg);
    *pLength = memRead(readReg.start, readReg.length, pValue);

    // checkCRC(pValue, *pLength);

    return 0;
    //TODO: Implement the CRC correction
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
 * @return Error code: Number of bytes written,
 *                     -1 for error
**/
gPNvm_Result gpNvm_SetAttribute(gPNvm_AttrId attrId,
                                UInt8 length,
                                UInt8 *pValue)
{
    UInt16 start;
    alloc_reg_t writeReg;

    //retrieve the next available address
    memRead(ALLOC_TABLE_LEN, SIZE_OF_MEM_ADDRESSING, (UInt8 *)&start);
    writeReg.start = start;
    writeReg.length = length;
    writeReg.crc = 0; //TODO: calculate 1 byte CRC for alloc table reg

    //update the next available address
    start += length;
    memWrite(ALLOC_TABLE_LEN, SIZE_OF_MEM_ADDRESSING, (UInt8 *)&start);

    //Store the allocation register
    memWrite(ID_ADDRESS(attrId), ALLOC_REG_LEN, (UInt8 *)&writeReg);
    //Store the value
    if (writeReg.length != memWrite(writeReg.start, writeReg.length, pValue))
      return -1;

    return 0;
}

/**
 * @brief main is the unit test function
 *
 * Just for unit testing...
 */

#if 0
UInt8 main(void)
{
    FILE *pMemory; //< This is the Flash/EEPROM model
    gPNvm_Result gpNvm_err = 0;
    void *pRead_len;
    void *pRead_val;
    //Open the file for binary access, create if doesn't exists
    pMemory = fopen(".\\mem.bin","ab+");
    UInt8 contErr = 0;

    UInt8 run_tests = UINT8_TEST \
                    | UINT16_TEST \
                    | UINT32_TEST \
                    | UINT16_ARRAY_TEST \
                    | STRING_TEST \
                    | SIMPLESTRUCT_TEST \
                    | COMPLEXSTRUCT_TEST;

    printf("uint8: %d bytes\nuint16: %d bytes\nuint32: %d bytes\n",
            sizeof(UInt8), sizeof(UInt16), sizeof(UInt32));

    printf("Tests: %x\n\n", run_tests);
    printf("UInt8 test: %d\n", (run_tests & UINT8_TEST));
    printf("UInt16 test: %d\n", (run_tests & UINT16_TEST));



    /**** int 8 test **/
    if (run_tests & UINT8_TEST)
    {
        UInt8 TestInt8 = TEST_VALUE_INT8;
        UInt8 *pTestInt8 = &TestInt8;

        gpNvm_err = gpNvm_SetAttribute(TEST_8BIT_ID, sizeof(UInt8), (UInt8 *)pTestInt8);
        if (!gpNvm_err)
        {
            printf("Test UInt8 write OK.\n");
        }
        else
            contErr++;
        gpNvm_err = gpNvm_GetAttribute(TEST_8BIT_ID, (UInt8 *)pRead_len, (UInt8 *)pRead_val);
        if (!gpNvm_err)
        {
            printf("Test UInt8 read OK.\n");
            pRead_val = pTestInt8;
            if (*(UInt8 *)pRead_val == TEST_VALUE_INT8)
            {
                printf("Test UInt8 read value OK.\n");
            }
            else
                contErr++;
        }
        else
            contErr++;
    }
    /**** int 16 test **/
    if (run_tests & UINT16_TEST)
    {
        UInt16 TestInt16 = TEST_VALUE_INT16;
        UInt16 *pTestInt16 = &TestInt16;

        gpNvm_err = gpNvm_SetAttribute(TEST_16BIT_ID, sizeof(UInt16), (UInt8 *)pTestInt16);
        if (!gpNvm_err)
        {
            printf("Test UInt16 write OK.\n");
        }
        else
            contErr++;
        gpNvm_err = gpNvm_GetAttribute(TEST_16BIT_ID, (UInt8 *)pRead_len, (UInt8 *)pRead_val);
        if (!gpNvm_err)
        {
            printf("Test UInt16 read OK.\n");
            pRead_val = pTestInt16;
            if (*(UInt16 *)pRead_val == TEST_VALUE_INT16)
            {
                printf("Test UInt16 read value OK.\n");
            }
            else
                contErr++;
        }
        else
            contErr++;
    }

    /**** int 32 test **/
    if (run_tests & UINT32_TEST)
    {
        UInt32 TestInt32 = TEST_VALUE_INT32;
        UInt32 *pTestInt32 = &TestInt32;

        gpNvm_err = gpNvm_SetAttribute(TEST_32BIT_ID, sizeof(UInt16), (UInt8 *)pTestInt32);
        if (!gpNvm_err)
        {
            printf("Test UInt32 write OK.\n");
        }
        else
            contErr++;
        gpNvm_err = gpNvm_GetAttribute(TEST_32BIT_ID, (UInt8 *)pRead_len, (UInt8 *)pRead_val);
        if (!gpNvm_err)
        {
            printf("Test UInt32 read OK.\n");
            pRead_val = pTestInt32;
            if (*(UInt32 *)pRead_val == TEST_VALUE_INT32)
            {
                printf("Test UInt32 read value OK.\n");
            }
            else
                contErr++;
        }
        else
            contErr++;
    }

    /**** float test **/
    /**** int 16 array test **/
    /**** string test **/
    /**** simple struct test **/
    /**** complex struct test **/

    printf("result: %d\n", contErr);
    printf("FIM\n");
    getchar();
    fclose(pMemory);
    return contErr;
}
#endif
