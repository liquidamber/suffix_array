#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <memory>

#define $ (*this)

extern "C"{
#include "gen_prob.h"
}

class SuffixArray
{
private:
  const char * reference;
  const size_t ref_length;
  std::vector<const char *> suffix_array;
  struct LessStr{
    bool operator() (const char * a, const char * b) {
      return std::strcmp(a, b) < 0;
    }
  };
  class LessStrLimited{
    size_t limit;
  public:
    LessStrLimited(size_t limit) : limit(limit) {}
    bool operator() (const char * a, const char * b) {
      return std::strncmp(a, b, limit) < 0;
    }
  };
public:
  SuffixArray(const char * reference, const size_t ref_length)
    : reference(reference), ref_length(ref_length), suffix_array(ref_length) {
    for(size_t i=0; i < ref_length; ++i)
    {
      suffix_array[i] = reference+i;
    }
    std::sort(suffix_array.begin(), suffix_array.end(), LessStr());
#if 0
    for(size_t i=0; i < ref_length; ++i)
    {
      printf("debug: suffix array: %d: %s\n", i, suffix_array[i]);
    }
#endif
  }
  answer findall_or_max(const char * query, const size_t max_hits) {
    typedef std::vector<const char *>::iterator itr_t;
    const size_t l = std::strlen(query);
    itr_t lb = std::lower_bound(suffix_array.begin(),
                                suffix_array.end()  , query, LessStrLimited(l));
    itr_t ub = std::upper_bound(lb,
                                suffix_array.end()  , query, LessStrLimited(l));
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
        pos[i] = *it - $.reference;
      }
      std::sort(pos, pos + ans_size);
    }
    return { ub - lb, pos };
  }
};

int main(int argc, char ** argv) {
  const int problem_type         = (argc > 1 ? atoi(argv[1]) : 0);
  const int alphabet_size        = (argc > 2 ? atoi(argv[2]) : 0);
  const int reference_string_len = (argc > 3 ? atoi(argv[3]) : 0);
  const int n_query_strings      = (argc > 4 ? atoi(argv[4]) : 0);
  const int max_query_string_len = (argc > 5 ? atoi(argv[5]) : 0);
  /* all defaults */
  problem_t p = gen_problem(problem_type, alphabet_size, 
			    reference_string_len, n_query_strings, 
			    max_query_string_len);
  printf("%s: solving...\n", __FILE__);
  std::vector<answer> ans(p->n_query_strings);
  std::vector<int> I(p->max_hits_to_answer);
  std::unique_ptr<SuffixArray> SA(new SuffixArray(p->reference_string,
                                                  p->reference_string_len));
  for(int i=0; i < p->n_query_strings; i++)
  {
    ans[i] = SA->findall_or_max(p->query_strings[i],
                                static_cast<size_t>(p->max_hits_to_answer));
  }
  printf("%s: done, checking answers\n", __FILE__);
  check_answer(p, &ans[0]);
#if 0
  answer_t truthans = tell_me_answer(p);
#endif
  for(int i=0; i < p->n_query_strings; i++)
  {
#if 0
    printf("debug: true #%d: num: %d\n", i, truthans[i].n_hits);
    if(truthans[i].n_hits <= p->max_query_string_len)
    {
      for(int j=0; j < truthans[i].n_hits; ++j)
      {
        printf("debug: true #%d: %d: %d\n", i, j, truthans[i].hit_indices[j]);
      }
    }
    printf("debug: my   #%d: num: %d\n", i, ans[i].n_hits);
    if(ans[i].n_hits <= p->max_query_string_len)
    {
      for(int j=0; j < ans[i].n_hits; ++j)
      {
        printf("debug: my   #%d: %d: %d\n", i, j, ans[i].hit_indices[j]);
      }
    }
#endif
    delete[] ans[i].hit_indices;
  }
  return 0;
}
