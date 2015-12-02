#include <stdio.h>
#include <string.h>
#include <unistd.h>

/** boop boop boop
 *
 */
int valid_arguments(int argc, char* argv[]) {
  if (argc == 3) {
    int valid_m3u = 0;
    int valid_dest = 1;

    // check m3u
    char *result = strstr(argv[1], ".m3u");
    if (result != 0) {
      int position = result - argv[1];
      if (position + 4 == strlen(argv[1])) {
        valid_m3u = 1;
      }
    }

    // check dest

    return (valid_m3u && valid_dest);
  }
  return 0;
}


int main(int argc, char* argv[]) {
  if (valid_arguments(argc, argv) == 0) {
    printf("usage: %s <m3u file> <dest folder>\n", argv[0]);
    return 0;
  }
  return 0;
}
