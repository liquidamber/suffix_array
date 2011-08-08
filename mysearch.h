#pragma once

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <memory>
#include <vector>
#include <algorithm>

extern "C"{
#include "gen_prob.h"
}

#define $ (*this)

class SuffixArray
{
private:
  const char * ref_str;
  const size_t ref_length;
  std::vector<size_t> suffix_array;
  struct LessStr{
    const char * str;
    LessStr(const char * str) : str(str) {}
    bool operator() (size_t a, size_t b) {
      return std::strcmp(str+a, str+b) < 0;
    }
  };
  class LessStrLimited{
    const char * str;
    size_t limit;
  public:
    LessStrLimited(const char * str, size_t limit) : str(str), limit(limit) {}
    bool operator() (const char * a, const size_t b) {
      return std::strncmp(a, str+b, limit) < 0;
    }
    bool operator() (const size_t a, const char * b) {
      return std::strncmp(str+a, b, limit) < 0;
    }
  };
public:
  SuffixArray(const char * ref_str, const size_t ref_length)
    : ref_str(ref_str), ref_length(ref_length), suffix_array(ref_length) {
    for(size_t i=0; i < ref_length; ++i)
    {
      suffix_array[i] = i;
    }
    std::sort(suffix_array.begin(), suffix_array.end(), LessStr(ref_str));
#if 0
    for(size_t i=0; i < ref_length; ++i)
    {
      printf("debug: suffix array: %d: %s\n", i, suffix_array[i]);
    }
#endif
  }
  answer findall_or_max(const char * query, const size_t max_hits) {
    typedef std::vector<size_t>::iterator itr_t;
    const size_t l = std::strlen(query);
    itr_t lb = std::lower_bound(suffix_array.begin(),
                                suffix_array.end()  , query, LessStrLimited(ref_str, l));
    itr_t ub = std::upper_bound(lb,
                                suffix_array.end()  , query, LessStrLimited(ref_str, l));
    if ( !(lb < ub) )
    {
      return {0, nullptr};
    }
    int * pos = nullptr;
    const size_t ans_size = ub - lb;
    if( ans_size <= max_hits )
    {
      pos = new int[ans_size];
      size_t i;
      itr_t it;
      for(it = lb, i = 0; it != ub; ++it, ++i)
      {
        pos[i] = *it;
      }
      std::sort(pos, pos + ans_size);
    }
    return { ub - lb, pos };
  }
};
