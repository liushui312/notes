#!/bin/bash

CUR_FILE_PATH=`pwd`

function am_patch() {
    for dir in `find ./patch/ -type d`
    do
        echo $dir | grep "NEWTRUNK" > /dev/null 2>&1 && \
        {
            continue
        }

        cd $dir
        find -maxdepth 1 -type f | grep "\.patch" > /dev/null 2>&1
        HAS_PATCH=$?
        if [ "$HAS_PATCH" -eq 0 ]; then
            project_path=$(echo $dir | cut -b 9-)
            path="${CUR_FILE_PATH}/${project_path}"
            echo "project: $project_path"
            rm $path/*.patch ~ > /dev/null 2>&1
            cp *.patch $path
            cd $path || \
            {
                echo "***** cd $path failed! *******"
                return 2
            }

            line=$(cat $CUR_FILE_PATH/patch/targetcommit.log | grep $project_path)
            targetcommit=$(echo $line | awk '{print $2}')
            targetcommit=$(echo $targetcommit | cut -b 10-)
            git reset --hard $targetcommit

            for patch in `find ./ -maxdepth 1 -name "*.patch" | sort`
            do
                echo "  patch: $patch"
                git am --ignore-whitespace --3way $patch || \
                {
                    echo "*** git am failed! ****" 
                    return 2
                }
            done
            #rm *.patch
        fi

        cd ${CUR_FILE_PATH} 
    done
}

############ main begin ##################
if [ -f .repo ]; then
    echo "this script must run in \"source code root\" dir."
    exit 1
fi

am_patch
ret=$?
if [ "$ret" -eq 0 -a -e "${CUR_FILE_PATH}/patch/NEWTRUNK" ]; then
    cp ${CUR_FILE_PATH}/patch/NEWTRUNK/* -rvf $CUR_FILE_PATH
elif [ ! "$ret" -eq 0 ]; then
    echo "************ am patch failed *************"
    exit 2
fi

echo "*********** am patch successful **************"
