CC = gcc
buildPath = build
#objects = main.o 
objects := $(patsubst %.c,%.o,$(wildcard *.c)) 

xunmadb : $(objects)
	$(CXX) -o xunmadb $(objects)

$(objects) : src/unit/xmtypedef.h

.PHONY : clean
clean :
	rm xunmadb $(objects);