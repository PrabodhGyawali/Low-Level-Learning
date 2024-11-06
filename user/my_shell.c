#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"



/* Read a line of characters from stdin. */
int getcmd(char* buf, int nbuf) {
  int rc = read(0, buf, nbuf);
  if (rc < 0) {
    fprintf(1, "-my_shell: error reading stdin\n");
    return -1;
  }

  return 0;
}

/*
  A recursive function which parses the command
  at *buf and executes it.
*/
__attribute__((noreturn))
void run_command(char* buf, int nbuf, int* pcp) {

  /* Useful data structures and flags. */
  char* arguments[10] = {0};
  int numargs = 0;
  /* Word start/end */
  int ws = 1;
  int we = 0;

  int redirection_left = 0;
  int redirection_right = 0;
  // char* file_name_l = 0;
  // char* file_name_r = 0;

  // int p[2];
  int pipe_cmd = 0;

  int sequence_cmd = 0;

  int i = 0;
  /* Parse the command character by character. */
  for (; i < nbuf; i++) {
    /* Parse the current character and set-up various flags:
       sequence_cmd, redirection, pipe_cmd and similar. */
    if (buf[i] == 10) {
      buf[i] = '\0'; // tokenize last argument
      break;
    } // Save CPU cycles

    // TODO: ASCII: 0

    /* Start a word */
    if (ws && buf[i] != 32) {
      numargs++;
      arguments[numargs - 1] = &buf[i];
      we = 1;
      ws = 0;
    } 

    if (we == 1 && buf[i] == 32) {
      buf[i] = '\0';
      ws = 1;
      we = 0;
    }
    
    if (!(redirection_left || redirection_right)) {
      /* No redirection, continue parsing command. */

      // Place your code here.
    } else {
      /* Redirection command. Capture the file names. */

      // ##### Place your code here.
    }
  }

  /*
    Sequence command. Continue this command in a new process.
    Wait for it to complete and execute the command following ';'.
  */
  if (sequence_cmd) {
    sequence_cmd = 0;
    if (fork() != 0) {
      wait(0);
      // ##### Place your code here.

    }
  }

  /*
    If this is a redirection command,
    tie the specified files to std in/out.
  */
  if (redirection_left) {
    // ##### Place your code here.
  }
  if (redirection_right) {
    // ##### Place your code here.
  }
  

 
  /* If this command is a CD command, write the arguments to the pcp pipe 
   * and exit with '2' to tell the parent process about this. */
  if (strcmp(arguments[0], "cd") == 0) {
    // ##### Place your code here.
    if (numargs > 2) {
      fprintf(2, "-my_shell: cd: too many arguments\n");
      exit(0);
    }
      
    // Write the arguments to the pcp pipe
    write(pcp[1], arguments[1], strlen(arguments[1]));
    close(pcp[1]);
    exit(2);
  } else if (strcmp(arguments[0], "exit") == 0) {
    exit(1);
  } else if (strcmp(arguments[0], "clear") == 0) {
    exit(3);
  }
  else {
    /*
      Pipe command: fork twice. Execute the left hand side directly.
      Call run_command recursion for the right side of the pipe.
    */
    if (pipe_cmd) {
      // ##### Place your code here.
    } else {
      // ##### Place your code here.
    }
  }
  

  /* Parsing done. Execute the command. */
  int pid = fork();
  if (pid > 0) {
    pid = wait((int *)0);
  } else if (pid == 0) {
    exec(arguments[0], arguments);
    fprintf(2, "exec failed: %s\n", arguments[0]);
    printf(">>>");
    exit(0);
  } else {
    fprintf(2, "run_command exec fork error\n");
  }
  

  /* Test */
  printf("Num of args: %d\n", numargs);
  for (int i = 0; i < numargs; i++)
  {
    printf("arguments[%d]: %s\n", i, arguments[i]);
  }

  printf(">>>");
  exit(0);
}

int main(void) {
  static char buf[100];

  /*
    pcp[0] : read fd
    pcp[1] : write fd
  */
  int pcp[2];
  

  /* Read and run input commands. */
  printf(">>>");
  while(getcmd(buf, sizeof(buf)) >= 0) {
    pipe(pcp);
    if (fork() == 0) {
      close(pcp[1]);
      run_command(buf, 100, pcp);
    }

    /*
      Check if run_command found this is
      a CD command and run it if required.
    */
    close(pcp[1]);
    int child_status;
    // ##### Place your code here
    wait(&child_status);

    /* Exit my_shell.c */
    if (child_status == 1) {
      close(pcp[0]);
      close(pcp[1]);
      exit(1);
    }

    if (child_status == 2) {
      static char path[100];
      int read_rc = read(pcp[0], path, sizeof(path));
      if (read_rc == 0) {
        int chdir_rc = chdir(path);
        if (chdir(path) < 0) 
          fprintf(2, "-my_shell: cd: %s: No such file or directory\n", path);
        printf("chdir status: %d\n", chdir_rc);
        printf("changing to path: %s\n", path);
      }
      close(pcp[0]);
      printf("read_rc: %d\n", read_rc);
      printf(">>>");
    }
  }
  exit(0);
}