#ifndef _XM_TEST_H
#define _XM_TEST_H

#define xmAssert(what, test) do { if (!(test)) return what; } while (0)
#define xmRunTest(test) do { char *what = test(); gtests++; if (what) return what; } while (0)

extern int gtests;

#endif