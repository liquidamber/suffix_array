/* 
 * sample.c
 */
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "gen_prob.h"

#define check(e) if (e) ; else { fprintf(stderr, "check failed: %s\n", #e); exit(1); }

void * chk_malloc(size_t sz) {
  void * a = malloc(sz);
  check(a);
  return a;
}

int find_all_or_max_hits(char * A, char * q, int * I, int max_hits_to_answer) {
  int i;
  int n = strlen(A);
  int nq = strlen(q);
  int k = 0;
  for (i = 0; i < n; i++) {
    if (strncmp(A + i, q, nq) == 0) {
      if (k < max_hits_to_answer) I[k] = i;
      k++;
    }
  }
  return k;
}

int * copy_index_array(int * I, int n) {
  int * c = (int *)chk_malloc(sizeof(int) * n);
  int i;
  for (i = 0; i < n; i++) {
    c[i] = I[i];
  }
  return c;
}

int main(int argc, char ** argv) {
  int problem_type         = (argc > 1 ? atoi(argv[1]) : 0);
  int alphabet_size        = (argc > 2 ? atoi(argv[2]) : 0);
  int reference_string_len = (argc > 3 ? atoi(argv[3]) : 0);
  int n_query_strings      = (argc > 4 ? atoi(argv[4]) : 0);
  int max_query_string_len = (argc > 5 ? atoi(argv[5]) : 0);
  /* all defaults */
  problem_t p = gen_problem(problem_type, alphabet_size, 
			    reference_string_len, n_query_strings, 
			    max_query_string_len);
  printf("%s: solving...\n", __FILE__);
  answer_t ans = (answer_t)chk_malloc(sizeof(answer) * p->n_query_strings);
  int i;
  int * I = (int *)chk_malloc(sizeof(int) * p->max_hits_to_answer);
  for (i = 0; i < p->n_query_strings; i++) {
    int n = find_all_or_max_hits(p->reference_string, p->query_strings[i], I, p->max_hits_to_answer);
    ans[i].n_hits = n;
    ans[i].hit_indices = NULL;
    if (n <= p->max_hits_to_answer) {
      ans[i].hit_indices = copy_index_array(I, n);
    }
  }
  printf("%s: done, checking answers\n", __FILE__);
  check_answer(p, ans);
  return 0;
}
