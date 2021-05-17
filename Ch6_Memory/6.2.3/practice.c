#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>

typedef struct
{
    char str[128];
    long lval;
    double dval;
} TestData;

#define NUMBER (100)

int main()
{
    int fd;
    char c;
    long psize, size;
    TestData *ptr;
    long i, lval;
    double dval;
    char buf[512], *p;

    /* mapping file open */
    if ((fd = open("test.txt", O_RDWR | O_CREAT, 0666)) == -1)
    {
        perror("open");
        exit(-1);
    }

    /* Calculate size in which boundary match is done by page size. */
    psize = sysconf(_SC_PAGE_SIZE);
    size = (NUMBER * sizeof(TestData) / psize + 1) * psize;

    /* Seek and write 0 value */
    /* To make the size of file a size that is mapped. */
    if (lseek(fd, size, SEEK_SET) < 0)
    {
        perror("lseek");
        exit(-1);
    }

    if (read(fd, &c, sizeof(char)) == -1)
    {
        c = '\0';
    }

    if (write(fd, &c, sizeof(char)) == -1)
    {
        perror("write");
        exit(-1);
    }

    /* Map */
    ptr = (TestData *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if ((int)ptr == -1)
    {
        perror("mmap");
        exit(-1);
    }

    strncpy(ptr, "That", 4);

    /* Synchronize a file with a memory map */
    msync(ptr, size, MS_ASYNC);

    /* Unmap */
    if (munmap(ptr, size) == -1)
    {
        perror("munmap");
    }

    /* File close */
    close(fd);

    return (0);
}