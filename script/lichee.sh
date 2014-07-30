#!/bin/sh

cd lichee
echo "********* now in lichee *************"

echo ""
echo "******* repo branch ***********"
repo branch

echo ""
echo "****** repo status **********"
repo status

echo ""
echo "******* git diff remotes ***********"
repo forall -p -v -c 'git diff remotes/lichee/sunxi-kitkat-a23'

read -p "是否同步代码? [Y/n]" check
if [ $check = "Y" -o $check = "y" ]; then
    echo ""
    echo "********* repo sync ***********"
    repo sync
else
    echo "退出"
    exit 
fi
