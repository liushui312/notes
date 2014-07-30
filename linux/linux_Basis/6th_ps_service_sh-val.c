一、进程管理
    
    进程概念
    
        程序存放在硬盘上是静态，把它复制到内存上并在CPU上运行就叫做进程

        一个运行的系统上有很多进程并且为每个进程分配了进程号PID

        一号进程是所有进程的祖先，除一号进程外所有进程都是父进程(树形结构)

        每个进程都是由父进程启动的，并且可以启动另一个进程成为自己的子进程

    查看进程
        
        pstree                          以树形的结构显示进程
            
            -p                          显示进程号

        ps                              查看进程

            -a                          显示控制台的进程
            -u                          显示带用户名的控制台进程
            -l                          显示详细信息
            -x                          显示所有进程
            -p 3104                     只显示PID为3104的进程
            -C bash                     显示所有bash的进程
            -U root                     显示属于root的进程

            ps -ef / ps -aux            显示所有进程
            
        pgrep                           过滤进程查看
            
            -l                          列出进程PID
            -u root                     列出root用户所有的进程

            -l vim                      匹配vim的所有进程和PID

        jobs                            列出后台运行的程序

        top                             实时显示进程信息

            h or ?                      帮助
            q                           退出
            s                           设定两次更新之间的时候，以秒为单位
            space(空格)                 更新显示
            M                           根据内存大小对进程排序
            P                           根据CPU占用对进程排序
            u                           显示特定用户的进程
            k                           杀死进程
            r                           更改进程优先级

            top -b -n 1 > top.txt       保存为静态文件
            
            top头部显示的信息
                
                users                   几个用户正在登录

                up                      前面是当前时间，后面为运行了多久

                load average: 0.15,0.12, 0.04   这三个数字代表1分钟，5分钟，15分钟内CPU平均负载数，当负载值长期大于5以上的话就要注意可能是CPU负载过大

            这些信息也可以使用uptime得到

            
    进程状态
        
        S                               sleep，睡眠状态，有可能会被其它程序被唤醒
        R                               正在运行状态
        D                               不可中断的睡眠，一般要等待硬件的唤醒
        Z                               僵尸进程,父进程死了，子进程来不及处理
        T                               进程暂停状态

    进程优先级
        
        nice -19 ./test.sh              设置程序运行的优先级，优先级越低，数值越高，中间数字取值范围-20到19

        renice                          和nice一样，只是针对已经运行的程序

            
    进程控制
        
        kill                            信号发送工具
            
            kill -15 3214               友好的终止程序
            kill -9 3214                发信号给内核，直接干掉进程，进程本身收不到这个信号
        
        killall                         和kill同功能，不同的是killall可以针对进程名


================================================================================


二、服务管理

    服务分为独立启动服务(stand alone)和由超级服务(super daemon)启动的服务.
 
    独立服务一经启动，不管有没有被使用将一直运行，除关机或手动关闭服务
    由超级服务启动和管理的服务, 则在客户端连接的时候启动, 在客户端连接关闭后关闭.

    所有独立服务程序的启动脚本都放在/etc/init.d/ 目录里
        
    /etc/rc.d/rc0.d/    ...     /etc/rc.d/rc6.d/        

    这六个目录对应6个运行级别，放各个级别要运行哪些服务
        
    S                       S开头代表开机自动启动
    K                       K开头代表开机不自动启动
    数字                    代表顺序


--------------------------------------------------------------------------------

    服务管理命令
        
       service                                  redhat独有命令
            
            service nfs restart                 重启服务

            service nfs stop                    关闭服务

            service nfs start                   启动服务

            service nfs reload                  重新加载配置文件

            service nfs status                  查看服务状态


        /etc/init.d/nfs restart                 重启服务
        
        /etc/init.d/nfs stop                    关闭服务

        /etc/init.d/nfs start                   启动服务

        /etc/init.d/nfs reload                  重新加载配置文件

        /etc/init.d/nfs status                  查看服务状态
            

            以上命令的区别，/etc/init.d/这种方法比较通用，任何Linux都可以用

