CTYPEFLAGS = -g -Wall -Wextra -lm -Winline -lrt
CFLAGS = $(CTYPEFLAGS)
CXXFLAGS = $(CTYPEFLAGS) -std=c++0x -Dnullptr=0

CSRC = $(wildcard *.c)
CPPSRC = $(wildcard *.cpp)
TGT_SYS = $(addsuffix .O3, $(basename $(CSRC)))
TGT_O0 = $(addsuffix .O0, $(basename $(CPPSRC)))
TGT_O2 = $(addsuffix .O2, $(basename $(CPPSRC)))
TGT_O3 = $(addsuffix .O3, $(basename $(CPPSRC)))

O3: $(TGT_O3) $(TGT_SYS)

O2: $(TGT_O2) $(TGT_SYS)

O0: $(TGT_O0) $(TGT_SYS)

all: O0 O2 O3

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
