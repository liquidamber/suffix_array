#include "myutil.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace liquid;

template <class T>
void createTimeList(const std::vector<char> & buf)
{
  MyTimer mytimer;
  T SAIS(&buf[0], buf.size());
  std::cout << mytimer.elapsed() << "\n";
}

#define CREATE_TIME_TEST(r, data, impl)         \
  do {                                          \
    std::cout << BOOST_PP_STRINGIZE(impl);      \
    createTimeList<BOOST_PP_CAT(impl, SuffixArray)>(buf);    \
  } while(0);

#define CREATE_TIME_TEST_ALL BOOST_PP_SEQ_FOR_EACH(CREATE_TIME_TEST, 0, IMPL_SEQ)

int main(int argc, char ** argv)
{
  if(argc < 2)
  {
    std::cerr << "Specify files.\n";
    return 1;
  }
  std::ifstream ifs(argv[1]);
  if(!ifs)
  {
    std::cerr << "File read error\n";
    return 1;
  }
  std::ifstream::pos_type len;
  ifs.seekg (0, std::ios::end);
  len = ifs.tellg ();
  ifs.seekg (0, std::ios::beg);
  std::vector<char> buf(len);
  ifs.read(&buf[0], len);
  buf.push_back('\0');
  CREATE_TIME_TEST_ALL;
  return 0;
}
