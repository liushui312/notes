
#define DEBUG0(...)
#define DEBUG1(...)     fprintf(stderr, __VA_ARGS__);
#define ERRP(con, act, flag, ...)   do              \
            {                                       \
                if (con)                            \
                {                                   \
                    DEBUG##flag(__VA_ARGS__)        \
                    act;                            \
                }                                   \
            } while (0)
/**
flag: 0 不打印
      1 打印

act:  处理方法, 一般, goto ERR1
  在正确返回后面逆序写ERR, 
  ERR3:
  ERR2:
  ERR1:

遇到内存分配、打开文件等需要错误处理时需增加另外一个ERR
**/
-------------------------------------------------------------------

#define GETS(string, op, ...)   do          \
            {                               \
                int ret;                    \
                printf(string);             \
                ret = scanf(__VA_ARGS__);   \
                while (getchar() != '\n')   \
                    continue;               \
                if (ret == 0)               \
                    op;                     \
            } while (0)

#define GETLINES(string, buf)   do                  \
            {                                       \
                printf(string);                     \
                fgets(buf, sizeof(buf), stdin);     \
                if (buf[strlen(buf) - 1] == '\n')   \
                    buf[strlen(buf) - 1] = '\0';    \
                else                                \
                    while (getchar() != '\n')       \
                        continue;                   \
            } while (0)


#define container_of(ptr, type, member) \
        ((type *)(((char *)ptr) - ((unsigned long)(&(((type *)0)->member)))))
