CTYPEFLAGS = -g -Wall -Wextra -Winline
LDFLAGS = -lm -lrt
CFLAGS = $(CTYPEFLAGS)
CXXFLAGS = $(CTYPEFLAGS) -std=c++0x

CSRC = $(wildcard *.c)
CPPSRC = $(wildcard *.cpp)
TGT_SYS = $(addsuffix .O3, $(basename $(CSRC)))
TGT_O0 = $(addsuffix .O0, $(basename $(CPPSRC)))
TGT_O2 = $(addsuffix .O2, $(basename $(CPPSRC)))
TGT_O3 = $(addsuffix .O3, $(basename $(CPPSRC)))
MYS_HEADERS = $(wildcard mysearch*.h)

O3: $(TGT_O3) $(TGT_SYS)

O2: $(TGT_O2) $(TGT_SYS)

O0: $(TGT_O0) $(TGT_SYS)

all: O0 O2 O3

%.O0: %.cpp $(MYS_HEADERS) gen_prob.h
	$(CXX) $(CXXFLAGS) -O0 $< libgen_prob.a -o $@ $(LDFLAGS)

%.O2: %.cpp $(MYS_HEADERS) gen_prob.h
	$(CXX) $(CXXFLAGS) -O2 $< libgen_prob.a -o $@ $(LDFLAGS)

%.O3: %.cpp $(MYS_HEADERS) gen_prob.h
	$(CXX) $(CXXFLAGS) -O3 $< libgen_prob.a -o $@ $(LDFLAGS)

mysearch.O0: mysearch.cpp $(MYS_HEADERS) gen_prob.h libgen_prob.a
	$(CXX) $(CXXFLAGS) -O0 $< libgen_prob.a -o $@ $(LDFLAGS)

mysearch.O2: mysearch.cpp $(MYS_HEADERS) gen_prob.h libgen_prob.a
	$(CXX) $(CXXFLAGS) -O2 $< libgen_prob.a -o $@ $(LDFLAGS)

mysearch.O3: mysearch.cpp $(MYS_HEADERS) gen_prob.h libgen_prob.a
	$(CXX) $(CXXFLAGS) -O3 $< libgen_prob.a -o $@ $(LDFLAGS)

%.O0: %.c $(MYS_HEADERS) gen_prob.h libgen_prob.a
	$(CC) $(CFLAGS) -O0 $< libgen_prob.a -o $@ $(LDFLAGS)

%.O2: %.c $(MYS_HEADERS) gen_prob.h libgen_prob.a
	$(CC) $(CFLAGS) -O2 $< libgen_prob.a -o $@ $(LDFLAGS)

%.O3: %.c $(MYS_HEADERS) gen_prob.h libgen_prob.a
	$(CC) $(CFLAGS) -O3 $< libgen_prob.a -o $@ $(LDFLAGS)
