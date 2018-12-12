#include "nvm.h"
#include "memory.h"
#include "nvm_tests.h"
#include "..\Unity\src\unity.h"

FILE *pTestMemory; //< The testing file
gPNvm_Result gpNvm_err = 0;
void *pRead_len;
void *pRead_val;

void setUp(void)
{
    //Open the file for binary access, create if doesn't exists
    pTestMemory = fopen(".\\mem.bin", "ab+");
}

void tearDown(void)
{
    fclose(pTestMemory);
}


void test_restore_uint8(void)
{
    //This is intended to test the reading, so the data will be put there
    // manually, avoiding dependency on other methods
    alloc_reg_t manualMemTest8bit[MAX_REG_ALLOC];
    UInt8 resFseek;

    resFseek = fseek(pTestMemory, 0L, SEEK_SET);
    TEST_ASSERT_EQUAL(0, resFseek);

    memset((UInt8 *)manualMemTest8bit, 0xFF, sizeof(manualMemTest8bit));
    manualMemTest8bit[TEST_8BIT_ID].start = TEST_8BIT_MANUAL_START;
    manualMemTest8bit[TEST_8BIT_ID].length = 1;
    manualMemTest8bit[TEST_8BIT_ID].crc = 0; //TODO: Calc CRC

     //Fill up the allocation table with 0xFF, but the TEST_8BIT_ID with valid data
    fwrite(manualMemTest8bit, sizeof(manualMemTest8bit), 1, pTestMemory);

    //Now perform the reading
    gpNvm_err = gpNvm_GetAttribute(TEST_8BIT_ID, (UInt8 *)pRead_len, (UInt8 *)pRead_val);
    TEST_ASSERT_EQUAL(0, gpNvm_err);
    TEST_ASSERT_EQUAL_INT8(TEST_VALUE_INT8, *(UInt8 *)pRead_val);

}

void test_backup_uint8(void)
{
    UInt8 testInt8 = TEST_VALUE_INT8;
    UInt8 *pTestInt8 = &testInt8;

    gpNvm_err = gpNvm_SetAttribute(TEST_8BIT_ID, sizeof(UInt8), (UInt8 *)pTestInt8);
    TEST_ASSERT_FALSE(gpNvm_err);
    gpNvm_err = gpNvm_GetAttribute(TEST_8BIT_ID, (UInt8 *)pRead_len, (UInt8 *)pRead_val);
    TEST_ASSERT_FALSE(gpNvm_err);
    TEST_ASSERT_EQUAL(sizeof(UInt8), *(UInt8 *)pRead_len);
    TEST_ASSERT_EQUAL_INT8(TEST_VALUE_INT8, *(UInt8 *)pRead_val);
}
