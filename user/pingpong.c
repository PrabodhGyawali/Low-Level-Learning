/* ping-pong
 * 1 byte -> 2 processes
 * More Reading: ex on p.19 of xv6 book
 * Pseudocode:
 *  - PP send byte to child
 *  - Child prints "<pid>: received ping", <pid> is its PID
 *  - Write byte on the pipe to the parent & exit
 */

#include "user.h"
#include "types.h"

int main() {
    // Create a pipe
    int* pointer;
    int pipe_fd = pipe(pointer);
    // Write a byte to that pipe
    if (pipe_fd < 0) {
        fprintf(2, "pipe creation failed\n");
        exit(1);
    }
    if (write(pipe_fd, "") == 0) {
        fprintf(2, "write failed\n");
        exit(1);
    }
    // Create a child
    int rc = fork();
    if (rc < 0) {
        fprintf(2, "Error, forking process!\n");
        exit(1);
    }
    if (rc > 0) {
        write(pipe_fd, 'P', 1);
        rc = wait((int*) 0);
        int my_pid = getpid();
        char received_byte;
        read(pipe_fd, &received_byte, 1);
        printf("%d:Received pong, %c \n", my_pid, received_byte);

    }
    if (rc == 0) {
        char the_byte;
        read(pipe_fd, &the_byte, 1);
        int my_pid = getpid();
        printf("%d:Received ping, %c \n", my_pid, the_byte);
        if (the_byte == 'P')
            the_byte = 'R';
        write(pipe_fd, &the_byte, 1);
        exit(0);
    }
    exit(0);
}