#ifndef __RSTRING_H__
#define __RSTRING_H__

#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//指向字符串文本内容的指针
typedef char *pstr;
typedef uint8_t xmbyte1;
typedef uint16_t xmbyte2;
typedef uint32_t xmbyte4;
typedef uint64_t xmbyte8;

#define RSTRING_MAX_PREALLOC (1024 * 1024)
//标记类型
#define RSTRING_TYPE_5 0
#define RSTRING_TYPE_8 1
#define RSTRING_TYPE_16 2
#define RSTRING_TYPE_32 3
#define RSTRING_TYPE_64 4

size_t gAlloc[] = {0, 255, 65535, 4294967295, 18446744073709551615};
#define xmRepeat(n, m, p) xmAppend(xmRepeat_, n)(m, p)
#define xmAppend(x, y) x##y
#define xmRepeat_0(m, p)
#define xmRepeat_5(m, p)
#define xmRepeat_8(m, p) xmRepeat_5(m, p) m(8, p)
#define xmRepeat_16(m, p) xmRepeat_8(m, p) m(16, p)
#define xmRepeat_32(m, p) xmRepeat_16(m, p) m(32, p)
#define xmRepeat_64(m, p) xmRepeat_32(m, p) m(64, p)

//宏：rstring结构体
#define rstringStruct(a, b)  \
    typedef struct           \
    {                        \
        xmbyte##b len;       \
        xmbyte##b alloc;     \
        unsigned char flags; \
        char buf[];          \
    } rstring##a

#define xmType(n, string_size)   \
    if (string_size < 1 << n)    \
    {                            \
        return RSTRING_TYPE_##n; \
    }

#define xmRstringTypeEqual(n, p, f) \
    if (##p == RSTRING_TYPE_##n)    \
    {                               \
        ##f(n)                      \
    }
#define xmRstringTypeEqualElse(n, p, f) \
    else xmRstringTypeEqual(n, p, f)

#define xmRstringStructStart(n) \
    rstring##n *start = (rstring##n *)(s - sizeof(rstring##n))

#define xmHeaderFunc(n) return sizeof(rstring##n);

#define xmAvailFunc5(n)                        \
    xmRstringStructStart(n);                   \
    size_t avail = 31 - ((start->flags) >> 3); \
    return avail;

#define xmAvailFunc(n)                        \
    xmRstringStructStart(n);                  \
    size_t avail = start->alloc - start->len; \
    return avail;

#define xmStringLenFunc(n)      \
    xmRstringStructStart(n);    \
    size_t length = start->len; \
    return length;

#define xmStringLenFunc5(n)              \
    xmRstringStructStart(n);             \
    size_t length = (start->flags) >> 3; \
    return length;

#define xmStringSetFunc5(n)  \
    xmRstringStructStart(n); \
    start->flags = RSTRING_TYPE_5 | (newlen << 3);

#define xmStringSetFunc(n)   \
    xmRstringStructStart(n); \
    int order = ##n / 8;     \
    start->len = newlen;     \
    start->alloc = gAlloc[order];

#define xmStringNewLenFunc(n)     \
    xmRstringStructStart(n);      \
    int order = ##n / 8;          \
    start->len = initlen;         \
    start->alloc = gAlloc[order]; \
    start->flags = type;

#define xmHeader(n, p) xmRstringTypeEqualElse(n, p, xmHeaderFunc)
#define xmHeader5(n, p) xmRstringTypeEqual(n, p, xmHeaderFunc)
#define xmStringLen(n, p) xmRstringTypeEqualElse(n, p, xmStringLenFunc)
#define xmStringLen5(n, p) xmRstringTypeEqual(n, p, xmStringLenFunc5)
#define xmAvail(n, p) xmRstringTypeEqualElse(n, p, xmAvailFunc)
#define xmAvail5(n, p) xmRstringTypeEqual(n, p, xmAvailFunc5)
#define xmStringSet(n, p) xmRstringTypeEqualElse(n, p, xmStringSetFunc)
#define xmStringSet5(n, p) xmRstringTypeEqual(n, p, xmStringSetFunc5)

#define xmStringPstrFunc(f)              \
    char currType = structType(strlen(s)); \
    xmAppend(f,5)(5, currType)              \
    xmRepeat(64, ##f, currType)

#define xmStringNewLen(n, p) xmRstringTypeEqual(n, p, xmStringNewLenFunc)
//定义结构体
//5位字符串结构体（字符串大小为2^5-1以内）
typedef struct
{
    unsigned char flags; //前三位类型，后五位长度
    char buf[];          //存储字符串文本内容的数组
} rstring5;

//8位字符串结构体（字符串大小为2^8-1以内）
rstringStruct(8, 1);
//16位字符串结构体（字符串大小为2^16-1以内）
rstringStruct(16, 2);
//32位字符串结构体（字符串大小为2^32-1以内）
rstringStruct(32, 4);
//64位字符串结构体（字符串大小为2^64-1以内）
rstringStruct(64, 8);

size_t rstringAvail(pstr s);
size_t rstringLen(pstr s);
void rstringSet(pstr s, size_t newlen);
pstr rstringNew(const char *init);

#endif