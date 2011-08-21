#include <memory>
#include <iostream>
#include "mysearch_naive.h"
#include "mysearch_ls.h"
#include "mysearch_is.h"
#include "mysearch_isorg.h"

template <class T>
void calc_problem(int problem_type,
                  int alphabet_size,
                  int reference_string_len,
                  int n_query_strings,
                  int max_query_string_len)
{
  printf("%s: solve problem %d\n", __FILE__, problem_type);
  problem_t p = gen_problem(problem_type,
                            alphabet_size,
                            reference_string_len,
                            n_query_strings,
                            max_query_string_len);
  std::vector<answer> ans(p->n_query_strings);
  std::vector<int> I(p->max_hits_to_answer);
  T SA(p->reference_string,
       p->reference_string_len+1);
  for(int i=0; i < p->n_query_strings; i++)
  {
    ans[i] = SA.findall_or_max(p->query_strings[i],
                               static_cast<size_t>(p->max_hits_to_answer));
  }
  check_answer(p, &ans[0]);
  printf("%s: done\n", __FILE__);
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
}

template <class T>
void finaltest()
{
  calc_problem<T>(100, 0, 0, 0, 0);
  calc_problem<T>(101, 0, 0, 0, 0);
  calc_problem<T>(102, 0, 0, 0, 0);
  calc_problem<T>(103, 0, 0, 0, 0);
  calc_problem<T>(110, 0, 0, 0, 0);
  calc_problem<T>(111, 0, 0, 0, 0);
  calc_problem<T>(112, 0, 0, 0, 0);
  calc_problem<T>(113, 0, 0, 0, 0);
}

#define CHECK_TYPE(x)                                       \
  else if(std::strcmp(implementation, #x) == 0) {           \
    calc_problem<x ## SuffixArray>(problem_type,            \
                                   alphabet_size,           \
                                   reference_string_len,    \
                                   n_query_strings,         \
                                   max_query_string_len);   \
  }

int main(int argc, char ** argv) {
  using namespace liquid;
  if(argc <= 1)
  {
    std::cerr << "Specify the implementation\n";
    return 1;
  }
  const char * implementation    = argv[1];
  const int problem_type         = (argc > 2 ? atoi(argv[2]) : 0);
  const int alphabet_size        = (argc > 3 ? atoi(argv[3]) : 0);
  const int reference_string_len = (argc > 4 ? atoi(argv[4]) : 0);
  const int n_query_strings      = (argc > 5 ? atoi(argv[5]) : 0);
  const int max_query_string_len = (argc > 6 ? atoi(argv[6]) : 0);
  /* all defaults */
  if(false) { }
  CHECK_TYPE(Naive)
  CHECK_TYPE(LS)
  CHECK_TYPE(IS)
  CHECK_TYPE(ISorg)
  else{ std::cerr << "Incorrect implementation type\n"; return 1; }
  return 0;
}
