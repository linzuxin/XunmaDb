#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rstring.h"

/*创建字符串*/
sds sdsnewlen(const void *init, size_t initlen)
{   
    void *sh;//指向动态字符串结构体的的指针
    sds s;//指向动态字符串内容的指针
    char type = sdsReqTyp(initlen);//获取动态字符串结构体的类型
    //若SDS_TYPE_5类型字符串为空，转换成SDS_TYPE_8类型
    if(type == SDS_TYPE_5 && initlen == 0)
    {
        type = SDS_TYPE_8;
    }
    int hdrlen = sdsHdrSize(type);//获取动态字符串结构体的头部大小
    sh = malloc(hdrlen + initlen + 1);//给指向动态字符串结构体的头部的指针分配空间
    if(sh == NULL)
    {
        return NULL;
    }
    if(!init)
    {
        //指针初始化
        memset(sh, 0, hdrlen + initlen + 1);
    }
    //计算指向动态字符串内容的指针的位置：
    //位置=动态字符串头部的指针地址+头部长度
    s = (char*)sh + hdrlen;
    //计算指向动态字符串类型的指针的位置：
    //位置=动态字符串头部的指针地址-1
    switch (type)
    {
        case SDS_TYPE_5:
        {
            struct sdshdr5 *start = (void*)((s) - (sizeof(struct sdshdr5)));
            //存储flag结构：高五位存字符串长度，低三位存类型
            start->flags = type | (initlen << 3);
            break;
        }
        case SDS_TYPE_8:
        {
            struct sdshdr8 *start = (void*)((s) - (sizeof(struct sdshdr8)));
            start->len = initlen;
            start->alloc = 255;
            start->flags = type;
            break;
        }
        case SDS_TYPE_16:
        {
            struct sdshdr16 *start = (void*)((s) - (sizeof(struct sdshdr16)));
            start->len = initlen;
            start->alloc = 65535;
            start->flags = type;
            break;
        }
        case SDS_TYPE_32:
        {
            struct sdshdr32 *start = (void*)((s) - (sizeof(struct sdshdr32)));
            start->len = initlen;
            start->alloc = 4294967295;
            start->flags = type;
            break;
        }
        case SDS_TYPE_64:
        {
            struct sdshdr64 *start = (void*)((s) - (sizeof(struct sdshdr64)));
            start->len = initlen;
            start->alloc = 18446744073709551615;
            start->flags = type;
            break;
        }    
        default:
            break;
    }
    if(initlen && init)
    {
        //复制init中的内容到s中去。
        memcpy(s, init, initlen);
    }
    s[initlen] = '\0';
    return s;
}

sds sdsnew(const char *init)
{
    size_t initlen = (init == NULL)? 0 : strlen(init);//获取动态字符串长度
    return sdsnewlen(init, initlen);
}


/*为字符串重新分配容量*/
sds sdsMakeRoomFor(sds s, size_t addlen)
{
    // size_t avail = sdsavail(s);
    //剩余未分配长度满足当前新增需求的直接返回
    // if(avail > addlen)
    // {
    //     return s;
    // }
    //剩余未分配长度不满足当前新增需求，重新分配
    size_t len = sdslen(s);
    char type = sdsReqTyp(len);
    size_t newlen = (len + addlen);
    char newtype = sdsReqTyp(newlen);
    void *sh, *newsh;
    sh = (char*)s - sdsHdrSize(type);
    size_t newhdrlen = sdsHdrSize(newtype);
    if(newtype == SDS_TYPE_5)
    {
        newtype = SDS_TYPE_8;
    }
    //类型相同则重新分配结构体空间即可
    if(type == newtype)
    {
        //结构体部分的位置不改变，但是柔型数组的长度改变
        newsh = realloc(sh, newhdrlen + newlen + 1);
        printf("location of newsh: %p\n",newsh);
        printf("location of sh: %p\n",sh);
        if(newsh == NULL)
        {
            return NULL;
        }
        s = (char*)newsh + newhdrlen;
        printf("content:%s\n",s);
    }
    else
    {
        //结构体部分的位置和柔型数组的长度都改变
        newsh = malloc(newhdrlen + newlen + 1);
        printf("location of newsh: %p\n",newsh);
        printf("location of sh: %p\n",sh);
        if(newsh == NULL)
        {
            return NULL;
        }
        memcpy((char*)newsh + newhdrlen, s, len + 1);
        free(sh);
        printf("location of sh: %p\n",sh);
        s = (char*)newsh + newhdrlen;
        printf("content:%s\n",s);
    }
    return s;    
}


/*拼接字符串*/
sds sdscatlen(sds s, const void *t, size_t addlen) {
    size_t currlen = sdslen(s);
    s = sdsMakeRoomFor(s,addlen);
    if (s == NULL)
    {
        return NULL;
    } 
    memcpy(s+currlen, t, addlen);
    sdsset(s, currlen + addlen);
    s[currlen + addlen] = '\0';
    return s;
}

sds sdscat(sds s, const char *t) {
    return sdscatlen(s, t, strlen(t));
}


/*释放字符串*/
void sdsfree(sds s)
{
    if(s == NULL)
    {
        return;
    }
    char type = sdsReqTyp(strlen(s));
    int hdrSize = sdsHdrSize(type);
    void *sh = s-hdrSize;
    free(sh);
    sh = NULL;
}