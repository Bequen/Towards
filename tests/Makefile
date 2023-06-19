LIBS=`sdl2-config --cflags` \
	`sdl2-config --libs` \
	-lGL \
	-limgui -L../external/imgui \
	../external/glad/glad.o \
	`/usr/bin/python3.11-config  --cflags` \
	-L/usr/lib -lpython3.11 \
	-lcrypt \
	-ldl \
	-lm

drw/formats: drw/formats.cpp
	@clang++ $^ -o $@ -g3 -ltowards -L../ -I../src/ ${LIBS}
