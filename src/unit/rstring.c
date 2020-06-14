#include "rstring.h"

/***判断字符串类型***/
//size_t就是unsigned long long
char structType(size_t string_size)
{
    xmRepeat(16, xmType, string_size);
    //32位系统下LONG_MAX占4字节，LLONG_MAX占8字节；
    //64位系统下LONG_MAX占8字节，LLONG_MAX占8字节；
    //故若为64位系统要区分类型是RSTRING_TYPE_32还是RSTRING_TYPE_64，32位系统不区分
#if (LONG_MAX == LLONG_MAX)
    {
        xmType(32, string_size);
        return RSTRING_TYPE_64;
    }
#else
    return RSTRING_TYPE_32;
#endif
}

/***根据类型判断动态字符串结构体的大小（不包含字符串内容）***/
int HeaderSize(char type)
{
    char currType = type & 7;
    xmHeader5(5, currType)
    xmRepeat(64, xmHeader, currType)
    return 0;
}

/***字符串剩余空闲长度***/
size_t rstringAvail(pstr s)
{
    char currType = structType(strlen(s));
    xmAvail5(5, currType)
    xmRepeat(64, xmAvail, currType)
    return 0;
}

/***获取字符串长度***/
size_t rstringLen(pstr s)
{
    char currType = structType(strlen(s));
    xmStringLen5(5, currType)
    xmRepeat(64, xmStringLen, currType)
    return 0;
}

/***设置字符串***/
void rstringSet(pstr s, size_t newlen)
{
    xmStringPstrFunc(xmStringSet);
}

/***创建字符串***/
pstr rstringNewLen(const void *init, size_t initlen)
{
    void *sh;                        //指向动态字符串结构体的的指针
    pstr s;                          //指向动态字符串内容的指针
    char type = structType(initlen); //获取动态字符串结构体的类型
    //若RSTRING_TYPE_5类型字符串为空，转换成RSTRING_TYPE_8类型
    if (type == RSTRING_TYPE_5 && initlen == 0)
    {
        type = RSTRING_TYPE_8;
    }
    int hdrlen = HeaderSize(type);     //获取动态字符串结构体的头部大小
    sh = malloc(hdrlen + initlen + 1); //给指向动态字符串结构体的头部的指针分配空间
    if (sh == NULL)
    {
        return NULL;
    }
    if (!init)
    {
        //指针初始化
        memset(sh, 0, hdrlen + initlen + 1);
    }
    //计算指向动态字符串内容的指针的位置：
    //位置=动态字符串头部的指针地址+头部长度
    s = (char *)sh + hdrlen;
    xmRepeat(64, xmStringNewLen, type);
    if (initlen && init)
    {
        //复制init中的内容到s中去。
        memcpy(s, init, initlen);
    }
    s[initlen] = '\0';
    return s;
}

pstr rstringNew(const char *init)
{
    size_t initlen = (init == NULL) ? 0 : strlen(init); //获取动态字符串长度
    return sdsnewlen(init, initlen);
}

/***为字符串重新分配容量***/
pstr sdsMakeRoomFor(pstr s, size_t addlen)
{
    // size_t avail = sdsavail(s);
    //剩余未分配长度满足当前新增需求的直接返回
    // if(avail > addlen)
    // {
    //     return s;
    // }
    //剩余未分配长度不满足当前新增需求，重新分配
    size_t len = rstringLen(s);
    char type = structType(len);
    size_t newlen = (len + addlen);
    char newtype = structType(newlen);
    void *sh = NULL, *newsh = NULL;
    sh = (char *)s - HeaderSize(type);
    size_t newhdrlen = HeaderSize(newtype);
    if (newtype == RSTRING_TYPE_5)
    {
        newtype = RSTRING_TYPE_8;
    }
    //类型相同则重新分配结构体空间即可
    if (type == newtype)
    {
        //结构体部分的位置不改变，但是柔型数组的长度改变
        newsh = realloc(sh, newhdrlen + newlen + 1);
        printf("location of newsh: %p\n", newsh);
        printf("location of sh: %p\n", sh);
        if (newsh == NULL)
        {
            return NULL;
        }
        s = (char *)newsh + newhdrlen;
        printf("content:%s\n", s);
    }
    else
    {
        //结构体部分的位置和柔型数组的长度都改变
        newsh = malloc(newhdrlen + newlen + 1);
        printf("location of newsh: %p\n", newsh);
        printf("location of sh: %p\n", sh);
        if (newsh == NULL)
        {
            return NULL;
        }
        memcpy((char *)newsh + newhdrlen, s, len + 1);
        free(sh);
        printf("location of sh: %p\n", sh);
        s = (char *)newsh + newhdrlen;
        printf("content:%s\n", s);
    }
    return s;
}

/***拼接字符串***/
pstr sdscatlen(pstr s, const void *t, size_t addlen)
{
    size_t currlen = rstringLen(s);
    s = sdsMakeRoomFor(s, addlen);
    if (s == NULL)
    {
        return NULL;
    }
    memcpy(s + currlen, t, addlen);
    rstringSet(s, currlen + addlen);
    s[currlen + addlen] = '\0';
    return s;
}

pstr sdscat(pstr s, const char *t)
{
    return sdscatlen(s, t, strlen(t));
}

/***释放字符串***/
void sdsfree(pstr s)
{
    if (s == NULL)
    {
        return;
    }
    char type = structType(strlen(s));
    int hdrSize = HeaderSize(type);
    void *sh = s - hdrSize;
    free(sh);
    sh = NULL;
}