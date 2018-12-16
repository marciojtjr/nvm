/**
 * @file nvm_tests.c
 * @brief Unit tests file, based on the UNITY unit test harness
 *
 * This file contains all the individual unit tests functions.
 * The control of the tests is done on the main.c file.
 *
 * @author Marcio J Teixeira Jr.
 * @date 09/12/18
 *
 */


#include "nvm.h"
#include "memory.h"
#include "nvm_tests.h"
#include "..\Unity\src\unity.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Macro to calculate the address of an AttrId
 *
 * This macro takes an Attribute ID and returns the
 * corresponding memory address where the record is saved at.
 *
 */
#define ID_ADDRESS(x) (x<<2) //Since the record length is 4 bytes, let's
                             //take advantage of bit shifting


FILE *pTestMemory; ///< The testing file
gPNvm_Result gpNvm_err = 0; ///< Variable to receive the result of Get and Set
void *pReadLen; ///< Pointer to receive the length from the Get function
void *pReadVal; ///< Pointer to receive the value from the Get function

/**
 * @brief Unity setUp function
 *
 * Unity tool requires this function to exist even being
 * empty. The Unity engine calls it before every test function.
 *
 */
void setUp(void)
{
}

/**
 * @brief Unity tearDown function
 *
 * Unity tool requires this function to exist even being
 * empty. The Unity engine calls it after every test function.
 *
 */
void tearDown(void)
{
    if (pTestMemory)
        fclose(pTestMemory);
}

/**
 * @brief This is a dummy test function
 *
 * Dummy test just to make sure the Unity tool is
 * working properly.
 *
 */
void test_will_always_pass(void)
{
    UInt16 rnd1;
    srand((UInt16) time(NULL));
    rnd1 = rand();
    TEST_ASSERT_EQUAL_INT(rnd1, rnd1);
} // test_will_always_pass(

/**
 * @brief Function to initialize the memory, maped on a file.
 *
 * This function creates and prepares a file where the unit test
 * will run on. It's not an actual test, and sould be called before
 * the actual tests, to make sure the test file is ready.
 *
 */
void test_manual_initialize_memory(void)
{
    //NOTE: This isn't an actual test, just a procedure that
    // fills up the memory, preparing to perform other tests.
    // This is particularly important to provide means to perform
    // tests independly in a way that we can verify each function,
    // not relying on the success of other features.

    alloc_reg_t allocRegAllFF[MAX_REG_ALLOC];
    size_t resFseek, resFwrite;
    UInt16 memNextAddr;
    UInt8 memValuesFF[MEM_VALUES_LEN];

    pTestMemory = fopen(".\\mem.bin", "wb");
    memset((UInt8 *)allocRegAllFF, 0xFF, sizeof(allocRegAllFF));
    //All 0xFF on the allocation table, meaning that there's no
    //data stored on the memory.

    //Fill up the allocation table with 0xFF.
    resFwrite = fwrite(allocRegAllFF, 1,
                       sizeof(allocRegAllFF), pTestMemory);
    TEST_ASSERT_EQUAL(sizeof(allocRegAllFF), resFwrite);

    memNextAddr = MEM_VALUES_START; //This value won't be used on this test.
    // It is here, just to fill the memory correctly. It's value doesn't matter
    fwrite(&memNextAddr, 1, sizeof(UInt16), pTestMemory);
    memset((UInt8 *)memValuesFF, 0xFF, sizeof(memValuesFF));
    resFwrite = fwrite(memValuesFF, 1, sizeof(memValuesFF), pTestMemory);
    TEST_ASSERT_EQUAL(sizeof(memValuesFF), resFwrite);
    fclose(pTestMemory);
} // test_manual_initialize_memory(

/**
 * @brief Function to test the reading of an 8 bit value
 *
 * This function is meant to test the reading process, not
 * relying in any other features. This way it will write the
 * testing data directly on the file, making sure the data will be
 * there regardless of correctness of any other feature (i.e. SetAttribute)
 *
 */
