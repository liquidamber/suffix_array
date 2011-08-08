CTYPEFLAGS = -g -Wall -Wextra -lm
CFLAGS = $(CTYPEFLAGS)
CXXFLAGS = $(CTYPEFLAGS) -std=c++0x -Dnullptr=0

SRC = $(wildcard *.cpp *.c)
MYSRC = $(wildcard *.cpp)
TGT = \
	$(addsuffix .O0, $(basename $(MYSRC))) \
	$(addsuffix .O2, $(basename $(SRC))) \
	$(addsuffix .O3, $(basename $(SRC)))

all: $(TGT)

%.O0: %.cpp
	$(CXX) $(CXXFLAGS) -O0 $< libgen_prob.a -o $@

%.O2: %.cpp
	$(CXX) $(CXXFLAGS) -O2 $< libgen_prob.a -o $@

%.O3: %.cpp
	$(CXX) $(CXXFLAGS) -O3 $< libgen_prob.a -o $@

mysearch.O0: mysearch.cpp mysearch.h gen_prob.h
	$(CXX) $(CXXFLAGS) -O0 $< libgen_prob.a -o $@

mysearch.O2: mysearch.cpp mysearch.h gen_prob.h
	$(CXX) $(CXXFLAGS) -O2 $< libgen_prob.a -o $@

mysearch.O3: mysearch.cpp mysearch.h gen_prob.h
	$(CXX) $(CXXFLAGS) -O3 $< libgen_prob.a -o $@

test.O0: test.cpp mysearch.h gen_prob.h
	$(CXX) $(CXXFLAGS) -O0 $< libgen_prob.a -o $@

test.O2: test.cpp mysearch.h gen_prob.h
	$(CXX) $(CXXFLAGS) -O2 $< libgen_prob.a -o $@

test.O3: test.cpp mysearch.h gen_prob.h
	$(CXX) $(CXXFLAGS) -O3 $< libgen_prob.a -o $@

%.O0: %.c
	$(CC) $(CFLAGS) -O0 $< libgen_prob.a -o $@

%.O2: %.c
	$(CC) $(CFLAGS) -O2 $< libgen_prob.a -o $@

%.O3: %.c
	$(CC) $(CFLAGS) -O3 $< libgen_prob.a -o $@
