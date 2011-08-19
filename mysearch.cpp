#include <memory>
#include "mysearch_naive.h"
#include "mysearch_is.h"

template <class T>
void calc_problem(int problem_type,
                  int alphabet_size,
                  int reference_string_len,
                  int n_query_strings,
                  int max_query_string_len)
{
  problem_t p = gen_problem(problem_type,
                            alphabet_size,
                            reference_string_len,
                            n_query_strings,
                            max_query_string_len);
  printf("%s: solving...\n", __FILE__);
  std::vector<answer> ans(p->n_query_strings);
  std::vector<int> I(p->max_hits_to_answer);
  T SA(p->reference_string,
       p->reference_string_len+1);
  for(int i=0; i < p->n_query_strings; i++)
  {
    ans[i] = SA.findall_or_max(p->query_strings[i],
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
}

int main(int argc, char ** argv) {
  using namespace liquid;
  const int problem_type         = (argc > 1 ? atoi(argv[1]) : 0);
  const int alphabet_size        = (argc > 2 ? atoi(argv[2]) : 0);
  const int reference_string_len = (argc > 3 ? atoi(argv[3]) : 0);
  const int n_query_strings      = (argc > 4 ? atoi(argv[4]) : 0);
  const int max_query_string_len = (argc > 5 ? atoi(argv[5]) : 0);
  /* all defaults */
  calc_problem<NaiveSuffixArray>(problem_type,
                                 alphabet_size,
                                 reference_string_len,
                                 n_query_strings,
                                 max_query_string_len);
  return 0;
}
