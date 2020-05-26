#ifndef _XM_TYPEDEF_H__    
#define _XM_TYPEDEF_H__   

#include <stdio.h>
#include <stdlib.h>

#define xmprintfd(a,b) printf("Time :%s "#a" :%d\n",__TIME__,#b);
#define xmprintfe(a) printf("Time :%s File :%s Line :%s error :%s\n",__TIME__,__FILE__ ,__LINE__,#a);

typedef unsigned char xmbyte;

#endif