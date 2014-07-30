#!/bin/sh

CUR_FILE_PATH=`dirname $0`
install_dir="${HOME}/usr"

echo ""
echo "********** install ant ***********"
ant_tar=`find ${CUR_FILE_PATH} -name apache-ant*.tar.gz`
tar zvxf ${ant_tar} -C ${install_dir} | awk 'BEGIN{FS="/"}{if(NR == 1) print $1}' | xargs -t -i mv ${install_dir}/{} ${install_dir}/ant/

grep "ANT_HOME" $HOME/.bashrc
ret=$?
if [ ! "$ret" -eq 0 ]; then
    echo "\n#for ant\nexport ANT_HOME=${install_dir}/ant\nexport PATH=\$PATH:\$ANT_HOME/bin\n" >> ${HOME}/.bashrc
    . ${USER}/.bashrc
fi

find ${CUR_FILE_PATH} -name "jsch-0.1.50.jar" | xargs -t -i cp {} -v ${install_dir}/ant/lib/ || \
{
    echo "ssh 功能将不能使用"
}
echo "********** ant 安装成功 **********"
