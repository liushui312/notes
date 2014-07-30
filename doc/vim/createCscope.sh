#!/bin/sh

#CodeRootDir=`pwd`
#LangOptions="--langmap=c:+.inl"
#ROptions="-R"
#CtagsPath="/home/billzhu/working/ctags/NoOSTags"
#echo "ctags -o $CtagsPath $LangOptions $ROptions $CodeRootDir"
#ctags -o $CtagsPath $LangOptions $ROptions $CodeRootDir

CsPath=$HOME/.vim/cscope
echo ${CsPath}
if [ ! -d $CsPath ]; then
    mkdir -p ${CsPath}
fi
rm ${CsPath}/*

find `pwd` -name "*.c" -o -name "*.h" -o -name "*.inl" -o -name "*.mk" | \
         sed 's/\(["'\''\]\)/\\\1/g;s/^/"/;s/$/"/' > $CsPath/cscope.files
cd $CsPath
cscope -bqk -i cscope.files
cd -
#cscope -bqk -i $CsPath/cscope.files
#pwd