--------------------------------------------------------------------------------
    
    控制开机是否启动服务

        1.手工修改/etc/rc.d/rc5.d/里的脚本名称，把S改成K就不会开机启动
        
        2.ntsysv                                  文本图形服务管理
            
            --level 35                          代表控制运行级别3和5的服务

                用方向键选择用空格键选中是否开机启动，完成tab到ok即可

        3.chkconfig                               

            --list                              列出服务在各个运行级别是否启动
                
                chkconfig --list | grep nfs     查看nfs服务状态

            --level                             选择运行级别列出服务状态
                
                chkconfig --level 35            选择运行级别3和5查看服务状态

            on                                  开启服务

                chkconfig --level 35 nfs on     选择运行级别3和5 nfs服务开机自动启动

            off                                 关闭服务

                chkconfig --level 35 nfs off    选择运行级别3和5 nfs服务开机自动不启动
            --add                               添加服务
                
                chkconfig --add nfs             把nfs服务添加到chkconfig管理

            --del                               删除服务
                
                chkconfig --del nfs             把nfs服务chkconfig管理删除
            
            要成功添加一个服务，需要做的：
                
                把服务的脚本放到/etc/rc.d/init.d

                服务的启动脚本必须有两行的注释内容
                    
                    #chkconfig: - 85 15
                        -   代表所有级别默认不自动启动，如果是35代表运行级别3和5自动
                        85  代表如果服务开机启动，那么启动的顺序就是85
                        15  代表关机的时候，服务关闭的顺序

                    #description: nfs       服务的描述，可以随便写

                给予脚本运行权限
                    
                    chmod 755 /etc/rc.d/init.d/nfs

                    不给权限重启服务会出现: 未被识别的服务

--------------------------------------------------------------------------------

    服务介绍
        
        acpid                   高级电源管理
        autofs                  自动挂载移动设备
        kudzu                   检查硬件改动
        crond                   计划任务，必开
        netfs                   nfs依赖服务
        portmap                 nfs依赖服务 
        rpcgssd                 nfs依赖服务
        rpcidmapd               nfs依赖服务    
        gpm                     鼠标支持
        haldaemon               u-dev相关，必须打开
        iptables                软件防火墙，建议打开
        messagebus              进程间通讯
        network                 网络服务
        syslogd                 日志

================================================================================

三、时间任务
    
    常用的有三个：at、crontab、anacron
    
    对应的服务： atd、crond、anacron

--------------------------------------------------------------------------------

    at
        
        如果设定的时间点过了，那么任务就失效，不会再执行，任务只是一次性，每次都要自动去建立

        /etc/init.d/atd start           启动atd服务

        at 10:32 011311                 定义在2011年1月13日的上午10点32分执行
        at> echo "hello" > /dev/pts/2   
        at>                             按ctrl+d结束

        at -l                           查看还没有运行的at任务,相当于atq

        at -c 5                         查看第五个at任务内容

        at -d 5                         删除每五个at任务，相当于atrm

        at -f test.sh 10:35 011311      时间一到运行脚本

        at -f test.sh now +1 minutes    以当前时间点往后推移1分钟执行test.sh脚本

        at -f test.sh now +1 hours      以当前时间点往后推移1小时执行test.sh脚本

        at -f test.sh now +1 days       以当前时间点往后推移1天执行test.sh脚本

        at -f test.sh now +1 weeks      以当前时间点往后推移1星期执行test.sh脚本
        
        at -f test.sh now +1 months     以当前时间点往后推移1月执行test.sh脚本
        
        at -f test.sh now +1 years      以当前时间点往后推移1年执行test.sh脚本

        /etc/at.deny                    里面记录拒绝执行at服务的普通用户名,一行写一个,如果at.allow不存在的话才生效

        /etc/at.allow                   里面记录允许执行at服务的普通用户名,一行写一个,此文件存在就不考虑at.deny文件


