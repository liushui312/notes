#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    struct stat s;

    if (lstat(argv[1], &s))
    {
        perror("stat");
        return 0;
    }

    switch (s.st_mode & S_IFMT)
    {
        case S_IFCHR:
            printf("c");
            break;
        case S_IFREG:
            printf("-");
            break;
        case S_IFBLK:
            printf("b");
            break;
        case S_IFIFO:
            printf("p");
            break;
        case S_IFDIR:
            printf("d");
            break;
        case S_IFLNK:
            printf("l");
            break;
        case S_IFSOCK:
            printf("s");
            break;
        default:
            break;
    }

    switch (s.st_mode & S_IRWXU)
    {
        case 0000:
            printf("---");
            break;
        case 0100:
            printf("--x");
            break;
        case 0200:
            printf("-w-");
            break;
        case 0300:
            printf("-wx");
            break;
        case 0400:
            printf("r--");
            break;
        case 0500:
            printf("r-x");
            break;
        case 0600:
            printf("rw-");
            break;
        case 0700:
            printf("rwx");
            break;
        default:
            break;
    }

    printf(" %d %d %d %ld\n", s.st_nlink, s.st_uid, s.st_gid, s.st_size);

    return 0;
}
