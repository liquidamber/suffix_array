#pragma once

#include <time.h>
#include <cstring>
#include <boost/preprocessor.hpp>

#include "mysearch_naive.h"
#include "mysearch_ls.h"
#include "mysearch_is.h"
#include "mysearch_isorg.h"

#define IMPL_SEQ (Naive)(LS)(ISorg)(IS)

#define PRINT_IMPL(r, data, elem) std::cerr << data << BOOST_PP_STRINGIZE(elem) << "\n";
#define PRINT_ALL_IMPL(prefix) BOOST_PP_SEQ_FOR_EACH(PRINT_IMPL, prefix, IMPL_SEQ)

#define CHECK_TYPE(r, macro, impl)                                      \
  else if(std::strcmp(implementation, BOOST_PP_STRINGIZE(impl)) == 0) { \
    macro(impl);                                                        \
  }

#define FAIL_PRINT(message) std::cerr << message << "\n"; PRINT_ALL_IMPL("    "); return 1;

#define LIST_TYPE(macro)                            \
  if(false) {}                                      \
  BOOST_PP_SEQ_FOR_EACH(CHECK_TYPE,macro,IMPL_SEQ)  \
  else{ FAIL_PRINT("Incorrect impl.") }

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))

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
