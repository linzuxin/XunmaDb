#include "xmtest.h"

int gtests = 0;

int foo = 7;
int bar = 4;

static char * test_foo()
{
    xmAssert("error, foo != 7", foo == 7);
    return 0;
}

static char * test_bar()
{
    xmAssert("error, bar != 5", bar == 5);
    return 0;
}

static char * all_tests() 
{
    xmRunTest(test_foo);
    xmRunTest(test_bar);
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