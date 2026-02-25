CFLAGS := -Wall -Werror -Wextra -pedantic -std=c99 -g

all: jsonLoader log termdine

termdine: src/main.c
	clang src/main.c src/jsonLoader.c -o termdine  -lcjson -Llib -ljsonLoader -llog -lncurses $(CFLAGS)

jsonLoader: src/jsonLoader.c include/termdine/jsonLoader.h
	clang -c src/jsonLoader.c -o lib/jsonLoader.o $(CFLAGS)
	ar rcs lib/libjsonLoader.a lib/jsonLoader.o

log: src/log.c include/termdine/log.h
	clang -c src/log.c -o lib/log.o $(CFLAGS)
	ar rcs lib/liblog.a lib/log.o

clean:
	rm -rf termdine lib/*

