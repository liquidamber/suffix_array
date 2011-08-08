CTYPEFLAGS = -g -Wall -Wextra -O0 -lm
CFLAGS = $(CTYPEFLAGS)
CXXFLAGS = $(CTYPEFLAGS) -std=c++0x -Dnullptr=0

SRC = $(wildcard *.cpp *.c)
TGT = $(basename $(SRC))

all: $(TGT)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< libgen_prob.a -o $@

mysearch: mysearch.cpp mysearch.h gen_prob.h
	$(CXX) $(CXXFLAGS) $< libgen_prob.a -o $@

test: test.cpp mysearch.h gen_prob.h
	$(CXX) $(CXXFLAGS) $< libgen_prob.a -o $@

%: %.c
	$(CC) $(CFLAGS) $< libgen_prob.a -o $@