void test_restore_uint8(void)
{
    //This is intended to test the reading, so the data will be put there
    //manually, avoiding dependency on other methods
    alloc_reg_t manualAllocReg;
    size_t resFseek, resFwrite;
    UInt8 valTestUInt8;
    UInt8 readValue, readLen;
    UInt16 dataCRC;

    pReadVal = &readValue;
    pReadLen = &readLen;
    valTestUInt8 = rand();
    pTestMemory = fopen(".\\mem.bin", "rb+");

    manualAllocReg.start = TEST_8BIT_MANUAL_START;
    manualAllocReg.length = 1;
    manualAllocReg.crc = calcCRC8((UInt8 *)&manualAllocReg, ALLOC_REG_NO_CRC);

    resFseek = fseek(pTestMemory, ID_ADDRESS(TEST_8BIT_ID), SEEK_SET);
    TEST_ASSERT_EQUAL(0, resFseek);
     //Store the allocation register at
     //the TEST_8BIT_ID address with valid data
    resFwrite = fwrite(&manualAllocReg, 1, sizeof(manualAllocReg), \
                                                     pTestMemory);
    TEST_ASSERT_EQUAL(sizeof(manualAllocReg), resFwrite);

    //Position the file for writing the data manually
    resFseek = fseek(pTestMemory, TEST_8BIT_MANUAL_START, SEEK_SET);
    TEST_ASSERT_EQUAL(0, resFseek);
    resFwrite = fwrite(&valTestUInt8, 1, sizeof(UInt8), pTestMemory);
    TEST_ASSERT_EQUAL(sizeof(UInt8), resFwrite);
    dataCRC = calcCRC16((UInt8 *)&valTestUInt8, sizeof(valTestUInt8));
    //The file is at correct position, just need to write the CRC
    fwrite ((UInt8 *)&dataCRC, 1, CRC_LEN, pTestMemory);
    fclose(pTestMemory);

    //Now perform the reading
    gpNvm_err = gpNvm_GetAttribute(TEST_8BIT_ID, (UInt8 *)pReadLen, \
                                                 (UInt8 *)pReadVal);
    TEST_ASSERT_FALSE(gpNvm_err);
    TEST_ASSERT_EQUAL_UINT(valTestUInt8, *(UInt8 *)pReadVal);
} // test_restore_uint8(

/**
 * @brief Function to test the writing of an 8 bit value
 *
 * This function is meant to test the writing process, not
 * relying in any other features. This way it will read the
 * testing data directly from the file, making sure the data will be
 * there regardless of correctness of any other feature (i.e. GetAttribute)
 *
 */
void test_backup_uint8(void)
{
    UInt8 testInt8 = TEST_VALUE_INT8;
    UInt8 *pTestInt8 = &testInt8;
    UInt8 bytesRead;
    UInt8 valueRead, *pValueRead;
    UInt16 valueAddr, *pValueAdd;

    pValueRead = &valueRead;
    pValueAdd = &valueAddr;
    pTestMemory = fopen(".\\mem.bin", "rb+");

    //Check what should be the address to save the data.
    fseek(pTestMemory, NEXT_FREE_ADDR, SEEK_SET);
    fread(pValueAdd, 1, SIZE_MEM_ADDRESS, pTestMemory);

    //Write the testing value
    gpNvm_err = gpNvm_SetAttribute(TEST_8BIT_ID, sizeof(UInt8), \
                                   (UInt8 *)pTestInt8);
    TEST_ASSERT_FALSE(gpNvm_err);

    //Then read it, manually
    fseek(pTestMemory, valueAddr, SEEK_SET);
    fread(pValueRead, 1, sizeof(UInt8), pTestMemory);
    TEST_ASSERT_EQUAL_UINT(TEST_VALUE_INT8, valueRead);
} // test_backup_uint8(

/**
 * @brief Function to test the writing and reading of a 32 bit value
 *
 * This function's goal is to test how the memory deals with 32 bit values.
 * Differently from the 8 bit functions it relies on the memory access low
 * level functions. Therefore, it will call the GetAttribute and SetAttribute
 * for reading and writing the values.
 *
 */
