#!/bin/sh

CUR_FILE_PATH=`dirname $0`
FILES=`ls`

#if [ -f "$CUR_FILE_PATH/../note.tar" ]; then
#    echo 00
#    tar vuf $CUR_FILE_PATH/../note.tar $CUR_FILE_PATH/$FILES .gitignore
#else
#    echo 11
#    tar vcf $CUR_FILE_PATH/../note.tar $CUR_FILE_PATH/$FILES .gitignore
#fi

rm $CUR_FILE_PATH/../note.tar
tar vcf $CUR_FILE_PATH/../note.tar $CUR_FILE_PATH/$FILES .gitignore
