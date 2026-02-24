CFLAGS=-Wall -Werror -pedantic -std=c99 -g

all: termdine jsonLoader

termdine: src/main.c
	clang src/main.c -o termdine -lncurses -lcjson $(CFLAGS)

jsonLoader: src/jsonLoader.c
	clang src/jsonLoader.c -o jsonLoader -lcjson $(CFLAGS)

clean:
	rm -rf jsonLoader termdine

