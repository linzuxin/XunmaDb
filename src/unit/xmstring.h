#ifndef _XM_STRING_H__    
#define _XM_STRING_H__   

typedef struct
{
    xmbyte xmstr;
    struct xmstring *next;
    struct xmstring *nnext;
    struct xmstring *prev;
    struct xmstring *pprev;
} xmstring;

xmstring *newXmStr();
xmstring *addXmStr();
xmstring *delXmStr();

#endif