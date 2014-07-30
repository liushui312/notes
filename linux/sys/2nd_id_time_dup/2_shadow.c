#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <shadow.h>

extern char *crypt(const char *, const char *);

int main(void)
{
    char name[256];
    char *pwd = NULL;
    struct spwd *sp = NULL;

    printf("请输入用户名 : ");
    fgets(name, sizeof(name), stdin);
    if (name[strlen(name) - 1] == '\n')
        name[strlen(name) - 1] = '\0';
    pwd = getpass("请输入密  码 : ");
    printf("name = %s, pwd = %s\n", name, pwd);

    sp = getspnam(name);
    if (sp == NULL)
    {
        perror("getspnam");
        return 0;
    }
    printf("sp->spwd = %s\n", sp->sp_pwdp);

    printf("crypt = %s\n", crypt(pwd, sp->sp_pwdp));

    return 0;
}
