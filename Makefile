# CppAutomata

FILES=CppAutomata.cpp Class-Automato.cpp
OUT=CppAutomata
CC=g++
FLAGS=-Wall

all :
    $(CC) $(FLAGS) -o $(OUT) $(FILES)
