/**
 * @file memory.h
 * @brief Header file for the memory module.
 *
 * This file is the header of the memory module. Here
 * we have the prototypes of the low level functions.
 *
 * @author Marcio J Teixeira Jr.
 * @date 09/12/18
 *
 */

#if !defined(__MEMORY_H__)
#define __MEMORY_H__

#include "nvm.h"

UInt8 memRead (UInt16 start, UInt8 length, UInt8 *buffRead);
UInt8 memWrite (UInt16 start, UInt8 length, UInt8 *buffWrite);
UInt8 memInit (void);

#endif
