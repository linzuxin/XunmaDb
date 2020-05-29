
#ifndef _XM_FILE_MANAGER_H__    
#define _XM_FILE_MANAGER_H__   

#include "xmtypedef.h"

typedef struct
{
    xmbyte *context;
} fileblock;

fileblock *newFileBlock();

#endif