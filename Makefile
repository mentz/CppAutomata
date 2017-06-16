# CppAutomata

FILES = Automatos.cpp Main.cpp
OUT = a.out
CC = g++
WARN = -Wall
FLAGS = -std=c++14

default: $(OUT)
a.out: $(FILES)
	$(CC) $(WARN) -o $(OUT) $(FILES) $(FLAGS)

.PHONY: a.out