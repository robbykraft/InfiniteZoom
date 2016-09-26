# Linux (default)
EXE2 = two
EXE3 = three
CFLAGS = -std=gnu99
LDFLAGS = -lGL -lGLU -lglut -lm

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXE2 = two.exe
	EXE3 = three.exe
	LDFLAGS = -lopengl32 -lglu32 -lglut32
endif

# OS X, OSTYPE not being declared
ifndef OSTYPE
  OSTYPE = $(shell uname -s|awk '{print tolower($$0)}')
  #export OSTYPE
endif
ifeq ($(OSTYPE),darwin)
	LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT  -Wno-deprecated
endif

all: two three

$(EXE2): two.c
	gcc -o $@ $< $(CFLAGS) $(LDFLAGS)

$(EXE3): three.c
	gcc -o $@ $< $(CFLAGS) $(LDFLAGS)

run2:
	./$(EXE2) $(ARGS)

run3:
	./$(EXE3) $(ARGS)