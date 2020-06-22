#include <stdio.h>
#include "xmtest.h"
#include "skiplist.h"

int gtests = 0;
skList *g_sl1;
void showSkList(skList *sl)
{
    for(int i = sl->level-1; i >- 1; i--)
    {
        skNode * currNode = sl->header;
        printf("line %d\n",i);
        while (currNode->level[i].forward)
        {
            printf(" %lf , ",currNode->level[i].forward->score);
            currNode = currNode->level[i].forward;
        }
        printf("\n");
    }
}


static char * test_skNodeRandomLevel()
{
    int j;
    for(int i = 0; i < 100; i++)
    {
        j = skNodeRandomLevel();
        // printf("(%d) random: %d\n",i,j);
        xmAssert("error, RandomLevel is bigger than 64",  j <= SKLIST_MAXLEVEL);
    }    
    return 0;
}

static char * test_skNodeCreate()
{
    skNode *n = skNodeCreate(5,28,"hi");
    xmAssert("error, new node's string is error", n->ele == "hi");
    xmAssert("error, new node's score is error", n->score == 28);    
    return 0;
}

static char * test_skListCreate()
{
    skList *sl = skListCreate();
    xmAssert("error, skList's header inexist", sl->header);
    xmAssert("error, skList's level is error", sl->level == 1);
    xmAssert("error, skList's length is error", sl->length == 0);    
    return 0;
}

static char * test_skNodeInsert()
{
    g_sl1 = skListCreate(); 
    printf("insert1------------------------------------------------------------------------\n");
    skNode *insertNode1 = skNodeInsert(g_sl1,35,"good");
    printf("insert2------------------------------------------------------------------------\n");
    skNode *insertNode2 = skNodeInsert(g_sl1,40,"nice");
    printf("insert3------------------------------------------------------------------------\n");
    skNode *insertNode3 = skNodeInsert(g_sl1,20,"hello");
    printf("insert4------------------------------------------------------------------------\n");
    skNode *insertNode4 = skNodeInsert(g_sl1,60,"selina");
    printf("insert5------------------------------------------------------------------------\n");
    skNode *insertNode5 = skNodeInsert(g_sl1,50,"pretty");

    printf("----------------g_sl1's struct print-----------------\n");
    printf("skiplist's length: %d\n",g_sl1->length);
    printf("skiplist's level: %d\n",g_sl1->level);
    printf("skiplist's head's score: %lf\n",g_sl1->header->score);
    printf("skiplist's tail's score: %lf\n",g_sl1->tail->score);
    for(int i = g_sl1->level-1; i >- 1; i--)
    {
        skNode * currNode = g_sl1->header;
        printf("line %d\n",i);
        while (currNode->level[i].forward)
        {
            printf(" %lf , ",currNode->level[i].forward->score);
            currNode = currNode->level[i].forward;
        }
        printf("\n");
    }

    skNode *slHeader = g_sl1->header;
    skNode *n1 = slHeader->level[0].forward;
    skNode *n2 = n1->level[0].forward;
    skNode *n3 = n2->level[0].forward;
    skNode *n4 = n3->level[0].forward;
    skNode *n5 = n4->level[0].forward;
    xmAssert("error, skNodeInsert error hello", strcmp(n1->ele,"hello")==0);
    xmAssert("error, skNodeInsert error good", strcmp(n2->ele,"good")==0);
    xmAssert("error, skNodeInsert error nice", strcmp(n3->ele,"nice")==0);
    xmAssert("error, skNodeInsert error pretty", strcmp(n4->ele,"pretty")==0);
    xmAssert("error, skNodeInsert error selina", strcmp(n5->ele,"selina")==0);
    return 0;
}

static char * test_skNodeGet()
{
    printf("get1------------------------------------------------------------------------\n");
    skNode *nodeGet1 = skNodeGet(g_sl1, 35.00, "good");
    printf("get2------------------------------------------------------------------------\n");
    skNode *nodeGet2 = skNodeGet(g_sl1, 40.00, "nice");
    printf("get3------------------------------------------------------------------------\n");
    skNode *nodeGet3 = skNodeGet(g_sl1, 20.00, "hello");
    printf("get4------------------------------------------------------------------------\n");
    skNode *nodeGet4 = skNodeGet(g_sl1, 60.00, "selina");
    printf("get5------------------------------------------------------------------------\n");
    skNode *nodeGet5 = skNodeGet(g_sl1, 50.00, "pretty");
    xmAssert("error, skNodeInsert error hello", strcmp(nodeGet1->ele,"good")==0);
    xmAssert("error, skNodeInsert error good", strcmp(nodeGet2->ele,"nice")==0);
    xmAssert("error, skNodeInsert error nice", strcmp(nodeGet3->ele,"hello")==0);
    xmAssert("error, skNodeInsert error pretty", strcmp(nodeGet4->ele,"selina")==0);
    xmAssert("error, skNodeInsert error selina", strcmp(nodeGet5->ele,"pretty")==0);
}

static char * test_skNodeDelete()
{
    printf("delete1------------------------------------------------------------------------\n");
    skNode *del1 = skNodeGet(g_sl1, 35.00, "good");
    skNodeDelete(g_sl1,del1);
    showSkList(g_sl1);
    xmAssert("error, skNodeDel error", skNodeGet(g_sl1, 35.00, "good") == NULL);
    printf("delete2------------------------------------------------------------------------\n");
    skNode *del2 = skNodeGet(g_sl1, 20.00, "hello");
    skNodeDelete(g_sl1,del2);
    showSkList(g_sl1);
    xmAssert("error, skNodeDel error", skNodeGet(g_sl1, 20.00, "hello") == NULL);
    printf("delete3------------------------------------------------------------------------\n");
    skNode *del3 = skNodeGet(g_sl1, 60.00, "selina");
    skNodeDelete(g_sl1,del3);
    showSkList(g_sl1);
    xmAssert("error, skNodeDel error", skNodeGet(g_sl1, 60.00, "selina") == NULL);
}

static char * all_tests() 
{
    // xmRunTest(test_skNodeRandomLevel);
    // xmRunTest(test_skNodeCreate);
    // xmRunTest(test_skListCreate);
    xmRunTest(test_skNodeInsert);
    xmRunTest(test_skNodeGet);
    xmRunTest(test_skNodeDelete);
    
    return 0;
}

int main(int argc, char **argv)
{
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }

    printf("Tests run: %d\n", gtests);

    return result != 0;
}