#ifndef _XM_MEMORY_H__
#define _XM_MEMORY_H__

#include "../unit/xmtypedef.h"

typedef struct
{
    xmbyte *context;
    struct block *next;
} block;

typedef struct
{
    int defbyte[8];
    int sizebyte[8];

} blockprofile;

blockprofile *initDefBlockProfile();


#endif