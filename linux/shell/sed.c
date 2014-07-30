
Sed 流编辑器
  sed 是面向字符流的，从文本中一次一行的输入，处理，输出到终端；

sed脚本的四种类型
    对同一个文件的多种编辑
        仔细测试每一个命令

    改变一组文件
        选择有代表性的且包含异常测试的文件进行测试
        用grep来检查大量输入

    提取文件的内容

    编辑工作转移
        管道

使用方法：
  1. 在命令行上执行编辑指令：sed [options] 'instruction' file(s)  (分号分隔多个指令)
  2. 编辑指令放在文件中    ：sed [options] -f scriptfile file(s)

参数：
  -e    只有多个指令时才需要，表示下一个参数为指令。或者用 
  -r    替换-e，支持扩展正则表达式
  -i    替换-e，对源文件内容进行实际操作
  -n    sed默认输出每个输入行，此选项阻止自动输出 

指令instruction：  [address]command
  1.模式匹配，选择要处理的行 [address]
  2.过程，如何处理           command


  过程1和过程2作用于同一个地址
  address {
    command1 
    command2
  } //必须单独一行, 且后面没空格

  地址嵌套 
  address1 {
    [address]command 
  }

1.模式匹配[address] (可选，省略则全部行都参与过程)

  行号: N     第N行
        N,M   N到M行
  有些命令只能接受单个行

  正则表达式:
        /regular/

2.过程
  (1) d  删除(过滤输出)
   指令：模式+过程
     '行号d'
     '/regular/d'
例： 
  sed -e '2,4d' /etc/passwd     删除第二行到第四行
  sed -e '1,3d;5d' /etc/passwd  删除第一行到第三行和第五行，分号代表两条指令
  sed -e '/root/d' /etc/passwd  删除匹配root的行
  sed -e '/^$/d' /etc/passwd    删除空行

  sed -i '1d' /tmp/passwd       对源文件内容进行实际操作 ,把-e换成-i 

   (2) s  替换
   指令：模式+过程
     [address]s/regular/replacement/flags

  flags:
    缺省    第1次出现，进行替换
    n       1-512 第n次出现，进行替换
    g       把一行的全部替换
    p       打印模式空间的内容
    w file  将模式空间的内容写到文件file中


     '行号s/regular/replacement/g'          
     '/regular/s/regular/replacement/g'     行号和/regular/为模式匹配

     replacement中包含
       1. &    表示前面被替换的字符

       2. \    转义换行符(另起一行)和转义，需转义的字符:
                                                & \ . 定界符
       3. \n   匹配第n个字符串, 这个字串之前在regular中用"\("和"\)"指定
       使用域进行定位, 最多可以保存9个域, 即n (1-9)
        -e 就要用"\(" "\)"
        -r 不需要转义直接"(" ")"

例：
  sed -e 's/root/kyo/' /etc/passwd         把每行的第一个root替换成kyo 
  sed -e '2,10s/root/kyo/g' /etc/passwd    指定2到10行范围把root替换成kyo 
  sed -e 's/root/&boot/' /etc/passwd       把root替换成rootboot, 
  sed -e '/^bin/,/^adm/s/a/&bc/g'          指定以bin开头的行到以adm开头的行，把a换成abc 
   sed -r '/^a+/&b/g'                      支持扩展正则表达式

    (3) p  打印(sed默认输出每个输入行, 必须加-n参数)
    指令：模式+过程
     '行号p'
     '/regular/p'
例：
 sed -ne '/^[a-z]/p;/nologin$/p'  打印以小写字母开头，和以nologin结尾的行

    (4) a  追加 
        [line-address]a\
            text          //换行需加\

        i  插入
        [line-address]i\
            text

        c  更改
        [address]c\
            text
        
      追加将text放在当前行之后
      插入将text放在当前行之前
      更改将text则替换

    (5) l 列表，显示内容，将非打印字符显示为ASCLL码
    

----------------------------------------------------------------------------

        删除每行的第一个字符
            
            sed -e 's/.\(.*\)/\1/'      
        
        删除每行的第二个字符
            
            sed -e 's/\(.\).\(.*\)/\1\2/'

        删除最行最后一个字符
            
            sed -e 's/\(.*\)./\1/'

        删除每行的倒数第二个字符
            
            sed -e 's/.\(.\)$/\1/'

        删除每行的第二个单词

            sed -r 's/[a-zA-Z]+//2'

        删除每行倒数第二个单词

            sed -r 's/[a-zA-Z]+([ \t]+[a-zA-Z]+)$/\1/'

        删除每行的最后一个单词
            
            sed -r 's/[a-zA-Z]+$//'

        交换每行的第一个字符和第二个字符
            
            sed -r 's/^(.)(.)/\2\1/'

        交换每行的第一个单词和第二个单词
            
            sed -r 's/([a-zA-Z]+)([^a-zA-Z]+)([a-zA-Z]+)/\3\2\1/'

        交换每行的第一个单词和最后一个单词
            
            sed -r 's/([a-zA-Z]+)([^a-zA-Z]+.*[^a-zA-Z]+)([a-zA-Z]+)$/\3\2\1/'

        删除文件中所有的数字

            sed -e 's/[0-9]//g'

        把所有大写字母用括号()括起来
            
            sed -r 's/[A-Z]/(&)/g'

        只显示每行的第一个单词
            
            sed -r 's/^([a-zA-Z]+).*/\1/'

        打印每行的第一个单词和第三个单词

            sed -r 's/^([a-zA-Z]+)[^a-zA-Z]+[a-zA-Z]+[^a-zA-Z]+([a-zA-Z]+).*/\1\2/'
---------------------------------------------------

文件的行数
line=`sed -n '$=' file`
