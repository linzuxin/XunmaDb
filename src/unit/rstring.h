#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __RSTRING_H__    
#define __RSTRING_H__   
#endif

#define SDS_MAX_PREALLOC (1024*1024)
//标记类型
#define SDS_TYPE_5 0
#define SDS_TYPE_8 1
#define SDS_TYPE_16 2
#define SDS_TYPE_32 3
#define SDS_TYPE_64 4

//指向字符串文本内容的指针
typedef char *sds;
typedef uint8_t xmbyte1;
typedef uint16_t xmbyte2;
typedef uint32_t xmbyte4;
typedef uint64_t xmbyte8;

//5位字符串结构体（字符串大小为2^5-1以内）
struct sdshdr5
{
    unsigned char flags;//前三位类型，后五位长度
    char buf[];//存储字符串文本内容的数组
};
//8位字符串结构体（字符串大小为2^8-1以内）
struct  sdshdr8
{
    xmbyte1 len;//字符串长度
    xmbyte1 alloc;//分配空间
    unsigned char flags;//前三位类型，后五位放空
    char buf[];
};
//16位字符串结构体（字符串大小为2^16-1以内）
struct  sdshdr16
{
    xmbyte2 len;
    xmbyte2 alloc;
    unsigned char flags;
    char buf[];
};
//32位字符串结构体（字符串大小为2^32-1以内）
struct  sdshdr32
{
    xmbyte4 len;
    xmbyte4 alloc;
    unsigned char flags;
    char buf[];
};
//64位字符串结构体（字符串大小为2^64-1以内）
struct  sdshdr64
{
    xmbyte8 len;
    xmbyte8 alloc;
    unsigned char flags;
    char buf[];
};

//判断字符串类型
//size_t就是unsigned long long
char sdsReqTyp(size_t string_size)
{
    if (string_size < 1<<5)
    {
        return SDS_TYPE_5;
    }
    if (string_size < 1<<8)
    {
        return SDS_TYPE_8;
    }
    if (string_size < 1<<16)
    {
        return SDS_TYPE_16;
    }
    //32位系统下LONG_MAX占4字节，LLONG_MAX占8字节；
    //64位系统下LONG_MAX占8字节，LLONG_MAX占8字节；
    //故若为64为系统要区分类型是SDS_TYPE_32还是SDS_TYPE_64，32位系统不区分
    #if (LONG_MAX == LLONG_MAX)
    {
        if (string_size < 1<<32)
        {
            return SDS_TYPE_32;
        }
        return SDS_TYPE_64;
    }
    #else
        return SDS_TYPE_32;
    #endif    
}

//根据类型判断动态字符串结构体的大小（不包含字符串内容）
int sdsHdrSize(char type)
{
    switch (type & 7)
    {
    case SDS_TYPE_5:
        return sizeof(struct sdshdr5);
    case SDS_TYPE_8:
        return sizeof(struct sdshdr8);
        break;
    case SDS_TYPE_16:
        return sizeof(struct sdshdr16);
        break;
    case SDS_TYPE_32:
        return sizeof(struct sdshdr32);
        break;
    case SDS_TYPE_64:
        return sizeof(struct sdshdr64);
    default:
        printf("字符串类型错误");
        return 0;
    }
}

//字符串剩余空闲长度
size_t sdsavail(const sds s)
{
    char currType = sdsReqTyp(strlen(s));
    switch (currType)
    {
        case SDS_TYPE_5:
        {
            return 0;
        }
        case SDS_TYPE_8:
        {
            struct sdshdr8 *start = (struct sdshdr8*)(s - sizeof(struct sdshdr8));
            size_t avail = start->alloc - start->len;
            return avail;
        }
        case SDS_TYPE_16:
        {
            struct sdshdr16 *start = (struct sdshdr16*)(s - sizeof(struct sdshdr16));
            size_t avail = start->alloc - start->len;
            return avail;
        }            
        case SDS_TYPE_32:
        {
            struct sdshdr32 *start = (struct sdshdr32*)(s - sizeof(struct sdshdr32));
            size_t avail = start->alloc - start->len;
            return avail;
        }        
        case SDS_TYPE_64:
        {
            struct sdshdr64 *start = (struct sdshdr64*)(s - sizeof(struct sdshdr64));
            size_t avail = start->alloc - start->len;
            return avail;
        }
        default:
        {
            printf("无匹配结果");
            return 0;
        }
            
    }
}

//获取字符串长度
size_t sdslen(const sds s)
{
    size_t length;
    char currType = sdsReqTyp(strlen(s));
    switch (currType)
    {
        case SDS_TYPE_5:
        {
            struct sdshdr5 *start = (struct sdshdr5*)(s - sizeof(struct sdshdr5));
            length = (start->flags) >> 3;
            return length;
        }
        case SDS_TYPE_8:
        {
            struct sdshdr8 *start = (struct sdshdr8*)(s - sizeof(struct sdshdr8));
            length = start->len;
            return length;
        }
        case SDS_TYPE_16:
        {
            struct sdshdr16 *start = (struct sdshdr16*)(s - sizeof(struct sdshdr16));
            length = start->len;
            return length;
        }            
        case SDS_TYPE_32:
        {
            struct sdshdr32 *start = (struct sdshdr32*)(s - sizeof(struct sdshdr32));
            length = start->len;
            return length;
        }        
        case SDS_TYPE_64:
        {
            struct sdshdr64 *start = (struct sdshdr64*)(s - sizeof(struct sdshdr64));
            length = start->len;
            return length;
        }
        default:
            return 0;
    }
}

//设置字符串
void sdsset(sds s, size_t newlen)
{
    char currType = sdsReqTyp(strlen(s));
    switch (currType)
    {
        case SDS_TYPE_5:
        {
            struct sdshdr5 *start = (struct sdshdr5*)(s - sizeof(struct sdshdr5));
            start->flags = SDS_TYPE_5 | (newlen << 3);
            printf("len set result:%d\n",(start->flags >> 3));
            break;
        }
        case SDS_TYPE_8:
        {
            struct sdshdr8 *start = (struct sdshdr8*)(s - sizeof(struct sdshdr8));
            start->len = newlen;
            start->alloc = 255;
            printf("len set result:%d\n",start->len);
            break;
        }
        case SDS_TYPE_16:
        {
            struct sdshdr16 *start = (struct sdshdr16*)(s - sizeof(struct sdshdr16));
            start->len = newlen;
            start->alloc = 65535;
            printf("len set result:%d\n",start->len);
            break;
        }            
        case SDS_TYPE_32:
        {
            struct sdshdr32 *start = (struct sdshdr32*)(s - sizeof(struct sdshdr32));
            start->len = newlen;
            start->alloc = 4294967295;
            printf("len set result:%d\n",start->len);
            break;
        }        
        case SDS_TYPE_64:
        {
            struct sdshdr64 *start = (struct sdshdr64*)(s - sizeof(struct sdshdr64));
            start->len = newlen;
            start->alloc = 18446744073709551615;
            printf("len set result:%d\n",start->len);
            break;
        }
    }
}