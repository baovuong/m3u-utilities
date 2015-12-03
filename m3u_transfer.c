#include <stdio.h>    // printf
#include <stdlib.h>   // malloc, realloc, NULL
#include <string.h>   // strlen, strstr
#include <unistd.h>   // exec
#include <sys/stat.h> // S_ISDIR,




void path_friendly(char** output, const char* text) {

  // count number of spaces
  int num_spaces = 0;
  char *pch = strchr(text, ' ');
  while (pch != NULL) {
    num_spaces++;
    pch = strchr(pch+1, ' ');
  }

  char* o = (char*)malloc(sizeof(char) * (strlen(text) + num_spaces + 1));
  int j = 0;
  for (int i=0; i<strlen(text); i++) {
    if (text[i] == ' ') {
      o[j] = '\\';
      j++;
    }
    o[j] = text[i];
    j++;
  }
  o[j] = '\0';
  *output = o;

}

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
    int status;
    struct stat st_buf;
    status = lstat(argv[2], &st_buf);
    if (status != 0) {
      return 0;
    }
    valid_dest = S_ISDIR(st_buf.st_mode);


    return (valid_m3u && valid_dest);
  }
  return 0;
}

int filepaths_from_m3u(char*** filepaths, char* file_name) {
  FILE *m3u_file = fopen(file_name, "r");

  if (m3u_file == NULL) {
    printf("error opening file\n");
    return 0;
  }

  char** temp_filepaths = (char**)malloc(sizeof(char*) * 10);
  int max = 10;
  int current_index = -1;

  int line_length = 512;
  char line[line_length];
  int state = 0;

  while (fgets(line, sizeof(line), m3u_file)) {
    char l[strlen(line)];
    strncpy(l, line, strlen(line)-1);
    l[strlen(line)-1] = '\0';

    // parsing time
    switch (state) {
      case 0:
        if (strcmp(l, "#EXTM3U") == 0) {
          state = 1;
        }
        break;
      case 1:
        if (strstr(l, "#EXTINF") != 0) {
          state = 2;
        }
        break;
      case 2:
        // add l to list of filepaths
        current_index++;
        if ((current_index+1) >= max) {
          temp_filepaths = (char**)realloc(temp_filepaths, sizeof(char*) * (max * 2));
          max *= 2;
        }
        *(temp_filepaths+current_index) = (char*)malloc(sizeof(char) * (strlen(l)+1));
        //*(temp_filepaths+current_index) = (char*)malloc(sizeof(char) * 512);
        strncpy(*(temp_filepaths+current_index), l, sizeof(char) * (strlen(l)+1));
        //printf("%s\n", *(temp_filepaths) );
        state = 1;
        break;
      default:
        break;
    }
  }
  *filepaths = temp_filepaths;
  fclose(m3u_file);
  return current_index+1;
}

int main(int argc, char* argv[]) {
  if (valid_arguments(argc, argv) == 0) {
    printf("usage: %s <m3u file> <dest folder>\n", argv[0]);
    return 0;
  }

  printf("%s %s\n", argv[1], argv[2]);

  char** paths;
  int paths_length;
  paths_length = filepaths_from_m3u(&paths, argv[1]);
  printf("number of paths: %i\n", paths_length);

  for (int i=0; i<paths_length; i++) {
    int lso = strrchr(paths[i], '/')-paths[i]+1;
    lso = lso >=0 ? lso : 0;
    char file_name[ strlen(paths[i]) - lso + 1 ];
    strncpy(file_name, paths[i]+lso, strlen(paths[i]) - lso + 1);

    char* pf_file_name;
    path_friendly(&pf_file_name, file_name);

    char* pf_src_path;
    path_friendly(&pf_src_path, paths[i]);

    printf("copying %s to %s%s\n", pf_src_path, argv[2], pf_file_name);

    free(pf_src_path);
    free(pf_file_name);
  }
  for (int i=0; i<paths_length; i++) {
    free(paths[i]);
  }
  free(paths);
  return 0;
}
