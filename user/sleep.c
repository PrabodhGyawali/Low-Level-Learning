#include "kernel/types.h"
#include "user/user.h"


int check_args(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(2, "Usage: Invalid arguments!");
    return 1;
  }
  /* Check if argument is an integer */
  if (atoi(argv[1]) == 0) {
    fprintf(2, "Usage: argument must be an integer");
    return 1;
  }
  return 0;
}


int main(int argc, char* argv[]) {
  int ARGS_CHECK = check_args(argc, argv);  
  if (ARGS_CHECK != 0) {
    return 1;
  }
  
  if (sleep(atoi(argv[1])) != 0) {
    fprintf(2, "E: unknwon\n");
    return 1;
  }   

  exit(0);
};
