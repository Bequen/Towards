EXECUTABLE=towards

LIBS=`sdl2-config --cflags` \
	 `sdl2-config --libs` \
	 -lGL \
	 -limgui -L./external/imgui \
	 ./external/glad/glad.o \
	 `/usr/bin/python3.11-config  --cflags` \
	 -L/usr/lib -lpython3.11 \
	 -lcrypt \
	 -ldl \
	 -lm

INCLUDE=-I./internal/ \
		-I./external/glad/glad \
	 	-I./external/cglm/include/ \
		-I./external/imgui/ \
		-I./external/imgui/backends \
		-I/usr/include/SDL2 \
		`python3-config --includes` \
		-I./src/ \
		-I./external/

SUBDIRS=src/drw src/drw/graph src/drw/scene \
		src/editor \
		src/io/ \
		src

FILES=$(wildcard $(addsuffix /*.cpp,$(SUBDIRS)))
OBJ=$(addprefix obj/,$(FILES:%.cpp=%.o))

CFLAGS:=
CXX=clang++

.PHONY: debug release clean

debug: CFLAGS+=-g3
debug: ${EXECUTABLE}

info:
	printf "${OBJ}"

${EXECUTABLE}: ${OBJ}
	${CXX} $^ -o $@ ${CFLAGS} -fuse-ld=mold ${LIBS}

obj/%.o:%.cpp
	@mkdir -p $(dir $@)
	${CXX} ${INCLUDE} -c $^ -o $@ ${CFLAGS}

clean:
	@printf "Cleaning up\n"
	@rm -rf ./obj/ ./${EXECUTABLE}
	@printf "Done cleaning up\n"

compile_flags.txt:
	@rm -f ./compile_flags.txt
	@echo ${INCLUDE} > $@

run: ${EXECUTABLE}
	@printf "Running...\n";
	./${EXECUTABLE}
