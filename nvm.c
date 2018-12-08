#include <stdio.h>

typedef unsigned char UInt8;
typedef UInt8 gPNvm_AttrId;
typedef UInt8 gPNvm_Result;

gPNvm_Result gpNvm_GetAttribute (gPNvm_AttrId attrId,
                                 UInt8*       pLength,
                                 UInt8*       pValue);

gPNvm_Result gpNvm_SetAttribute (gPNvm_AttrId attrId,
                                 UInt8        length,
                                 UInt8*       pValue);



void main(void)
{

    return 0;
}
