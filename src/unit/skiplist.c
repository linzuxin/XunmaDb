#include <stdio.h>
#include <math.h>
#include <string.h>
#include "skiplist.h"

//设置跳跃表节点的随机层数
int skNodeRandomLevel(void)
{
    int level = 1;//初始层级
    while ((rand() & 65535) < (SKLIST_p * 65535))
    {
        level += 1;
    }
    return (level < SKLIST_MAXLEVEL) ? level : SKLIST_MAXLEVEL;
}

//创建跳跃表节点
skNode* skNodeCreate(int level, double score, char* ele)
{
    printf("enter skNodeCreate\n");
    skNode *n = (skNode *)malloc(sizeof(*n) + level*sizeof(skLevel));
    n ->score = score;
    n ->ele = ele;
    return n;
}

//创建跳跃表
skList* skListCreate()
{
    printf("enter skNodeListCreate\n");
    skList *sl = (skList*)malloc(sizeof(*sl));
    sl->header = skNodeCreate(SKLIST_MAXLEVEL, 0, NULL);
    sl->header->backward = NULL;
    sl->level = 1;
    sl->length = 0;
    sl->tail = NULL;
    //初始化跳跃表头节点
    for (int i = 0; i < SKLIST_MAXLEVEL; i++)
    {
        sl->header->level[i].forward = NULL;
        sl->header->level[i].span = 0;
    }
    return sl;
}

//插入跳跃表节点
skNode* skNodeInsert(skList* sl, double score, char* ele)
{
    //获取header，header的level，从柔型数组skLevel中找到数组最层级最高的元素
    //通过这个元素往右找其他node的柔性数组在相同层级的元素，与score比较大小，直到元素值大于score或者为空时往下一层，在往右找节点进行比较
    //当比到最底层时候，当前元素小于score，当前元素的下一个元素大于score，这两个元素之间的位置就是score要插入的位置
    skNode* cNode = sl->header;
    skNode* newlist[SKLIST_MAXLEVEL];
    size_t layer[SKLIST_MAXLEVEL];
    int i;    
    //寻找新节点的插入位置
    //for循环，通过跳跃表的header指针，从上往下逐层寻找
    printf("enter skNodeInsert\n");
    printf("list level:%d\n",sl->level);
    for(i =  sl->level - 1; i > -1; i--)
    {
        printf("first for i:%d\n",i);
        skNode* fNode = cNode->level[i].forward;
        printf("fNode exists: %d\n",fNode == NULL);
        //while循环，在当前层不断往右找，当当前节点的右侧节点为空或者大于插入节点时停止
        while (cNode->level[i].forward && (cNode->level[i].forward->score < score || (cNode->level[i].forward->score == score && strcmp(cNode->level[i].forward->ele, ele) < 0)))
        {
            printf("enter search circle\n");
            cNode = cNode->level[i].forward;
            printf("cNode's score: %lf\n",cNode->score);
        }
        //将当前节点存在新节点数组中（实际存下来的会是头结点，或者当前层插入节点左侧的第一个节点）
        printf("out of search circle \n");
        newlist[i] = cNode;  
        printf("newlist[%d]: %d\n",i,newlist[i] != NULL);
        printf("newlist[%d]'s score: %lf\n",i,newlist[i]->score);
    }
    //生成插入节点的层级（64内的随机数）
    int insLevel = skNodeRandomLevel();
      printf("insLevel: %d,  slLevel: %d \n",insLevel, sl->level);
    //若生成层数大于跳跃表当前层数，则将对应新节点数组的元素赋值成头部节点，并将跳跃表层数变更
    if(insLevel > sl->level)
    {
        printf("enter override level\n");
        for (i = sl->level; i < insLevel; i++)
        {
            printf("second for i:%d\n",i);
            newlist[i] = sl->header;
            printf("newlist[%d]: %d\n",i,newlist[i] != NULL);
        }
        sl->level = insLevel; 
        printf("new slLevel: %d\n",sl->level);
    }
    //生成插入节点
    skNode *insNode = skNodeCreate(insLevel, score, ele);
    printf("createnode string: %s\n",insNode->ele);
    printf("createnode score: %lf\n",insNode->score);
    //for循环
    //为插入节点的每个层级的forward指针赋值（应为查寻插入节点的路径下每层查寻的最后一个节点）
    //修改查寻插入节点路径中每层对应节点的forward指针为插入节点
    for (i = 0; i < insLevel; i++)
    {
        printf("third for i:%d\n",i);
        insNode->level[i].forward = newlist[i]->level[i].forward;
        printf("newnodeLevel[%d]forward: %d\n",i,insNode->level[i].forward == NULL);
        if(insNode->level[i].forward)
        {
            printf("newnodeLevel[%d]forward's score: %lf\n",i,insNode->level[i].forward->score);
        }
        newlist[i]->level[i].forward = insNode;
        printf("newlist[%d]forward: %d\n",i,newlist[i]->level[i].forward == NULL);
        if(newlist[i]->level[i].forward)
        {
            printf("newlist[%d]forward's score: %lf\n",i,newlist[i]->level[i].forward->score);
        }

    }
    //若插入节点的前一节点为头结点，则backward指针为空，否则为前一节点。
    insNode->backward = (newlist[0] == sl->header) ? NULL : newlist[0];
    printf("isNode's backward exist: %d\n",insNode->backward==NULL);
    //设置插入节点的后一节点的backward指针
    if (insNode->level[0].forward)
    {
        skNode *insfNode = insNode->level[0].forward;
        insfNode->backward = insNode;
        printf("insfNode's backward's score:%lf\n",insfNode->backward->score);
    }
    else
    {
        sl->tail = insNode;
        printf("sl's tail's score: %lf\n",sl->tail->score);
    }
    sl->length++;
    printf("sl's length: %d\n",sl->length);
    return insNode;
}