--------------------------------------------------------------------------------

    crontab

        周期性的时间计划任务，时间过了也不执行，到下一个周期再执行

        /etc/init.d/crond start         开启crond服务

        vim /etc/crontab                crontab配置文件
            
            01 * * * * root run-parts /etc/cron.hourly      
            02 4 * * * root run-parts /etc/cron.daily
            22 4 * * 0 root run-parts /etc/cron.weekly
            42 4 1 * * root run-parts /etc/cron.monthly

            第一字段：分钟,取值范围0到59
            第二字段：小时,取值范围0到24
            第三字段：日,取值范围具体看月份
            第四字段：月,取值范围1到12
            第五字段：星期,取值范围0到7， 0和7都代表星期天
            第六字段：执行任务的身份
            第七字段：要执行的命令，多条命令用分号隔开，用括号括起来；
                     run-parts定义后面是一个目录，后面必须写目录路径，代表运行此目录里的所有脚本
            
            时间格式
                
                *   代表每分钟，每小时，每天，每月，每周
                ,   逗号代表分隔时间
                -   代表时间段
                /n  代表每隔n个单位

            例：
                每个小时的前30分钟执行任务
                    
                    1-30    *   *   *   *

                每个小时的第29，58分钟执行

                    29,58   *   *   *   *

                每隔2分钟执行

                    */2     *   *   *   *

                每个月的15号每三个小时，每两分钟，执行任务
                    
                    */2     */3     15  *   *

                每个月的15号每三个小时，每两分钟，执行任务或者星期七每三个小时，每两分钟执行任务
                    
                    */2     */3     15  *   7

                练习：
                    
                    每天的1，3，7，8，13，21点的05分执行
                    每月的5号到20号的4点到13点的15分执行
                    每个星期三的每两小时
                    每年的5到8月的星期三和星期天的4点01分执行

            
            例：
                vim /etc/crontab
                    
                    * * * * * root echo 'hello' > /dev/pts/2

                保存退出，不需要重启服务，保证服务是启动状态，每分钟我们第二个终端就会收到hello

        
        crontab -e                      设定自己的时间任务
            
            * 21 * *　*　init 0       每天21点关机

        
        crontab -l                      查看自己的时间任务

        crontab -l -u root              查看root用户的时间任务

        crontab -e -u root              编辑root用户的时间任务

        crontab -r                      删除当前用户所有时间任务

        crontab -i                      删除前要求确认

    
        /etc/cron.allow                 允许使用crond的普通用户名，有此文件不用考虑cron.deny

        /etc/cron.deny                  拒绝使用crond的普通用户名


--------------------------------------------------------------------------------
    
    anacron
        
        与crond服务配合使用，保证crond服务在一些特殊的情况而没有运行的任务推后执行

        /etc/init.d/anacron start       开启服务

        一般开启此服务就行了，保证crond服务没有按时运行的任务去执行



===============================================================================

