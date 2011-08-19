#include <time.h>
#include <random>
#include <iostream>
#include <algorithm>
#include <functional>
#include <boost/timer.hpp>
#include "mysearch_naive.h"

//#define FIND_BUG_MODE
constexpr double FIND_BUG_MODE_SIZE = 8;

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

template <class T>
void test(const char * str)
{
  printf("#==test %s==\n", str);
  boost::timer t;
  T SA(str, std::strlen(str) + 1);
  printf("#elapsed time: %lf\n", t.elapsed());
}

template <class T>
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
#ifdef FIND_BUG_MODE
    size = FIND_BUG_MODE_SIZE;
#endif
    buf.resize(size);
    for(size_t i=0; i < size; ++i)
    {
      if(false && i&1) buf[i] = 'a';
      else buf[i] = rnd();
    }
    MyTimer t;
#ifdef FIND_BUG_MODE
    std::cerr << buf << "\n";
#endif
    T SA(buf.c_str(), size + 1);
    result.push_back(std::make_pair(size, t.elapsed()));
  }
  for(result_itrator it = result.begin(); it != result.end(); ++it)
  {
    printf("%zd %lf\n", it->first, it->second);
  }
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

int main()
{
  using namespace liquid;
  test<NaiveSuffixArray>("yakafaqafaqafana");
  test<NaiveSuffixArray>("mississippi");
  test<NaiveSuffixArray>("ababababcabcdabcdeabcdefabababacab");
  //findbug<NaiveSuffixArray>();
  printf("#random test\n");
  randomtest<NaiveSuffixArray>();
}
