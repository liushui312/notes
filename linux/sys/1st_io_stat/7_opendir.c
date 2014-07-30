#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
    DIR *dp = NULL;
    struct dirent *dirp = NULL;
    struct stat s;
    char buf[1024];

    dp = opendir(argv[1]);
    if (dp == NULL)
        perror("opendir");

    /*chdir(argv[1]);*/

    while ((dirp = readdir(dp)) != NULL)
    {
        if (dirp->d_name[0] == '.')
            continue;
        printf("%s\n", dirp->d_name); 
        snprintf(buf, sizeof(buf), "%s/%s", argv[1], dirp->d_name);
        if (stat(buf, &s))
            perror("stat");
    }

    printf("=====================\n");
    rewinddir(dp);

    while ((dirp = readdir(dp)) != NULL)
    {
        printf("%s\n", dirp->d_name); 
    }

    closedir(dp);


    return 0;
}
