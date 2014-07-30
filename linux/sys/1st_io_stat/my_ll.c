#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <time.h>

const char file_type[13] = " pc d b - l s";
const char * const file_per[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
const char *size_type = "KMGT";

int show_file(struct stat s, char *file_name, int len_nlink, int len_oname, int len_gname)
{
    int i = 0, type, per;
    float size = 0;
    char own_name[20], group_name[20], date[20];
    struct passwd *pd = NULL;
    
    type = (s.st_mode & S_IFMT) >> 12;
    printf("%c", file_type[type]);

   while(i < 3)
    {
         per = ((s.st_mode << 3 * i++) & S_IRWXU) >> 6;
         printf("%s", file_per[per]);
    }
    
    pd = getpwuid(s.st_uid);
    strcpy(own_name, pd->pw_name);
    strcpy(group_name, pd->pw_name);

    printf(" %*d",len_nlink, s.st_nlink);
    printf(" %*s",len_oname, own_name);
    printf(" %*s",len_gname, group_name);

    size = s.st_size;
    i = 0;
    while (size / 1024 > 1)
    {
        size /= 1024;
        i++;
    }
    if (i == 0)
        printf(" %6.0f", size);
    else 
        printf(" %5.1f%c", size, *(size_type + i -1));

    strftime(date, sizeof(date), "%F %H:%M", localtime(&s.st_mtime));
    printf(" %16s", date);
    printf(" %s\n", file_name);

    if (file_type[type] == 'd')
        show_dir(file_name);

}

void show_dir(const char *path)
{
    DIR *dp = NULL;
    struct dirent *dirp = NULL;
    struct stat s;
    char buf[1024];

    dp = opendir(path);
    if (dp == NULL)
        perror("opendir");
       

    while ((dirp = readdir(dp)) != NULL)
    {
        if (dirp->d_name[0] == '.')
            continue;
        snprintf(buf, sizeof(buf), "%s", dirp->d_name);
        if (stat(buf, &s))
            perror("stat");
        show_file(s, dirp->d_name, 1, 4, 4);
    } 
}

int main(int argc, char *argv[])
{
    struct stat s;
    int i = 1;

    if (argc == 1)
        show_dir("./");     
    else 
    {
        while (i  < argc)
        {
            if (lstat(argv[i], &s))
            {
                perror("stat");
                return 0;
            }

            show_file(s, argv[i], 1, 4, 4);
            i++;
        }
    }

    return 0;
}
