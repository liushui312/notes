#!/bin/sh

echo ""
echo "********* config vim  ***********"
CUR_FILE_PATH=`dirname $0`
cp ${CUR_FILE_PATH}/vim/*  ${HOME}/.vim/ -rfvu
cp ${CUR_FILE_PATH}/vimrc ${HOME}/.vimrc -vu

if [ ! -d "${HOME}/bin" ]; then
    mkdir -p ${HOME}/bin
    echo "export PATH=\$PATH:${HOME}/bin" >> ${HOME}/.bashrc
    source ${HOME}/.bashrc
fi
cp ${CUR_FILE_PATH}/createCscope.sh ${HOME}/bin/ -vu
chmod 755 ${HOME}/bin/createCscope.sh
echo "********* successful ***********"
