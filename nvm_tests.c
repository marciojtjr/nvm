#include "nvm.h"
#include "memory.h"
#include "nvm_tests.h"
#include "..\Unity\src\unity.h"

#include <stdio.h>
#include <string.h>

//Macro to calculate the address of an AttrId
#define ID_ADDRESS(x) (x<<2)

FILE *pTestMemory; //< The testing file
gPNvm_Result gpNvm_err = 0;
void *pRead_len;
void *pRead_val;

void setUp(void)
{
}

void tearDown(void)
{
    if (pTestMemory)
        fclose(pTestMemory);
}

void test_will_always_pass(void)
{
    TEST_ASSERT_EQUAL_INT(33, 33);
}

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
    resFwrite = fwrite(memValuesFF, 1,
                       sizeof(memValuesFF), pTestMemory);
    TEST_ASSERT_EQUAL(sizeof(memValuesFF), resFwrite);
    fclose(pTestMemory);
}

void test_restore_uint8(void)
{
    //This is intended to test the reading, so the data will be put there
    //manually, avoiding dependency on other methods
    alloc_reg_t manualAllocReg;
    size_t resFseek, resFwrite;
    UInt8 valTestUInt8 = TEST_VALUE_INT8;
    UInt8 readValue, readLen;

    pRead_val = &readValue;
    pRead_len = &readLen;
    pTestMemory = fopen(".\\mem.bin", "rb+");

    manualAllocReg.start = TEST_8BIT_MANUAL_START;
    manualAllocReg.length = 1;
    manualAllocReg.crc = 0; //TODO: Calc CRC

    resFseek = fseek(pTestMemory, ID_ADDRESS(TEST_8BIT_ID), SEEK_SET);
    TEST_ASSERT_EQUAL(0, resFseek);
     //Store the allocation register at
     //the TEST_8BIT_ID address with valid data
    resFwrite = fwrite(&manualAllocReg, 1, \
                       sizeof(manualAllocReg), pTestMemory);
    TEST_ASSERT_EQUAL(sizeof(manualAllocReg), resFwrite);

    resFseek = fseek(pTestMemory, TEST_8BIT_MANUAL_START, SEEK_SET);
    TEST_ASSERT_EQUAL(0, resFseek);
    resFwrite = fwrite(&valTestUInt8, 1, sizeof(UInt8), pTestMemory);
    TEST_ASSERT_EQUAL(sizeof(UInt8), resFwrite);
    fclose(pTestMemory);

    //Now perform the reading
    gpNvm_err = gpNvm_GetAttribute(TEST_8BIT_ID, (UInt8 *)pRead_len, (UInt8 *)pRead_val);
    TEST_ASSERT_FALSE(gpNvm_err);
    TEST_ASSERT_EQUAL_INT8(TEST_VALUE_INT8, *(UInt8 *)pRead_val);
}

void test_backup_uint8(void)
{
    UInt8 testInt8 = TEST_VALUE_INT8;
    UInt8 *pTestInt8 = &testInt8;
    UInt8 readValue, readLen;

    pRead_val = &readValue;
    pRead_len = &readLen;
    gpNvm_err = gpNvm_SetAttribute(TEST_8BIT_ID, sizeof(UInt8), (UInt8 *)pTestInt8);
    TEST_ASSERT_FALSE(gpNvm_err);
    gpNvm_err = gpNvm_GetAttribute(TEST_8BIT_ID, (UInt8 *)pRead_len, (UInt8 *)pRead_val);
    TEST_ASSERT_FALSE(gpNvm_err);
    TEST_ASSERT_EQUAL(sizeof(UInt8), *(UInt8 *)pRead_len);
    TEST_ASSERT_EQUAL_INT8(TEST_VALUE_INT8, *(UInt8 *)pRead_val);
}
