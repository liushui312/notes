#include <stdio.h>
#include <pwd.h>

int main(int argc, char *argv[])
{
    struct passwd *pw = NULL;

    pw = getpwuid(atoi(argv[1]));
    if (pw == NULL)
    {
        perror("getpwuid"); 
        return 0;
    }

    printf("name = %s\n", pw->pw_name);

    return 0;
}
