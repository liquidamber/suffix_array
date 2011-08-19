#pragma once

#include <cstdio>
#include <cstring>
#include <climits>
#include <iostream>
#include <functional>
#include "mysearch.h"

//#define DEBUG

namespace
{
  size_t cwidth(intptr_t x)
  {
    size_t w = 0;
    if(x < 0){
      ++w;
      x = -x;
    }
    w += x / 10 + 1;
    return w;
  }

  template <class T>
  inline void setMinusOne(T & vec)
  {
    std::memset(&vec[0], -1, vec.size()*sizeof(vec[0]));
    // std::fill(vec.begin(), vec.end(), -1);
  }

  template <class Iterator>
  inline void setMinusOne(Iterator begin, Iterator end)
  {
    std::memset(&begin[0], -1, (end - begin) * sizeof(*begin));
    // std::fill(begin, end, -1);
  }

  template <class T>
  inline void setZero(T & vec)
  {
    std::memset(&vec[0], 0, vec.size()*sizeof(vec[0]));
    // std::fill(vec.begin(), vec.end(), 0);
  }
}

namespace liquid
{
  namespace SAIS
  {
    inline bool isLMSindex(const std::vector<bool> & type, size_t i)
    {
      return i != 0 && type[i] && !type[i-1];
    }
    typedef BaseSuffixArray::signed_length_t signed_length_t;
    typedef BaseSuffixArray::unsigned_length_t unsigned_length_t;
    template <class Iterator>
    void getBuckets(std::vector<unsigned_length_t> & bkt,
                    Iterator begin,
                    Iterator end,
                    bool is_get_end) {
      setZero(bkt);
      for(Iterator it = begin; it != end; ++it)
      {
        ++bkt[static_cast<size_t>(*it)];
      }
      unsigned_length_t sum = 0;
      for(auto it = bkt.begin(); it != bkt.end(); ++it)
      {
        sum += *it;
        *it = is_get_end ? sum : sum - *it;
      }
    }
    template <class Iterator>
    void induceSAl(Iterator begin,
                   Iterator end,
                   const std::vector<bool> & type,
                   std::vector<signed_length_t> & SA,
                   std::vector<unsigned_length_t> & bkt) {
      const size_t n = end - begin;
#ifdef DEBUG
      std::cerr << "debug: ==induceSAl: begin\n";
      std::cerr << "debug: induce L: " << PRT(n) << "\n";
#endif
      getBuckets(bkt, begin, end, false);
      for(size_t i=0; i < n; ++i)
      {
        size_t j = SA[i] - 1;
#ifdef OLD_DEBUG
        std::cerr << "debug: induce L: try: " << PRT(i) << PRT(SA[i]) << PRT(j) << "\n";
#endif
        if(SA[i] > 0 && !type[j])
        {
#ifdef DEBUG
          std::cerr << "debug: induce L: i:" << i << " j:" << j
                    << " char(j):" << begin[j]
                    << " bucket[char(j)]:" << bkt[begin[j]] << std::endl;
#endif
          SA[bkt[begin[j]]++] = j;
        }
      }
    }
    template <class Iterator>
    void induceSAs(Iterator begin,
                   Iterator end,
                   const std::vector<bool> & type,
                   std::vector<signed_length_t> & SA,
                   std::vector<unsigned_length_t> & bkt) {
      const size_t n = end - begin;
      getBuckets(bkt, begin, end, true);
      for(size_t i=n; i != 0; )
      {
        --i;
        size_t j = SA[i] - 1;
        if(SA[i] != 0 && type[j])
        {
#ifdef DEBUG
          std::cerr << "debug: induce S: i:" << i << " j:" << j
                    << " char(j):" << begin[j]
                    << " bucket[char(j)]:" << bkt[begin[j]] << std::endl;
#endif
          SA[--bkt[begin[j]]] = j;
        }
      }
    }
    template <class Iterator>
    void SA_IS(Iterator begin,
               Iterator end,
               std::vector<signed_length_t> & SA,
               size_t alnum) {
      // length of string.
      const size_t n = end - begin;
      // type array: true if S, false if L.
      std::vector<bool> type(n);
      type[n-2] = false;
      type[n-1] = true;
      for(size_t i = n-2; i != 0; --i)
      {
        type[i-1] = ((begin[i-1] < begin[i]) ||
                     (begin[i-1] == begin[i] && type[i]));
      }

#ifdef DEBUG
      std::cerr << "debug: ==SA_IS begin\n"
                << "debug: alnum: " << alnum << "\n";
      std::cerr << "debug: str:  ";
      for(size_t i=0; i != n; ++i)
      {
        if(sizeof(*begin) != sizeof(char)) std::cerr << ' ';
        std::cerr << begin[i];
      }
      std::cerr << std::endl;
      std::cerr << "debug: type: ";
      for(size_t i=0; i < n; ++i)
      {
        std::cerr << std::setw(sizeof(*begin) == sizeof(char) ? 1 : 1 + cwidth(begin[i]))
                  << std::right << (type[i] ? 'S' : 'L');
      }
      std::cerr << std::endl;
#endif

      using namespace std::placeholders;
      std::function<bool (size_t)> isLMS = std::bind(&isLMSindex, type, _1);
#ifdef DEBUG
      std::cerr << "debug: ==begin stage 1." << std::endl;
#endif
      // stage 1: reduce problem.
      {
        std::vector<unsigned_length_t> bkt(alnum, 0);
        getBuckets(bkt, begin, end, true);
        setMinusOne(SA.begin(), SA.begin() + n);
        for(size_t i = 1; i<n; ++i)
        {
          if(isLMS(i)) SA[--bkt[begin[i]]] = i;
        }
        induceSAl(begin, end, type, SA, bkt);
        induceSAs(begin, end, type, SA, bkt);
      }
#ifdef DEBUG
      std::cerr << "debug: ==reduced problem." << std::endl;
      for(size_t i=0; i < n; ++i)
      {
        std::cerr << "debug: i: " << i << " SA[i]: " << SA[i] << std::endl;
      }
#endif

      // compact all the sorted LMS-substrings.
      size_t n1 = 0;
      for(size_t i=0; i < n; ++i)
      {
        if(isLMS(SA[i])) SA[n1++] = SA[i];
      }

      // find lexicographic names of all LMS-substrings.
      setMinusOne(SA.begin() + n1, SA.begin() + n);
      size_t name=0, prev=0;
      bool visited = false;
      for(size_t i=0; i < n1; ++i) {
        signed_length_t pos = SA[i];
        bool diff = false;
        for(size_t d = 0; d < n; ++d)
        {
          if(!visited ||
             begin[pos+d] != begin[prev+d] ||
             type[pos+d]  !=  type[prev+d])
          {
            diff = true;
            break;
          }
          else if (d > 0 && (isLMS(pos+d) || isLMS(prev+d)))
          {
            break;
          }
        }
        if(diff) { ++name; }
        prev = pos; visited = true;
        pos = (pos - pos % 2) / 2;
        SA[n1+pos] = name - 1;
      }
      for(size_t i = n - 1, j = n; i >= n1; --i)
      {
        if(SA[i] >= 0) SA[--j] = SA[i];
      }

#ifdef DEBUG
      std::cerr << "debug: ==before stage 2" << std::endl;
      std::cerr << "debug: n: " << n << std::endl;
      std::cerr << "debug: n1: " << n1 << std::endl;
      std::cerr << "debug: name: " << name << std::endl;
      for(size_t i=0; i < n; ++i)
      {
        std::cerr << "debug: i: " << i << " SA[i]: " << SA[i] << std::endl;
      }
#endif

      // stage 2: solve the reduced problem.
      std::vector<signed_length_t>::iterator s1 = SA.begin() + n - n1;
      std::vector<signed_length_t>::iterator s1end = SA.begin() + n;
      if(name < n1)
      {
        SA_IS(s1, s1end, SA, name);
      }
      else
      {
        for(size_t i=0; i<n1; ++i) SA[s1[i]] = i;
      }

#ifdef DEBUG
      std::cerr << "debug: ==before stage 3" << std::endl;
      for(size_t i=0; i < n; ++i)
      {
        std::cerr << "debug: i: " << i << " SA[i]: " << SA[i] << std::endl;
      }
#endif

      // stage 3: induce the result for original
      {
        std::vector<unsigned_length_t> bkt(alnum, 0);
        getBuckets(bkt, begin, end, true); // find ends of buckets
        for(size_t i=1, j=0; i < n; ++i)
        {
          if(isLMS(i)){ s1[j++] = i; }
        }
        for(size_t i=0; i < n1; ++i){ SA[i] = s1[SA[i]]; }
        setMinusOne(SA.begin() + n1, SA.begin() + n);
        for(size_t i=n1; i != 0; ) {
          --i;
          size_t j=SA[i];
          SA[i] = -1;
          SA[--bkt[begin[j]]] = j;
        }
#ifdef DEBUG
        std::cerr << "debug: ==before induce L & S" << std::endl;
        for(size_t i=0; i < n; ++i)
        {
          std::cerr << "debug: i: " << i << " SA[i]: " << SA[i] << std::endl;
        }
#endif
#ifdef DEBUG
        std::cerr << "debug: s1:";
        for(size_t i=0; i < n; ++i)
        {
          std::cerr << begin[i];
        }
        std::cerr << "\n";
#endif
        induceSAl(begin, end, type, SA, bkt);
        induceSAs(begin, end, type, SA, bkt);
      }
    }
  }

  class ISSuffixArray : public BaseSuffixArray
  {
  public:
    ISSuffixArray(const char * ref_str, const size_t ref_length)
      : BaseSuffixArray(ref_str, ref_length) {
      const size_t K = UCHAR_MAX + 1;
      SAIS::SA_IS(ref_str, ref_str + ref_length, index, K);
#ifdef DEBUG
      for(size_t i=0; i < ref_length; ++i)
      {
        printf("debug: index[%2d]: %2d: %s\n", i, index[i], ref_str + index[i]);
      }
#endif
    }
  };
}
