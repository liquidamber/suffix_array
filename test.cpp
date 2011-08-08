#include <time.h>
#include <random>
#include <iostream>
#include <algorithm>
#include <functional>
#include <boost/timer.hpp>
#include "mysearch.h"

class MyTimer
{
  double t1;
public:
  static double getCurrClocktime() {
    timespec tp;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp);
    return tp.tv_sec + static_cast<double>(tp.tv_nsec) / 1000 / 1000 / 1000;
  }
  MyTimer() : t1(getCurrClocktime()) {}
  double elapsed() {
    return getCurrClocktime() - t1;
  }
};

void test(const char * str, bool print=false)
{
  if(print) printf("#==test %s==\n", str);
  boost::timer t;
  SuffixArray SA(str, std::strlen(str));
  printf("#elapsed time: %lf\n", t.elapsed());
}

void randomtest()
{
  std::string buf;
  const size_t MAX_SIZE = 10000000;

  std::random_device rnddev;
  std::vector< std::uint_least32_t> v(10);
  std::generate(v.begin(), v.end(), std::ref(rnddev));
  std::seed_seq seed(v.begin(), v.end());

  std::mt19937 engine(seed);
  std::uniform_int_distribution<int> distribution('a', 'z');
  std::function<int()> rnd = [&]{ return distribution(engine); };

  typedef std::vector<std::pair<size_t, double> > result_t;
  typedef result_t::iterator result_itrator;
  result_t result;

  for(size_t size=2; size < MAX_SIZE; size*=2)
  {
    buf.resize(size);
    for(size_t i=0; i < size; ++i)
    {
      if(i&1) buf[i] = 'a';
      else buf[i] = rnd();
    }
    MyTimer t;
    SuffixArray SA(buf.c_str(), size);
    result.push_back(std::make_pair(size, t.elapsed()));
  }
  for(result_itrator it = result.begin(); it != result.end(); ++it)
  {
    printf("%zd %lf\n", it->first, it->second);
  }
}

int main()
{
  //test("mississippi");
  test("ababababcabcdabcdeabcdefabababacab", true);
  randomtest();
}
