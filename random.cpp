#include <cstdio>
#include <iostream>
#include <random>
#include <algorithm>
#include <functional>
#include "myutil.h"

template <class T>
void randomtest(const size_t MAX_SIZE = 50000000)
{
  std::string buf;

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
      else if(i % 3 == 0) buf[i] = 'b';
      else if(i % 5 == 0) buf[i] = 'c';
      else if(i % 7 == 0) buf[i] = 'd';
      else if(i % 11 == 0) buf[i] = 'e';
      else buf[i] = rnd();
    }
    MyTimer t;
    T SA(buf.c_str(), size + 1);
    printf("%zd %lf\n", size, t.elapsed());
  }
}

#define RND_TEST(x) (randomtest<BOOST_PP_CAT(x,SuffixArray)>())

int main(int argc, char ** argv)
{
  using namespace liquid;
  if(argc <= 1)
  {
    std::cerr << "Specify the implementation\n";
    return 1;
  }
  const char * implementation = argv[1];
  LIST_TYPE(RND_TEST)
}
