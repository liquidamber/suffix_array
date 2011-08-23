#include <cstdio>
#include <iostream>
#include <boost/timer.hpp>
#include "myutil.h"

template <class T>
void test(const char * str)
{
  printf("#==test %s==\n", str);
  boost::timer t;
  T SA(str, std::strlen(str) + 1);
  printf("#elapsed time: %lf\n", t.elapsed());
}

template <class T>
void findbug()
{
  printf("#find bug\n");
  std::string str;
  str.resize(6);
  size_t i=0;
  bool first = true;
  while(first || i)
  {
    first = false;
    size_t j, k = i;
    j = k % 26; k = k / 26; str[0] = j + 'a';
    j = k % 26; k = k / 26; str[1] = j + 'a';
    j = k % 26; k = k / 26; str[2] = j + 'a';
    j = k % 26; k = k / 26; str[3] = j + 'a';
    j = k % 26; k = k / 26; str[4] = j + 'a';
    j = k % 26; k = k / 26; str[5] = j + 'a';
    printf("%s\n", str.c_str());
    T SA(str.c_str(), 7);
    ++i;
  }
}

const char * probs[] = {
  "yakafaqafaqafana",
  "mississippi",
  "ababababcabcdabcdeabcdefabababacab",
};

#define SIMPLE_TEST(r,data,impl) std::cout << "#" BOOST_PP_STRINGIZE(impl) "\n"; for(size_t i=0; i < LENGTH(data); ++i) { test<BOOST_PP_CAT(impl,SuffixArray)>(data[i]); }

int main(int argc, char ** argv)
{
  using namespace liquid;
  BOOST_PP_SEQ_FOR_EACH(SIMPLE_TEST,probs,IMPL_SEQ)
  // findbug<ISSuffixArray>();
}
