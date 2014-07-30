install:
  1.安装git curl
    $ sudo apt-get install git-core curl

  2.安装repo, 通过curl下载repo
    $ mkdir ~/bin
    $ PATH=~/bin:$PATH
    $ curl http://git-repo.googlecode.com/files/repo-1.12> ~/bin/repo

  3.修改执行权限
    $ chmod a+x ~/bin/repo
  
  4.新建一个目录，然后进入该目录，执行repo init

分支:
  查看可切换的分支:
    cd .repo/manifests
    git branch -a

  切换分支:
    repo init -b  xxx
    repo sync
    repo start xxx --all

comand:
  1. repo init -u url [options]

    -b REVISION     manifest branch or revision
    -m NAME.xml     initial manifest file, 默认为 default.xml

    --mirror            只在repo第一次初始化的时候使用,以和服务器同样的结构在本地建立镜像。
    --reference=DIR     location of mirror directory
    --depth=DEPTH       create a shallow clone with given depth; see git clone

    (1) 完成 repo 这一工具的完整下载, 即克隆repo.git 产生.repo/repo
    (2) 克隆 android 的清单库 manifest.git(地址来自于 -u 参数)
        克隆的清单库位于 .repo/manifests.git 中, 本地克隆到 .repo/manifests
        清单库中的 manifest.xml 文件, 列出了多个版本库的克隆方式。包括版本库的地
      址和工作区地址的对应关系,以及分支的对应关系。
    (3) 询问用户的姓名和邮件地址, 如果和 Git 默认的用户名、邮件地址不同,则记录
      在 .repo/manifests.git 库的 config 文件中

  2. repo sync
    开始同步,即分别克隆多个版本库到本地的工作区中。
    可以在 repo sync 后面跟上项目的名称。项目的名称来自于 .repo/manifest.xml 
  这个 XML 文件中 project 元素的 name 属性值。
    如果某个项目版本库尚不存在,则执行 repo sync 命令相当于执行 git clone
  如果项目版本库已经存在,则执行
        git remote update           相当于对每一个remote源执行 fetch 操作。
        git rebase origin/branch    针对当前分支的跟踪分支执行 rebase 操作

    如果在merge 的过程中出现冲突， 这需要手动运行  git rebase --continue

  3. repo start --all branch
    同时对多个版本库执行切换分支操作, 相当与git checkout -b

    reop abandon xxx;  删除分支

  4. repo status
    对 git diff-index、git diff-files 命令的封装,同时显
    示暂存区的状态和本地文件修改的状态。
  
  5. repo checkout
    对 git checkout 命令的封装。检出之前由 repo start 创建的分支。

  6. repo branches
    读取各个项目的分支列表并汇总显示。该命令实际上通过直接读取 .git/refs
  目录下的引用来获取分支列表,以及分支的发布状态等。

  7. repo diff
    显示提交的代码和当前工作目录代码之间的差异。

  8. repo forall -p -v -c 'git_cmd' 
        repo forall -p -v -c 'git checkout -b branch_name tag_name'

  9. repo update [project-list]
    上传修改的代码 ，上传到 Gerrit (基于web 的代码review 系统), Gerrit 收到上传的代码，
  会转换为一个个变更，从而可以让人们来review 修改的代码。

