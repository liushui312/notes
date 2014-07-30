#!/bin/bash

function am_patch() {

    find ./patch/ -type f | grep "\.patch" | while read line
    do
        tmp=$(dirname $line | cut -b 8-)
        project_path=${CUR_FILE_PATH}${tmp}

        echo "project: $project_path"
        cp $line $project_path

        cd $project_path || \
        {
            echo "cd $project_path failed!"
            cd - /dev/null 2>&1
        }
        
        patch=`basename $line`
        echo "  patch: $patch"
        git am --ignore-whitespace --3way  $patch
        cd - > /dev/null 2>&1
        
    done
}

CUR_FILE_PATH=`pwd`

if [ -f .repo ]; then
    echo "this script must run in \"source code root\" dir."
    exit 1
fi

am_patch
