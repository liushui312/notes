
git merge [选项...] <commit>...
            
常用选项
  --commit      自动提交，默认
  --no-commit   仅改变工作区的内容 
  -m            自动提交时所使用的注释

  --abort

<commit> 提交ID或对应的引用：分支、里程碑等


git checkout master
git merge development

如果出现冲突，系统会自动以一种特殊的格式将冲突写入文件

<<<<<<< HEAD
this is C5                  //当前分支内容
=======
this is A_2                 //指定分支内容
>>>>>>> development

冲突解决
  1. 手动修改
  2. git mergetool 

当手工merge后，需要调用git commit 提交

