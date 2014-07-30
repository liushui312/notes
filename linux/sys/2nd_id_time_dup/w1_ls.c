#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

const char *size_type = "KMGT";
const char *file_type = " pc d b - l s";
const char * const rwx[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};
const char fcolor[] = {10, 33, 31, 10, 34, 10, 31, 10, 10, 10, 36, 10, 35};
const char bcolor[] = {10, 40, 40, 10, 10, 10, 40, 10, 10, 10, 10, 10, 10};

int do_ls(const char *path, int lnk_len, int user_len, int grp_len)
{
    struct stat s;
    char buf[1024];
    int ret, type;
    int c = -1;
    struct passwd *pw = NULL;
    struct group *gr = NULL;
    int bc, fc;
    char *p = NULL;
    
    if (lstat(path, &s))
    {
        perror("stat");
        return -1;
    }

    type = (s.st_mode & S_IFMT) >> 12; 
    printf("%c", file_type[type]);

    bc = bcolor[type];
    fc = fcolor[type];

    ret = (s.st_mode & S_IRWXU) >> 6;
    printf("%c%c", rwx[ret][0], rwx[ret][1]);
   if ((s.st_mode & 07000) == S_ISUID)
    {
        bc = 41;
        fc = 37;
        if (s.st_mode & S_IXUSR)
            printf("s");
       else
            printf("S");
    }
    else
        printf("%c", rwx[ret][2]);


    ret = (s.st_mode & S_IRWXG) >> 3;
    printf("%c%c", rwx[ret][0], rwx[ret][1]);
    if ((s.st_mode & 07000) == S_ISGID)
    {
        bc = 41;
        fc = 37;
        if (s.st_mode & S_IXGRP)
            printf("s");
        else
            printf("S");
    }
    else
        printf("%c", rwx[ret][2]);

    ret = s.st_mode & S_IRWXO;
    printf("%c%c", rwx[ret][0], rwx[ret][1]);
    if ((s.st_mode & 07000) == S_ISVTX)
    {
        if (s.st_mode & S_IXOTH)
            printf("t ");
        else
            printf("T ");
    }
    else
        printf("%c ", rwx[ret][2]);

    printf("%*d ", lnk_len, s.st_nlink);


    pw = getpwuid(s.st_uid);
    if (NULL == pw)
        printf("%*d ", user_len, s.st_uid);
    else
        printf("%*s ", user_len, pw->pw_name);

    gr = getgrgid(s.st_gid);
    if (NULL == gr)
        printf("%*d ", grp_len, s.st_gid);
    else
        printf("%*s ", grp_len, gr->gr_name);

    while (s.st_size / 1024 > 0)
    {
        s.st_size /= 1024;
        c++;
    }
    if (c == -1)
        printf("%5ld ", s.st_size);
    else
        printf("%4ld%c ", s.st_size, size_type[c]);

    strftime(buf, sizeof(buf), "%F %H:%M", localtime(&s.st_mtime));
    printf("%16s ", buf);
        
    if ((p = strrchr(path, '/')) == NULL)
        p = (char *)path;
    else
        p++;

    printf("\033[%d;%dm\033[1m%s\033[0m", bc, fc, p);
    if (S_ISLNK(s.st_mode))
    {
        ret = readlink(path, buf, sizeof(buf));
        buf[ret] = '\0';
        if (stat(buf, &s))
        {
            bc = 41; 
            fc = 40;
        }
        else
        {
            type = (s.st_mode & S_IFMT) >> 12;
            bc = bcolor[type];
            fc = fcolor[type];
        }
        printf(" -> \033[%d;%dm\033[1m%s\033[0m", bc, fc, buf); 
    }
    printf("\n");

    return 0;
}

int get_int_len(int num)
{
    int i, bit;

    for (i = num, bit = 0; i > 0; i /= 10, bit++)
        ;
    return bit;
}

int do_ls_dir(const char *path)
{
    DIR *dp = NULL;
    struct dirent *dirp = NULL;
    char buf[1024];
    struct stat s;
    struct passwd *pw = NULL;
    struct group *gr = NULL;
    int user_len = 0, grp_len = 0, lnk_len = 0, ret;

    dp = opendir(path);
    if (NULL == dp)
    {
        do_ls(path, 0, 0, 0); 
        return 0;
    }
    while ((dirp = readdir(dp)) != NULL)
    {
        if (dirp->d_name[0] == '.')
            continue;
        snprintf(buf, sizeof(buf), "%s/%s", path, dirp->d_name);
        if (lstat(buf, &s))
            continue;
        
        ret = get_int_len(s.st_nlink);
        if (lnk_len < ret)
            lnk_len = ret;

        pw = getpwuid(s.st_uid);
        if (NULL == pw)
            ret = get_int_len(s.st_uid);
        else
            ret = strlen(pw->pw_name);

        if (user_len < ret)
            user_len = ret;

        gr = getgrgid(s.st_gid);
        if (NULL == gr)
            ret = get_int_len(s.st_gid);
        else
            ret = strlen(gr->gr_name);

        if (grp_len < ret)
            grp_len = ret;
    }

    rewinddir(dp);

    while ((dirp = readdir(dp)) != NULL)
    {
        if (dirp->d_name[0] == '.')
            continue;
        snprintf(buf, sizeof(buf), "%s/%s", path, dirp->d_name);
        do_ls(buf, lnk_len, user_len, grp_len);
    }

    closedir(dp);

    return 0;
}

int do_ls_lR(const char *path)
{
    DIR *dp = NULL;
    struct dirent *dirp = NULL;
    struct stat s;
    struct passwd *pw = NULL;
    struct group *gr = NULL;
    int user_len = 0, grp_len = 0, lnk_len = 0, ret;

    dp = opendir(path);
    if (NULL == dp)
    {
        do_ls(path, 0, 0, 0); 
        return 0;
    }

    printf("%s:\n", path);
    chdir(path);

    while ((dirp = readdir(dp)) != NULL)
    {
        if (dirp->d_name[0] == '.')
            continue;
        if (lstat(dirp->d_name, &s))
            continue;
        
        ret = get_int_len(s.st_nlink);
        if (lnk_len < ret)
            lnk_len = ret;

        pw = getpwuid(s.st_uid);
        if (NULL == pw)
            ret = get_int_len(s.st_uid);
        else
            ret = strlen(pw->pw_name);

        if (user_len < ret)
            user_len = ret;

        gr = getgrgid(s.st_gid);
        if (NULL == gr)
            ret = get_int_len(s.st_gid);
        else
            ret = strlen(gr->gr_name);

        if (grp_len < ret)
            grp_len = ret;
    }

    rewinddir(dp);

    while ((dirp = readdir(dp)) != NULL)
    {
        if (dirp->d_name[0] == '.')
            continue;
        do_ls(dirp->d_name, lnk_len, user_len, grp_len);
    }

    printf("\n");

    rewinddir(dp);

    while ((dirp = readdir(dp)) != NULL)
    {
        if (dirp->d_name[0] == '.')
            continue;
        if (dirp->d_type == 4)
            do_ls_lR(dirp->d_name);
        /*snprintf(buf, sizeof(buf), "%s/%s", path, dirp->d_name);*/
    }
    chdir("..");

    closedir(dp);

    return 0;
}

int main(int argc, char *argv[])
{
    do_ls_lR(argv[1]);

    return 0;
}

