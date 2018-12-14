/**
 * @file nvm.h
 *
 */

#if !defined(__NVM_H__)
#define __NVM_H__

//types used
typedef unsigned char UInt8;
typedef signed char Int8;
typedef unsigned short int UInt16;
typedef signed int Int32;
typedef unsigned int UInt32;
typedef UInt8 gPNvm_AttrId;
typedef UInt8 gPNvm_Result;

#define ALLOC_REG_LEN   4   //< Length of each allocation register, in bytes
#define MAX_REG_ALLOC   256 //< Max ammount of allocation registers
#define ALLOC_TABLE_LEN  1024 //< Length of the allocation table (256 * 4)
#define SIZE_OF_MEM_ADDRESSING 2 // Length of memory addressing, in bytes
#define NEXT_FREE_ADDR ALLOC_TABLE_LEN //Pointer to the next available address
#define MEM_VALUES_START (ALLOC_TABLE_LEN \
                        + SIZE_OF_MEM_ADDRESSING) //< Beggining of
                                                  //value storing area
#define MEM_VALUES_LEN   ((1<<16) - MEM_VALUES_START) //< Length of memory
                         // available to store values (64510 bytes)

/**
 * Local functions prototypes
 */

gPNvm_Result gpNvm_GetAttribute (gPNvm_AttrId attrId,
                                 UInt8*       pLength,
                                 UInt8*       pValue);

gPNvm_Result gpNvm_SetAttribute (gPNvm_AttrId attrId,
                                 UInt8        length,
                                 UInt8*       pValue);

/**
 * @brief Allocation table register structure
 *
 * The allocation table register structure is considering
 * a 16 bit a addressing memory, 1 byte per address (max  64 kbyte).
 * The allocation table will be located at the beggining of the memory
 * and initialized with 0xFF, meaning all memory registers are invalid.
 * This way, the actual data starts at address 1024 (ALLLOC_TABLE_LEN)
 * and there is a maximum of 63 kbyte of usable memory.
 */
typedef struct alloc_reg{
    //NOTE: In this approach, the AttrId don't need to be stored,
    // since the allocation table will always be filled up for all
    // possible AttribId and it's address at the table can be easily
    // calculated by multiplying the Id itself by the register size (4).
    // This way we can save space and increase performance.
    // Ex.: AttrId = 84 will be stored at address 84*4 = 336 (0x0150)
    //
    // PS. If the solution has to consider a 32-bit or 64-bit length AttrId
    // the use of a hash table could be needed, avoiding to have a big
    // (and possibly empty) space reserved for the full allocation table.
    // This is a trade-off that must be analised.

    UInt16 start; //< Starting address (16 bits)
    UInt8 length; //< Lenght of the data stored under this attribute (1 byte)
    UInt8 crc;    //< 1 byte CRC for allocation table integrity
} alloc_reg_t;


/*******************************
 * Alloc table
 *
 *  | AttrID | Start Addr | Length | CRC(?)
 *
 * Fixed size record * 256 length
 * The file will always have this filled,
 * being initialized with invalid start address
 * for all AttrID.
 *
 * In the FILE mapping, access will be achieved with FSEEK.
 * Real life would be just reading the addr, directly.
 *
 * *****************************/

#endif
