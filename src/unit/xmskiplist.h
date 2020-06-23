#ifndef _XM_SKIPLIST__
#define _XM_SKIPLIST__

#include <stdlib.h>
#include "xmtypedef.h"
//跳跃表现实机制
//第一是个表
//行数0  是个完整链表  xmLinkNode链表结点
//行数1  是跳跃表索引表  是链表的中间值  64个就会创建一个中间值，指向第32个元素。
//行数2  是跳跃表索引表  是链表的中间值  128个就会创建一个中间值，指向第64个元素。同时更新行1的索引
// ....同理
#define xmLinkLen 64
#define xmLevelLen 64
//行数0的结点
typedef struct
{
    char *ele;    //字符串类型
    double score; //存储排序分值
} xmLinkNode;

typedef struct
{
    xmLinkNode link[xmLinkLen]; //固定大小
    xmbyte len;                 //使用长度
} xmSkipNodeArray;              //固定大小数组

typedef struct
{
    xmSkipNodeArray *linkNode; //链表
    xmSkipNodeArray *nextNode; //下个链表
    xmbyte len;                //长度
} xmSkipZeroLink;              //跳跃表  0层链表

//行数1以上的结点
typedef struct
{
    xmSkipNodeArray **linkNode; //关联的0层链表
} xmLevelNode;

typedef struct
{
    xmSkipZeroLink *zeroLink;                //零层链
    xmLevelNode *levelNodeArray[xmLevelLen]; //指向xmLevelNode基本
    xmbyte len;                              //层级初始化长度
} xmSkipLink;

//创建零层链
xmSkipZeroLink *xmSkipZeroLinkCreate();
//创建跳跃表
xmSkipLink *xmSkipLinkCreate();
//创建数据结点
xmLinkNode *xmLinkNodeCreate(double score, char *ele);
//插入节点
xmbyte xmLinkNodeInsert(xmLinkNode *node);
//查找要插入的节点数组
xmSkipNodeArray *findSkipNodeArray(xmSkipLink *skipLink, xmLinkNode *node);

//查找要插入的节点数组,下面两个函数一样的结果,写法不一样
xmSkipNodeArray *findLevelNodeFor(xmLevelNode *levelNode, xmLinkNode *node, xmbyte level);
xmSkipNodeArray *findLevelNodeWhile(xmLevelNode *levelNode, xmLinkNode *node, xmbyte level);

//是否可以插入到SkipNode,下面两个函数一样的结果,写法不一样,定义函数里面参数的可以省略，实现里面不可以省略
xmSkipNodeArray *isSkipNodeArray(xmSkipNodeArray *, xmLinkNode *);
xmSkipNodeArray *isSkipNodePoint(xmSkipNodeArray *, xmLinkNode *);

//skNode* updateLevel(skList* sl, double score, char* ele);

#endif