四、nfs服务器搭建与autofs使用
    
    涉及的软件包                    nfs-utils

    服务依赖的软件包                portmap
        
        /etc/init.d/portmap start      

        /etc/init.d/nfs start

    vim /etc/exports                服务配置文件
        
        /share/6day     *(ro)       把/share/6day目录共享给所有人，权限为只读
            
            第一列：代表要共享目录的路径
            第二列：*代表共享给所有人，可以改成某个IP或某个网段
            第三列：共享的参数，共享的权限等
                
                ro                  只读
                rw                  可读可写
                sync                同步
                async               异步
                root_squash         把root用户权限降级,默认把root映射成nfsnobody的普通用户
                no_root_squash      不降root用户的权限


    showmount -e 2.2.2.192          查看2.2.2.192服务共享的目录

    mount -t nfs 2.2.2.192:/share/6day /mnt     挂载到本地访问

    /etc/init.d/nfs reload          如果正在连接中的用户不会断开，再连接上来用新配置

    /etc/init.d/nfs restart         如果有正在连接的用户会断开


    开机自动挂载
        
        方法一、修改fstab
            
            2.2.2.192:/share  /mnt nfs defaults 0 0 

        方法二、写到/etc/rc.local
            
            mount -t nfs 2.2.2.192:/share /mnt

        方法三、写到环境变量文件

        方法四、使用autofs自动挂载，以上三种可以实现，但是不推荐，如果网络不正常会影响正常系统启动
            
            /etc/init.d/autofs              启动autofs服务

            vim /etc/auto.master            主配置文件

            /nfs    /etc/auto.nfs --timeout=30      表示/nfs目录的自动挂载情况是由/etc/auto.nfs这个文件来配置的,30秒后自动umount

            mkdir /nfs                      因为/nfs目录不存在，所以创建

            vim /etc/sysconfig/autofs         默认autofs设置
                
                TIMEOUT=300                 默认超时时间

            vim /etc/auto.nfs
                
                share    -   2.2.2.192:/share       自动把2.2.2.192共享的/share目录挂载到/nfs/share下

                share    -rw,no_root_squash  2.2.2.192:/share    可以设置挂载选项

                iso      -   2.2.2.192:/iso -


            /etc/init.d/autofs restart      重启服务
            
            ls /nfs                         没有任何反应

            cd /nfs/share                   直接进入挂载的目录

            如果一段时间没有使用，大概五六分钟后就会自动umount

================================================================================

五、samba服务器搭建和使用
    
    linux之间共享                           nfs 
    
    linux与windows之间共享                  samba
    
    yum list | grep samba
        
        samba-common.i386                   公共的工具包，包括samba.conf和检测其语法的测试工具testparm
        samba.i386                          samba服务端包
        samba-client.i386                   客户端的工具指令
        samba-swat.i386                     图形化samba配置工具，其于web接口

    yum install samba* -y                   完全安装samba

    /etc/samba/smb.conf                     服务器端主配置文件

        [global]                            定义全局的参数
            
            workgroup = MYGROUP             定义工作组
            server string = Samba...        定义服务器描述
            security = user                 定义工作模式

                user模式                    要使用用户名密码访问
                share模式                   可以使用匿名访问,不用密码

            passdb backend = tdbsam         使用本地的.tdb文件保存账号密码
            load printers = yes             加载打印机
            cpus options = raw              打印机类型

        [homes]                             保留的资源名称
            comment = Home Directories      资源描述 
            browseable = no                 指定在匿名用户使用-L时是否可见
            writable = yes                  表示可写，能够上传删除等操作

        [printers]                          保留的资源名称
            .....

        [kyo]                               自己新建的资源名
            comment = kyo                   资源描述
            browseable = no                 指定在匿名用户使用-L时是否可见
            writable = yes                  表示可写，能够上传删除等操作
            guest ok = yes                  可以匿名访问，不用密码，但是前提工作模式必须share才能生效
            path = /kyo                     共享资源路径
            read only = no                  这里如果是yes表示只读
            public = yes                    是否让所有可以登入的使用者看到
            create mask = 0444              定义创建文件默认权限
            directory mask = 0757           定义创建文件夹默认权限
            write list = kyo,bean           定义有写权限的用户列表，user模式
            valid users = kyo,@bean         定义可以访问资源的用户和组
            hosts deny = 2.2.2.0            定义禁止2.2.2.0网段主机访问
            hosts allow = 2.2.2.13          定义2.2.2.13可以访问smb
            hosts deny = ALL                定义禁止所有访问
            host allow = 2.2.2. EXCEPT 2.2.2.14  定义2.2.2.0这个网段可以访问，但是2.2.2.14除外

    
    /etc/init.d/smb start                   把smb服务启动

    客户端验证

            smbpasswd -a root                   把root用户添加进smb用户列表
            smbpasswd -d root                   禁用root不能登录smb
            smbpasswd -e                        启动root用户
            smbpasswd -x                        删除root用户


            smbclient -L //2.2.2.192 -U kyo     用kyo用户登录查看smb服务资源列表

            smclient //2.2.2.192/kyo -U kyo     用kyo用户登录进入kyo共享资源，实际路径kyo的家目录
                
                ls                              列出资源
                put install.log                 上传文件
                get aaa                         下载文件
                rm aaaa                         删除文件
                ?                               帮助
                help get                        得到get命令帮助

            smbclient //2.2.2.192/kyo               匿名进入smb

            mount -t cifs //2.2.2.192/kyo /mnt -o username=kyo,password=123     挂载到本地目录进行操作



