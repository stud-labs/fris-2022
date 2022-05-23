.PHONY: compile run debug

CFLAGS = -g -O0
LDFLAGS = -lm
GPP=g++ $(CFLAGS) $(LDFLAGS)
SRC=core.cpp  main.cpp
HDR=core.h

compile: fris

fris: $(SRC) $(HDR)
	$(GPP) -o $@ $(SRC)

run: compile
	./fris

debug: compile
	gdb ./fris
