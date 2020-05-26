#include "xmtypedef.h"
#include "xmstring.h"

xmstring *newXmStr()
{
    xmstring *root = (xmstring *)malloc(sizeof(xmstring));
    xmprintfd("newXmStr size", sizeof(xmstring));
    if (root == NULL)
    {
        xmprintfe("newXmStr");
    }
    else
    {
        root->next = NULL;
    }

    return root;
}

xmstring *addXmStr(xmstring *parent)
{
    xmstring *item = newXmStr();
    return item;
}

xmstring *delXmStr(xmstring *parent)
{
    xmstring *item = newXmStr();
    if (item){
        //item
    }
    return item;
}