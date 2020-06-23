#include "xmskiplist.h"

//创建零层链
xmSkipNodeList *xmSkipZeroLinkCreate(xmSkipNodeList *next)
{
    xmSkipNodeList *nodeList = (xmSkipNodeList *)malloc(sizeof(xmSkipNodeList));
    nodeList->len = 0;
    nodeList->next = next;
    return nodeList;
}

//创建跳跃表
xmSkipLink *xmSkipLinkCreate()
{
    xmSkipLink *skipLink = (xmSkipLink *)malloc(sizeof(xmSkipLink));     //申请跳跃表空间
    xmSkipNodeList *zeroLink = xmSkipZeroLinkCreate(NULL);               //创建零层链
    skipLink->zeroLink = zeroLink;                                       //赋值零层链
    skipLink->linkLen = 1;                                               //链表长度为1
    xmLevelNode **levelNode = skipLink->levelNodeList;                   //表头获取 数组的指针 可以当做指针的指针
    *levelNode = (xmSkipNodeList **)calloc(1, sizeof(xmSkipNodeList *)); //申请指针的指针内存
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
xmbyte xmLinkNodeInsert(xmSkipLink *skipLink, xmLinkNode *node)
{
    xmbyte result = 0;
    xmSkipNodeList *nodeList = findSkipNodeList(skipLink, node);
    xmbyte addSkipNodeList = xmSkipNodeListInsert(nodeList, node);
    skipLink->linkLen += addSkipNodeList; //添加新增的链表结点
    return result;
}
//插入节点
xmbyte xmSkipNodeListInsert(xmSkipNodeList *nodeList, xmLinkNode *node)
{
    xmbyte result = 0;
    if (xmSkipNodeListIsFull(nodeList))
    {
        xmLinkNode *nodeMax = &nodeList[xmLinkLen - 1];
        xmSkipNodeListSortInsert(nodeList, node);
        xmSkipNodeList *next = nodeList->next;
        xmSkipNodeList *nextNext = NULL; //下一个的下一个结点
        xmbyte isNextCreate = 0;         //是否要创建下个结点
        if (next)
        { //有下级链表，则判断是否已经满了
            if (xmSkipNodeListIsFull(next))
            {
                isNextCreate = 1; //满了就要重新创建个
                nextNext = next->next;
            }
        }
        else
        { //没有下级则创建
            isNextCreate = 1;
        }
        if (isNextCreate) //创建新链表，暂时，后续要修改
        {
            next = xmSkipZeroLinkCreate(nextNext);
            nodeList->next = next;
            result = 1;
        }
        xmSkipNodeListSortInsert(next, node);
    }
    else
    {
        xmSkipNodeListSortInsert(nodeList, node);
    }
    return result;
}
//拷贝结点
void copyNode(xmLinkNode *desc, xmLinkNode *source)
{
    desc->score = source->score;
    desc->ele = source->ele;
}
//添加结点数
xmbyte addSkipNodeListLen(xmSkipNodeList *nodeList)
{
    if (nodeList->len < xmLinkLen - 1)
    {
        ++(nodeList->len);
    }
    return nodeList->len;
}
//排序插入
xmbyte xmSkipNodeListSortInsert(xmSkipNodeList *nodeList, xmLinkNode *node)
{
    xmbyte result = 0;
    xmbyte insertPos = 0;
    xmbyte len = nodeList->len;
    if (len == 0)
    {
        copyNode(nodeList->link, node);
    }
    else if (nodeList->link[len].score <= node->score)
    {
        len = addSkipNodeListLen(nodeList);
        copyNode(&(nodeList->link[len]), node); // 等同写法 copyNode(nodeList->link+len, node);
    }
    else
    {
        //排序插入
        xmbyte startPos = len; //开始位置
        xmbyte midpos = (startPos - startPos % 2) / 2;
        while (1)
        {
            xmbyte bInsert = 0;
            startPos = midpos;
            midpos = (midpos - midpos % 2) / 2;
            if (nodeList->link[midpos].score < node->score) //小于时候继续取中间
            {
                startPos -= midpos;
            }
            else if (nodeList->link[midpos].score > node->score) //大于时候继续取中间
            {
                startPos += midpos;
            }
            else
            {
                midpos = 0;
            }
            if (midpos == 0) //没有中间位置就是要插入的位置
            {
                bInsert = 1;
            }
            if (bInsert)
            {
                //后面的进行排序移动
                for (xmbyte i = len; i >= startPos; i--)
                {
                    copyNode(nodeList->link + i + 1, nodeList->link + i);
                }

                copyNode(nodeList->link + startPos + 1, node); //插入数据
                addSkipNodeListLen(nodeList);
                break;
            }
        }
    }
    return result;
}
//查找要插入的节点数组
xmSkipNodeList *findSkipNodeList(xmSkipLink *skipLink, xmLinkNode *node)
{
    xmSkipNodeList *nodeList = NULL;

    //循环查找跳跃表
    for (xmbyte i = 0; i < skipLink->len; i++)
    {
        xmLevelNode *levelNode = skipLink->levelNodeList[i];
        nodeList = findLevelNodeFor(levelNode, node, i + 1); //findLevelNodeWhile一样的函数，只是不同的写法
        if (nodeList)
        {
            break;
        }
    }
    return nodeList;
}

//查找要插入的节点数组
xmSkipNodeList *findLevelNodeFor(xmLevelNode *levelNode, xmLinkNode *node, xmbyte level)
{
    xmSkipNodeList *nodeList = NULL;
    xmSkipNodeList **linkNode = levelNode->linkNode;
    xmbyte i = 0;
    //写法1
    do
    {
        xmbyte len = (*linkNode)->len;
        nodeList = isSkipNodeList(linkNode[i], node); //可以用isSkipNodePoint替换
        if (nodeList)
        {
            break;
        }
        i++;
        linkNode++;
    } while (i < level);
    return nodeList;
}

//查找要插入的节点数组
xmSkipNodeList *findLevelNodeWhile(xmLevelNode *levelNode, xmLinkNode *node, xmbyte level)
{
    xmSkipNodeList *nodeList = NULL;
    xmSkipNodeList **linkNode = levelNode->linkNode;
    //写法二
    for (xmbyte i = 0; i < level; i++)
    {
        nodeList = isSkipNodePoint(linkNode[i], node); //isSkipNodeList
        if (nodeList)
        {
            break;
        }
    }
    return nodeList;
}

//是否可以插入到SkipNode
xmSkipNodeList *isSkipNodePoint(xmSkipNodeList *nodeArraySource, xmLinkNode *node)
{
    xmSkipNodeList *nodeList = NULL;
    xmbyte len = nodeArraySource->len;
    xmLinkNode *begin = nodeArraySource->link;
    xmLinkNode *end = nodeArraySource->link + len;
    if (xmSkipNodeListIsFull(nodeArraySource))
    {
        //当数组满了则进行区间判断
        if (begin->score < node->score && end->score > node->score)
        {
            nodeList = nodeArraySource;
        }
    }
    else
    { //数组不满则直接返回要插入的SkipNode
        nodeList = nodeArraySource;
    }
    return nodeList;
}

//是否可以插入到SkipNode
xmSkipNodeList *isSkipNodeList(xmSkipNodeList *nodeArraySource, xmLinkNode *node)
{
    xmSkipNodeList *nodeList = NULL;
    xmbyte len = nodeArraySource->len;
    if (xmSkipNodeListIsFull(nodeArraySource))
    {
        //当数组满了则进行区间判断
        if (nodeArraySource->link[0].score < node->score && nodeArraySource->link[len].score > node->score)
        {
            nodeList = nodeArraySource;
        }
    }
    else
    { //数组不满则直接返回要插入的SkipNode
        nodeList = nodeArraySource;
    }
    return nodeList;
}
//是否数组已经满了
xmbyte xmSkipNodeListIsFull(xmSkipNodeList *nodeList)
{
    xmbyte result = 0;
    if (nodeList->len == xmLinkLen - 1)
    {
        result = 1;
    }
    return result;
}