标准I/O:
    fopen fclose fread fwrite fgetc fputc fgets fputs fscanf fprintf 
    fseek ftell rewind feof ferror atoi, snprintf (setvbuf) perror
    strcpy strcmp strchr strcat strstr strtok memcpy memmove memset

系统I/O:
    open close read write lseek 
        ioctl fcntl dup2
    pread/pwrite readv/writev
--------------------------------------------------------------------------
文件操作:
  创建        touch                (open,creat,access, umask)
  删除        rm                   (unlink)
  修改内容    vim                  (read,write)
  修改属性    chown/chmod/touch/mv (chown/chmod/utime/rename)
  获取信息    ls                   (stat)

链接:
  创建        ln -s                (symlink)
  删除        rm                   (unlink)
  读软链接    readlink             (readlink)

目录:
  创建    mkdir                    (mkdir)
  路径    cd pwd                   (chdir/getcwd)
  删除    rmdir                    (rmdir)
  遍历    ls        (opendir/readdir/colosedir/rewinddir|glob/glob_free)
