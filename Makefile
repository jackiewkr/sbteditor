CC := clang
CFLAGS := -Weverything -pedantic
INCLUDES := main.c libs/file.c
LDFLAGS := lncurses
DEBUGFLAGS := -g

CPU=$(lscpu | grep -oP 'Architecture:\s*\K.+')

build: 
	$(CC) -o sbteditor.$(CPU) $(INCLUDES) $(LDFLAGS) $(CFLAGS)

debug: 
	$(CC) -o sbteditor.debug_$(CPU) $(INCLUDES) $(LDFLAGS) $(CFLAGS) $(DEBUGFLAGS)
