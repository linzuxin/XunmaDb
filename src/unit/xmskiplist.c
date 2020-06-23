#include "xmskiplist.h"

//创建零层链
xmSkipZeroLink *xmSkipZeroLinkCreate()
{
    xmSkipZeroLink *zeroLink = (xmSkipZeroLink *)malloc(sizeof(xmSkipZeroLink));
    zeroLink->linkNode = (xmSkipNodeArray *)malloc(sizeof(xmSkipNodeArray));
    zeroLink->linkNode->len = 0;
    zeroLink->nextNode = NULL;
    zeroLink->len = 1;
    return zeroLink;
}

//创建跳跃表
xmSkipLink *xmSkipLinkCreate()
{
    xmSkipLink *skipLink = (xmSkipLink *)malloc(sizeof(xmSkipLink));       //申请跳跃表空间
    xmSkipZeroLink *zeroLink = xmSkipZeroLinkCreate();                     //创建零层链
    skipLink->zeroLink = zeroLink;                                         //赋值零层链
    xmLevelNode **levelNode = skipLink->levelNodeArray;                    //表头获取 数组的指针 可以当做指针的指针
    *levelNode = (xmSkipNodeArray **)calloc(1, sizeof(xmSkipNodeArray *)); //申请指针的指针内存
    (*(*levelNode)->linkNode) = zeroLink->linkNode;                        //赋值表头
    skipLink->len = 1;
    return skipLink;
}
//创建节点
xmLinkNode *xmLinkNodeCreate(double score, char *ele)
{
    xmLinkNode *node = (xmLinkNode *)malloc(sizeof(xmLinkNode));
    node->ele = ele;
    node->score = score;
    return node;
}
//插入节点
xmbyte xmLinkNodeInsert(xmLinkNode *node)
{
    xmbyte result = 0;

    return result;
}

//查找要插入的节点数组
xmSkipNodeArray *findSkipNodeArray(xmSkipLink *skipLink, xmLinkNode *node)
{
    xmSkipNodeArray *nodeArray = NULL;

    //循环查找跳跃表
    for (xmbyte i = 0; i < skipLink->len; i++)
    {
        xmLevelNode *levelNode = skipLink->levelNodeArray[i];
        nodeArray = findLevelNodeFor(levelNode, node, i + 1); //findLevelNodeWhile一样的函数，只是不同的写法
        if (nodeArray)
        {
            break;
        }
    }
    return nodeArray;
}

//查找要插入的节点数组
xmSkipNodeArray *findLevelNodeFor(xmLevelNode *levelNode, xmLinkNode *node, xmbyte level)
{
    xmSkipNodeArray *nodeArray = NULL;
    xmSkipNodeArray **linkNode = levelNode->linkNode;
    xmbyte i = 0;
    //写法1
    do
    {
        xmbyte len = (*linkNode)->len;
        nodeArray = isSkipNodeArray(linkNode[i], node);  //可以用isSkipNodePoint替换
        if (nodeArray)
        {
            break;
        }
        i++;
        linkNode++;
    } while (i < level);
    return nodeArray;
}

//查找要插入的节点数组
xmSkipNodeArray *findLevelNodeWhile(xmLevelNode *levelNode, xmLinkNode *node, xmbyte level)
{
    xmSkipNodeArray *nodeArray = NULL;
    xmSkipNodeArray **linkNode = levelNode->linkNode;
    //写法二
    for (xmbyte i = 0; i < level; i++)
    {
        nodeArray = isSkipNodePoint(linkNode[i], node);  //isSkipNodeArray
        if (nodeArray)
        {
            break;
        }
    }
    return nodeArray;
}

//是否可以插入到SkipNode
xmSkipNodeArray *isSkipNodePoint(xmSkipNodeArray *nodeArraySource, xmLinkNode *node)
{
    xmSkipNodeArray *nodeArray = NULL;
    xmbyte len = nodeArraySource->len;
    xmLinkNode *begin = nodeArraySource->link;
    xmLinkNode *end = nodeArraySource->link + len;
    if (begin->score < node->score && end->score > node->score)
    {
        nodeArray = nodeArraySource;
    }
    return nodeArray;
}

//是否可以插入到SkipNode
xmSkipNodeArray *isSkipNodeArray(xmSkipNodeArray *nodeArraySource, xmLinkNode *node)
{
    xmSkipNodeArray *nodeArray = NULL;
    xmbyte len = nodeArraySource->len;

    if (nodeArraySource->link[0].score < node->score && nodeArraySource->link[len].score > node->score)
    {
        nodeArray = nodeArraySource;
    }
    return nodeArray;
}