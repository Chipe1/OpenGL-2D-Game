FLAGS = -lm -lglfw -lGLEW -lGL
OBJS = main.o glm.o shader.o texture.o mesh.o object.o anim.o
TARGET = a.out
COMPILER = gcc

.PHONY: clean pack

all: $(TARGET)

re: clean all

%.o: src/%.c src/%.h src/struct.h src/include.h
	$(COMPILER) -c $< $(FLAGS)

$(TARGET): $(OBJS)
	$(COMPILER) $^ -o $@ $(FLAGS)

clean:
	-rm $(OBJS) $(TARGET)

pack: all
	-rm $(OBJS)