================================================================================

六、超级守护程序
    
    超级守护程序管理许多服务的守护程序，用它管理可以提高服务的安全性

    /etc/xinetd.conf                        配置文件
        
        includedir /etc/xinetd.d             更多的设置在/etc/xinetd.d/目录里


    /etc/xinetd.d/                          管理的服务配置文件
        
        service tftp                        管理的服务名称
            
            disable = yes                   设置服务是否关闭，yes为关闭，no为启动服务
            socket_type = dgram             设置联机机制，TCP用stream，UDP用dgram
            protocol = udp                  设置联机状态使用的协议
            wait = yes                      设置服务是否支持多线程，no为支持，可以同时处理用户的连接
            user = root                     设置服务运行的身份，也可以支持UID
            group = 502                     设置服务运行组身份，值为GID
            instances = 30                  设置服务同时联机数最多为30台
            nice = -15                      设置服务运行的优先级，取值范围-19到19,数据越小，运行级别越高
            server = /usr/sbin/in.tftpd     设置服务启动程序
            server_args = -s /tftpboot      设置服务启动程序的参数
            log_on_success += PID           设置成功登录服务记录的信息，+代表之前加入当前设置的参数，-+代表在原有基础舍弃设置的参数
                    PID                     记录PID
                    HOST                    记录IP地址
                    USERID                  记录登录用户
                    EXIT                    记录离开信息
                    DURATION                记录登录时间
            log_on_failure = HOST           设置登录失败，记录哪些信息
                    HOST                    记录IP地址
                    USERID                  记录用户名
                    ATTEMPT                 记录失败原因
                    RECORD                  记录远程主机信息
            port = 65                       设置服务的端口
            redirect = 2.2.2.192 65         设置转发到2.2.2.192,转发到另一服务器
            bind = 2.2.2.192                绑定IP，在有双网卡时绑定哪网卡的IP通信
            only_from = 2.2.2.191           限制只能2.2.2.191能访问此服务，也可以设置网段
            no_access = 2.2.2.{32,56}       限制2.2.2.32和2.2.2.56不许登录此服务
            access_times = 1:00-9:00 20:00-23:00    每天在这两个时段此服务才开放
            umask = 022                     创建文件默认权限的掩码
            cpu = 20 30                     同一秒内最大联机数为20，超过20该服务会暂时停止30秒


        
    /etc/init.d/xinetd restart              重启超级守护程序，这样不需要独立重启管理的服务


--------------------------------------------------------------------------------

    tftp服务的搭建和使用

        yum install tftp -y                 安装客户端
        yum install tftp-server -y          安装服务端

        vim /etc/xinetd.d/tftp              tftp配置文件，因为默认tftp是被xinetd管理
            
            disable = no                    默认是关闭的，所以这里把它改成no，就可以启动
            service_args = -c /tftpboot     设置默认共享文件夹

        /etc/init.d/xinetd restart          重启超级守护程序

        tftp 2.2.2.192                      连接tftp服务器,进入tftp命令行
            
            get aa                          下载aa文件
            put aa                          上传aa文件

        tftp 2.2.2.192 -c get aa            下载aa文件
        tftp 2.2.2.192 -c put aa            上传aa文件

        

================================================================================

