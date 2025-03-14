#include <stdio.h>      /* printf */
#include <sys/mman.h>   /* mmap, madvise */
#include <stdlib.h>     /* strtol */
#include <unistd.h>     /* sysconf, usleep */
#include <string.h>     /* memset */
#include <errno.h>      /* errno */

#include "util.h"       /* DIE */

int main(int argc, char *argv[])
{
    size_t  num_pages;
    long    page_size;
    void    *buffer;
    int     ans;

    /* input arg check */
    DIE(argc != 2, RED "Run it like this: ./ksm <NUM_PAGES>\n" CLR);
    num_pages = atol(argv[1]);

    /* get page size */
    page_size = sysconf(_SC_PAGE_SIZE);
    DIE(page_size == -1, RED "sysconf failed (%d)\n" CLR, errno);

    /* KSM can merge only anonymous (private) pages; not file backed ones */
    buffer = mmap(NULL, num_pages * page_size, PROT_READ | PROT_WRITE,
        MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    DIE(buffer == MAP_FAILED, RED "mmap failed (%d)\n" CLR, errno);

    /* advise regarding ksm of buffer pages */
    ans = madvise(buffer, num_pages * page_size, MADV_MERGEABLE);
    DIE(ans == -1, RED "madvise failed (%d)\n" CLR, errno);

    /* fill the pages with the exact same data */
    for (size_t i=0; i<num_pages; ++i)
        memset(buffer + i * page_size, 0xaa, page_size);

    printf("finished initializing pages\n");

    /* sleep for a bit; give time to check memory consumption */
    ans = usleep(10E6);
    DIE(ans == -1, RED "usleep failed (%d)\n" CLR, errno);

    /* clean up */
    ans = munmap(buffer, num_pages * page_size);
    DIE(ans == -1, RED "munmap failed (%d)\n" CLR, errno);


    return 0;
}