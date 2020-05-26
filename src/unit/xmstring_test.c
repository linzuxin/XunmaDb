#include "xmtypedef.h"
#include "xmstring.h"
#include "xmtest.h"

int gtests = 0;

static char * test_newXmStr()
{
    xmstring *root = newXmStr();
    xmAssert("error, root != null", root == NULL);
    return 0;
}

static char * test_addXmStr()
{
    xmstring *item = addXmStr();
    xmAssert("error, addXmStr != 5", item == NULL);
    return 0;
}

static char * all_tests() 
{
    xmRunTest(test_newXmStr);
    xmRunTest(test_addXmStr);
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