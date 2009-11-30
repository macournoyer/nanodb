CC = gcc
CFLAGS = -DDEBUG -g -O2
INCS = -Isrc -Ivendor/tokyocabinet -Ivendor/libev
LIBS = vendor/tokyocabinet/libtokyocabinet.a -lz -lbz2

SRC = src/store.c \
      src/server.c \
      src/interp.c \
      src/main.c
OBJ = ${SRC:.c=.o}
OBJ_MIN = vm/tr.o

all: nanodb

.c.o:
	@echo "   cc $<"
	@${CC} -c ${CFLAGS} ${INCS} -o $@ $<

nanodb: ${OBJ}
	@echo " link nanodb"
	@${CC} ${CFLAGS} ${OBJ} ${LIBS} -o nanodb

clean:
	$(RM) src/*.o

rebuild: clean all

.PHONY: all clean rebuild
