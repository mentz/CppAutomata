# CppAutomata

FILES = Automatos.cpp Main.cpp
OUT = cppautomata
CC = g++
WARN = -Wall
FLAGS = -std=c++14

default: $(OUT)
cppautomata: $(FILES)
	$(CC) $(WARN) -o $(OUT) $(FILES) $(FLAGS)

.PHONY: cppautomata
