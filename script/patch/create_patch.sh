#!/bin/bash

# 与当前仓库比较的远程分支
remote_branch="timehold-server/kk-timehold-0523"

CUR_FILE_PATH=`pwd`

function create_patch() {
    branch=$1
    cat .repo/manifest.xml | grep "<project" | while read line
    do
        tmp=$(echo $line | awk '{print $2}')
        tmp=$(echo $tmp | cut -b7-)
        project_path=$(echo $tmp | cut -c1-$((${#tmp}-1)))

        if [ -d $project_path ]; then
            cd $project_path 

#            git checkout $branch  || \
#            {
#                echo "switch branch failed" 
#                cd -
#                continue
#            }

            git_diff_output=`git diff $remote_branch`
            if [ -n "${git_diff_output}" ]; then
                echo "project: $project_path"
                rm 0*.patch > /dev/null 2>&1
                git format-patch $remote_branch
                path=$CUR_FILE_PATH/patch/$project_path
                mkdir -p $path
                mv 0*.patch $path
            fi
            cd - > /dev/null 2>&1
        fi
    done
}

#if [ $# -ne 1 ]; then
#    echo -e "  Parameters are not available.\n  usage: you need put the branch name to switch.\n  eg: $0 branch_name"
#    exit 1
#fi

if [ -f .repo ]; then
    echo "this script must run in \"source code root\" dir."
    exit 1
fi

create_patch $1
