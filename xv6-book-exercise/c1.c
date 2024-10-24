#include "user/user.h"
#include "types.h"

// 3 fd open
/* user/sh.c:152 */

int xv6_read(int fd, char* buf, int n) {
    
}

int main() {
    char buf[512];
    int n;

    for (;;) {
        n = read(0, buf, sizeof buf);
        if (n == 0)
            break;
        if (n < 0) {
            fprintf(2, "read error\n");
            exit(1);
        }
        if (write(1, buf, n) != n) {
            fprintf(2, "write error\n");
            exit(1);
        }
    }

    // int dup(int fd);     /* ls path > tmp1 2>&1/dev/null */
        // I/O redirection
    int fd = dup(1);
    write(1, "hello ", 6);
    write(1, "world\n", 6);
}

// open(const char*, int flags);       (kernel/fcntl.h:1-5)

// wait syscall


