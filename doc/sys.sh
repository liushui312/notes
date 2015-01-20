#!/bin/bash

echo ""
#apt-add-repository ppa:tualatrix/ppa        #for ubuntu-tweak
#add-apt-repository ppa:phablet-team/tools   #for android-tools-adb
#apt-get update

echo""
echo "********* 解决gedit乱码 *********"
gsettings set org.gnome.gedit.preferences.encodings auto-detected "['UTF-8','GB18030','GB2312','GBK','BIG5','CURRENT','UTF-16']"
echo "********* 乱码解决完毕 *********"

echo ""
echo "******** 删除一些没用的软件 ***********"
apt-get remove totem totem-gstreamer totem-mozilla --force-yes  -y
apt-get remove rhythmbox evolution bittorrent empathy --force-yes  -y
apt-get purge ibus ibus-gtk ibus-gtk3 --force-yes  -y
killall ibus-daemon
rm -rf ~/.config/ibus
echo "******** 删除软件完成 ***********"

echo ""
echo "*********** 安装必要软件 *************"
#apt-get install zsh  autojump --force-yes -y
apt-get install vim cscope --force-yes -y
apt-get install git --force-yes -y
#apt-get install jekyll --force-yes -y

apt-get install minicom --force-yes -y
apt-get install libncurse --force-yes -y
apt-get install bison flex libxml2-utils autoconf automake --force-yes -y
apt-get install gnupg gperf lzop gcc-4.4 g++-4.4 g++-4.4-multilib --force-yes -y
#apt-get install ubuntu-tweak unity-control-center --force-yes -y
apt-get install tree wget curl --force-yes -y
apt-get install zip unzip rar unrar --force-yes -y
apt-get install dos2unix --force-yes -y
#apt-get install ksnapshot --force-yes -y
#apt-get install calibre --force-yes -y
echo "********* 安装完成 ***********"

echo ""
echo "*********** 动态库 ***********"
ln -s -f /usr/local/mpc-0.9/lib/libmpc.so.2 /usr/lib/libmpc.so.2
ln -s -f /usr/local/mpc-0.9/lib/libmpc.so /usr/lib/libmpc.so

echo ""
echo "*********** 安装 oh-my-zsh *************"
curl -L https://github.com/liushui312/oh-my-zsh/raw/master/tools/install.sh | sh
