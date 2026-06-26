CFLAGS := -Wall -Werror -Wextra -pedantic -std=c99 -g

all: log jsonLoader tuidine termdine

termdine: src/main.c
	clang src/main.c src/jsonLoader.c -o termdine -lcjson -lncurses -lm -Llib -ljsonLoader -llog -ltuidine $(CFLAGS)

jsonLoader: src/jsonLoader.c include/termdine/jsonLoader.h
	clang -c src/jsonLoader.c -o lib/jsonLoader.o $(CFLAGS)
	ar rcs lib/libjsonLoader.a lib/jsonLoader.o

log: src/log.c include/termdine/log.h
	clang -c src/log.c -o lib/log.o $(CFLAGS)
	ar rcs lib/liblog.a lib/log.o

math: src/math.c
	clang src/math.c -o math -lm $(CFLAGS)

tuidine: src/tuidine.c include/termdine/tuidine.h 
	clang -c src/tuidine.c -o lib/tuidine.o $(CFLAGS)
	ar rcs lib/libtuidine.a lib/tuidine.o

clean:
	rm -rf termdine math lib/*

