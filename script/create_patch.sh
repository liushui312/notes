#!/bin/bash

CUR_FILE_PATH=`pwd`

function create_patch() {
#    xml=$1
	
    cat xmldiff | grep "<project" | while read line
    do
#        tmp=$(echo $line | awk '{print $2}')
#        tmp=$(echo $tmp | cut -b7-)
#        project_path=$(echo $tmp | cut -c1-$((${#tmp}-1)))
#        echo $line
		
		project_path=${line#*path=\"}
		if [ "$project_path"x = "$line"x ]; then
			project_path=${line#*name=\"}
		fi
		project_path=${project_path%%\"*}
		
		oldcommit=${line#*revision=\"}
		oldcommit=${oldcommit%%\"*}

		#echo $project_path
		#echo $oldcommit
		
        if [ -d $project_path ]; then
            echo "Project: $project_path"
            cd $project_path 

            newcommit=`git log -n 1 | head -n 1 | cut -b 8-`

#            revision=$(cat $xml | grep -Irs "path=\"${project_path}\"" | awk '{print $4}')
#            revision=$(echo $revison | cut -b11-)
#            oldcommit=$(echo $revision | cut -c1-$((${#tmp}-1)))

            rm *.patch > /dev/null 2>&1

            targetcommit=$(git merge-base $oldcommit $newcommit)
            git format-patch $targetcommit
            path=$CUR_FILE_PATH/patch/$project_path
            mkdir -p $path
            mv *.patch $path > /dev/null 2>&1
				
            cd - > /dev/null 2>&1
			
			echo "$project_path  revision=$targetcommit" >> ./patch/targetcommit.log
        fi
    done
}

####################### main branch #############################

if [ ! -d ".repo" ]; then
    echo "this script must run in \"source code root\" dir."
    exit 1
fi

create_patch
