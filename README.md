# Non-valatile memory storage component

### This exercise is the implementation of a memory component capable of backing up and restoring values corresponding to a unique attribute identifier.
### For simplicity, the underlying memory was modeled as a binary file (mem.bin), which models a memory of 64 kbytes total, 16-bit addressed, 1 byte per address.

----
### The API file is *nvm.c* which has the access functions: *gpNvm_GetAttribute* and *gpNvm_SetAttribute*, defined as below:


    gpNvm_Result gpNvm_GetAttribute(gpNvm_AttrId attrId,
                                    UInt8* pLength,
                                    UInt8* pValue);
    gpNvm_Result gpNvm_SetAttribute(gpNvm_AttrId attrId,
                                    UInt8 length,
                                    UInt8* pValue);

----
### The *utils.c* file has some utility functions, meant to be used on any module.
### The *memory.c* file has the low level memory access functions, specially: *memRead* and *memWrite*. If one needs to change it to a real memory device, it only need to provide those functions, as below:

    UInt8 memWrite (UInt16 start, UInt8 length, UInt8 *buffWrite)
    UInt8 memRead (UInt16 start, UInt8 length, UInt8 *buffRead)

### The *nvm_tests.c* file has all the unit tests, based on the [UNITY](http://www.throwtheswitch.org/unity/) harness.
### The *maic.c* file is unit tests control, which calls all the tests, properly.


