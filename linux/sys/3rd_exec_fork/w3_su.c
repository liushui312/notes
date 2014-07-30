#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <shadow.h>
#include <unistd.h>

char *crypt(const char *, const char *);

int main(int argc, char *argv[])
{
    int flag = 0;
    char name[128] = "root";  //su / su -
    struct passwd *pw = NULL;
    struct spwd *sp = NULL;
    uid_t id;

    if (argc > 1)
    {
        if (!strcmp(argv[1], "-"))  
        {
            flag = 1;
            if (argc > 2)
                strcpy(name, argv[2]);  //su - kyo
        }
        else
            strcpy(name, argv[1]);  //su kyo
    }

    id = getuid();

    pw = getpwnam(name);
    if (pw == NULL)
    {
        printf("%s 用户不存在!\n", name); 
        return 0;
    }

    if (id != 0)
    {
        sp = getspnam(name);
        if (strcmp(crypt(getpass("password: "), sp->sp_pwdp), sp->sp_pwdp))
        {
            printf("密码验证失败!\n");
            return 0;
        }
    }
    if (flag)
    {
        chdir(pw->pw_dir);
        if (!strcmp(name, "root"))
            setenv("PATH", "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games", 1);
        else
            setenv("PATH", "/usr/local/bin:/usr/bin:/bin:/usr/local/games:/usr/games", 1);
    }
    setenv("HOME", pw->pw_dir, 1);
    setenv("USER", name, 1);
    setenv("USERNAME", name, 1);
    setenv("LOGNAME", name, 1);

    initgroups(name, pw->pw_gid);
    setgid(pw->pw_gid);
    setuid(pw->pw_uid);

    if (fork() == 0)
    {
        execl(pw->pw_shell, pw->pw_shell, NULL); 
        perror("execl");
        exit(0);
    }
    wait(NULL);

    return 0;
}
