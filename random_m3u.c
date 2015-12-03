#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define AVAILABLE_CHARACTERS "abcdefghikjlmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/ "


int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("usage: %s <output m3u file>\n", argv[0]);
    return 0;
  }

  srand(time(0));
  int char_range[] = {10,200};
  int num_paths = 50;

  char buffer[char_range[1]+6];
  FILE *output = fopen(argv[1], "wb");

  fputs("#EXTM3U\n", output);
  for (int i=0; i<num_paths; i++) {
    int random_length = (rand() % (char_range[1] - char_range[0] + 1)) + char_range[0];
    buffer[0] = AVAILABLE_CHARACTERS[rand() % strlen(AVAILABLE_CHARACTERS)-1];
    for (int j=1; j<random_length; j++) {
      buffer[j] = AVAILABLE_CHARACTERS[rand() % strlen(AVAILABLE_CHARACTERS)];
    }
    buffer[random_length] = '.';
    buffer[random_length+1] = 'm';
    buffer[random_length+2] = '3';
    buffer[random_length+3] = 'u';
    buffer[random_length+4] = '\n';
    buffer[random_length+5] = '\0';
    fputs("#EXTINF\n", output);
    fputs(buffer, output);
  }

  fclose(output);
  return 0;
}