void test_backup_read_uint32(void)
{
    UInt32 testInt32 = TEST_VALUE_INT32;
    UInt32 *pTestInt32 = &testInt32;
    UInt32 readValue;
    UInt8 readLen;

    pReadVal = &readValue;
    pReadLen = &readLen;
    //Write the testing value
    gpNvm_err = gpNvm_SetAttribute(TEST_32BIT_ID, \
                                   sizeof(UInt32), \
                                   (UInt8 *)pTestInt32);
    TEST_ASSERT_FALSE(gpNvm_err);
    //Then read it.
    gpNvm_err = gpNvm_GetAttribute(TEST_32BIT_ID, \
                                   (UInt8 *)pReadLen, \
                                   (UInt8 *)pReadVal);
    TEST_ASSERT_FALSE(gpNvm_err);
    TEST_ASSERT_EQUAL(sizeof(UInt32), *(UInt8 *)pReadLen);
    TEST_ASSERT_EQUAL_UINT32(TEST_VALUE_INT32, *(UInt32 *)pReadVal);
} // test_backup_read_uint32(

/**
 * @brief Function to test the writing and reading of an array of UInt8 values
 *
 * This function's goal is to test how the memory deals with data in arrays.
 * Just like the 32 bit function it relies on the memory access low
 * level functions. Therefore, it will call the GetAttribute and SetAttribute
 * for reading and writing the values.
 * For test simplicity it will check just the 1st and 5th values.
 *
 */
void test_backup_read_array_uint8(void)
{
    UInt8 testArrayUint8[ARRAY_SIZE], i;
    UInt8 readValue[ARRAY_SIZE];
    UInt8 readLen, testValue;

    pReadVal = &readValue;
    pReadLen = &readLen;
    // Generate pseudo random number to initialize the array.
    testValue = (UInt8) (rand() % (sizeof(UInt8) - ARRAY_SIZE));
    //Fill the testing array to be saved
    for (i = 0; i < ARRAY_SIZE; ++i)
    {
        testArrayUint8[i] = testValue + i;
    }

    //Write the testing value
    gpNvm_err = gpNvm_SetAttribute(TEST_8BIT_ARRAY_ID, \
                                   sizeof(testArrayUint8), \
                                   (UInt8 *)testArrayUint8);
    TEST_ASSERT_FALSE(gpNvm_err);
    //Then read it.
    gpNvm_err = gpNvm_GetAttribute(TEST_8BIT_ARRAY_ID, \
                                   (UInt8 *)pReadLen, \
                                   (UInt8 *)pReadVal);
    TEST_ASSERT_FALSE(gpNvm_err);
    TEST_ASSERT_EQUAL(sizeof(testArrayUint8), *(UInt8 *)pReadLen);
    //Test the first element
    TEST_ASSERT_EQUAL_UINT(testArrayUint8[0], *(UInt8 *)pReadVal);
    //Then test the fifth element
    TEST_ASSERT_EQUAL_UINT(testArrayUint8[5], *(UInt8 *)(pReadVal+5));

} // test_backup_read_array_uint8(

/**
 * @brief Function to test the writing and reading of a simple struct
 *
 * This function's goal is to test how the memory deals with data in a struct.
 * The testing struct is very simple, in order to make it easy to debug whether
 * there is any error on dealing with structs.
 * Like the latest functions, it relies on the memory access low level
 * functions.
 * Therefore, it will call the GetAttribute and SetAttribute for reading and
 * writing the values.
 */
