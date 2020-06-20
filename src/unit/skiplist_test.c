#include <stdio.h>
#include "xmtest.h"
#include "skiplist.h"

int gtests = 0;

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
    skNode *n = skNodeCreate(5,28,"hello");
    xmAssert("error, new node's string is error", n->ele == "hello");
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
    skList *sl1 = skListCreate(); 
    printf("insert1------------------------------------------------------------------------\n");
    skNode *insertNode1 = skNodeInsert(sl1,35,"good");
    printf("insert2------------------------------------------------------------------------\n");
    skNode *insertNode2 = skNodeInsert(sl1,40,"nice");
    printf("insert3------------------------------------------------------------------------\n");
    skNode *insertNode3 = skNodeInsert(sl1,20,"hello");
    printf("insert4------------------------------------------------------------------------\n");
    skNode *insertNode4 = skNodeInsert(sl1,60,"selina");
    printf("insert5------------------------------------------------------------------------\n");
    skNode *insertNode5 = skNodeInsert(sl1,50,"pretty");

    printf("----------------sl1's struct print-----------------\n");
    printf("skiplist's length: %d\n",sl1->length);
    printf("skiplist's level: %d\n",sl1->level);
    printf("skiplist's head's score: %lf\n",sl1->header->score);
    printf("skiplist's tail's score: %lf\n",sl1->tail->score);
    for(int i = sl1->level-1; i >- 1; i--)
    {
        skNode * currNode = sl1->header;
        printf("line %d\n",i);
        while (currNode->level[i].forward)
        {
            printf(" %lf , ",currNode->level[i].forward->score);
            currNode = currNode->level[i].forward;
        }
        printf("\n");
    }

    printf("insert   result------------------------------------------------------------------------\n");
    skNode *slHeader = sl1->header;
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

static char * all_tests() 
{
    xmRunTest(test_skNodeRandomLevel);
    xmRunTest(test_skNodeCreate);
    xmRunTest(test_skListCreate);
    xmRunTest(test_skNodeInsert);
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