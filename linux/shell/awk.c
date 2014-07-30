
Awk是一门编程语言，有很多版本，我们用的是GNU的gawk.

行输入驱动, 循环读入一行

  1.命令行语法: awk 'instruction' file  (分号分隔多个指令)
  2.用脚本文件: awk -f scriptfile file

参数：
  -f
  -F    指定分隔符
  -v

特殊符号：
  $0         代表所有输入行
  $1-n       代表一个输入行的第几个字段

eg:
  awk -F : '{print $3}' /etc/passwd

特殊例程： 后接语句块{}
  BEGIN      在第一个输入行读入之前执行
  END        在读完所有输入行后再执行

eg:
    以:为分隔符统计/sbin/nologin有多少个
        awk 'BEGIN{FS=":" ;count=0}{if($7 = "/sbin/nologin")count+=1}END{print count}' /etc/passwd

主输入循环执行的指令： 模式+过程

  1. 模式匹配

    正则表达式： /regular/

        $N~/regular/    输入行的第N个字段匹配regular    的输入行
        $N!~/regular/   输入行的第N个字段不匹配regular  的输入行

    关系表达式：<关系表达式>
eg:
    'NR==1{print $0}'

  2. 过程
    {}语句块

eg:                
    使用awk脚本
        
        #!/bin/awk
        
        BEGIN{              
            FS=:
            count=0         定义变量
        }

        {
            if($7 == "/sbin/nologin")
            {
                count += 1
                print $0
            }
        }

        END{
            print "count="count 
        }
        
        awk -f 1.awk /etc/passwd        运行上面的脚本
-----------------------------------------------------------------------
变量：
  定义：直接赋值，大小写区分，
    x = 1
    y="hello"
    z="hello" ",world"
  引用：$ , 直接变量名
    w = $1

  常量：
     字符串：必须用引号扩起来，eg，"red"
     数字：eg, 123

系统变量： 
  FILENAME    当前处理的文件名字

  FS          输入字段分隔符, 默认为空格        支持正则表达式
  OFS         输出字段分隔符, 默认为空格
  ORS         输出记录分隔符, 默认为换行符
  NR          当前处理到第几个记录
  NF          当前输入记录的字段个数
  FNR         当前处理到第几个文件

  OFMT        输出数值默认精度

-----------------------------------------------------------------------
算术操作符：
    +
    -
    *
    /
    %
    ^   取幂

赋值操作符：
    ++
    --
    +=  -+ *= /+ %= ^=

关系操作符：
    <   >
    <=  >=
    ==  !=
    ~   匹配    
    !~  不匹配      
    
    注意：~ 和 !~ 后面可跟正则表达式常量和正则表达式变量
            ~/regular/
            或
            re=/regular/
            ~re
布尔操作符
    &&
    ||
    ！

-----------------------------------------------------------------------
条件和循环：
1. if

  if ( expression ) action1

  if ( expression ) {
      statement1
      statement2
  }

  if ( expression1 ) 
    action1  
  else if (expression2)
    action2
  else 
    action3

expression 可用模式匹配操作符 ~

2. expr ? action1 : action2

3. while ( condition ) {
        action
   }

   do 
        action
   while (condition)

4. for (i = 1; i <= NF; i++) {
    ...
   }


continue

break

next  能够导致读入下一个输入行

eg:
   FILENAME == "acronyms" {
	action
	next
   }
   {print}

exit	退出主输入循环，进入END

在END中调用无参数的exit， 则使用第一个值

-----------------------------------------------------------------------
数组,  不必指明数组的大小, 数组不是顺序存储

    array[subscript] = value

eg:
    student_avg[NR] = avg

数组下标可以是数字和字符（关联数组）, 所有的数字下标都是字符串类型
eg：
    array[$1] = $2

for (variable in 数组名)
    do something with array[variable]

awk 'instruction' -
减号代表从标准输入读入输入行

eg: 
    glossary文件
        术语    术语定义
        ...
 
awk '
BEGIN {
    #prompt user
    printf("Enter a glossary ter: ")
}

#1 reads local glossary file
FILENAME == "glassary" {
    entry[$1] = $2
    next
}

#2 scan for command to exit program 
$0~/^(quit|[qQ]|exit|[Xx])$/{exit}

#3 process any non-empty line
$0 != "" {
    if ($0 in entry) {
        print entry[$0] 
    } else
        print $0 "not found"
}

#4 prompt user again for another term
{
    printf("Enter another glossary term (q to quit):")
}
' glossary -

用split()创建数组， 内置函数split()能将任何字符串分解到数组中
    
  n = split(string, 数组名， separator)

    separator   字符或正则表达式
    n           数组元素的个数

删除数组元素
    delete array[subscipt]

多维数组
    ....

----------------------------------------------------------------------
脚本传递参数
    awk 'instruction' var1=value1 var2=value2 file
    awk -f scriptfile var1=value1 var2=value2 file

    value 可以是环境变量或命令的输出结果，eg. var=`pwd`
    var   可以是awk的系统变量

注意：1. 在BEGIN过程中是用不到的
      2. 变量必须在指令和file之间

----------------------------------------------------------------------
系统函数：
  1. print      打印，以逗号为参数间隔
     
     无参数，输出当前行

  2. printf     格式化输出, 需加换行

      %-width.precision format-specifier (有"-"左对齐, 没有右对齐)
  
  3. sort

-----------------------------------------------------------------------
eg:
    awk '{ print $0 }' /etc/passwd        打印所有行

    awk -F ":" '{print $1}' /etc/passwd   打印第一列
        
    awk -F ":" '{print $1"\t"$3" welcome"}' /etc/passwd  打印第一列和第三列

    awk -F : '{print $NF}' /etc/passwd      打印每行的最后一列

    awk -F : '{if (NR == 8){print $0}}' /etc/passwd
    awk -F : 'NR == 8 {print $0}' /etc/passwd           打印第八行

    awk -F : 'END{print NR}' /etc/passwd        打印行总数

    awk -F : '{print NF}' /etc/passwd           打印每行的字段数
            
    awk -F : 'END{print $NF}' /etc/passwd       打印最后一行的最后一列
            
    awk -F : 'NF > 4 {printf $0}' /etc/passwd   打印字段数大于4的行

    awk -F : 'BEGIN{c = 0}{c=c+NF}END{print c}' /etc/passwd    打印文件里所有字段总数

    awk -F : '$3 >= 30 && $3 <= 40{print $1}' /etc/passwd   打印uid在30~40范围内的用户名
        
    awk -F : 'NR >= 5 && NR <= 56 {print $0}' /etc/passwd   打印5到56行
            
    awk '{if (NR % 2 == 0)print $0}' /etc/passwd        隔行删除

    awk 'BEGIN{FS="[^A-Za-z]"}{print $1}' /etc/passwd   打印每行的第一单词
            
    awk 'BEGIN{FS="[^a-zA-Z]+"}{print $1,$3}' /etc/passwd   打印每行的第一个和第三个单词
            
    awk 'BEGIN{FS=":";sum=0}{NF > 5{sum=sum+1}}END{print sum}' /etc/passwd  打印字段数大于5个的行总数

