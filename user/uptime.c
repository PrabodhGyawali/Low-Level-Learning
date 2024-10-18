#include "user.h"
#include "types.h"


int main() {
    uint64 ticks = uptime();
    printf("Uptime: %d", ticks);
    return 1;
}

