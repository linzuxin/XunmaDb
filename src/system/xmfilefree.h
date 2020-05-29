
#ifndef _XM_FILE_MANAGER_H__    
#define _XM_FILE_MANAGER_H__   

#include "xmtypedef.h"

typedef struct
{
    xmbyte *context;
    uint32_t start;
    uint32_t length;
} fileblockindex;

typedef struct
{
    xmbyte *context;
    uint32_t start;   //start block
    uint32_t length;  //start 
} fileindex;

fileblock *newFileBlock();

#endif