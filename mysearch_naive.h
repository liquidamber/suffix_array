#pragma once

#include <cstdio>
#include <algorithm>
#include "mysearch.h"

namespace liquid
{
  class NaiveSuffixArray : public PortableBaseSuffixArray
  {
  public:
    NaiveSuffixArray(const char * ref_str, const size_t ref_length)
      : PortableBaseSuffixArray(ref_str, ref_length) {
      for(size_t i=0; i < ref_length; ++i)
      {
        $.index[i] = i;
      }
      std::sort($.index.begin(), $.index.end(), LessStr(ref_str));
#if 0
      for(size_t i=0; i < ref_length; ++i)
      {
        printf("debug: suffix array: %d: %s\n", i, suffix_array[i]);
      }
#endif
    }
  };
}
