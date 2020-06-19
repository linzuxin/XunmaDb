#ifndef _SKIPLIST__    
#define _SKIPLIST__  

#include <stdlib.h>

#define SKLIST_MAXLEVEL 64
#define SKLIST_p 0.25
//柔型数组结构体
typedef struct skLevel
{
    struct skNode *forward;//下一个节点
    unsigned long span;//forward和当前节点之间的元素个数
} skLevel;//柔性数组，随机生成一个1~64的值(对应最底层的链表的某个元素，存的是同一个值)

//跳跃表节点结构体
typedef struct skNode
{
    char* ele;//字符串类型
    double score;//存储排序分值
    struct skNode *backword;//后退指针（前一个指针）
    skLevel level[];
} skNode;

//跳跃表结构体
typedef struct skList
{
    skNode *header;//头节点
    skNode *tail;//尾结点
    unsigned long length;//跳跃表长度（最底层长度）
    int level;//跳跃表高度（层级）
} skList;

int skNodeRandomLevel(void);
skList* skListCreate();
skNode* skNodeCreate(int level, double score, char* ele);
skNode* skNodeInsert(skList* sl, double score, char* ele);

#endif