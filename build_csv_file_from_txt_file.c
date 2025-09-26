#include <direct.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>

int main() {
  printf("!! HELLO WELCOME TO MY TXT FILE EDITOR PROGRAM !!\n");
  int phase = 1;

  char *phase_paths[] = {"C:\\Users\\smaus\\Documents\\banfield\\RQID_1015\\active_and_inactive_records_phase_1",
                         "C:\\Users\\smaus\\Documents\\banfield\\RQID_1015\\active_and_inactive_records_phase_2",
                         "C:\\Users\\smaus\\Documents\\banfield\\RQID_1015\\active_and_inactive_records_phase_3"
  };

  char *phase_1_files[12];
  char *phase_2_files[98];
  char *phase_3_files[40];

  int on_path = 1;

  // loop through phase paths
  for (int h = 0; h < 3; h++) {

    printf("===== DOING PHASE %d", on_path);

    char *cwd = malloc(1024 * sizeof(char));
    int rez = _chdir(phase_paths[h]);

    if (rez == 0) {
      _getcwd(cwd, 1024);
      printf("===== current working dir: %s\n", cwd);

      WIN32_FIND_DATA file_data;
      HANDLE handle;

      handle = FindFirstFile("*", &file_data);
      // find first file but don't add because it's going to be '.'
      printf("FIRST file name: %s\n", file_data.cFileName);
      int counter = 0;

      while (FindNextFileA(handle, &file_data)) {
        int len = strlen(file_data.cFileName);
        if (strncmp((file_data.cFileName + len - 4), ".txt", 4) == 0) {
          printf("adding next flle name at position %d : %s for phase %d\n",
                 counter, file_data.cFileName, on_path);
          if (on_path == 1) {
            phase_1_files[counter] =
                malloc((strlen(file_data.cFileName) + 1) * sizeof(char));
            strcpy(phase_1_files[counter], file_data.cFileName);
          } else if (on_path == 2) {
            phase_2_files[counter] =
                malloc((strlen(file_data.cFileName) + 1) * sizeof(char));
            strcpy(phase_2_files[counter], file_data.cFileName);
          } else {
            phase_3_files[counter] =
                malloc((strlen(file_data.cFileName) + 1) * sizeof(char));
            strcpy(phase_3_files[counter], file_data.cFileName);
          }
          counter++;
        }
      }
      on_path++;

    } else {
      printf("changing dirs not working for path %s. .\n", phase_paths[h]);
      return EXIT_FAILURE;
    }

    // free cwd
    free(cwd);
  }

  char *HEADER_ROW = "client_id,hosp_num,client_status,close_hosp_phase";

  // phase 1 file changes
  _chdir(phase_paths[0]);

  for (int i = 0; i < 1; i++) {
    printf("==== reading file name from phase 1 str arry at position %d: %s\n",
           i, phase_1_files[i]);

    // open file
    FILE *file = fopen(phase_1_files[i], "r");

    if (file == NULL) {
      printf("error opening file :%s\n", phase_1_files[i]);
      continue;
    }

    // new csv file name
    char *output_file_name = malloc(256 * sizeof(char));
    strcpy(output_file_name, phase_1_files[i]);
    int file_name_len = strlen(output_file_name);
    strcpy(output_file_name + file_name_len - 4, ".csv");

    // create additions for lines
    char *str_adds = malloc(50 * sizeof(char));
    int hosp_num_cntr = 1;
    str_adds[0] = ',';

    // construct additions
    for (size_t t = 17; t < 21; t++) {
      str_adds[hosp_num_cntr] = phase_1_files[i][t];
      hosp_num_cntr++;
    }
    str_adds[hosp_num_cntr] = '\0';

    if (strstr(phase_1_files[i], "Active") != NULL) {
      // strcat relies on '\0' and it will move it for you
      strcat(str_adds, ",Active,1");
    } else {
      strcat(str_adds, ",Not Active,1");
    }

    printf("addition string add on will be : %s\n", str_adds);

    // open new csv file
    FILE *outfile_stream = fopen(output_file_name, "w");

    // add header row
    for (size_t t = 0; HEADER_ROW[t] != '\0'; t++) {
      fputc(HEADER_ROW[t], outfile_stream);
    }
    fputc('\n', outfile_stream);

    // read in lines
    char *line = malloc(2048 * sizeof(char));

    int c;
    int line_has_content = 0;

    // read in characters and put in new file
    while ((c = fgetc(file)) != EOF) {
      if (c != '\n') {
        if (isprint(c) != 0) {
          line_has_content = 1;
          fputc(c, outfile_stream);
        }
      } else {
        if (line_has_content) {
          for (size_t t = 0; str_adds[t] != '\0'; t++) {
            fputc(str_adds[t], outfile_stream);
          }
          fputc('\n', outfile_stream);
        } else {
          printf("===== LINE IS JUST NEW LINE SO SKIPPING\n");
        }
        line_has_content = 0;
      }
    }

    printf("==== complete new file processing %s ---> %s\n", phase_1_files[i],
           output_file_name);

    fclose(file);
    fclose(outfile_stream);

    free(phase_1_files[i]);
    free(line);
    free(str_adds);
    free(output_file_name);
  }

  // phase 2 file changes
  _chdir(phase_paths[1]);

  for (int i = 0; i < 98; i++) {
    printf("==== reading file name from phase 2 str arry at position %d: %s\n",
           i, phase_2_files[i]);

    free(phase_2_files[i]);
  }

  // phase 3 files changes
  _chdir(phase_paths[2]);

  for (int i = 0; i < 40; i++) {
    printf("==== reading file name from phase 3 str arry at position %d: %s\n",
           i, phase_3_files[i]);

    free(phase_3_files[i]);
  }

  return EXIT_SUCCESS;
}
