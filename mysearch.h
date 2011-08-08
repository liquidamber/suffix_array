#pragma once

#include <cinttypes>
#include <cstdio>
#include <cstring>
#include <climits>
#include <cstdlib>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

extern "C"{
#include "gen_prob.h"
}

#define $ (*this)

//#define DEBUG_PRINT_IDXRNK
//#define DEBUG_PRINT_PREFIXED_RANK

namespace
{
  char mkspace(char c)
  {
    return std::isalnum(c) ? c : '-';
  }
  template<class T>
  T select_middle(T c1, T c2, T c3) {
    return c1 < c2
      ? (c2 < c3 ? c2 : std::max(c1, c3))
      : (c3 < c2 ? c2 : std::min(c1, c3));
  }
}

class SuffixArray
{
private:
  typedef intptr_t signed_length_t;
  typedef uintptr_t unsigned_length_t;
  typedef signed_length_t rank_t;
  const char * ref_str;
  const size_t ref_length;
  std::vector<signed_length_t> index, rank;
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
  rank_t get_rank(size_t idx, size_t offset=0) {
    return ($.index[idx] + offset < ref_length) ? $.rank[$.index[idx] + offset] : 0;
  }
  void dprint_index_and_rank() {
#ifdef DEBUG_PRINT_IDXRNK
    for(size_t i=0; i < $.ref_length; ++i)
    {
      fprintf(stderr,
              "debug: i: %2zd index: %2"PRIdPTR" rank: %2"PRIdPTR" suffix: %s\n",
              i, $.index[i], ($.index[i] >= 0) ? $.rank[$.index[i]] : -1,
              ($.index[i] >= 0) ? $.ref_str + $.index[i] : nullptr);
    }
#endif
  }
  void dprint_rank_prefixed(size_t lower, size_t upper, size_t prefix_length) {
#ifdef DEBUG_PRINT_PREFIXED_RANK
    fprintf(stderr, "debug: prefix: %zd rank[+p]:", prefix_length);
    for(size_t x=lower; x < upper; ++x)
    {
      fprintf(stderr, " %"PRIdPTR, get_rank(x, prefix_length));
    }
    fprintf(stderr, "\n"); fflush(stderr);
#endif
  }
  void init_index_and_rank() {
    typedef unsigned short dchar_t;
    const size_t COUNT_SIZE = (UCHAR_MAX+1)*(UCHAR_MAX+1);
    std::vector<unsigned_length_t> count(COUNT_SIZE, 0);
    std::vector<unsigned_length_t> count_sum(COUNT_SIZE + 1);
    for(size_t i=0; i < $.ref_length; ++i)
    {
      dchar_t c = ($.ref_str[i] << CHAR_BIT) + $.ref_str[i+1];
      ++count[c];
    }
    count_sum[0] = 0;
    for(size_t i=0; i < COUNT_SIZE; ++i)
    {
      count_sum[i+1] = count_sum[i] + count[i];
      count[i] = count_sum[i+1];
    }
    for(size_t i=0; i < $.ref_length; ++i)
    {
      dchar_t c = ($.ref_str[i] << CHAR_BIT) + $.ref_str[i+1];
      $.index[--count[c]] = i;
      $.rank[i] = count_sum[c+1];
    }
  }
  void mqsort(const size_t lower, const size_t upper, const size_t prefix_length) {
    size_t l = lower, r = upper - 1;
    rank_t pivot = select_middle(get_rank(l,       prefix_length),
                                 get_rank((l+r)/2, prefix_length),
                                 get_rank(r,       prefix_length));
    // fprintf(stderr, "debug: lower: %zd upper: %zd pivot: %"PRIdPTR"\n", l, r+1, pivot);
    dprint_rank_prefixed(lower, upper, prefix_length);
    size_t i = l;
    size_t j = r;
    while(true)
    {
      while(i <= j)
      {
        rank_t k = get_rank(i, prefix_length);
        if(k > pivot) break;
        if(k == pivot)
        {
          std::swap($.index[i], $.index[l]);
          ++l;
        }
        ++i;
      }
      while(i <= j)
      {
        rank_t k = get_rank(j, prefix_length);
        if(k < pivot) break;
        if(k == pivot)
        {
          std::swap($.index[j], $.index[r]);
          --r;
        }
        --j;
      }
      if(i > j) break;
      std::swap($.index[i], $.index[j]);
      ++i;
      --j;
    }
    dprint_rank_prefixed(lower, upper, prefix_length);
    size_t lower_move = std::min(l - lower, i - l);
    for(size_t k=0; k < lower_move; ++k)
    {
      std::swap($.index[lower+k], $.index[i-k-1]);
    }
    size_t upper_move = std::min(upper - r - 1, r - j);
    for(size_t k=0; k < upper_move; ++k)
    {
      std::swap($.index[j + k + 1], $.index[upper - k - 1]);
    }
    dprint_rank_prefixed(lower, upper, prefix_length);
    l = lower + (i - l);
    r = upper - (r - j);
    // [lower, l) , [l, r), [r, upper)
    // fprintf(stderr, "debug: sorted range: lower: %zd l: %zd r: %zd upper: %zd\n",
    //         lower, l, r, upper);
    if(lower < l)
    {
      mqsort(lower, l, prefix_length);
      // fprintf(stderr, "debug: finish 1st children call\n");
      // dprint_index_and_rank();
    }
    if(l < r)
    {
      if(1 == r - l)
      {
        // const signed_length_t idx_l = $.index[l];
        $.rank[$.index[l]] = r;
        $.index[l] = -1;
        // fprintf(stderr,
        //         "debug: finish 2nd: reset rank of pivot: l: %zd idx: %zd rank:%zd\n",
        //         l, idx_l, $.rank[idx_l]);
      }
      else
      {
        for(size_t x=l; x<r; ++x)
        {
          $.rank[$.index[x]] = r;
        }
        // fprintf(stderr, "debug: finish 2nd: reset rank of pivot: l: %zd r: %zd\n", l, r);
      }
      dprint_index_and_rank();
    }
    if(r < upper)
    {
      mqsort(r, upper, prefix_length);
      // fprintf(stderr, "debug: finish 3rd children call\n");
      dprint_index_and_rank();
    }
  }
public:
  SuffixArray(const char * ref_str, const size_t ref_length)
    : ref_str(ref_str), ref_length(ref_length),
      index(ref_length), rank(ref_length) {
    init_index_and_rank();
    dprint_index_and_rank();
    size_t n = 2;
    while(n < $.ref_length)
    {
      size_t lower = 0;
      bool flag_sortend = true;
      while(lower < $.ref_length)
      {
        size_t sortend_upper = lower;
        // find index[i] < 0 and skip
        while(sortend_upper < $.ref_length && $.index[sortend_upper] < 0)
          sortend_upper += -$.index[sortend_upper];
        // set skip index
        if(lower < sortend_upper)
        {
          $.index[lower] = -(sortend_upper - lower);
          lower = sortend_upper;
        }
        // sort range
        if(lower < $.ref_length)
        {
          size_t upper = $.rank[$.index[lower]];
          mqsort(lower, upper, n);
          lower = upper;
          flag_sortend = false;
        }
      }
      if(flag_sortend) break;
      n *= 2;
    }
    for(size_t i=0; i < $.ref_length; ++i)
    {
      $.index[$.rank[i]-1] = i;
    }
    dprint_index_and_rank();
  }
  answer findall_or_max(const char * query, const size_t max_hits) {
    typedef std::vector<signed_length_t>::iterator itr_t;
    const size_t l = std::strlen(query);
    itr_t lb = std::lower_bound(index.begin(),
                                index.end()  , query, LessStrLimited(ref_str, l));
    itr_t ub = std::upper_bound(lb,
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
