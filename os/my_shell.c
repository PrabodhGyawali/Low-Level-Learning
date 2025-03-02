#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"


int strin(char c, const char *str) {
  if (!str) {
    return 0;
  }
  while (*str != '\0')
  {
    if (*str == c) {
      return 1;
    }
    str++;
  }
  return 0;
}


/* Test Cases
  FIXME: wierd behavior of ">>> "
*/

/* Read a line of characters from stdin. */
int getcmd(char* buf, int nbuf) {
  printf(">>> ");
  memset(buf, 0, nbuf);
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
  /* my structures 
      - [0]: file_name_l
      - [1]: file_name_r
   */
  // char* static_values[2] = {0};
  /* Useful data structures and flags. */
  char* arguments[10] = {0};
  int numargs = 0;
  /* Word start/end */
  int ws = 1;
  int we = 0;
  // int double_quotes = 0;

  int redirection_left = 0;
  int redirection_right = 0;
  char* file_name_l = 0;
  char* file_name_r = 0;
  /* Remove position of file_name from arguments array */
  int file_name_l_ind = -1; 
  int file_name_r_ind = -1;

  int p[2];
  int pipe_cmd = 0;
  int sequence_cmd = 0;
  char* sequence_pointer = 0;
  
  int i = 0;
  /* Parse the command character by character. */
  for (; i < nbuf; i++) {
    /* Parse the current character and set-up various flags:
       sequence_cmd, redirection, pipe_cmd and similar. */
    if (buf[i] == 10) {
      buf[i] = '\0'; // tokenize last argument
      break;
    } // Save CPU cycles

    if (buf[i] == ';') {
      sequence_cmd = 1;
      if (we) {
        buf[i] = '\0';
        we = 0;
        ws = 1;
      }

      int j = i + 1;
      while(j < nbuf && strin(buf[j], " \t") == 1) {
        j++;
      }
      if (j >= nbuf || strin(buf[j], "\0\n") == 1) {
        fprintf(2, "my_shell: missing command after ;\n");
        exit(1);
      }
      sequence_pointer = &buf[j];
      break;
    }

    if (buf[i] == '|') {
      pipe_cmd = 1;
      if (we) {
        buf[i] = '\0';
        we = 0;
        ws = 1;
      }

      if (pipe(p) < 0) {
        fprintf(2, "my_shell: pipe creation failed\n");
        exit(1);
      }
      break;
    }
    
    /* Start a word */
    if (ws && strin(buf[i], " \t>|<;") == 0) {
      numargs++;
      if (file_name_r_ind == (numargs - 1) || file_name_l_ind == (numargs - 1)) {
        // decrement numargs
        numargs--;
      } else {
        // save next arg to arg list
        arguments[numargs - 1] = &buf[i];
      }
      
      we = 1;
      ws = 0;
    } 

    /* End a word */
    if (we == 1 && buf[i] == 32 && buf[i] != 60 && buf[i] != 62) {
      buf[i] = '\0';
      ws = 1;
      we = 0;
    }

    /* Redirection left */
    if (buf[i] == '<') {
        redirection_left = 1;
        if (we) {
          buf[we] = '\0';
          we = 0;
          ws = 1;
        } 
        if (numargs == 0)  {
          fprintf(2, "my_shell: missing file for redirection\n");
          exit(0);
        }
    }
    /* Redirection right */
    if (buf[i] == '>') {
      redirection_right = 1;
      if (we) {
        buf[i] = '\0'; 
        we = 0;
        ws = 1;
      }
      
      if (numargs == 0)  {
        fprintf(2, "my_shell: missing file for redirection\n");
        exit(0);
      }
    }

    // TODO: Fix errors with: `ls; echo hello > output; ls`, where file_name_r = `output; ls`
    if (!(redirection_left || redirection_right)) {
      /* No redirection, continue parsing command. */
      
      // Place your code here
    } 
    else {
      /* Redirection command. Capture the file names. */
      /* TODO: Figure out a way to remove file_name_l and file_name_r from arguments */
      if (redirection_left && file_name_l == 0) {
        int j = i+1;
        while (strin(buf[j], " \t><|;\n") == 1)
        {
          if (strin(buf[j], "><|;\n") == 1) {
            fprintf(2, "my_shell: invalid use of redirection token\n");
            exit(1);
          }
          j++;
        }

        file_name_l = &buf[j];
        if (we) {
          file_name_l_ind = numargs - 1;
          we = 0;
          ws = 1;
        }
        file_name_l_ind = numargs;
        
      } else if (redirection_right && file_name_r == 0) {
        int j = i+1;
        while (strin(buf[j], " \t><|;\n") == 1)
        {
          if (strin(buf[j], "><|;\n") == 1) {
            fprintf(2, "my_shell: invalid use of redirection token\n");
            exit(1);
          }
          j++;
        }
        file_name_r = &buf[j];
        if (we) {
          file_name_r_ind = numargs - 1;
          we = 0;
          ws = 1;
        }
        file_name_r_ind = numargs;
      } else {
        if (buf[i] == ' ' || buf[i] == '\n') {
          buf[i] = '\0';
        }
      }
    }
  }

  /* Testing */
  // printf("\n\nNum of args: %d\n", numargs);
  // for (int i = 0; i < numargs; i++)
  // {
  //   printf("arguments[%d]: %s\n", i, arguments[i]);
  // }
  // printf("flags: pipe_cmd=%d, sequence_cmd=%d, numargs=%d, ws=%d, we=%d\n", pipe_cmd, sequence_cmd, numargs, ws, we);
  // printf("redir_l: %d\n", redirection_left);
  // printf("redir_r: %d\n", redirection_right);
  // printf("file_l: %s\n", file_name_l);
  // printf("file_r: %s\n", file_name_r);
  // printf("file_l_pos: %d\n", file_name_l_ind);
  // printf("file_r_pos: %d\n\n\n", file_name_r_ind);
  /*
    Sequence command. Continue this command in a new process.
    Wait for it to complete and execute the command following ';'.
  */
  if (sequence_cmd) {
    sequence_cmd = 0;

    int pid = fork();
    if (pid < 0) {
      fprintf(2, "my_shell: fork failed\n");
      exit(1);
    } else if (pid == 0) {
      /* Child process */
      if (redirection_left) {
        int redir_fd = open(file_name_l, O_RDONLY);
        if (redir_fd < 0) {
            fprintf(2, "my_shell: cannot open %s\n", file_name_l);
            exit(1);
        }
        close(0);
        if (dup(redir_fd) < 0) {
          fprintf(2, "redirection dup error\n");
          close(redir_fd);
          exit(1);
        }
        close(redir_fd);
      }
      if (redirection_right) {
        int redir_fd = open(file_name_r, O_CREATE | O_TRUNC | O_WRONLY);
        if(redir_fd < 0) {
            fprintf(2, "my_shell: cannot open %s\n", file_name_r);
            exit(1);
        }
        close(1);
        dup(redir_fd);
        close(redir_fd);
      }
      exec(arguments[0], arguments);
      exit(1);
    } else {
      wait(0);
      // printf("sequence returned, running: {%s}\n", sequence_pointer);
      run_command(sequence_pointer, nbuf - (sequence_pointer - buf), pcp);
    }
  }


  /*
    If this is a redirection command,
    tie the specified files to std in/out.
  */
  if (redirection_left) {
    int redir_fd = open(file_name_l, O_RDONLY);
    if (redir_fd < 0) {
        fprintf(2, "my_shell: cannot open %s\n", file_name_l);
        exit(1);
    }
    close(0);
    if (dup(redir_fd) < 0) {
      fprintf(2, "redirection dup error\n");
      close(redir_fd);
      exit(1);
    }
    close(redir_fd);
  }
  if (redirection_right) {
    int redir_fd = open(file_name_r, O_CREATE | O_TRUNC | O_WRONLY);
    if(redir_fd < 0) {
        fprintf(2, "my_shell: cannot open %s\n", file_name_r);
        exit(1);
    }
    close(1);
    dup(redir_fd);
    close(redir_fd);
  }
  /* Parsing done. Execute the command. */
  
  /* If this command is a CD command, write the arguments to the pcp pipe 
   * and exit with '2' to tell the parent process about this. */
  if (strcmp(arguments[0], "cd") == 0) {
    // ##### Place your code here.
    if (numargs > 2) {
      fprintf(2, "-my_shell: cd: too many arguments\n");
      exit(0);
    }
    if (numargs == 1) {
      exit(0);
    }
    // Rule: Close before and after write
    close(pcp[0]);
    write(pcp[1], arguments[1], strlen(arguments[1]));
    close(pcp[1]);
    exit(2);
  } else if (strcmp(arguments[0], "exit") == 0) {
    exit(3);
  } 
  else {
    /*
      Pipe command: fork twice. Execute the left hand side directly.
      Call run_command recursion for the right side of the pipe.
    */
    if (pipe_cmd) {
      // ##### Place your code here.
      /* Get next address to RHS of pipe */
      int j = i + 1;
      while(j < nbuf && strin(buf[j], " \t<|>;\n") == 1) {
        if (strin(buf[j], "<|>;\n") == 1) {
          fprintf(2, "my_shell: invalid use of pipe\n");
          exit(1);
        }
        j++;
      }
      if (numargs == 0) {
        fprintf(2, "my_shell: missing commands before pipe\n");
        exit(1);
      }
      int pid = fork();
      if (pid == 0) {
        close(1);
        dup(p[1]);
        close(p[0]);
        close(p[1]);
        

        if (exec(arguments[0], arguments) < 0) {
          fprintf(2, "exec failed: %s\n", arguments[0]);
          exit(1);
        }
      } else if (pid > 0) {
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);

        /* Reset RHS args */
        memset(arguments, 0, sizeof(arguments));
        numargs = 0;

        run_command(&buf[j], nbuf - j, pcp);
      }
    } else {
      // ##### Place your code here.
      int pid = fork();
      if (pid > 0) {
        close(pcp[0]);
        close(pcp[1]);
        pid = wait((int*)0);
      } else if (pid == 0) {
        close(pcp[0]);
        close(pcp[1]);
        exec(arguments[0], arguments);
        fprintf(2, "exec failed: %s\n", arguments[0]);
        exit(0);
      } else {
        fprintf(2, "my_shell: run_command internal fork error\n");
      } 
    }
  }

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
  while(getcmd(buf, sizeof(buf)) >= 0) {
    if (pipe(pcp) < 0) {
      fprintf(2, "my_shell: pipe creation failed\n");
      continue;
    }

    if (fork() == 0) {
      close(pcp[0]);
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

    /* Exit my_shell */
    if (child_status == 3) {
      close(pcp[0]);
      break;
    }

    if (child_status == 2) {  
      char path[100] = {0};
      int read_rc = read(pcp[0], path, 100);
      if (read_rc > 0) {
        // printf("read_rc: %d\n", read_rc);
        // printf("path={%s}\n", path);
        int cd_rc = chdir(path);
        // printf("cd return: %d\n", cd_rc);
        if (cd_rc < 0) {
          fprintf(2, "my_shell: cd: %s not found\n", path);
        }
      }
      else if (read_rc == 0) {
        fprintf(2, "my_shell: end of main pipe reached\n");
      } else {
        fprintf(2, "my_shell: main pipe read error\n");
      }
    }
    close(pcp[0]);

    // Monitor Process table for link to make sure all pipe processes are completed
    // struct stat st;
    // while (fstat(1, &st) == 0 && st.nlink > 0) {
    //   sleep(1);
    // }
  }
  exit(0);
}
