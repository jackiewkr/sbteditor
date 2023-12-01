CC := clang
CFLAGS := -Wall -pedantic
INCLUDES := main.c libs/file.c
LDFLAGS := -lncurses
DEBUGFLAGS := -g

build: 
	$(CC) -o bin/sbteditor $(INCLUDES) $(LDFLAGS) $(CFLAGS)

debug: 
	$(CC) -o bin/sbteditor_debug $(INCLUDES) $(LDFLAGS) $(CFLAGS) $(DEBUGFLAGS)