void test_backup_read_simple_struct(void)
{
    gpSimpleData_t testWriteStruct, testReadStruct, *pTestWriteStruct;
    UInt8 readLen;

    pTestWriteStruct = &testWriteStruct;
    pReadVal = (gpSimpleData_t *)&testReadStruct;
    pReadLen = &readLen;

    //Fill the structure with testing values
    testWriteStruct.id = TEST_VALUE_STRUCT_ID;
    testWriteStruct.value16 = TEST_VALUE_INT16;
    testWriteStruct.value32 = TEST_VALUE_INT32;
    // Generate pseudo random number to initialize the array.

    //Write the testing struct with values
    gpNvm_err = gpNvm_SetAttribute(TEST_SIMPLESTRUCT_ID, \
                                   sizeof(testWriteStruct), \
                                   (UInt8 *)pTestWriteStruct);
    TEST_ASSERT_FALSE(gpNvm_err);
    //Then read it.
    gpNvm_err = gpNvm_GetAttribute(TEST_SIMPLESTRUCT_ID, \
                                  (UInt8 *)pReadLen, \
                                  (UInt8 *)pReadVal);
    TEST_ASSERT_FALSE(gpNvm_err);
    TEST_ASSERT_EQUAL(sizeof(testWriteStruct), *(UInt8 *)pReadLen);

    //Test the struct values
    TEST_ASSERT_EQUAL_UINT((*pTestWriteStruct).id, \
                           (*(gpSimpleData_t *)pReadVal).id);
    TEST_ASSERT_EQUAL_UINT16((*pTestWriteStruct).value16, \
                           (*(gpSimpleData_t *)pReadVal).value16);
    TEST_ASSERT_EQUAL_UINT32((*pTestWriteStruct).value32, \
                             (*(gpSimpleData_t *)pReadVal).value32);

} // test_backup_read_simple_struct

/**
 * @brief Function to test the writing and reading of a complex struct
 *
 * This function's goal is to test how the memory deals with data in a more
 * complex structure having more members, one being an array.
 * Like the latest functions, it relies on the memory access low level
 * functions.
 * Therefore, it will call the GetAttribute and SetAttribute for reading and
 * writing the values.
 */
void test_backup_read_complex_struct(void)
{
    gpTestData_t testWriteStruct, testReadStruct, *pTestWriteStruct;
    UInt8 readLen;
    UInt8 testValue, i;

    pTestWriteStruct = &testWriteStruct;
    pReadVal = (gpTestData_t *)&testReadStruct;
    pReadLen = &readLen;

    //Fill the structure with testing values
    testWriteStruct.id = TEST_VALUE_STRUCT_ID;
    testWriteStruct.options = TEST_VALUE_STRUCT_OPT;
    testWriteStruct.length = TEST_VALUE_STRUCT_LEN;
    // Generate pseudo random number to initialize the array.
    testValue = (UInt8) (rand() % (sizeof(UInt8) - MAX_STRUCT_DATA_LENGTH));
    //Fill the testing array to be saved
    for (i = 0; i < testWriteStruct.length; ++i)
    {
        //Fill up using a simple sequence of <length> size randomly init.
        testWriteStruct.data[i] = testValue + i;
    }
    for ( ; i < MAX_STRUCT_DATA_LENGTH; ++i)
    {
        //Pad with 0xFF til the end of the array.
        testWriteStruct.data[i] = 0xFF;
    }

    //Write the testing struct with values
    gpNvm_err = gpNvm_SetAttribute(TEST_COMPLEXSTRUCT_ID, \
                                   sizeof(testWriteStruct), \
                                   (UInt8 *)pTestWriteStruct);
    TEST_ASSERT_FALSE(gpNvm_err);
    //Then read it.
    gpNvm_err = gpNvm_GetAttribute(TEST_COMPLEXSTRUCT_ID, \
                                  (UInt8 *)pReadLen, \
                                  (UInt8 *)pReadVal);
    TEST_ASSERT_FALSE(gpNvm_err);
    TEST_ASSERT_EQUAL(sizeof(testWriteStruct), *(UInt8 *)pReadLen);

    //Test the struct values
    TEST_ASSERT_EQUAL_UINT((*pTestWriteStruct).id, \
                           (*(gpTestData_t *)pReadVal).id);
    TEST_ASSERT_EQUAL_UINT((*pTestWriteStruct).length,
                           (*(gpTestData_t *)pReadVal).length);
    TEST_ASSERT_EQUAL_UINT32((*pTestWriteStruct).options, \
                             (*(gpTestData_t *)pReadVal).options);
    //Test array positions 1 and 7
    TEST_ASSERT_EQUAL_UINT((*pTestWriteStruct).data[1], \
                           (*(gpTestData_t *)pReadVal).data[1]);
    TEST_ASSERT_EQUAL_UINT((*pTestWriteStruct).data[7], \
                           (*(gpTestData_t *)pReadVal).data[7]);
} // test_backup_read_complex_struct(
