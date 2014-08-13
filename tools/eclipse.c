
1、工作空间
 删除：
  修改 eclipse/configuration/.settings/org.eclipse.ui.ide.prefs
  删除掉“RECENT_WORKSPACES”项中对应的工作空间；然后删除对应的工作空间目录；

 创建：
  file->switch workspace->other

 切换：
  file->switch workspace->[name]

 修改名字：
    1.修改“RECENT_WORKSPACES”项中对应的工作空间目录的名字
    2.修改实际目录的名字

2、拷贝工作空间配置：
    导入导出: Import -> General -> Preferences

3、显示行号：Windows->Preference:General_Editors->Text Editors->Show line numbers。

4、打造更强大的代码提示：Windows->Preference:Java->Editor->Contest assist->auto activation triggers for java_
    添上abcdefghijklmnopqrstuvwxyz._。

5、行注释：Ctrl+/，去除再按一遍；块注释：Ctrl+Shift+/，去除Ctrl+Shift+\。

6、放大缩小代码栏：Ctrl+M。 ctrl+W 关闭 ctrl+shift+F4 关闭所有

   改变代码编辑器字体大小：window——prefences——General——Appearance——Color and Fonts，选择basic，选择Text Font，Edit，更改为你想要的大小就ok了。

7、切换不同代码：Ctrl+F6。

8、字母全变小写：Ctrl+Shift+Y；字母全变大写：Ctrl+Shift+X。

9、快速删除当前行：Ctrl+D；复制当前行到下一行：Ctrl+Alt+向下键；复制当前行到上一行：Ctrl+Alt+向上键；快速提取某行到下边指定位置：Alt+向下键；快速提取某行到上边指定位置：Alt+向上键。、

10、快速复写超类方法，批量生产Get、Set方法、快速生产构造函数：Source->很多选项。

11、快速Import，去除没用到得包：Ctrl+Shift+O。

12、快速添加javadoc注释：Alt+Shift+J。

13、重命名:F2。

生产javadoc：Project->generate javadoc。

14、回到上次编辑点：Ctrl+Q。

15、格式化代码：Ctrl+Shift+F。

16、成员排序：Source->Sort Members。

17、代码重构，重新命名：选中单词，Refactor->Rename，所有的单词同时替换。
  
  移动类： Refactor->Move
  改变方法状态：Refactor->Change Method Signature，方法的参数改变，同时引用方法的地方也会变化。
  把匿名内部类转换为外部类：Refactor->Convert Anonymous Class to Nested。    
  把子类方法属性移动到父类：或者父类的方法属性移动到子类：Refactor->Pull Up Push Down。 
  把类里面的方法提取为接口：Refactor->Extract Interface。
  把一段代码提出成方法：Refactor->Extract Method。
  把字符串变成局部变量：Refactor->Extract Local Variable。
  把字符串变成常量：Refactor->Extract Constant。
  把局部变量提升为全局变量：Refactor->Convert Local Variable to Field。
  把变量封闭，类似于get,set：Refactor->Encapsulate Field。

18、Android工程引用java工程：新建java工程，点击Android工程右键->Properties：Java Build Path->Project->Add把java工程引入。

  如果向让其他人用生产JAR包：右键点击java工程->Export:Java->JAR file。

  Android工程引入JAR包：右键单击Android工程->Properties：Java Build Path->Libraries->AddJARs把JAR包引入。

19、快速查看方法和类的内容：按住Shift。

  快速进入方法和类的内部：Ctrl + 单击
  快速进入方法和类的内部后，回后一步：Alt+向左键；向前一步：Alt+向右键。

  列出类的纲要（方法，变量，属性）：Ctrl+O，再按显示父类的纲要。
  查看类的继承关系树：Ctrl+T。
  
  查看某个类的方法被谁调用：Ctrl+Alt+G。
  快速查看某个类：Ctrl+Shift+T。
  快速定位选中词：Ctrl+K。Ctrl+K 跳转到下一个匹配的词，Ctrl+Shift+K 跳转到上一个匹配的词。
  快速定位括号、花括号：Ctrl+Shift+P。
  把匹配的括号变成红色：Windows->Preference：Java->Editor:Matching brackets highlight。
  增量查找（利用首字母去查找）：正向：Ctrl+J；反向：Ctrl+Shift+J
  快速跳转某行：Ctrl+L。
  Quick diff：在行号一栏，能得知增加减少改变的内容。
  添加书签和任务：在行号左一侧点击右键->Add Bookmark或Add Task。Window->Show View可以把Bookmark或者Task放到下面栏。
  搜索：Search，可以搜索关键字和文件。

20、调试代码

  使用Watch查看修改变量
  跳过断点：Run->Skip All Breakpoints，通过右上栏Breakpoints来查看断点值。
  条件断点：debug模式代码左边栏，单击右键->Breakpoints Propeties。
  异常断点:Run->Add Java Exception Points。

21、F2查看类的继承关系。

22、Ctrl+F11 运行当前Project F11 调试当前Project

23、Alt+Enter 快速查看项目的属

乱码：
    选中项目右键-properties-resource Text file encoding -> other ==> GBK
