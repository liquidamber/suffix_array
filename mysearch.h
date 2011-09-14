#pragma once

#include <cstring>
#include <cstdint>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <iostream>

extern "C"{
#include "gen_prob.h"
}

#define $ (*this)
#define PRT(x) #x << ':' << (x) << ' '

namespace liquid
{
  template <class T>
  class BaseSuffixArray
  {
  public:
    typedef intptr_t signed_length_t;
    typedef uintptr_t unsigned_length_t;
    class LessStr {
      const char * str;
    public:
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
  protected:
    const char * ref_str;
    const size_t ref_length;
    std::vector<T> index;
  public:
    BaseSuffixArray(const char * ref_str, const size_t ref_length)
      : ref_str(ref_str), ref_length(ref_length), index(ref_length) {
    }
    virtual ~BaseSuffixArray() {}
    virtual answer findall_or_max(const char * query, const size_t max_hits) {
      const size_t l = std::strlen(query);
      auto lb = std::lower_bound(index.begin(),
                                 index.end()  , query, LessStrLimited(ref_str, l));
      auto ub = std::upper_bound(lb,
                                 index.end()  , query, LessStrLimited(ref_str, l));
      if ( !(lb < ub) )
      {
        return {0, nullptr};
      }
      int * pos = nullptr;
      const size_t ans_size = ub - lb;
      if( ans_size <= max_hits )
      {
        pos = new int[ans_size];
        size_t i = 0;
        for(auto it = lb; it != ub; ++it, ++i)
        {
          pos[i] = *it;
        }
        std::sort(pos, pos + ans_size);
      }
      return { static_cast<int>(ub - lb), pos };
    }
  };

  typedef BaseSuffixArray<BaseSuffixArray<int>::signed_length_t> PortableBaseSuffixArray;
}
