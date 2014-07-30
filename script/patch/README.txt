create_patch.sh : 创建patch 的脚本
    1. 修改 create_patch.sh 脚本的remote_branch 变量，用于指定生成那些patch
        即：本地仓库相比与此远程分支的所有提交，都将生成patch
    2.  将 create_patch.sh 放到代码根目录执行，将在此目录下生成patch目录，其包含对应patch

am_patch.sh：打patch 的脚本
    1. 将create_patch.sh 生成的patch目录和am_patch.sh 放到代码根目录
    2. 执行am_patch.sh即可

注意：打patch 的时候，看log 是否有冲突，如有，记得解决。