七、Bash Shell变量
    
    变量 : 以一组文字或符号来替换一些比较复杂或者容易变动的数据
    
    变量的用途
        
        简单的用途就是为了方便搜索
        常用于编程里，对经常使用的值使用变量，方便引用

    变量的分类
        
        自定义变量                  不影响子进程

        环境变量                    影响子进程

    自定义变量转环境变量
        
        export  自定义变量名        

    变量的定义
        
        定义规则
            
            区分大小写，同名称但大小写不同的变量名是不同的变量
            定义变量时两边不能有空格，有空格的字符串要用引号引起来
            单引号和双引号的区别，单引号会转义所有特殊字符
            变量名可以是数字或字符，但不能以数字开头

        直接赋值定义变量
            
            var=200                 定义var变量，变量值为200
        
        使用declare/typeset定义变量
            
            declare -a arr          定义arr为数组
            declare -i  sum=350+20  定义sum为整数数字
            declare -x var          定义var为环境变量
            declare -r var          定义var为只读变量，不能unset

    释放变量
        
        unset var                   释放var变量

    
    变量赋值可以有多个
        
        var=a                       定义var变量，值为a
        var="$var"c                 给var变量的值后面加上c
        var=${var}d                 给var变量的值后面继续加上d

    特殊的变量定义方式
        
        var=`ls -l`                 把ls -l的标准输出结果赋给var变量
        var=$(ls -l)                和上面效果一样


    提示符变量设置
        
        man bash                    查找PS1关健词

        \d                          代表日期，格式为星期 月 日
        \H                          完整的主机名称
        \h                          仅取主机名称的第一个名字
        \t                          显示时间，为24小时格式，如HH:MM:SS
        \T                          显示时间，12小时格式
        \A                          显示时间，24小时格式,HH:MM
        \u                          当前登录用户名
        \v                          bash的版本信息
        \w                          工作目录的绝对路径
        \W                          显示当前工作目录名
        \#                          命令记数器

--------------------------------------------------------------------------------

    交互式赋值
        
        read var                    等待用户给var变量赋值

        read -p "input your name:" var  提示信息等待用户给var变量赋值

        read -p "input your name:" -t 10 var  提示信息等待用户给var变量赋值,10秒不输入的话直接跳过

        read -s -p "input your password:" passwd    提示信息等待用户给passwd变量赋值并且隐藏显示


--------------------------------------------------------------------------------

    访问变量
        
        $var                        引用var变量的值

        ${var}                      引用var变量的值

        ${var%a*}                   引用var变量的值并且截取a左边的字符串显示，如果字符串有多个a，则按从左向右最后一个a截取

        ${var%%a*}                  引用var变量的值并且截取a左边的字符串显示，如果字符串有多个a，则按从左向右第一个a截取

        ${var#*a}                   引用var变量的值并且截取a右边的字符串显示，如果字符串有多个a，则按从左向右第一个a截取

        ${var##*a}                  引用var变量的值并且截取a右边的字符串显示，如果字符串有多个a，则按从左向右最后一个a截取

        ${var:3}                    引用var变量的值并且从第3个字符开始截取

        ${var:3:6}                  引用var变量的值并且从第3个字符开始截取，截取6个字符显示

        ${#var}                     返回var变量值的长度
    
        ${var/a/b}                  替换变量值的字符串，这里是把var变量值里的a换成b,如果有多个a也只能换一次

        ${var//a/b}                 替换变量值的字符串，这里把var变量值里的a全换成b


    
--------------------------------------------------------------------------------

    特殊变量
        
        echo $$                     返回程序的PID

        echo $0                     脚本名

        echo $1                     指第一个参数
        echo $2                     指第二个参数
        echo $3                     指第三个参数
        ....
        echo $n                     指第n个参数

        echo $*                     代表所有参数
        echo $@                     代表所有参数
        echo $#                     代表参数的个数 

        echo $?                     查看上一条命令执行结果，执行成功返回0，执行失败返回非0


--------------------------------------------------------------------------------
    双向重定向
        
        tee                         把结果传给文件同时又传给屏幕

            less | tee  tee.txt     屏幕会显示结果，tee.txt文件也会产生

        cat > cat.txt               编辑cat.txt文件
            输入内容
            ctrl+d      结束

        cat > cat.txt << EOF        编辑cat.txt以EOF为结束符
        >hello
        >world
        >EOF            结束符，不会写进cat.txt文件            


--------------------------------------------------------------------------------

================================================================================

