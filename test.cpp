#include "mysearch.h"

void test(const char * str)
{
  printf("==test %s==\n", str);
  SuffixArray SA(str, std::strlen(str));
}

int main()
{
  //test("mississippi");
  test("ababababcabcdabcdeabcdefabababacab");
}
