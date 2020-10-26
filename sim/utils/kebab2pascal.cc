#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int usage(const char *prog) {
  fprintf(stderr, "Usage: %s <string>\n", prog);
  return 1;
}

int main(int argc, char *argv[]) {
  if (argc != 2) return usage(argv[0]);
  char *res = (char *)malloc(strlen(argv[1]) * sizeof(char));
  if (!res) {
    perror("Could not copy the input string.");
    return 1;
  }

  int to_upper = 1;
  char *p_in = argv[1];
  char *p_out = res;
  char c;
  while((c = *p_in++) != '\0') {
    if (c == '-') {
      to_upper = 1;
      continue;
    }
    if (to_upper) {
      *p_out = toupper(c);
      to_upper = 0;
    } else *p_out = c;
    p_out++;
  }
  printf("%s\n", res);
  free(res);
}
