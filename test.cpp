#include <random>
#include <algorithm>
#include <functional>
#include <boost/progress.hpp>
#include "mysearch.h"

void test(const char * str, bool print=false)
{
  if(print) printf("==test %s==\n", str);
  boost::progress_timer t;
  SuffixArray SA(str, std::strlen(str));
}

int main()
{
  //test("mississippi");
  test("ababababcabcdabcdeabcdefabababacab", true);
  std::string buf;
  const size_t SIZE = 10000000;
  buf.resize(SIZE);

  std::random_device rnddev;
  std::vector< std::uint_least32_t> v(10);
  std::generate(v.begin(), v.end(), std::ref(rnddev));
  std::seed_seq seed(v.begin(), v.end());

  std::mt19937 engine(seed);
  std::uniform_int_distribution<int> distribution('a', 'z');
  std::function<int()> rnd = [&]{ return distribution(engine); };

  for(size_t i=0; i < SIZE; ++i)
  {
    if(i&1) buf[i] = 'a';
    else buf[i] = rnd();
  }
  test(buf.c_str());
}
