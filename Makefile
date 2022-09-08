CC = gcc
CFLAGS = -Wall -g
SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c, bin/linux/obj/%.o, ${SRC})
LIBS = -lm -lglfw -lGL -lGLEW
ASSETS=$(patsubst asset/%, bin/linux/asset/%, $(wildcard asset/*/*.*))

bin/linux/Aechmea: $(OBJ) $(ASSETS)
	$(CC) $(CFLAGS) $(LIBS) $(OBJ) -o bin/linux/Aechmea

bin/linux/obj/%.o: src/%.c | bin/linux/obj
	$(CC) $(CFLAGS) -c -o $@ $<

bin/linux/asset/%: asset/% | bin/linux/asset
	cp $< $@

bin/linux/obj: | bin/linux
	mkdir $@

bin/linux/asset: | bin/linux
	mkdir $@
	-mkdir $@/tex
	-mkdir $@/shader
	-mkdir $@/mesh

bin/linux: | bin
	mkdir $@

bin:
	mkdir $@

run:
	./bin/linux/Aechmea
