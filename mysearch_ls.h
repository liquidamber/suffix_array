#pragma once

#include <cstdio>
#include <climits>
#include <cstdlib>
#include <cinttypes>
#include "mysearch.h"
//#define DEBUG_PRINT_IDXRNK
//#define DEBUG_PRINT_PREFIXED_RANK
//#define DEBUG_PRINT_HIGHT

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



namespace liquid
{
  class LSSuffixArray : public BaseSuffixArray
  {
    typedef signed_length_t rank_t;
    std::vector<signed_length_t> rank;
    std::vector<rank_t> hgt;
    inline rank_t get_rank(size_t idx, size_t offset=0) {
      return $.get_rank_from_stridx($.index[idx], offset);
    }
    inline rank_t get_rank_from_stridx(size_t idx, size_t offset=0) {
      return (idx + offset < ref_length) ? $.rank[idx + offset] : 0;
    }
    void dprint_index_and_rank() const {
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
    void dprint_rank_prefixed(size_t lower, size_t upper, size_t prefix_length) const {
#ifdef DEBUG_PRINT_PREFIXED_RANK
      fprintf(stderr, "debug: prefix: %zd rank[+p]:", prefix_length);
      for(size_t x=lower; x < upper; ++x)
      {
        fprintf(stderr, " %"PRIdPTR, get_rank(x, prefix_length));
      }
      fprintf(stderr, "\n"); fflush(stderr);
#endif
    }
    void dprint_hgt() const {
#ifdef DEBUG_PRINT_HIGHT
      for(size_t i=0; i < $.ref_length; ++i)
      {
        fprintf(stderr, "debug: i: %2zd hgt: %"PRIdPTR"\n", i, $.hgt[i]);
      }
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
      // if(upper - lower <= SORT_SIZE_LIMIT)
      // {
      //   std::sort($.index.begin()+lower,
      //             $.index.begin()+upper,
      //             [this, prefix_length](signed_length_t a, signed_length_t b) -> bool {
      //               return $.get_rank_from_stridx(a, prefix_length)
      //                 < $.get_rank_from_stridx(b, prefix_length);
      //             });
      //   for(size_t x = lower; x < upper; ++x)
      //   {
      //     rank_t r = get_rank(x, prefix_length);
      //     size_t y;
      //     for(y = x + 1; y < upper && get_rank(y, prefix_length) == r; ++y) ;
      //     if(1 == y - x)
      //     {
      //       $.index[x] = -1;
      //     }
      //     else
      //     {
      //       for(size_t i = x; i != y; ++i)
      //         $.rank[i] = y;
      //     }
      //   }
      //   return;
      // }
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
        // dprint_index_and_rank();
      }
      if(r < upper)
      {
        mqsort(r, upper, prefix_length);
        // fprintf(stderr, "debug: finish 3rd children call\n");
        // dprint_index_and_rank();
      }
    }
    size_t lcp(size_t i, size_t j) const {
      size_t k=0;
      for(k = 0; ; ++k)
      {
        char a = ref_str[i + k];
        char b = ref_str[j + k];
        if(a != b || a == '\0' || b == '\0')
          break;
      }
      return k;
    }
    void mk_hgt() {
      rank_t h = 0;
      $.hgt.resize($.ref_length);
      for(size_t i=0; i < $.ref_length; ++i)
      {
        if($.rank[i] == static_cast<signed_length_t>($.ref_length))
        {
          $.hgt[$.rank[i]-1] = -1;
          // fprintf(stderr,
          //         "debug: i: %2zd rank[i]-1: %2"PRIdPTR" skip\n",
          //         i, rank[i]-1);fflush(stderr);
          continue;
        }
        size_t j = $.index[$.rank[i]];
        if(h == 0)
        {
          h = $.hgt[$.rank[i]-1] = lcp(i, j);
        }
        else
        {
          h = $.hgt[$.rank[i]-1] = h-1 + lcp(i+h-1, j+h-1);
        }
        // fprintf(stderr,
        //         "debug: lcp: compare i: %2zd j: %2zd h: %2"PRIdPTR"\n",
        //         i, rank[i]-1, j, h);fflush(stderr);
      }
    }
    /**
     * Return LCP value between a & b (assume a < b)
     */
    size_t get_lcp_from_hgt(size_t a, size_t b) const {
      signed_length_t lcp = $.hgt[a];
      for(size_t i = a+1; i < b; ++i)
      {
        lcp = std::min(lcp, $.hgt[i]);
      }
      return static_cast<size_t>(lcp);
    }
  public:
    const static size_t SORT_SIZE_LIMIT = 10;
    LSSuffixArray(const char * ref_str, const size_t ref_length)
      : BaseSuffixArray(ref_str, ref_length), rank(ref_length) {
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
      // mk_hgt();
      // dprint_hgt();
    }
  };
}
