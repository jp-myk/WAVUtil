#!/bin/make
CC = gcc
CFLAGS = -O3 -Wall
CXX = g++
CXXFLAGS = -O3 -Wall
SRC=./src
LIBS=
INCLUDES=-I$(SRC)
LDFLAGS=
SIMPLE_WAV_OBJS=$(SRC)/SimpleWAV/WAV.o $(SRC)/SimpleWAV/WAVUtil.o

.PHONY: all clean
.SUFFIXES: .c .cpp .o

all: make_bindir wav_test wav_combine wav_trim wav_split

make_bindir:
	mkdir -p bin

wav_test: $(SIMPLE_WAV_OBJS) $(SRC)/wav_test.o 
	$(CXX) $(CXXFLAGS)  -o bin/$@  $^ $(LIBS) $(LDFLAGS) $(INCLUDES)
#	valgrind --error-limit=no --leak-check=full --show-reachable=yes -v ./bin/wav_test data/test.wav

wav_combine: $(SIMPLE_WAV_OBJS) $(SRC)/wav_combine.o 
	$(CXX) $(CXXFLAGS)  -o bin/$@  $^ $(LIBS) $(LDFLAGS) $(INCLUDES)


wav_trim: $(SIMPLE_WAV_OBJS) $(SRC)/wav_trim.o 
	$(CXX) $(CXXFLAGS)  -o bin/$@  $^ $(LIBS) $(LDFLAGS) $(INCLUDES)

wav_split: $(SIMPLE_WAV_OBJS) $(SRC)/wav_split.o 
	$(CXX) $(CXXFLAGS)  -o bin/$@  $^ $(LIBS) $(LDFLAGS) $(INCLUDES)

clean:
	find . -name "*.o" | xargs -n 1 $(RM)
	$(RM) bin/*

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $^
.cpp.o:
	$(CXX) $(CXXFLAGS) $(LIBS) $(INCLUDES) $(LDFLAGS) -o $@ -c $^
.c:
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
.cpp:
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
