
#ifndef _XM_FILE_MANAGER_H__    
#define _XM_FILE_MANAGER_H__   

#include "xmtypedef.h"
#include "xmfileblock.h"

typedef struct
{
    int blocksize;
    int blockcount;

} fileblock;

fileblock *newFileBlock();

#endif